# WAL Storage Engine

A fault-tolerant storage engine written in modern **C++23** implementing a **Write-Ahead Logging (WAL)** architecture for durable event persistence, deterministic crash recovery, and asynchronous metadata indexing.

The project implements:

* Append-Only Binary Write-Ahead Logging
* Sequential File Persistence
* Batched In-Memory Buffering
* SQLite Metadata Indexing
* Crash Recovery via WAL Replay
* Multithreaded Background Indexing
* Python Monitoring Dashboard

---

# Features

## Storage Engine

Implements a durable Write-Ahead Logging (WAL) storage engine using append-only binary files.

Features:

* Append-only transaction logging
* Binary serialization
* Sequential file persistence
* Batched in-memory buffering
* Automatic buffer flushing

---

## Crash Recovery

Reconstructs application state by replaying the Write-Ahead Log.

Supports:

* WAL replay
* State reconstruction
* Binary log parsing
* Deterministic recovery

---

## Metadata Indexing

Stores transaction metadata separately for efficient lookup and analytics.

Features:

* SQLite metadata database
* Transaction indexing
* File offset tracking
* Transaction volume storage
* Background asynchronous indexing

---

## Monitoring Dashboard

Python-based dashboard for monitoring storage engine activity.

Displays:

* Total transactions
* Total processed volume
* WAL file size
* Indexed records
* Recovery execution
* Storage metrics

---

# System Architecture

```text
                 Client Transactions
                         │
                         ▼
                 WAL Storage Engine
                         │
                 In-Memory Buffer
                         │
                Batch Buffer Flush
                         │
                         ▼
            Append-Only Binary WAL
                         │
          ┌──────────────┴──────────────┐
          ▼                             ▼
   Recovery Engine            SQLite Metadata
          │                             │
          └──────────────┬──────────────┘
                         ▼
            Python Monitoring Dashboard
```

---

# Project Structure

```text
WAL Storage Engine
│
├── include/
│   ├── config/
│   ├── metadata/
│   ├── serializer/
│   ├── utils/
│   └── wal/
│
├── src/
│   ├── config/
│   ├── metadata/
│   ├── utils/
│   ├── wal/
│   └── main.cpp
│
├── dashboard/
│   └── app.py
│
├── database/
│   └── metadata.db
│
├── logs/
│   ├── wal.log
│   └── system.log
│
├── config.ini
│
└── README.md
```

---

# Build

### Compile (Windows - MSYS2 / MinGW)

```powershell
g++ -std=c++23 main.cpp -lsqlite3 -o engine.exe
```

---

# Run

### Run the Storage Engine

```powershell
.\engine.exe
```

### Recover from WAL

```powershell
.\engine.exe --recover
```

### Launch Dashboard

```powershell
streamlit run app.py
```

---

# Dashboard

The monitoring dashboard displays:

* Total Transactions
* Total Processed Volume
* WAL File Size
* Indexed Records
* Recovery Status
* Storage Analytics

---

# Applications

This project demonstrates concepts used in:

* Storage Engines
* Database Systems
* Write-Ahead Logging (WAL)
* Crash Recovery
* Fault-Tolerant Systems
* Backend Infrastructure
* Financial Trading Infrastructure
* System Design

---

# Future Improvements

* Snapshot-Based Recovery
* Log Rotation
* Performance Benchmarking
* Configurable Storage Parameters
* Advanced Dashboard Analytics
* Comprehensive Unit Testing

---

# Technologies Used

* C++23
* SQLite
* Python
* Streamlit
* STL
* Multithreading (`std::thread`)
* Binary Serialization

---

# Author

Nidhi