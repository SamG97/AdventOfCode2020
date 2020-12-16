#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

const std::regex field_regex("[a-z ]+(?=:)");
const std::regex range_regex("[\\d-]+");

struct Condition {
    Condition() = default;
    Condition(std::string _name) : name(_name){};
    std::string name;
    std::vector<std::pair<int, int>> ranges;
    bool operator==(Condition other) const { return name == other.name; }
};

using InputType =
    std::pair<std::vector<Condition>, std::vector<std::vector<int>>>;

InputType readInput() {
    std::vector<Condition> conditions;
    std::ifstream ifs{"inputs/day16.txt"};
    std::string line;
    std::smatch field_match;
    std::smatch range_match;
    while (std::getline(ifs, line), !line.empty()) {
        std::regex_search(line, field_match, field_regex);
        Condition condition{field_match[0].str()};
        line = field_match.suffix();
        while (std::regex_search(line, range_match, range_regex)) {
            auto range = split(range_match.str(), '-');
            condition.ranges.emplace_back(std::stoi(range[0]),
                                          std::stoi(range[1]));
            line = range_match.suffix();
        };
        conditions.push_back(condition);
    }

    std::vector<std::vector<int>> tickets;
    while (ifs >> line) {
        auto fields = split(line, ',');
        if (fields.size() > 1) {
            std::vector<int> field_values;
            std::transform(fields.begin(), fields.end(),
                           std::back_inserter(field_values),
                           [](std::string s) { return std::stoi(s); });
            tickets.push_back(field_values);
        }
    }

    return {conditions, tickets};
}

bool validCondition(int value, const Condition& condition) {
    for (auto& range : condition.ranges) {
        if (value >= range.first && value <= range.second) {
            return true;
        }
    }
    return false;
}

bool validField(int value, const std::vector<Condition>& conditions) {
    for (auto& condition : conditions) {
        if (validCondition(value, condition))
            return true;
    }
    return false;
}

int partOne(const InputType& input) {
    auto [conditions, tickets] = input;
    int errorRate = 0;
    for (auto it = tickets.begin() + 1; it != tickets.end(); ++it) {
        for (auto field : *it) {
            if (!validField(field, conditions))
                errorRate += field;
        }
    }
    return errorRate;
}

long partTwo(const InputType& input) {
    auto [conditions, tickets] = input;

    // First filter out invalid tickets
    std::vector<std::vector<int>> validTickets;
    for (auto it = tickets.begin() + 1; it != tickets.end(); ++it) {
        bool valid = true;
        for (auto field : *it) {
            if (!validField(field, conditions)) {
                valid = false;
                break;
            }
        }
        if (valid)
            validTickets.push_back(*it);
    }

    // Work out which columns can take which fields
    std::vector<std::vector<Condition>> allocations;
    for (size_t i = 0; i < conditions.size(); ++i) {
        std::vector<Condition> possibleConditions{conditions};
        for (auto& ticket : validTickets) {
            std::vector<Condition> stillValidConditions;
            for (auto& condition : possibleConditions) {
                if (validCondition(ticket[i], condition))
                    stillValidConditions.push_back(condition);
            }
            possibleConditions = stillValidConditions;
        }
        allocations.push_back(possibleConditions);
    }

    // Work out the which single field can be applied to each column
    // Problem is set up so that one column has only one possible field,
    // another could have that field or one else, and so on so it is sufficient
    // to look for the column with only one possible field, fix that assignment
    // and then remove that field as a possibility from all other columns until
    // all columns have an assignment
    std::vector<Condition> finalAllocations(conditions.size());
    int remainingAllocations = conditions.size();
    while (remainingAllocations > 0) {
        for (size_t i = 0; i < conditions.size(); ++i) {
            if (allocations[i].size() == 1) {
                finalAllocations[i] = allocations[i][0];
                --remainingAllocations;
                // Remove final allocation from all columns (including this one
                // so that the if statement won't trigger next time around)
                for (size_t j = 0; j < conditions.size(); ++j)
                    allocations[j].erase(std::remove(allocations[j].begin(),
                                                     allocations[j].end(),
                                                     finalAllocations[i]),
                                         allocations[j].end());
            }
        }
    }

    long result = 1L;
    for (size_t i = 0; i < conditions.size(); ++i) {
        if (finalAllocations[i].name.substr(0, 9) == "departure")
            result *= tickets[0][i];
    }

    return result;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
