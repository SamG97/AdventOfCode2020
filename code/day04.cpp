#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Passport {
    bool hasRequiredFields() {
        // cid is not required
        return byr && iyr && eyr && hgt != "" && hcl != "" && ecl != ""
               && pid != "";
    }

    bool isValid() {
        if (!hasRequiredFields())
            return false;
        if (byr < 1920 || byr > 2002)
            return false;
        if (iyr < 2010 || iyr > 2020)
            return false;
        if (eyr < 2020 || eyr > 2030)
            return false;

        auto height_unit = hgt.substr(hgt.size() - 2, 2);
        if (height_unit != "cm" && height_unit != "in")
            return false;
        int height_size = std::stoi(hgt.substr(0, hgt.size() - 2));
        if (height_unit == "cm" && (height_size < 150 || height_size > 193))
            return false;
        if (height_unit == "in" && (height_size < 59 || height_size > 76))
            return false;

        if (hcl[0] != '#')
            return false;
        for (auto c : hcl.substr(1, hcl.size() - 1)) {
            if (!isxdigit(c))
                return false;
        }

        if (!(ecl == "amb" || ecl == "blu" || ecl == "brn" || ecl == "gry"
              || ecl == "grn" || ecl == "hzl" || ecl == "oth"))
            return false;

        if (pid.size() != 9 || std::all_of(pid.begin(), pid.end(), isdigit))
            return false;

        return true;
    }

    int byr = 0;
    int iyr = 0;
    int eyr = 0;
    std::string hgt = "";
    std::string hcl = "";
    std::string ecl = "";
    std::string pid = "";
    int cid = 0;
};

std::vector<Passport> readInput() {
    std::vector<Passport> input;
    std::ifstream ifs{"inputs/day04.txt"};

    std::string line;
    Passport currentEntry;
    while (std::getline(ifs, line)) {
        if (line.empty()) {
            input.push_back(currentEntry);
            currentEntry = Passport{};
        }
        for (auto& field : split(line, ' ')) {
            auto parts = split(field, ':');
            auto key = parts[0];
            if (key == "byr")
                currentEntry.byr = std::stoi(parts[1]);
            else if (key == "iyr")
                currentEntry.iyr = std::stoi(parts[1]);
            else if (key == "eyr")
                currentEntry.eyr = std::stoi(parts[1]);
            else if (key == "hgt")
                currentEntry.hgt = parts[1];
            else if (key == "hcl")
                currentEntry.hcl = parts[1];
            else if (key == "ecl")
                currentEntry.ecl = parts[1];
            else if (key == "pid")
                currentEntry.pid = parts[1];
            else if (key == "cid")
                currentEntry.cid = std::stoi(parts[1]);
        }
    }
    input.push_back(currentEntry);

    return input;
}

int partOne(const std::vector<Passport>& passports) {
    int num_valid = 0;
    for (auto p : passports) {
        if (p.hasRequiredFields()) {
            ++num_valid;
        }
    }
    return num_valid;
}

int partTwo(const std::vector<Passport>& passports) {
    int num_valid = 0;
    for (auto p : passports) {
        if (p.isValid()) {
            ++num_valid;
        }
    }
    return num_valid;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
