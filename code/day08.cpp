#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>

struct Instruction {
    Instruction(std::string _opcode, int _operand)
        : opcode(_opcode), operand(_operand){};
    std::string opcode;
    int operand;
};

std::vector<Instruction> readInput() {
    std::vector<Instruction> input;
    std::ifstream ifs{"inputs/day08.txt"};
    std::string opcode;
    int operand;
    while (ifs >> opcode >> operand) {
        input.emplace_back(opcode, operand);
    }
    return input;
}

int partOne(const std::vector<Instruction>& instructions) {
    std::vector<bool> visited;
    visited.resize(instructions.size(), false);

    int pc = 0;
    int accumulator = 0;
    while (true) {
        if (visited[pc]) {
            return accumulator;
        }
        visited[pc] = true;

        Instruction instruction = instructions[pc];
        if (instruction.opcode == "jmp") {
            pc += instruction.operand;
            continue;
        } else if (instruction.opcode == "acc")
            accumulator += instruction.operand;
        ++pc;
    }
}

int runProgram(const std::vector<Instruction>& instructions,
               std::vector<bool> visited, int pc, int accum, bool canFlip) {
    if (pc >= instructions.size()) {
        return accum;
    }
    if (visited[pc]) {
        return -1;
    }
    visited[pc] = true;

    Instruction instruction = instructions[pc];
    if (instruction.opcode == "jmp") {
        int result = runProgram(instructions, visited,
                                pc + instruction.operand, accum, canFlip);
        if (result == -1 && canFlip) {
            return runProgram(instructions, visited, pc + 1, accum, false);
        } else {
            return result;
        }
    } else if (instruction.opcode == "acc") {
        return runProgram(instructions, visited, pc + 1,
                          accum + instruction.operand, canFlip);
    } else {
        int result = runProgram(instructions, visited, pc + 1, accum, canFlip);
        if (result == -1 && canFlip) {
            return runProgram(instructions, visited, pc + instruction.operand,
                              accum, false);
        } else {
            return result;
        }
    }
}

int partTwo(const std::vector<Instruction>& instructions) {
    std::vector<bool> visited;
    visited.resize(instructions.size(), false);
    return runProgram(instructions, visited, 0, 0, true);
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
