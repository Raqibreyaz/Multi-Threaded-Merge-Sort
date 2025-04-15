#include "thread-pool.hpp"

ThreadPool::ThreadPool()
{
    // add worker threads
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        this->workers.emplace_back([this]()
                                   { this->workerLoop(); });
    }
}

// will add the task to the task queue
void ThreadPool::addTask(const std::function<void()> &task)
{
    // lock to avoid race condition
    std::lock_guard<std::mutex> lock(this->mtx);

    // add the task to the task queue
    this->taskQueue.push(task);

    // notify one waiting thread to do this task
    this->cond.notify_one();
}

// will return the first task
std::function<void()> ThreadPool::popTask()
{
    // lock to avoid race condition
    std::lock_guard<std::mutex> lock(this->mtx);

    // take the front task
    auto task = this->taskQueue.front();

    // remove the task from task queue
    this->taskQueue.pop();

    return task;
}

// this function is a threads whole life cycle from wait to execution
void ThreadPool::workerLoop()
{
    while (1)
    {
        // lock the mutex, will be auto freed
        std::unique_lock<std::mutex> lock(this->mtx);

        // waiting for task
        this->cond.wait(lock, [this]()
                        { return taskQueue.empty(); });

        // we dont need lock anymore
        lock.unlock();

        // take the front task
        auto task = this->popTask();

        // execute the task
        task();
    }
}
ThreadPool::~ThreadPool()
{
    std::cout << "thread pool being destroyed" << std::endl;
    for (auto &thread : this->workers)
    {
        if (thread.joinable())
        {
            thread.join();
            std::cout << "thread joined\n";
        }
    }
}