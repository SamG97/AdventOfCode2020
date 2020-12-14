#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
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

void updateTerminates(const std::vector<std::vector<int>>& prev,
                     std::vector<bool>& terminates, int root) {
    // Already seen this point so don't bother updating reachable nodes again
    if (terminates[root])
        return;

    terminates[root] = true;  // Reached from a point that terminates
    for (auto& toUpdate : prev[root]) {
        updateTerminates(prev, terminates, toUpdate);
    }
}

int partTwo(const std::vector<Instruction>& instructions) {
    std::vector<std::vector<int>> prev;
    prev.resize(instructions.size(), std::vector<int>{});
    std::vector<int> immediateTermination{};

    // Create a mapping from each pc count to all instructions that could
    // immediately preceed it
    for (int i = 0; i < static_cast<int>(instructions.size()); ++i) {
        Instruction instruction = instructions[i];
        int nextPc =
            i + (instruction.opcode == "jmp" ? instruction.operand : 1);
        if (nextPc >= static_cast<int>(instructions.size())) {
            immediateTermination.push_back(i);
        } else {
            prev[nextPc].push_back(i);
        }
    }

    // Trace along the graph of previous instructions to find all instructions
    // that can reach an instruction that immediately terminates
    std::vector<bool> terminates;
    terminates.resize(instructions.size(), false);
    for (auto& i : immediateTermination) {
        updateTerminates(prev, terminates, i);
    }

    // Run the program flipping an instruction which currently is not in the
    // terminating set but by changing its opcode could be
    int pc = 0;
    int accumulator = 0;
    while (true) {
        if (pc >= static_cast<int>(instructions.size())) {
            return accumulator;
        }

        Instruction instruction = instructions[pc];
        if (instruction.opcode == "jmp") {
            if (!terminates[pc] && terminates[pc + 1])
                ++pc;
            else
                pc += instruction.operand;
        } else if (instruction.opcode == "acc") {
            accumulator += instruction.operand;
            ++pc;
        } else {
            if (!terminates[pc] && terminates[pc + instruction.operand])
                pc += instruction.operand;
            else
                ++pc;
        }
    }
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
