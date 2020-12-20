#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

size_t MAX_DEPTH = 10;

struct Rule {
    Rule() : base(false){};
    Rule(char c) : base(true), baseChar(c){};
    bool base;
    std::vector<std::vector<int>> subrules;
    char baseChar;
};

using Rules = std::unordered_map<int, Rule>;
using InputType = std::pair<Rules, std::vector<std::string>>;

InputType readInput() {
    std::unordered_map<int, Rule> rules;
    std::ifstream ifs{"inputs/day19.txt"};
    std::string line;
    while (std::getline(ifs, line), line.size() > 1) {
        auto parts = split(line, ' ');
        int key = std::stoi(parts[0].substr(0, parts[0].size() - 1));
        Rule rule;
        if (parts[1][0] == '"') {
            rule = Rule{parts[1][1]};
        } else {
            rule = {};
            std::vector<int> subRule{};
            for (auto it = parts.begin() + 1; it != parts.end(); ++it) {
                if (*it == "|") {
                    rule.subrules.push_back(subRule);
                    subRule = {};
                } else {
                    subRule.push_back(std::stoi(*it));
                }
            }
            rule.subrules.push_back(subRule);
        }
        rules[key] = rule;
    }

    std::vector<std::string> messages{};
    while (ifs >> line)
        messages.push_back(line);
    return {rules, messages};
}

bool parseRule(const Rules& rules, int currentRule, size_t depth,
               const std::string& toParse, std::vector<size_t>& possiblePos) {
    ++depth;
    if (depth >= toParse.size())
        return false; // Don't loop forever
    if (possiblePos.empty())
        return false; // If there are no possible possitions no point checking
                      // more

    Rule ruleToApply = rules.find(currentRule)->second;
    if (ruleToApply.base) {
        std::vector<size_t> newPos;
        for (auto pos : possiblePos)
            if (pos < toParse.size() && toParse[pos] == ruleToApply.baseChar)
                newPos.push_back(pos + 1);
        possiblePos = newPos;
        return !possiblePos.empty();
    } else {
        std::vector<size_t> newPos;
        for (auto& rule : ruleToApply.subrules) {
            std::vector<size_t> rulePos = possiblePos;
            bool valid = true;
            for (auto part : rule) {
                valid &= parseRule(rules, part, depth, toParse, rulePos);
                if (!valid)
                    break;
            }
            if (valid)
                for (auto& pos : rulePos)
                    newPos.push_back(pos);
        }
        if (newPos.empty())
            return false;
        possiblePos = newPos;
        return true;
    }
}

bool validString(const Rules& rules, const std::string& s) {
    std::vector<size_t> offsets{0};
    if (!parseRule(rules, 0, 0, s, offsets))
        return false;
    for (size_t n : offsets) {
        if (n == s.size())
            return true;
    }
    return false;
}

int partOne(const InputType& input) {
    auto [rules, messages] = input;
    int numValid = 0;
    for (auto& message : messages) {
        if (validString(rules, message))
            ++numValid;
    }
    return numValid;
}

int partTwo(const InputType& input) {
    auto [rules, messages] = input;

    // Augment rules for part 2
    Rules updatedRules{rules};
    Rule augmented8{};
    augmented8.subrules = std::vector<std::vector<int>>{{42}, {42, 8}};
    updatedRules[8] = augmented8;

    Rule augmented11{};
    augmented11.subrules =
        std::vector<std::vector<int>>{{42, 31}, {42, 11, 31}};
    updatedRules[11] = augmented11;

    int numValid = 0;
    for (auto& message : messages) {
        if (validString(updatedRules, message))
            ++numValid;
    }
    return numValid;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
