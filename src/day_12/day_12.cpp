

#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

#include "../common/int_vector2.h"

namespace {
using std::queue;
using std::vector;

vector<IntVector2> directions { { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };
// clang-format off
vector<vector<IntVector2>> corners { 
    { { -1, -1 }, { -1, 0 }, { 0, -1 } },
    { { -1,  1 }, { -1, 0 }, { 0,  1 } }, 
    { {  1, -1 }, {  1, 0 }, { 0, -1 } },
    { {  1,  1 }, {  1, 0 }, { 0,  1 } },
}; // clang-format on

vector<vector<char>> read_input(const char* input)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return {};
    }

    vector<vector<char>> result;

    std::string line;
    while (std::getline(file, line)) {
        result.push_back(vector<char>(line.begin(), line.end()));
    }

    return result;
}

bool in_bounds(const vector<vector<char>>& input, IntVector2 pos)
{
    // If x < 0, then it will overflow and be bigger than the size_t
    return static_cast<size_t>(pos.x) < input.size()
        && static_cast<size_t>(pos.y) < input[0].size();
}

bool get_equals(const vector<vector<char>>& input, char c, IntVector2 pos)
{
    if (!in_bounds(input, pos)) {
        return false;
    }
    return input[static_cast<size_t>(pos.x)][static_cast<size_t>(pos.y)] == c;
};

std::pair<int, int> area_x_perimeter(const vector<vector<char>>& input,
    vector<vector<bool>>& visited, IntVector2 start)
{
    auto c = input[static_cast<size_t>(start.x)][static_cast<size_t>(start.y)];

    queue<IntVector2> q;
    q.push(start);

    visited[static_cast<size_t>(start.x)][static_cast<size_t>(start.y)] = true;
    int area = 1;
    int perimeter = 0;
    int sides = 0;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        for (const auto& dir : directions) {
            auto next_pos = current + dir;
            // If the position around the cell is not the same, that means its
            // an edge.
            if (!get_equals(input, c, next_pos)) {
                ++perimeter;
                continue;
            }

            if (visited[static_cast<size_t>(next_pos.x)]
                       [static_cast<size_t>(next_pos.y)])
                continue;

            visited[static_cast<size_t>(next_pos.x)]
                   [static_cast<size_t>(next_pos.y)]
                = true;
            ++area;
            q.push(next_pos);
        }

        // Count corners, because corners and sides are the same
        for (const auto& corner_cells : corners) {
            // +-+-+
            // |A|X|
            // +-+-+
            // |C|B|
            // +-+-+
            // A = corner_side_a  X = current
            // C = corner_cell    B = corner_side_b
            bool corner_cell = get_equals(input, c, current + corner_cells[0]);
            bool corner_side_a
                = get_equals(input, c, current + corner_cells[1]);
            bool corner_side_b
                = get_equals(input, c, current + corner_cells[2]);

            // +---+
            // |A A|
            // +-+-+
            // |?|B|
            // +-+-+
            // If both corner sides are not the same, no corner
            if (corner_side_a != corner_side_b) {
                continue;
            }

            // +---+
            // |A A|
            // |   |
            // |A A|
            // +---+
            // If all 4 are the same, no corner
            if (corner_side_a && corner_cell) {
                continue;
            }

            // Sides and corners are equal
            ++sides;
        }
    }

    return { area * perimeter, area * sides };
}

void solve(const vector<vector<char>>& input)
{
    int p1_total { 0 };
    int p2_total { 0 };
    vector<vector<bool>> visited(
        input.size(), vector<bool>(input[0].size(), false));

    for (size_t x = 0; x < input.size(); ++x) {
        for (size_t y = 0; y < input[x].size(); ++y) {
            if (visited[x][y]) {
                continue;
            }

            auto res = area_x_perimeter(
                input, visited, { static_cast<int>(x), static_cast<int>(y) });
            p1_total += res.first;
            p2_total += res.second;
        }
    }

    std::cout << "Part 1: " << p1_total << std::endl;
    std::cout << "Part 2: " << p2_total << std::endl;
}

} // namespace

void day_12(const char* input_file)
{
    std::cout << "Day 12: " << std::endl;

    auto input = read_input(input_file);

    solve(input);
}
