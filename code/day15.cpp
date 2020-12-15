#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> readInput() {
    std::vector<int> input;
    std::ifstream ifs{"inputs/day15.txt"};
    std::string num;
    while (std::getline(ifs, num, ',')) {
        input.push_back(std::stoi(num));
    }
    return input;
}

int getCount(const std::vector<int>& nums, size_t target) {
    // The largest key is only a few million so we can use a vector as a map
    // instead of unordered_map to make our solution faster as we don't have
    // to hash the key
    // We know that at most two elements will be `target` apart (since we stop
    // adding new elements to the sequence after this) so we can just
    // pre-allocate this many elements at the start
    // We use -1 to represent a 'hasn't occurred yet' sequence element
    std::vector<int> history(target, -1);

    // Initialise with the given numbers
    for (size_t i = 0; i + 1 < nums.size(); ++i) {
        history[nums[i]] = i;
    }

    // Last initial number is not inserted and is treated as the previous
    // value for the first iteration of the main loop and will be inserted
    // there
    size_t lastSaid = nums[nums.size() - 1];
    for (size_t t = nums.size(); t < target; ++t) {
        if (history[lastSaid] == -1) {
            history[lastSaid] = t - 1;
            lastSaid = 0;
        } else {
            int gap = t - 1 - history[lastSaid];
            history[lastSaid] = t - 1;
            lastSaid = gap;
        }
    }
    return lastSaid;
}

int partOne(const std::vector<int>& nums) {
    return getCount(nums, 2020);
}

int partTwo(const std::vector<int>& nums) {
    return getCount(nums, 30'000'000);
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
