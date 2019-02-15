#include "BST.hpp"
#include "Quicksort.hpp"
#include "Hashing.hpp"

#include <lkCommon/Utils/Logger.hpp>

#include <algorithm>
#include <numeric>
#include <random>


int main()
{

    // quicksort
    std::vector<uint32_t> arr(20);
    std::iota(arr.begin(), arr.end(), 0);

    std::shuffle(arr.begin(), arr.end(), std::mt19937{std::random_device{}()});

    for (uint32_t i = 0; i < 20; ++i)
        LOGI(arr[i]);

    LOGI(" ");

    Quicksort(arr, 0, static_cast<int>(arr.size() - 1));

    for (uint32_t i = 0; i < 20; ++i)
        LOGI(arr[i]);

    std::string a("abcdef");
    std::string b("gbcdef");

    LOGI(a << ": " << Hash(a));
    LOGI(b << ": " << Hash(b));

    {
        BST tree;

        tree.Add(10, "ten");
        tree.Add(2 , "two");
        tree.Add(5 , "five");
        tree.Add(4 , "four");
        tree.Add(19, "nineteen");
        tree.Add(13, "thirteen");
        tree.Add(22, "twenty two");
        tree.Add(1 , "one");
        tree.Add(98, "ninety eight");
        tree.Add(0 , "zero");

        tree.Print();

        LOGI("");
        LOGI("SEARCHING TEST");
        LOGI("Finding 10: " << tree.Find(10));
        LOGI("Finding 22: " << tree.Find(22));
        LOGI("Finding 98: " << tree.Find(98));
        LOGI("Finding 3: " << tree.Find(3));
    }
}
