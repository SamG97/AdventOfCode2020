#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

template <int n>
class XMASCode {
public:
    void update(int i) {
        buffer[head++] = i;
        if (head == n) {
            head = 0;
            inPreamble = false;
        }
    }

    bool isValid(int target) {
        if (inPreamble)
            return true; // Preamble is always valid

        for (int i = 0; i < n; ++i) {
            int required = target - buffer[i];
            for (int j = 0; j < n; ++j) {
                if (i != j && buffer[j] == required)
                    return true;
            }
        }
        return false;
    }

private:
    std::array<int, n> buffer{0};
    int head = 0;
    bool inPreamble = true;
};

std::vector<int> readInput() {
    std::vector<int> input;
    std::ifstream ifs{"inputs/day09.txt"};
    int line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

int partOne(const std::vector<int>& code) {
    XMASCode<25> xmas{};
    for (auto& i : code) {
        if (!xmas.isValid(i))
            return i;
        xmas.update(i);
    }
    return -1;
}

int partTwo(const std::vector<int>& code) {
    int target = partOne(code);
    int i = 0;
    int j = 1;
    int runningTotal = code[i] + code[j];
    while (i < static_cast<int>(code.size())
           and j < static_cast<int>(code.size())) {
        if (runningTotal == target) {
            int min = INT32_MAX;
            int max = 0;
            for (int k = i; k <= j; ++k) {
                if (code[k] < min)
                    min = code[k];
                if (code[k] > max)
                    max = code[k];
            }
            return min + max;
        }

        if (runningTotal > target) {
            runningTotal -= code[i++]; // Current window too big
        } else {
            runningTotal += code[++j]; // Current window too small
        }
    }
    return -1;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
