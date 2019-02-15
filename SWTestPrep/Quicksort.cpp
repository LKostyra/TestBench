#include "Quicksort.hpp"


uint32_t QuicksortPartition(std::vector<uint32_t>& arr, int lo, int hi)
{
    uint32_t pivot = arr[(lo + hi) / 2];

    int i = lo - 1;
    int j = hi + 1;

    while (true)
    {
        do
            i++;
        while (arr[i] < pivot);

        do
            j--;
        while (arr[j] > pivot);

        if (i >= j)
            return j;

        uint32_t temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void Quicksort(std::vector<uint32_t>& arr, int i, int j)
{
    if (i >= j)
        return;

    uint32_t pivot = QuicksortPartition(arr, i, j);
    Quicksort(arr, i, pivot);
    Quicksort(arr, pivot + 1, j);
}
