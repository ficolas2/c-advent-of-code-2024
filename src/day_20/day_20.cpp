#include <fstream>
#include <iostream>
#include <vector>

#include "../common/vector2.h"

namespace {
using std::vector;
using Vec2 = Vector2<int>;

const Vec2 DIRECTIONS[4] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
//   X
//  X.X
// X.O.X
//  X.X
//   X
const Vec2 CHEAT_VECS[8] = { { 2, 0 }, { 0, 2 }, { -2, 0 }, { 0, -2 }, { 1, 1 },
    { -1, -1 }, { 1, -1 }, { -1, 1 } };

void parse_input(
    const char* input, vector<vector<bool>>& map, Vec2& start, Vec2& end)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        vector<bool> row;

        for (auto c : line) {
            switch (c) {
            case '#':
                row.push_back(false);
                break;
            case '.':
                row.push_back(true);
                break;
            case 'S':
                start = { static_cast<int>(row.size()),
                    static_cast<int>(map.size()) };
                row.push_back(true);
                break;
            case 'E':
                end = { static_cast<int>(row.size()),
                    static_cast<int>(map.size()) };
                row.push_back(true);
                break;
            }
        }
        map.push_back(row);
    }
}

int solve(
    const vector<vector<bool>>& map, Vec2 start, Vec2 end, int max_cheat_time)
{
    vector<vector<bool>> visited(map.size(), vector(map[0].size(), false));
    vector<vector<int>> distances(map.size(), vector(map[0].size(), 0));

    int distance = 0;
    Vec2 current = start;
    while (true) {
        ++distance;
        for (auto dir : DIRECTIONS) {
            Vec2 next = current + dir;
            if (!map[static_cast<size_t>(next.y)][static_cast<size_t>(next.x)]
                || distances[static_cast<size_t>(next.y)]
                            [static_cast<size_t>(next.x)]
                    != 0
                || next == start) {
                continue;
            }
            distances[static_cast<size_t>(next.y)][static_cast<size_t>(next.x)]
                = distance;
            if (next == end) {
                goto finish;
            }
            current = next;
            break;
        }
    }
finish:

    int total = 0;
    distance = 0;

    current = start;
    while (true) {
        ++distance;
        for (int x = -max_cheat_time; x <= max_cheat_time; x++) {
            for (int y = -max_cheat_time; y <= max_cheat_time; y++) {
                auto cheat_time = std::abs(x) + std::abs(y);
                if (cheat_time > max_cheat_time || cheat_time <= 1) {
                    continue;
                }
                Vec2 next_cheat = current + Vec2 { x, y };
                if (next_cheat.x < 0
                    || next_cheat.x >= static_cast<int>(map[0].size())
                    || next_cheat.y < 0
                    || next_cheat.y >= static_cast<int>(map.size())) {
                    continue;
                }
                auto time_saved = distances[static_cast<size_t>(next_cheat.y)]
                                           [static_cast<size_t>(next_cheat.x)]
                    - distance - cheat_time + 1;
                if (time_saved >= 100) {
                    ++total;
                }
            }
        }
        for (auto dir : DIRECTIONS) {

            Vec2 next = current + dir;
            if (!map[static_cast<size_t>(next.y)][static_cast<size_t>(next.x)]
                || distances[static_cast<size_t>(next.y)]
                            [static_cast<size_t>(next.x)]
                    < distances[static_cast<size_t>(current.y)]
                               [static_cast<size_t>(current.x)]) {
                continue;
            }

            if (next == end) {
                return total;
            }

            current = next;
            break;
        }
    }
}

} // namespace

void day_20(const char* input)
{
    std::cout << "Day 20: " << std::endl;

    Vec2 start { 0, 0 };
    Vec2 end { 0, 0 };
    vector<vector<bool>> map;
    parse_input(input, map, start, end);

    std::cout << "Part 1: " << solve(map, start, end, 2) << std::endl;
    std::cout << "Part 2: " << solve(map, start, end, 20) << std::endl;
}
