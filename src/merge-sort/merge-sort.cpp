#include "merge-sort.hpp"

void merge(std::vector<int> &arr, int low, int mid, int high)
{
    std::vector<int> temp;
    int i = low, j = mid + 1;

    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }

    while (i <= mid)
        temp.push_back(arr[i++]);
    while (j <= high)
        temp.push_back(arr[j++]);

    for (int k = low, t = 0; k <= high; ++k, ++t)
    {
        arr[k] = temp[t];
    }
}

void mergeSort(std::vector<int> &arr, int low, int high)
{

    if (low >= high)
        return;

    int mid = low + (high - low) / 2;

    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);

    merge(arr, low, mid, high);
}

// will merge two halves of array
void merge(const std::shared_ptr<std::vector<int>> arr, int low, int mid, int high)
{
    std::vector<int> temp;

    int i = low, j = mid + 1;

    // take the smallest one from either left or right side
    while (i <= mid && j <= high)
    {
        if ((*arr)[i] <= (*arr)[j])
        {
            temp.push_back((*arr)[i]);
            i++;
        }
        else
        {
            temp.push_back((*arr)[j]);
            j++;
        }
    }

    // take the left elements as it is from left side
    while (i <= mid)
    {
        temp.push_back((*arr)[i]);
        i++;
    }

    // take the left elements as it is from right side
    while (j <= high)
    {
        temp.push_back((*arr)[j]);
        j++;
    }

    // put back the merged part
    for (i = low, j = 0; i <= high; i++, j++)
    {
        (*arr)[i] = temp[j];
    }
}

// will sort the array
void mergeSort(
    const std::shared_ptr<ThreadPool> pool,
    const std::shared_ptr<std ::vector<int>> arr,
    int low,
    int high,
    std::shared_ptr<std::atomic<int>> parentCounter,
    const std::function<void()> &merger)
{
    // there should be at least 2 elements to sort
    if ((high - low + 1) <= 5000)
    {
        std::sort((*arr).begin() + low, (*arr).begin() + high + 1);

        // decrement the counter for merging, and merge if possible
        if (parentCounter->fetch_sub(1) == 1)
            merger();
        return;
    }

    int mid = low + (high - low) / 2;

    // a counter to keep track of the merge state
    std::shared_ptr<std::atomic<int>> cntr = std::make_shared<std::atomic<int>>(2);

    // when both left and right are done , child will merge them
    // when parents both side are done then we will merge them
    auto whenBothDone = [=]()
    {
        // for our left and right
        merge(arr, low, mid, high);

        // for parents left and right
        if (parentCounter->fetch_sub(1) == 1)
        {
            merger();
        }
    };

    // sort the left part and merge both the arrays
    pool->addTask([=]()
                  { mergeSort(
                        pool,
                        arr,
                        low,
                        mid,
                        cntr,
                        whenBothDone); });

    //    sort the right part and merge both the arrays
    pool->addTask([=]()
                  { mergeSort(
                        pool,
                        arr,
                        mid + 1,
                        high,
                        cntr,
                        whenBothDone); });
}
