#include "../common/int_vector2.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace {
using std::map;
using std::size_t;
using std::vector;

void parse_input(const char* input, map<char, vector<IntVector2>>& antennas,
    vector<vector<bool>>& grid)
{

    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file " << input << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        vector<bool> row;
        for (char c : line) {
            if (c != '.') {
                antennas[c].push_back({ static_cast<int>(row.size()),
                    static_cast<int>(grid.size()) });
            }
            row.push_back(false);
        }
        grid.push_back(row);
    }
}

int set_antinode(vector<vector<bool>>& grid, IntVector2 pos, int& total)
{
    size_t x_size = static_cast<size_t>(pos.x);
    size_t y_size = static_cast<size_t>(pos.y);
    if (pos.x < 0 || pos.y < 0 || x_size >= grid[0].size()
        || y_size >= grid.size()) {
        return false;
    }

    if (grid[y_size][x_size]) {
        return true;
    }

    grid[y_size][x_size] = true;
    total += 1;
    return true;
}


void part_1(map<char, vector<IntVector2>>& antennas, vector<vector<bool>> grid)
{
    int total { 0 };

    for (auto& [antenna, positions] : antennas) {
        for (auto pos_a_it = positions.begin(); pos_a_it != positions.end();
            ++pos_a_it) {
            for (auto pos_b_it = std::next(pos_a_it);
                pos_b_it != positions.end(); ++pos_b_it) {
                auto direction = *pos_b_it - *pos_a_it;

                auto antinode_1 = *pos_a_it - direction;
                auto antinode_2 = *pos_b_it + direction;

                set_antinode(grid, antinode_1, total);
                set_antinode(grid, antinode_2, total);
            }
        }
    }

    std::cout << "Part 1: " << total << std::endl;
}

void part_2(map<char, vector<IntVector2>>& antennas, vector<vector<bool>> grid)
{
    int total { 0 };

    for (auto& [antenna, positions] : antennas) {
        for (auto pos_a_it = positions.begin(); pos_a_it != positions.end();
            ++pos_a_it) {
            for (auto pos_b_it = std::next(pos_a_it);
                pos_b_it != positions.end(); ++pos_b_it) {
                auto direction = *pos_b_it - *pos_a_it;

                auto antinode = *pos_a_it;

                do {
                    antinode += direction;
                } while (set_antinode(grid, antinode, total));

                antinode = *pos_b_it;
                do {
                    antinode -= direction;
                } while (set_antinode(grid, antinode, total));
            }
        }
    }

    std::cout << "Part 2: " << total << std::endl;
}

} // namespace

void day_08(const char* input)
{
    std::cout << "Day 08" << std::endl;

    map<char, vector<IntVector2>> antennas;
    vector<vector<bool>> grid;

    parse_input(input, antennas, grid);

    part_1(antennas, grid);
    part_2(antennas, grid);
}
