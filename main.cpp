#include "thread-pool.hpp"
#include <iostream>
#include <atomic>
#include <memory>
#define ARRAY_SIZE 10

// will merge two halves of array
void merge(std::vector<int> &arr, int low, int mid, int high)
{
    std::vector<int> temp;

    int i = low, j = mid + 1;

    // take the smallest one from either left or right side
    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
        {
            temp.push_back(arr[i]);
            i++;
        }
        else
        {
            temp.push_back(arr[j]);
            j++;
        }
    }

    // take the left elements as it is from left side
    while (i <= mid)
    {
        temp.push_back(arr[i]);
        i++;
    }

    // take the left elements as it is from right side
    while (j <= high)
    {
        temp.push_back(arr[j]);
        j++;
    }

    // put back the merged part
    for (i = low, j = 0; i <= high; i++, j++)
    {
        arr[i] = temp[j];
    }
}

// will sort the array
void mergeSort(
    ThreadPool &pool,
    std::vector<int> &arr,
    int low,
    int high,
    std::shared_ptr<std::atomic<int>> &counter,
    const std::function<void()> &merger)
{
    // there should be at least 2 elements to sort
    if (high <= low)
        return;

    int mid = low + (high - low) / 2;

    // a counter to keep track of the merge state
    std::shared_ptr<std::atomic<int>> cntr = std::make_shared<std::atomic<int>>(2);

    // sort the left part and merge both the arrays
    pool.addTask([=, &pool, &arr, &cntr]()
                 { mergeSort(
                       pool,
                       arr,
                       low,
                       mid,
                       cntr,
                       [=, &arr]()
                       {
                           merge(arr, low, mid, high);
                       }); });

    //    sort the right part and merge both the arrays
    pool.addTask([=, &pool, &arr, &cntr]()
                 { mergeSort(
                       pool,
                       arr,
                       mid + 1,
                       high,
                       cntr,
                       [=, &arr]()
                       {
                           merge(arr, low, mid, high);
                       }); });

    //  indicating that one side is done
    (*counter)--;

    //  if both sides are done sorting then merge them
    if ((*counter) == 0)
        merger();
}

int main(int argc, char const *argv[])
{
    std::vector<int> arr(ARRAY_SIZE);
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = rand() % ARRAY_SIZE;
    }

    // create a thread pool, will auto create threads
    ThreadPool pool;

    int low = 0, high = arr.size();
    std::shared_ptr<std::atomic<int>> counter = std::make_shared<std::atomic<int>>(1);

    // pool.addTask(std::bind(mergeSort, std::ref(arr), low, high));
    pool.addTask([=, &pool, &arr, &counter]()
                 { mergeSort(pool, arr, low, high, counter, []() {}); });

    return 0;
}
