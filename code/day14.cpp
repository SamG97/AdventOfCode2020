#include "utils.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

static_assert(sizeof(unsigned long) >= 5);

enum class Opcode { Mask, MemSet };

struct Instruction {
    Instruction(Opcode _opcode, std::string _operand)
        : opcode(_opcode), operand(_operand){};
    Instruction(Opcode _opcode, int _address, std::string _operand)
        : opcode(_opcode), address(_address), operand(_operand){};
    Opcode opcode;
    unsigned long address;
    std::string operand;
};

std::vector<Instruction> readInput() {
    std::vector<Instruction> input;
    std::ifstream ifs{"inputs/day14.txt"};
    std::string opcode;
    std::string operand;
    std::string spacer; // Used to consume the "="
    while (ifs >> opcode >> spacer >> operand) {
        if (opcode == "mask") {
            input.emplace_back(Opcode::Mask, operand);
        } else {
            auto addrStart = opcode.find('[');
            auto addrEnd = opcode.find(']');
            unsigned long addr = std::stol(
                opcode.substr(addrStart + 1, addrEnd - addrStart - 1));
            input.emplace_back(Opcode::MemSet, addr, operand);
        }
    }
    return input;
}

std::pair<unsigned long, unsigned long> updateMask(const std::string& mask) {
    unsigned long oneMask = 0UL;
    unsigned long zeroMask = ~0UL;
    auto sz = mask.size();
    for (size_t i = 0; i < sz; ++i) {
        char c = mask[sz - i - 1];
        if (c == '1')
            oneMask |= 1UL << i;
        else if (c == '0')
            zeroMask &= ~(1UL << i);
    }
    return {oneMask, zeroMask};
}

unsigned long applyMask(unsigned long value, unsigned long oneMask,
                        unsigned long zeroMask) {
    value |= oneMask;
    value &= zeroMask;
    return value;
}

unsigned long partOne(const std::vector<Instruction>& instructions) {
    std::unordered_map<unsigned long, unsigned long> mem;
    unsigned long oneMask = 0L;
    unsigned long zeroMask = 1L;
    for (auto& instruction : instructions) {
        if (instruction.opcode == Opcode::Mask) {
            auto newMask = updateMask(instruction.operand);
            oneMask = newMask.first;
            zeroMask = newMask.second;
        } else {
            unsigned long value =
                applyMask(std::stol(instruction.operand), oneMask, zeroMask);
            auto match = mem.find(instruction.address);
            if (match == mem.end()) {
                mem.insert({instruction.address, value});
            } else {
                match->second = value;
            }
        }
    }
    return std::accumulate(
        mem.begin(), mem.end(), 0UL,
        [](unsigned long prev,
           const std::pair<unsigned long, unsigned long>& p) {
            return prev + p.second;
        });
}

std::vector<unsigned long> getAddresses(unsigned long baseAddr,
                                        const std::string& mask) {
    unsigned long oneMask = 0UL;
    unsigned long zeroMask = ~0UL;
    auto sz = mask.size();
    for (size_t i = 0; i < sz; ++i) {
        char c = mask[sz - i - 1];
        if (c == '1')
            oneMask |= 1UL << i;
        else if (c == 'X')
            zeroMask &= ~(1UL << i);
    }
    baseAddr |= oneMask;
    baseAddr &= zeroMask;
    std::vector<unsigned long> addresses{baseAddr};
    for (size_t i = 0; i < sz; ++i) {
        char c = mask[sz - i - 1];
        if (c == 'X') {
            auto currentAddresses = addresses.size();
            for (size_t j = 0; j < currentAddresses; ++j)
                addresses.push_back(addresses[j] | (1UL << i));
        }
    }
    return addresses;
}

unsigned long partTwo(const std::vector<Instruction>& instructions) {
    std::unordered_map<unsigned long, unsigned long> mem;
    std::string mask;
    for (auto& instruction : instructions) {
        if (instruction.opcode == Opcode::Mask) {
            mask = instruction.operand;
        } else {
            unsigned long value = std::stol(instruction.operand);
            for (auto& addr : getAddresses(instruction.address, mask)) {
                auto match = mem.find(addr);
                if (match == mem.end()) {
                    mem.insert({addr, value});
                } else {
                    match->second = value;
                }
            }
        }
    }
    return std::accumulate(
        mem.begin(), mem.end(), 0UL,
        [](unsigned long prev,
           const std::pair<unsigned long, unsigned long>& p) {
            return prev + p.second;
        });
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
