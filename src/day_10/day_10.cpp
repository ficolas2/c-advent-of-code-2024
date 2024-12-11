#include "../common/int_vector2.h"

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

namespace {
using std::vector;

const auto directions = { IntVector2(0, -1), IntVector2(1, 0), IntVector2(0, 1),
    IntVector2(-1, 0) };

vector<vector<int>> parse_input(const char* input)
{
    std::ifstream file(input);
    vector<vector<int>> result;

    if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
        return result;
    }

    std::string line;
    while (std::getline(file, line)) {
        vector<int> row;
        row.reserve(line.size());
        for (char c : line) {
            row.push_back(c - '0');
        }
        result.push_back(row);
    }

    return result;
}

int path_find(const vector<vector<int>>& input_data, IntVector2 start,
    bool allow_revisits)
{
    int total { 0 };

    bool* visited = new bool[input_data.size() * input_data[0].size()]();
    int map_height = static_cast<int>(input_data.size());
    int map_width = static_cast<int>(input_data[0].size());

    visited[static_cast<size_t>(start.y * map_width + start.x)] = true;
    std::queue<IntVector2> queue;
    queue.push(start);

    while (!queue.empty()) {
        IntVector2 current = queue.front();
        queue.pop();
        int height = input_data[static_cast<size_t>(current.y)][static_cast<size_t>(current.x)];

        for (auto dir : directions) {
            IntVector2 next = current + dir;
            if (next.x < 0 || next.x >= map_width || next.y < 0
                || next.y >= map_height) {
                continue;
            }
            int next_height = input_data[static_cast<size_t>(next.y)]
                                        [static_cast<size_t>(next.x)];

            if (!allow_revisits && visited[next.y * map_width + next.x]) {
                continue;
            }

            if (next_height == height + 1) {
                visited[next.y * map_width + next.x] = true;
                if (next_height == 9) {
                    total++;
                } else {
                    queue.push(next);
                }
            }
        }
    }

    delete[] visited;

    return total;
}

int solve(const vector<vector<int>>& input_data, bool allow_revisits)
{
    int total { 0 };
    for (size_t y = 0; y < input_data.size(); y++) {
        for (size_t x = 0; x < input_data[y].size(); x++) {
            if (input_data[y][x] == 0) {
                total += path_find(input_data,
                    IntVector2(static_cast<int>(x), static_cast<int>(y)),
                    allow_revisits);
            }
        }
    }

    return total;
}

} // namespace

void day_10(const char* input)
{
    std::cout << "Day 10:" << std::endl;
    vector<vector<int>> input_data = parse_input(input);

    std::cout << "Part 1: " << solve(input_data, false) << std::endl;
    std::cout << "Part 2: " << solve(input_data, true) << std::endl;
}
