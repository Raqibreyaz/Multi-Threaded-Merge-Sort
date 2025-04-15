#include "thread-pool.hpp"

ThreadPool::ThreadPool()
{
    std::cout << "no of threads: " << THREAD_COUNT << std::endl;

    // add worker threads
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        this->workers.emplace_back([this]()
                                   { this->workerLoop(); });
    }

    this->stop = false;
}

// will add the task to the task queue
void ThreadPool::addTask(const std::function<void()> &task)
{
    // lock to avoid race condition
    std::lock_guard<std::mutex> lock(this->mtx);

    // add the task to the task queue
    this->taskQueue.push(std::move(task));

    // notify one waiting thread to do this task
    this->cond.notify_one();
}

// will return the first task
std::function<void()> ThreadPool::popTask()
{
    std::lock_guard<std::mutex> lock(this->mtx);

    if (this->taskQueue.empty())
        return nullptr;

    // lock to avoid race condition

    // take the front task
    auto task = std::move(this->taskQueue.front());

    // remove the task from task queue
    this->taskQueue.pop();

    return task;
}

// this function is a threads whole life cycle from wait to execution
void ThreadPool::workerLoop()
{
    while (1)
    {
        std::function<void()> task = nullptr;
        {
            // lock the mutex, will be auto freed
            std::unique_lock<std::mutex> lock(this->mtx);

            // waiting for task
            this->cond.wait(lock, [this]()
                            { return !taskQueue.empty() || this->stop; });

            // if signalled to stop and no tasks avaialble then terminate thread
            if (this->stop && this->taskQueue.empty())
            {
                // std::cout << "terminating thread" << std::endl;
                break;
            }

            // we dont need lock anymore
            lock.unlock();

            // take the front task
            task = this->popTask();
        }
        // execute the task
        if (task)
        {
            task();
        }
    }
}
ThreadPool::~ThreadPool()
{
    // std::cout << "thread pool being destroyed" << std::endl;
    {
        std::unique_lock<std::mutex> lock(this->mtx);
        this->stop = true;
    }

    // wake up all threads to exit
    this->cond.notify_all();

    for (auto &thread : this->workers)
    {
        if (thread.joinable())
        {
            thread.join();
            // std::cout << "thread joined" << std::endl;
        }
    }
}

size_t ThreadPool::getTotalTasks()
{
    return this->taskQueue.size();
}