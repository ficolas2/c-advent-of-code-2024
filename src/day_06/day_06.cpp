#include "../common/int_vector2.h"
#include <atomic>
#include <bitset>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

namespace {
using std::size_t;
using std::thread;
using std::vector;

// ^ > V <
const vector<IntVector2> directions = { IntVector2(0, -1), IntVector2(1, 0),
    IntVector2(0, 1), IntVector2(-1, 0) };

class Tile {
    const size_t VISITED = 4;
    const size_t WALL = 5;
    std::bitset<6> state;

public:
    Tile(bool is_wall) { state.set(5, is_wall); }

    void set_direction(size_t direction)
    {
        assert(direction >= 0 && direction < 4);
        state.set(direction);
    }

    bool check_direction(size_t direction)
    {
        assert(direction >= 0 && direction < 4);
        return state.test(direction);
    }

    void set_visited() { state.set(VISITED); }

    void set_unvisited() { state &= ~0b11111ULL; }

    bool is_visited() const { return state.test(VISITED); }

    bool is_wall() const { return state.test(WALL); }

    void set_wall(bool is_wall) { state.set(WALL, is_wall); }
};

using Map = std::vector<std::vector<Tile>>;

void read_input(const char* input_file, Map& input, IntVector2& guard_pos)
{

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cout << "File could not be opened" << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<Tile> row;
        for (char c : line) {
            if (c == '#') {
                row.push_back(Tile(true));
            } else if (c == '.') {
                row.push_back(Tile(false));
            } else if (c == '^') {
                guard_pos = IntVector2(static_cast<int>(row.size()),
                    static_cast<int>(input.size()));
                row.push_back(Tile(false));
            }
        }
        input.push_back(row);
    }
}

[[maybe_unused]] void print_map(const Map& input, const IntVector2& guard_pos)
{
    for (size_t y = 0; y < input.size(); y++) {
        for (size_t x = 0; x < input[y].size(); x++) {
            if (guard_pos.x == static_cast<int>(x)
                && guard_pos.y == static_cast<int>(y)) {
                std::cout << "X";
            } else {
                const Tile* current_tile = &input[y][x];
                if (current_tile->is_wall()) {
                    std::cout << "#";
                } else if (current_tile->is_visited()) {
                    std::cout << ".";
                } else {
                    std::cout << " ";
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << "--------------" << std::endl;
}

bool in_bounds(const IntVector2& pos, const Map& input)
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < static_cast<int>(input[0].size())
        && pos.y < static_cast<int>(input.size());
}

Map part_1(Map input, IntVector2 guard_pos)
{
    int dir_index { 0 };
    int visited { 0 };

    while (true) {
        Tile* current_tile = &input[static_cast<size_t>(guard_pos.y)]
                                   [static_cast<size_t>(guard_pos.x)];
        if (!current_tile->is_visited()) {
            current_tile->set_visited();
            visited++;
        }

        auto next_pos = guard_pos + directions[static_cast<size_t>(dir_index)];
        if (!in_bounds(next_pos, input)) {
            break;
        }

        Tile* next_tile = &input[static_cast<size_t>(next_pos.y)]
                                [static_cast<size_t>(next_pos.x)];
        if (next_tile->is_wall()) {
            dir_index = (dir_index + 1) % 4;
        } else {
            guard_pos = next_pos;
        }
    }

    std::cout << "Result: " << visited << std::endl;

    return input;
}

bool has_loop(Map& input, IntVector2 guard_pos)
{
    size_t dir_index { 0 };

    while (true) {
        auto next_pos = guard_pos + directions[dir_index];
        if (!in_bounds(next_pos, input)) {
            break;
        }

        Tile* next_tile = &input[static_cast<size_t>(next_pos.y)]
                                [static_cast<size_t>(next_pos.x)];

        if (next_tile->is_wall()) {
            dir_index = (dir_index + 1) % 4;
        } else {
            if (next_tile->check_direction(dir_index)) {
                return true;
            }
            next_tile->set_direction(dir_index);
            guard_pos = next_pos;
        }
    }
    return false;
}

void clean_map(Map& input)
{
    for (auto& row : input) {
        for (auto& tile : row) {
            tile.set_unvisited();
        }
    }
}

void work(Map input, IntVector2 guard_pos, std::atomic<int>& total_result,
    vector<IntVector2> wallPositions, size_t start, size_t end)
{
    int total { 0 };
    for (size_t i = start; i < end; i++) {
        IntVector2 wall_pos = wallPositions[i];
        input[static_cast<size_t>(wall_pos.y)][static_cast<size_t>(wall_pos.x)]
            .set_wall(true);
        if (has_loop(input, guard_pos)) {
            total++;
        }
        clean_map(input);
        input[static_cast<size_t>(wall_pos.y)][static_cast<size_t>(wall_pos.x)]
            .set_wall(false);
    }

    total_result += total;
}

void part_2(Map input, IntVector2 guard_pos, Map visited_input)
{
    const int numThreads = 6;
    vector<thread> threads;
    std::atomic<int> total { 0 };

    vector<IntVector2> wall_positions;
    for (size_t x = 0; x < input.size(); x++) {
        for (size_t y = 0; y < input.size(); y++) {
            if (!visited_input[y][x].is_visited()) {
                continue;
            }

            wall_positions.push_back(
                IntVector2(static_cast<int>(x), static_cast<int>(y)));
        }
    }

    size_t total_work = wall_positions.size();
    size_t chunk_size = static_cast<size_t>(std::round(
        static_cast<float>(total_work) / static_cast<float>(numThreads)));

    for (size_t i = 0; i < numThreads; i++) {
        size_t start = i * chunk_size;
        size_t end = std::min((i + 1) * chunk_size, total_work);

        threads.push_back(thread(work, input, guard_pos, std::ref(total),
            wall_positions, start, end));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Result: " << total << std::endl;
}
}

void day_06(const char* input_file)
{
    std::cout << "Day 6" << std::endl;

    Map input;
    IntVector2 guard_pos = IntVector2(0, 0);
    read_input(input_file, input, guard_pos);

    Map visited_input = part_1(input, guard_pos);
    part_2(input, guard_pos, visited_input);
}
