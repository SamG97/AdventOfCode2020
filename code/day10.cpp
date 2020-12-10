#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<int> readInput() {
    std::vector<int> input;
    std::ifstream ifs{"inputs/day10.txt"};
    int line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

int partOne(const std::vector<int>& adaptors) {
    std::vector<int> sortedAdaptors{adaptors};
    sortedAdaptors.push_back(0);  // Initial jolts
    std::sort(sortedAdaptors.begin(), sortedAdaptors.end());

    int oneApart = 0;
    int threeApart = 1;  // Final jump is guaranteed to be 3 to our device
    for (int i = 1; i < sortedAdaptors.size(); ++i) {
        int diff = sortedAdaptors[i] - sortedAdaptors[i - 1];
        if (diff == 1)
            ++oneApart;
        else if (diff == 3)
            ++threeApart;
    }
    return oneApart * threeApart;
}

long partTwo(const std::vector<int>& adaptors) {
    std::vector<int> sortedAdaptors{adaptors};
    std::sort(sortedAdaptors.begin(), sortedAdaptors.end());

    int max = sortedAdaptors[sortedAdaptors.size() - 1];
    std::vector<long> combinations(max + 1, 0L);
    combinations[0] = 1L;  // We can always start with no adaptors

    // We can get to an adaptor's value based on the sum of the number of ways
    // that we could have gotten to any of the previous 3 values since we can
    // use our new adaptor to extend any of these sequences
    for (auto& adaptor : sortedAdaptors) {
        combinations[adaptor] += combinations[adaptor - 1];
        if (adaptor >= 2)
            combinations[adaptor] += combinations[adaptor - 2];
        if (adaptor >= 3)
            combinations[adaptor] += combinations[adaptor - 3];
    }
    // No need to include final jump since there's only one way to do this
    // from the maximum jolt value (since it's always 3 more than this)
    return combinations[max];
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
