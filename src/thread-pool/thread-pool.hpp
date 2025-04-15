//                  ┌──────────────┐
//                  │ Task Queue   │◄────┐
//                  └────┬─────────┘     │
//                       │               │
//               ┌───────▼────────┐      │
//               │ Worker Thread 1│      │
//               │ - Wait         │      │
//               │ - Pop Task     │      │
//               │ - Run Task     │      │
//               └───────▲────────┘      │
//               ┌───────▼────────┐      │
//               │ Worker Thread N│      │
//               └────────▲───────┘      │
//                       │               │
//               ┌───────┴──────┐        │
//               │ submit(task) ├────────┘
//               └──────────────┘

// pool.submit([=](){ mergeSort(left); });
// pool.submit([=](){ mergeSort(right); });

#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#define THREAD_COUNT 4

// a queue to add tasks
// a mutex for locking the queue

class ThreadPool
{
private:
    std::queue<std::function<void()>> taskQueue;
    std::vector<std::thread> workers;

    // these mutex and var are for modifying the task queue
    std::mutex mtx;
    std::condition_variable cond;
    bool stop;
    void workerLoop();

public:
    ThreadPool();
    ~ThreadPool();
    void addTask(const std::function<void()> &task);
    std::function<void()> popTask();
    size_t getTotalTasks();
};

#endif