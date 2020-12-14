#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum class Direction { North, South, East, West, Left, Right, Forward };

static const std::unordered_map<char, Direction> convertDirection{
    {'N', Direction::North},   {'S', Direction::South},
    {'E', Direction::East},    {'W', Direction::West},
    {'L', Direction::Left},    {'R', Direction::Right},
    {'F', Direction::Forward},
};

struct Move {
    Move(char _direction, int _size) : size(_size) {
        auto converted = convertDirection.find(_direction);
        if (converted != convertDirection.end())
            direction = converted->second;
        else
            throw std::invalid_argument("_direction");
    }
    Direction direction;
    int size;
};

std::vector<Move> readInput() {
    std::vector<Move> input;
    std::ifstream ifs{"inputs/day12.txt"};
    char direction;
    std::string num;
    while (ifs >> direction >> num) {
        int size = std::stoi(num);
        input.emplace_back(direction, size);
    }
    return input;
}

int partOne(const std::vector<Move>& moves) {
    std::pair<int, int> pos = {0, 0};
    int orientation = 1;
    for (auto& move : moves) {
        switch (move.direction) {
        case Direction::North:
            pos.second += move.size;
            break;
        case Direction::South:
            pos.second -= move.size;
            break;
        case Direction::East:
            pos.first += move.size;
            break;
        case Direction::West:
            pos.first -= move.size;
            break;
        case Direction::Left:
            // Hack because mod doesn't work with negative numbers...
            orientation = (orientation - move.size / 90 + 4) % 4;
            break;
        case Direction::Right:
            orientation = (orientation + move.size / 90 + 4) % 4;
            break;
        case Direction::Forward:
            switch (orientation) {
            case 0:
                pos.second += move.size;
                break;
            case 1:
                pos.first += move.size;
                break;
            case 2:
                pos.second -= move.size;
                break;
            case 3:
                pos.first -= move.size;
                break;
            }
        }
    }
    return abs(pos.first) + abs(pos.second);
}

int partTwo(const std::vector<Move>& moves) {
    std::pair<int, int> ship = {0, 0};
    std::pair<int, int> waypoint = {10, 1};
    for (auto& move : moves) {
        switch (move.direction) {
        case Direction::North:
            waypoint.second += move.size;
            break;
        case Direction::South:
            waypoint.second -= move.size;
            break;
        case Direction::East:
            waypoint.first += move.size;
            break;
        case Direction::West:
            waypoint.first -= move.size;
            break;
        case Direction::Left:
            for (int i = 0; i < move.size / 90; ++i)
                waypoint = {-waypoint.second, waypoint.first};
            break;
        case Direction::Right:
            for (int i = 0; i < move.size / 90; ++i)
                waypoint = {waypoint.second, -waypoint.first};
            break;
        case Direction::Forward:
            ship.first += move.size * waypoint.first;
            ship.second += move.size * waypoint.second;
        }
    }
    return abs(ship.first) + abs(ship.second);
}

int main() {
    auto input = readInput();
    std::cout << timeit(partOne)(input) << "\n";
    std::cout << timeit(partTwo)(input) << "\n";
    return 0;
}
