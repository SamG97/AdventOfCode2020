#include "utils.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

struct Bus {
    Bus() : running{false} {}
    Bus(int t) : running(true), frequency(t) {}
    bool running;
    int frequency;
};

using InputType = std::pair<int, std::vector<Bus>>;

InputType readInput() {
    std::vector<Bus> input;
    std::ifstream ifs{"inputs/day13.txt"};
    int departure;
    ifs >> departure;
    std::string bus;
    while (std::getline(ifs, bus, ',')) {
        if (bus == "x")
            input.emplace_back();
        else
            input.emplace_back(std::stoi(bus));
    }
    return {departure, input};
}

int partOne(const InputType& input) {
    int target = input.first;
    auto buses = input.second;
    int bestBus = 0;
    int bestWait = INT32_MAX;
    for (auto& bus : buses) {
        if (!bus.running)
            continue;
        if (target % bus.frequency == 0)
            return 0;
        int departure = (target / bus.frequency + 1) * bus.frequency;
        int wait = departure - target;
        if (wait < bestWait) {
            bestWait = wait;
            bestBus = bus.frequency;
        }
    }
    return bestBus * bestWait;
}

long partTwo(const InputType& input) {
    auto buses = input.second;

    // n stores the solution for the buses currently considered and base is
    // the frequency that these solutions occur (i.e. n + k * base meets the
    // requirements for all integers k)
    // We initialise with the solution for just the first bus and then
    // iteratively add each extra bus
    long base = buses[0].frequency;
    long n = base;
    for (int i = 1; i < buses.size(); ++i) {
        if (!buses[i].running)
            continue;

        // Increment our solution for past buses, n, until it is also a
        // solution for the new bus, whilst still keeping it valid for all
        // previous buses
        while ((n + i) % buses[i].frequency != 0) {
            n += base;
        }
        // Update base to be the LCM of all bus frequencies seen so far;
        // since all bus frequencies are prime, we can just multiply base
        // by the new frequency
        base *= buses[i].frequency;
    }
    return n;
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
