#include "utils.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <stack>
#include <string>
#include <vector>

const std::regex expression_regex("\\d+|[+*()]");

std::vector<std::vector<std::string>> readInput() {
    std::vector<std::vector<std::string>> expressions;
    std::ifstream ifs{"inputs/day18.txt"};
    std::string line;
    std::smatch expression_match;
    while (std::getline(ifs, line)) {
        std::vector<std::string> expression;
        while (std::regex_search(line, expression_match, expression_regex)) {
            expression.push_back(expression_match.str());
            line = expression_match.suffix();
        };
        expressions.push_back(expression);
    }
    return expressions;
}

std::vector<std::string> convertToRPN(std::vector<std::string> expression,
                                      bool operatorPrecedence) {
    std::stack<std::string> operator_stack;
    std::vector<std::string> output;

    for (auto& s : expression) {
        if (s == "+" || s == "*") {
            while (operator_stack.size() != 0) {
                auto nextOp = operator_stack.top();
                if (nextOp == "(")
                    break;
                // + has precedence over * so evaluate that first
                if (operatorPrecedence && s == "+" && nextOp == "*")
                    break;
                operator_stack.pop();
                output.push_back(nextOp);
            }
            operator_stack.push(s);
        } else if (s == "(") {
            operator_stack.push(s);
        } else if (s == ")") {
            std::string nextOp;
            while (nextOp = operator_stack.top(), nextOp != "(") {
                output.push_back(nextOp);
                operator_stack.pop();
            }
            // Pop the ( from the stack
            operator_stack.pop();
        } else { // Number
            output.push_back(s);
        }
    }
    while (operator_stack.size() != 0) {
        output.push_back(operator_stack.top());
        operator_stack.pop();
    }
    return output;
}

long evaluateExpression(std::vector<std::string> expression) {
    std::stack<long> working_stack;
    for (auto& s : expression) {
        if (s == "+" || s == "*") {
            auto lhs = working_stack.top();
            working_stack.pop();
            auto rhs = working_stack.top();
            working_stack.pop();
            if (s == "+")
                working_stack.push(lhs + rhs);
            else
                working_stack.push(lhs * rhs);
        } else {
            working_stack.push(std::stol(s));
        }
    }
    return working_stack.top();
}

long partOne(const std::vector<std::vector<std::string>>& expressions) {
    return std::accumulate(
        expressions.begin(), expressions.end(), 0L,
        [](long previous, std::vector<std::string> expression) {
            return previous
                   + evaluateExpression(convertToRPN(expression, false));
        });
}

long partTwo(const std::vector<std::vector<std::string>>& expressions) {
    return std::accumulate(
        expressions.begin(), expressions.end(), 0L,
        [](long previous, std::vector<std::string> expression) {
            return previous
                   + evaluateExpression(convertToRPN(expression, true));
        });
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
