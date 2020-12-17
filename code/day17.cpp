#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>

using Coordinate3D = std::tuple<int, int, int>;
using Coordinate4D = std::tuple<int, int, int, int>;

struct key_hash : public std::unary_function<Coordinate3D, size_t> {
    size_t operator()(const Coordinate3D& k) const {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
    }
};

struct key_hash3D : public std::unary_function<Coordinate3D, size_t> {
    size_t operator()(const Coordinate3D& k) const {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
    }
};

struct key_hash4D : public std::unary_function<Coordinate4D, size_t> {
    size_t operator()(const Coordinate4D& k) const {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k)
               ^ std::get<3>(k);
    }
};

using Coordinate3DSet = std::unordered_set<Coordinate3D, key_hash3D>;
using Coordinate4DSet = std::unordered_set<Coordinate4D, key_hash4D>;

std::vector<std::string> readInput() {
    std::vector<std::string> input;
    std::ifstream ifs{"inputs/day17.txt"};
    std::string line;
    while (ifs >> line) {
        input.push_back(line);
    }
    return input;
}

Coordinate3DSet
    processInitialState(const std::vector<std::string>& initialState) {
    Coordinate3DSet activateCells{};
    for (size_t i = 0; i < initialState.size(); ++i)
        for (size_t j = 0; j < initialState[i].size(); ++j)
            if (initialState[i][j] == '#')
                activateCells.insert(std::make_tuple(j, i, 0));
    return activateCells;
}

int countNeighbours(const Coordinate3D& state,
                    const Coordinate3DSet& activateCells) {
    auto [x, y, z] = state;
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dz = -1; dz <= 1; ++dz)
                if (!(dx == 0 && dy == 0 && dz == 0)
                    && activateCells.find(
                           std::make_tuple(x + dx, y + dy, z + dz))
                           != activateCells.end())
                    ++count;
    return count;
}

int countNeighbours(const Coordinate4D& state,
                    const Coordinate4DSet& activateCells) {
    auto [x, y, z, w] = state;
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx)
        for (int dy = -1; dy <= 1; ++dy)
            for (int dz = -1; dz <= 1; ++dz)
                for (int dw = -1; dw <= 1; ++dw)
                    if (!(dx == 0 && dy == 0 && dz == 0 && dw == 0)
                        && activateCells.find(
                               std::make_tuple(x + dx, y + dy, z + dz, w + dw))
                               != activateCells.end())
                        ++count;
    return count;
}

int partOne(const std::vector<std::string>& initialState) {
    Coordinate3DSet activateCells{};
    for (size_t i = 0; i < initialState.size(); ++i)
        for (size_t j = 0; j < initialState[i].size(); ++j)
            if (initialState[i][j] == '#')
                activateCells.insert(std::make_tuple(j, i, 0));

    Coordinate3DSet alreadyChecked;
    Coordinate3DSet newCells;
    for (int t = 0; t < 6; ++t) {
        alreadyChecked = Coordinate3DSet{};
        newCells = Coordinate3DSet{};
        for (auto& state : activateCells) {
            auto [x, y, z] = state;
            for (int dx = -1; dx <= 1; ++dx)
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dz = -1; dz <= 1; ++dz) {
                        Coordinate3D coord =
                            std::make_tuple(x + dx, y + dy, z + dz);
                        if (alreadyChecked.find(coord) != alreadyChecked.end())
                            continue;
                        alreadyChecked.insert(coord);
                        int count = countNeighbours(coord, activateCells);
                        if (count == 3
                            || (count == 2
                                && activateCells.find(coord)
                                       != activateCells.end()))
                            newCells.insert(coord);
                    }
        }
        activateCells = newCells;
    }
    return activateCells.size();
}

int partTwo(const std::vector<std::string>& initialState) {
    Coordinate4DSet activateCells{};
    for (size_t i = 0; i < initialState.size(); ++i)
        for (size_t j = 0; j < initialState[i].size(); ++j)
            if (initialState[i][j] == '#')
                activateCells.insert(std::make_tuple(j, i, 0, 0));

    Coordinate4DSet alreadyChecked;
    Coordinate4DSet newCells;
    for (int t = 0; t < 6; ++t) {
        alreadyChecked = Coordinate4DSet{};
        newCells = Coordinate4DSet{};
        for (auto& state : activateCells) {
            auto [x, y, z, w] = state;
            for (int dx = -1; dx <= 1; ++dx)
                for (int dy = -1; dy <= 1; ++dy)
                    for (int dz = -1; dz <= 1; ++dz)
                        for (int dw = -1; dw <= 1; ++dw) {
                            Coordinate4D coord = std::make_tuple(
                                x + dx, y + dy, z + dz, w + dw);
                            if (alreadyChecked.find(coord)
                                != alreadyChecked.end())
                                continue;
                            alreadyChecked.insert(coord);
                            int count = countNeighbours(coord, activateCells);
                            if (count == 3
                                || (count == 2
                                    && activateCells.find(coord)
                                           != activateCells.end()))
                                newCells.insert(coord);
                        }
        }
        activateCells = newCells;
    }
    return activateCells.size();
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
