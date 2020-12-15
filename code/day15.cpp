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

void updateHistory(std::vector<int>& history, size_t key, size_t val) {
    if (key >= history.size())
        history.resize(key + 1, -1);
    history[key] = val;
}

int getCount(const std::vector<int>& nums, size_t target) {
    // The largest key is only a few million so we can use a vector as a map
    // instead of unordered_map to make our solution faster as we don't have
    // to hash the key
    std::vector<int> history;
    size_t lastSaid = 0;
    for (size_t t = 0; t < target; ++t) {
        if (t < nums.size()) {
            lastSaid = nums[t];
            if (t > 0)
                updateHistory(history, nums[t - 1], t - 1);
            continue;
        }

        if (lastSaid >= history.size() || history[lastSaid] == -1) {
            updateHistory(history, lastSaid, t - 1);
            lastSaid = 0;
        } else {
            int gap = t - 1 - history[lastSaid];
            updateHistory(history, lastSaid, t - 1);
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
