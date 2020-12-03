#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> readInput() {
    std::vector<int> input;
    std::ifstream ifs{"inputs/day01.txt"};
    int line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

template <typename Iterator>
int sumSearch(const Iterator begin, const Iterator end, int target) {
    auto i = begin;
    // Decrement end pointer so that it points to the last element rather than
    // the one-past-last
    auto j = end - 1;
    while (i != j) {
        int sum = *i + *j;
        if (sum == target) {
            return *i * *j;
        } else if (sum < target) {
            ++i; // Make sum bigger
        } else { // sum > target
            --j; // Make sum smaller
        }
    }
    return -1; // Could not find a solution
}

int partOne(const std::vector<int>& input) {
    return sumSearch(input.begin(), input.end(), 2020);
}

int partTwo(const std::vector<int>& input) {
    for (auto i = input.begin(); i != input.end(); ++i) {
        // Start search from next element to avoid checking ourselves
        int prod = sumSearch(i + 1, input.end(), 2020 - *i);
        if (prod != -1) {
            return *i * prod;
        }
    }
    return -1; // Hopefully shouldn't trigger
}

int main() {
    auto input = readInput();
    std::sort(input.begin(), input.end()); // Solution assumes sorted input
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
