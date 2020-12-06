#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static_assert(sizeof(unsigned int) >= 4);
const char offset = 'a';

std::vector<std::vector<std::string>> readInput() {
    std::vector<std::vector<std::string>> input;
    std::ifstream ifs{"inputs/day06.txt"};

    std::string line;
    std::vector<std::string> currentGroup;
    while (std::getline(ifs, line)) {
        if (line.empty()) {
            input.push_back(currentGroup);
            currentGroup = std::vector<std::string>{};
            continue;
        }
        currentGroup.push_back(line);
    }
    input.push_back(currentGroup);

    return input;
}

int countBitsSet(unsigned int n) {
    int count = 0;
    while (n != 0) {
        n &= n - 1;
        ++count;
    }
    return count;
}

int partOne(const std::vector<std::vector<std::string>>& groups) {
    int total = 0;
    for (auto& group : groups) {
        unsigned int result = 0;
        for (auto& passenger : group) {
            for (auto& c : passenger) {
                result |= 0b1 << (c - offset);
            }
        }
        total += countBitsSet(result);
    }
    return total;
}

int partTwo(const std::vector<std::vector<std::string>>& groups) {
    int total = 0;
    for (auto& group : groups) {
        unsigned int result = ~0;
        for (auto& passenger : group) {
            unsigned int passengerBits = 0;
            for (auto& c : passenger) {
                passengerBits |= 0b1 << (c - offset);
            }
            result &= passengerBits;
        }
        total += countBitsSet(result);
    }
    return total;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
