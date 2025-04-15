#include "src/merge-sort/merge-sort.hpp"
#include <iostream>
#include <atomic>
#include <vector>
#include <memory>
#include <future>

#define ARRAY_SIZE 10000000

int main(int argc, char const *argv[])
{
    std::vector<int> array(ARRAY_SIZE);

    // std::cout << "before sorting" << std::endl;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        int x = rand() % ARRAY_SIZE;
        array[i] = x;
        // std::cout << x << " ";
    }

    auto start = std::chrono::high_resolution_clock::now();

    mergeSort(array, 0, array.size() - 1);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "time for simple merge sort: " << elapsed.count() << " seconds" << std::endl;

    // creating a dynamically allocated array
    std::shared_ptr<std::vector<int>>
        arr = std::make_shared<std::vector<int>>(array);

    // std::cout << std::endl;

    // create a thread pool, will auto create threads
    std::shared_ptr<ThreadPool> pool = std::make_shared<ThreadPool>();

    // a counter for holding the sorted children track
    std::shared_ptr<std::atomic<int>> counter = std::make_shared<std::atomic<int>>(1);

    std::promise<void> donePromise;
    std::future<void> doneFuture = donePromise.get_future();

    int low = 0,
        high = arr->size() - 1;

    try
    {
        start = std::chrono::high_resolution_clock::now();

        // pool.addTask(std::bind(mergeSort, std::ref(arr), low, high));
        pool->addTask([=, &donePromise]() mutable
                      { mergeSort(pool, arr, low, high, counter, [&donePromise]()
                                  {
                        // std::cout<<"done sorting"<<std::endl;
                        donePromise.set_value(); }); });

        // wait for all threads to finish
        doneFuture.get();

        end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;

        std::cout << "time for multi threaded merge sort: " << elapsed.count() << " seconds" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // std::cout << "after sorting" << std::endl;
    // for (auto &x : (*arr))
    // {
    //     std::cout << x << " ";
    // }
    // std::cout << std::endl;
    return 0;
}
