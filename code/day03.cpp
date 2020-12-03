#include <algorithm>
#include <execution>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

std::vector<std::string> readInput() {
    std::vector<std::string> input;
    std::ifstream ifs{"inputs/day03.txt"};
    std::string line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

int findTrees(const std::vector<std::string>& map, int downSteps,
              int rightSteps) {
    int row = 0;
    int column = 0;
    int rowLength = map[0].size();
    int numRows = map.size();

    int numTrees = 0;
    while (row < numRows) {
        if (map[row][column] == '#') {
            ++numTrees;
        }
        row += downSteps;
        column = (column + rightSteps) % rowLength;
    }
    return numTrees;
}

int partOne(const std::vector<std::string>& input) {
    return findTrees(input, 1, 3);
}

long partTwo(const std::vector<std::string>& input) {
    std::vector<std::pair<int, int>> options{
        {1, 1}, {1, 3}, {1, 5}, {1, 7}, {2, 1}};
    std::vector<long> results;
    // Transform won't update size properly with parallel execution so we need
    // to do it manually here
    results.resize(options.size());

    std::transform(std::execution::par_unseq, options.begin(), options.end(),
                   results.begin(), [input](std::pair<int, int>& option) {
                       return findTrees(input, option.first, option.second);
                   });
    return std::reduce(results.begin(), results.end(), 1L,
                       std::multiplies<>());
}

int main() {
    auto input = readInput();
    std::cout << partOne(input) << "\n";
    std::cout << partTwo(input) << "\n";
    return 0;
}
