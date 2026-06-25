#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdint>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <sqlite3.h>

#pragma pack(push, 1)
struct Transaction {
    uint64_t id;
    uint64_t timestamp;
    char type[4];
    double price;
    uint32_t quantity;
};
#pragma pack(pop)

struct IndexJob {
    std::vector<Transaction> transactions;
    size_t start_offset;
};

class WalEngine {
private:
    std::vector<Transaction> buffer;
    std::vector<Transaction> active_state;
    size_t threshold;
    std::string filename;
    
    std::mutex mtx;
    std::mutex queue_mtx;
    std::condition_variable cv;
    std::vector<IndexJob> job_queue;
    std::atomic<bool> running;
    std::thread worker;
    sqlite3* db;

    void background_worker() {
        while (running || !job_queue.empty()) {
            std::unique_lock<std::mutex> lock(queue_mtx);
            cv.wait(lock, [this]() { return !job_queue.empty() || !running; });
            
            while (!job_queue.empty()) {
                IndexJob job = std::move(job_queue.front());
                job_queue.erase(job_queue.begin());
                lock.unlock();

                size_t current_offset = job.start_offset;
                
                sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
                
                sqlite3_stmt* stmt;
                const char* sql = "INSERT INTO log_index (transaction_id, file_offset, volume) VALUES (?, ?, ?);";
                sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
                
                for (const auto& tx : job.transactions) {
                    double volume = tx.price * static_cast<double>(tx.quantity);
                    
                    sqlite3_bind_int64(stmt, 1, static_cast<sqlite3_int64>(tx.id));
                    sqlite3_bind_int64(stmt, 2, static_cast<sqlite3_int64>(current_offset));
                    sqlite3_bind_double(stmt, 3, volume);
                    
                    sqlite3_step(stmt);
                    sqlite3_reset(stmt);
                    
                    current_offset += sizeof(Transaction);
                }
                
                sqlite3_finalize(stmt);
                sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
                
                lock.lock();
            }
        }
    }

    void flush() {
        if (buffer.empty()) {
            return;
        }

        std::ofstream out(filename, std::ios::binary | std::ios::app);
        if (out) {
            out.seekp(0, std::ios::end);
            size_t start_offset = static_cast<size_t>(out.tellp());
            
            out.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(Transaction));
            out.close();

            {
                std::lock_guard<std::mutex> lock(queue_mtx);
                job_queue.push_back({buffer, start_offset});
            }
            cv.notify_one();
            
            buffer.clear();
        }
    }

public:
    WalEngine(size_t threshold, const std::string& filename) 
        : threshold(threshold), filename(filename), running(true) {
        
        sqlite3_open("metadata.db", &db);
        const char* create_table_sql = 
            "CREATE TABLE IF NOT EXISTS log_index ("
            "transaction_id INTEGER PRIMARY KEY,"
            "file_offset INTEGER,"
            "volume REAL"
            ");";
        sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);

        worker = std::thread(&WalEngine::background_worker, this);
    }

    ~WalEngine() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            flush();
        }
        running = false;
        cv.notify_one();
        if (worker.joinable()) {
            worker.join();
        }
        sqlite3_close(db);
    }

    void append(const Transaction& tx) {
        std::lock_guard<std::mutex> lock(mtx);
        buffer.push_back(tx);
        if (buffer.size() >= threshold) {
            flush();
        }
    }

    void recover() {
        std::lock_guard<std::mutex> lock(mtx);
        active_state.clear();
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            return;
        }

        Transaction tx;
        while (in.read(reinterpret_cast<char*>(&tx), sizeof(Transaction))) {
            active_state.push_back(tx);
        }
        std::cout << active_state.size() << "\n";
    }
};

int main(int argc, char* argv[]) {
    if (argc > 1 && std::strcmp(argv[1], "--recover") == 0) {
        WalEngine engine(5, "wal.log");
        engine.recover();
        return 0;
    }

    {
        WalEngine engine(5, "wal.log");
        for (int i = 0; i < 12; ++i) {
            Transaction tx;
            tx.id = static_cast<uint64_t>(i + 1);
            tx.timestamp = static_cast<uint64_t>(1700000000 + i);
            std::memcpy(tx.type, "BUY", 4);
            tx.price = 150.0 + i;
            tx.quantity = static_cast<uint32_t>(10 * (i + 1));
            engine.append(tx);
        }
    }

    {
        WalEngine engine2(5, "wal.log");
        engine2.recover();
    }

    return 0;
}
