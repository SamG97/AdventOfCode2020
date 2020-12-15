#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
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
    std::unordered_map<int, int> history;
    int lastSaid = 0;
    for (size_t t = 0; t < target; ++t) {
        if (t < nums.size()) {
            lastSaid = nums[t];
            if (t > 0)
                history.insert({nums[t - 1], t - 1});
            continue;
        }

        auto match = history.find(lastSaid);
        if (match == history.end()) {
            history.insert({lastSaid, t - 1});
            lastSaid = 0;
        } else {
            lastSaid = t - 1 - match->second;
            match->second = t - 1;
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
    std::cout << timeit(partOne)(input) << std::endl;
    std::cout << timeit(partTwo)(input) << std::endl;
    return 0;
}
