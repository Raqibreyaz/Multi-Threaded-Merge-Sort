#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP
#include "../thread-pool/thread-pool.hpp"
#include <memory>
#include <vector>
#include <functional>
#include <algorithm>
#include <atomic>

void merge(std::vector<int> &arr, int low, int mid, int high);

void mergeSort(std::vector<int> &arr, int low, int high);

void merge(const std::shared_ptr<std::vector<int>> arr, int low, int mid, int high);

void mergeSort(const std::shared_ptr<ThreadPool> pool,
               const std::shared_ptr<std ::vector<int>> arr,
               int low,
               int high,
               std::shared_ptr<std::atomic<int>> parentCounter,
               const std::function<void()> &merger);

#endif