#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::regex pieces_regex("([a-z ]+) bags contain ([a-z\\d ,]+)");
const std::regex bag_regex("([\\d]+) ([a-z][a-z ]+(?= bags?))");

struct Bag {
    Bag(std::string _colour, int _number) : colour(_colour), number(_number){};
    std::string colour;
    int number;
};
using BagRules = std::unordered_map<std::string, std::vector<Bag>>;

BagRules readInput() {
    BagRules input;
    std::ifstream ifs{"inputs/day07.txt"};

    std::string line;
    std::smatch pieces_match;
    std::smatch bag_match;
    while (std::getline(ifs, line)) {
        std::regex_search(line, pieces_match, pieces_regex);

        std::string bags = pieces_match[2].str();
        std::vector<Bag> requirements;
        while (std::regex_search(bags, bag_match, bag_regex)) {
            requirements.emplace_back(bag_match[2].str(),
                                      std::stoi(bag_match[1]));
            bags = bag_match.suffix();
        };
        input.insert({pieces_match[1].str(), requirements});
    }

    return input;
}

int partOne(const BagRules& bagRules) {
    std::unordered_set<std::string> allFound{};
    std::unordered_set<std::string> frontier{"shiny gold"};
    while (frontier.size() != 0) {
        std::unordered_set<std::string> newFrontier{};
        for (auto& rule : bagRules) {
            for (auto& bag : rule.second) {
                if (frontier.count(bag.colour)) {
                    if (!allFound.count(rule.first)) {
                        newFrontier.insert(rule.first);
                        allFound.insert(rule.first);
                        break;
                    }
                }
            }
        }
        frontier = newFrontier;
    }
    return allFound.size();
}

int findBags(const BagRules& bagRules,
             std::unordered_map<std::string, int>& memory,
             const std::string& target) {
    auto requirements = bagRules.find(target)->second;

    if (requirements.size() == 0) {
        memory.insert({target, 1});
        return 1;
    }

    auto search = memory.find(target);
    if (search == memory.end()) {
        int requiredBags = 1;
        for (auto& bag : requirements) {
            requiredBags +=
                bag.number * findBags(bagRules, memory, bag.colour);
        }
        memory.insert({target, requiredBags});
        return requiredBags;
    } else {
        return search->second;
    }
}

int partTwo(const BagRules& bagRules) {
    std::unordered_map<std::string, int> alreadyFound;
    return findBags(bagRules, alreadyFound, "shiny gold") - 1;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
