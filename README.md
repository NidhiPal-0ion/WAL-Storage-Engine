# WAL Storage Engine

A fault-tolerant **Write-Ahead Logging (WAL) Storage Engine** built in **C++23** for durable event persistence, asynchronous metadata indexing, and deterministic crash recovery. The project implements append-only binary logging, batched in-memory buffering, SQLite-based metadata indexing, and a Python dashboard for monitoring log activity and recovery execution.

---

## Features

### Write-Ahead Logging (WAL)

- Append-only binary transaction logging
- Sequential file persistence
- Packed binary transaction serialization
- Batched in-memory buffering
- Automatic buffer flushing

### Crash Recovery

- WAL replay for state reconstruction
- Deterministic recovery mechanism
- Binary log parsing
- Active state rebuilding

### Metadata Indexing

- SQLite metadata database
- Transaction ID indexing
- File offset tracking
- Transaction volume calculation
- Background asynchronous indexing

### Concurrency

- Producer-consumer architecture
- Dedicated background indexing thread
- Thread-safe buffering
- Condition-variable based task scheduling

### Monitoring Dashboard

- Streamlit-based dashboard
- Transaction statistics
- WAL size monitoring
- Indexed record visualization
- Recovery execution from dashboard

---

# System Architecture

```text
                  Client Transactions
                          │
                          ▼
                  WAL Storage Engine
                          │
             In-Memory Transaction Buffer
                          │
                  Batch Flush Threshold
                          │
                          ▼
               Append-Only Binary WAL
                          │
          ┌───────────────┴───────────────┐
          ▼                               ▼
   Recovery Engine              Background Indexer
          │                               │
          ▼                               ▼
  State Reconstruction            SQLite Metadata
                  └───────────────┬───────────────┘
                                  ▼
                     Streamlit Monitoring Dashboard
```

---

# Project Structure

```text
WAL-Storage-Engine/

│── main.cpp
│── engine.exe
│── app.py
│── metadata.db
│── wal.log
│── README.md
```

---

# Build

Compile using GCC:

```bash
g++ -std=c++23 main.cpp -lsqlite3 -o engine.exe
```

---

# Run

Generate sample transactions and create the WAL.

```bash
./engine.exe
```

Windows

```powershell
.\engine.exe
```

---

# Launch Dashboard

```bash
streamlit run app.py
```

The dashboard displays:

- Total Transactions
- Total Volume Processed
- WAL File Size
- Indexed Records
- Recovery Status

It also allows simulation of crash recovery directly from the interface.

---

# Recovery

Recover application state from the Write-Ahead Log.

```bash
./engine.exe --recover
```

Windows

```powershell
.\engine.exe --recover
```

---

# Technologies Used

- C++23
- SQLite3
- Python
- Streamlit
- STL
- Multithreading
- Binary Serialization

---

# Applications

This project demonstrates concepts used in:

- Storage Engines
- Database Systems
- Write-Ahead Logging (WAL)
- Fault-Tolerant Systems
- Crash Recovery
- Backend Infrastructure
- Financial Trading Infrastructure
- System Design

---

# Future Improvements

- Log Rotation
- Snapshot-Based Recovery
- Performance Benchmarking
- Configurable Buffer Size
- Persistent Configuration Files
- Advanced Dashboard Analytics
- Unit Testing Framework
- Multi-WAL File Support

---

# Author

**Nidhi**