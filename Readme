# Multithreaded Merge Sort in C++

This project implements a multithreaded version of the merge sort algorithm using a custom thread pool to efficiently manage task execution. The goal of the project was to optimize the sorting process for large datasets using parallelism, thus enhancing performance compared to the standard single-threaded merge sort.

## Features

- **Multithreaded Execution**: The merge sort is parallelized to take advantage of multiple cores in modern CPUs, significantly improving performance for large datasets.
- **Custom Thread Pool**: A custom thread pool is implemented to manage thread execution and task scheduling. This ensures efficient resource utilization and minimizes overhead.
- **Low-Level Programming**: Built in C++ to gain a deeper understanding of threading, synchronization, and performance optimization at the low level.

## Performance

- **Dataset Size**: 1,000,000 (1M) numbers.
- **Execution Time**:
  - **Multithreaded Merge Sort**: ~0.19 seconds.
  - **Simple Merge Sort (Single-threaded)**: ~1.1 seconds.

The multithreaded version demonstrates a significant performance boost due to parallelization, reducing the time complexity from linear in the case of single-threaded sorting to near-linear for multi-threading under optimal conditions.

## Design & Implementation

### Multithreaded Merge Sort

The algorithm splits the dataset into smaller chunks, sorts them concurrently using multiple threads, and then merges them in parallel. The number of threads is dynamically adjusted based on the system’s capabilities to ensure efficient use of resources.

### Custom Thread Pool

The custom thread pool was implemented to manage the threads efficiently. It avoids the overhead of creating and destroying threads for each task by reusing threads for multiple tasks. This reduces the overhead associated with thread creation and destruction and ensures that resources are utilized optimally.

### Synchronization

Threads are synchronized using mutexes and condition variables to prevent race conditions and ensure thread-safe operations during sorting and merging processes.

### Low-Level Optimizations

- Memory allocations are minimized to reduce overhead.
- The merging process is designed to reduce unnecessary memory copies and improve cache locality, which is critical for performance in large datasets.

## How to Run

### Prerequisites

- CMake to build the project (optional but recommended for easy configuration).

### Building the Project

1. Clone the repository:

   ```bash
   git clone https://github.com/raqibreyaz/Multi-Threaded-Merge-Sort.git
   cd Multi-Threaded-Merge-Sort
   ```

2. Build the project using CMake:

   ```bash
   make or make all
   ```

3. Run the program:
   ```bash
   ./app
   ```
