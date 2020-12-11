#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<std::string> readInput() {
    std::vector<std::string> input;
    std::ifstream ifs{"inputs/day11.txt"};
    std::string line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

bool seatTaken(const std::vector<std::string>& seats, int row, int col) {
    if (row < 0 || row >= seats.size() || col < 0 || col >= seats[0].size())
        return false;
    else
        return seats[row][col] == '#';
}

int partOne(const std::vector<std::string>& seats) {
    std::vector<std::string> _seats{seats};
    std::vector<std::string> newSeats;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int row = 0; row < _seats.size(); ++row) {
            std::string newRow;
            for (int col = 0; col < _seats[row].size(); ++col) {
                char currentState = _seats[row][col];
                if (currentState == '.') {
                    newRow.push_back('.');
                    continue;
                }

                int adjacentSeats = 0;
                for (int di = -1; di < 2; ++di) {
                    for (int dj = -1; dj < 2; ++dj) {
                        if (!(di == 0 && dj == 0)
                            && seatTaken(_seats, row + di, col + dj))
                            ++adjacentSeats;
                    }
                }

                if (currentState == 'L' && adjacentSeats == 0) {
                    changed = true;
                    newRow.push_back('#');
                } else if (currentState == '#' && adjacentSeats >= 4) {
                    changed = true;
                    newRow.push_back('L');
                } else {
                    newRow.push_back(currentState);
                }
            }
            newSeats.push_back(newRow);
        }
        _seats = newSeats;
        newSeats = std::vector<std::string>{};
    }
    int total = 0;
    for (auto& row : _seats) {
        total += std::count(row.begin(), row.end(), '#');
    }
    return total;
}

bool visibleSeat(const std::vector<std::string>& seats, int row, int col,
                 int di, int dj) {
    int i = row + di;
    int j = col + dj;
    while (i >= 0 && i < seats.size() && j >= 0 && j < seats[0].size()) {
        if (seats[i][j] == '#')
            return true;
        else if (seats[i][j] == 'L')
            return false;
        i += di;
        j += dj;
    }
    return false;
}

int partTwo(const std::vector<std::string>& seats) {
    std::vector<std::string> _seats{seats};
    std::vector<std::string> newSeats;
    bool changed = true;
    while (changed) {
        changed = false;
        for (int row = 0; row < _seats.size(); ++row) {
            std::string newRow;
            for (int col = 0; col < _seats[row].size(); ++col) {
                char currentState = _seats[row][col];
                if (currentState == '.') {
                    newRow.push_back('.');
                    continue;
                }

                int adjacentSeats = 0;
                for (int di = -1; di < 2; ++di) {
                    for (int dj = -1; dj < 2; ++dj) {
                        if (!(di == 0 && dj == 0)
                            && visibleSeat(_seats, row, col, di, dj))
                            ++adjacentSeats;
                    }
                }

                if (currentState == 'L' && adjacentSeats == 0) {
                    changed = true;
                    newRow.push_back('#');
                } else if (currentState == '#' && adjacentSeats >= 5) {
                    changed = true;
                    newRow.push_back('L');
                } else {
                    newRow.push_back(currentState);
                }
            }
            newSeats.push_back(newRow);
        }
        _seats = newSeats;
        newSeats = std::vector<std::string>{};
    }
    int total = 0;
    for (auto& row : _seats) {
        total += std::count(row.begin(), row.end(), '#');
    }
    return total;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
