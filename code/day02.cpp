#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct PasswordEntry {
    PasswordEntry(std::string _password, char _c, int _min, int _max)
        : password(_password), c(_c), min(_min), max(_max){};

    std::string password;
    char c;
    int min;
    int max;
};

std::vector<PasswordEntry> readInput() {
    std::vector<PasswordEntry> input;
    std::ifstream ifs{"inputs/day02.txt"};

    std::string line;
    while (std::getline(ifs, line)) {
        auto parts = split(line, ' ');
        auto range = split(parts[0], '-');
        input.emplace_back(parts[2], parts[1][0], std::stoi(range[0]),
                           std::stoi(range[1]));
    }
    return input;
}

int partOne(const std::vector<PasswordEntry>& passwords) {
    int num_valid = 0;
    for (auto& p : passwords) {
        auto occurrences =
            std::count(p.password.begin(), p.password.end(), p.c);
        if (occurrences >= p.min && occurrences <= p.max) {
            ++num_valid;
        }
    }
    return num_valid;
}

int partTwo(const std::vector<PasswordEntry>& passwords) {
    int num_valid = 0;
    for (auto& p : passwords) {
        bool left = p.password[p.min - 1] == p.c;
        bool right = p.password[p.max - 1] == p.c;
        if (left ^ right) {
            ++num_valid;
        }
    }
    return num_valid;
}

int main() {
    auto input = readInput();
    std::cout << partOne(input) << "\n";
    std::cout << partTwo(input) << "\n";
    return 0;
}
