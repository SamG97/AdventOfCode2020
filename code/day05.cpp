#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> readInput() {
    std::vector<std::string> input;
    std::ifstream ifs{"inputs/day05.txt"};
    std::string line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

int partition(int low, int high, std::string seq) {
    if (low == high)
        return low;

    int mid = (low + high) / 2;
    if (seq[0] == 'F' || seq[0] == 'L')
        return partition(low, mid, seq.substr(1, seq.size() - 1));
    else
        return partition(mid + 1, high, seq.substr(1, seq.size() - 1));
}

std::pair<int, int> findSeat(std::string seq) {
    int row = partition(0, 127, seq.substr(0, 7));
    int col = partition(0, 7, seq.substr(7, 3));
    return {row, col};
}

int partOne(const std::vector<std::string>& reservations) {
    int maxID = 0;
    for (auto& reservation : reservations) {
        auto seat = findSeat(reservation);
        int id = seat.first * 8 + seat.second;
        if (id > maxID)
            maxID = id;
    }
    return maxID;
};

int partTwo(const std::vector<std::string>& reservations) {
    // Store each row as an 8-bit int with the ith bit representing the ith
    // column of the row
    std::array<unsigned char, 128> taken{0};
    int minID = 128 * 8;
    for (auto& reservation : reservations) {
        auto seat = findSeat(reservation);
        taken[seat.first] |= (1 << seat.second);
        int id = seat.first * 8 + seat.second;
        if (id < minID)
            minID = id;
    }
    
    int startRow = (minID - 1) / 8 + 1;  // Start on first full row
    for (int row = startRow; row < taken.size(); ++row) {
        if (taken[row] == 0xFF) {
            continue;
        }
        unsigned char bitMap = taken[row];
        int column = 0;
        while (bitMap & 0b1) {
            ++column;
            bitMap >>= 1;
        }
        return row * 8 + column;
    }
    return -1; // Could not find seat
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
