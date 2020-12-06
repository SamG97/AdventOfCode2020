#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

int partOne(std::vector<std::vector<std::string>> groups) {
    int total = 0;
    for (auto& group : groups) {
        std::string result = "";
        for (auto& passenger : group) {
            std::string update;
            std::sort(passenger.begin(), passenger.end());
            std::set_union(result.begin(), result.end(), passenger.begin(),
                           passenger.end(), std::back_inserter(update));
            result = update;
        }
        total += result.size();
    }
    return total;
}

int partTwo(std::vector<std::vector<std::string>> groups) {
    int total = 0;
    for (auto& group : groups) {
        std::string result = "abcdefghijklmnopqrstuvwxyz";
        for (auto& passenger : group) {
            std::string update;
            std::sort(passenger.begin(), passenger.end());
            std::set_intersection(result.begin(), result.end(),
                                  passenger.begin(), passenger.end(),
                                  std::back_inserter(update));
            result = update;
            if (result == "") {
                break;
            }
        }
        total += result.size();
    }
    return total;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
