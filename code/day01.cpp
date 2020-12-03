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
int sumSearch(Iterator begin, Iterator end, int target) {
    auto i = begin;
    auto j = end;
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

int partOne(const std::vector<int> input) {
    return sumSearch(input.begin(), input.end(), 2020);
}

int partTwo(const std::vector<int> input) {
    for (auto i = input.begin(); i < input.end(); ++i) {
        int prod = sumSearch(i, input.end(), 2020 - *i);
        if (prod != -1) {
            return *i * prod;
        }
    }
    return -1; // Hopefully shouldn't trigger
}

int main() {
    auto input = readInput();
    std::sort(input.begin(), input.end()); // Solution assumes sorted input
    std::cout << partOne(input) << "\n";
    std::cout << partTwo(input) << "\n";
    return 0;
}
