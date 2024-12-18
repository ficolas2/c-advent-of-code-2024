#include "../common/vector2.h"
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

namespace {
using std::pair;
using std::vector;
using Vec2 = Vector2<int>;
using std::size_t;

const int WIDTH = 101;
const int HEIGHT = 103;

// Tree finder values
// Looks for at least LINE_COUNT lines of at least LINE_LENTH length
const int LINE_LENTH = 8;
const int LINE_COUNT = 4;

vector<pair<Vec2, Vec2>> read_input(const char* input)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return {};
    }

    std::string line;
    vector<pair<Vec2, Vec2>> result;
    while (std::getline(file, line)) {
        auto i = line.find(" ");
        auto pos_str = line.substr(2, i);
        auto vel_str = line.substr(i + 3);

        i = pos_str.find(",");
        auto pos = Vec2(
            std::stoi(pos_str.substr(0, i)), std::stoi(pos_str.substr(i + 1)));
        i = vel_str.find(",");
        auto vel = Vec2(
            std::stoi(vel_str.substr(0, i)), std::stoi(vel_str.substr(i + 1)));

        result.push_back(std::make_pair(pos, vel));
    }

    return result;
}

int euchlidean_mod(int num, int mod) {
    return ((num % mod) + mod) % mod;
}

void part_1(vector<pair<Vec2, Vec2>> input, int steps)
{
    int quadrant[2][2] = { { 0, 0 }, { 0, 0 } };

    for (auto& [pos, vel] : input) {
        int x = euchlidean_mod(pos.x + vel.x * steps, WIDTH);
        int y = euchlidean_mod(pos.y + vel.y * steps, HEIGHT);

        size_t qx = 3;
        size_t qy = 3;
        if (x < WIDTH / 2) {
            qx = 0;
        } else if (x > WIDTH / 2) {
            qx = 1;
        }

        if (y < HEIGHT / 2) {
            qy = 0;
        } else if (y > HEIGHT / 2) {
            qy = 1;
        }

        if (qx < 2 && qy < 2) {
            quadrant[qx][qy]++;
        }
    }

    std::cout << quadrant[0][0] + quadrant[0][1] + quadrant[1][0]
            + quadrant[1][1]
              << std::endl;

    int result = static_cast<int>(quadrant[0][0])
        * static_cast<int>(quadrant[0][1]) * static_cast<int>(quadrant[1][0])
        * static_cast<int>(quadrant[1][1]);
    std::cout << "Part 1: " << result << std::endl;
}

void print_grid(const vector<vector<int>>& grid)
{
    for (size_t y = 0; y < HEIGHT; y++) {
        for (size_t x = 0; x < WIDTH; x++) {
            if (grid[y][x] > 0) {
                if (grid[y][x] > 9) {
                    std::cout << "#";
                } else {
                    std::cout << grid[y][x];
                }
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
}

void next(vector<pair<Vec2, Vec2>>& robots, vector<vector<int>>& grid)
{
    for (auto& [pos, vel] : robots) {
        grid[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] -= 1;
        pos.x = euchlidean_mod(pos.x + vel.x, WIDTH);
        pos.y = euchlidean_mod(pos.y + vel.y, HEIGHT);
        grid[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] += 1;
    }
}

void part_2(vector<pair<Vec2, Vec2>> input)
{
    vector<vector<int>> grid(HEIGHT, vector<int>(WIDTH, 0));

    for (auto& [pos, vel] : input) {
        grid[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] += 1;
    }

    int step = 0;
    while (true) {
        step++;

        next(input, grid);

        int line_count = 0;
        for (size_t y = 0; y < HEIGHT; y++) {
            int length = 0;
            for (size_t x = 0; x < WIDTH; x++) {
                if (grid[y][x] > 0) {
                    length++;
                } else {
                    if (length >= LINE_LENTH) {
                        line_count++;
                    }
                    length = 0;
                }
            }
        }

        if (line_count >= LINE_COUNT) {
            print_grid(grid);
            std::cout << "Step " << step << std::endl;
            std::cout << "Continue? (y/n)";
            std::string yes;
            std::cin >> yes;
            if (yes != "y") {
                break;
            }
        }
    }
}

} // namespace

void day_14(const char* input_file)
{
    std::cout << "Day 13:" << std::endl;

    auto input = read_input(input_file);
    part_1(input, 100);
    part_2(input);
}
