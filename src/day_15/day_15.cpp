#include <fstream>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>

#include "../common/vector2.h"

#define ANIMATE 5
#ifdef ANIMATE
#include <thread>
#endif

namespace {
using std::tuple;
using std::vector;
using Vec2 = Vector2<int>;
using std::set;

tuple<vector<vector<char>>, vector<Vec2>, Vec2> parse_input(const char* input)
{
    std::ifstream file(input);
    vector<vector<char>> map;
    vector<Vec2> movements;
    Vec2 char_pos = { 0, 0 };

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return std::make_tuple(map, movements, char_pos);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }
        vector<char> row;
        for (char c : line) {
            if (c == '@') {
                char_pos = Vec2(
                    static_cast<int>(row.size()), static_cast<int>(map.size()));
                c = '.';
            }
            row.push_back(c);
        }
        map.push_back(row);
    }

    while (std::getline(file, line)) {
        for (char c : line) {
            switch (c) {
            case '>':
                movements.push_back({ 1, 0 });
                break;
            case '<':
                movements.push_back({ -1, 0 });
                break;
            case '^':
                movements.push_back({ 0, -1 });
                break;
            case 'v':
                movements.push_back({ 0, 1 });
                break;
            }
        }
    }

    return std::make_tuple(map, movements, char_pos);
}

[[maybe_unused]] void print_map(vector<vector<char>> map, Vec2 char_pos)
{
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (char_pos.x == static_cast<int>(x)
                && char_pos.y == static_cast<int>(y)) {
                std::cout << '@';
            } else {
                std::cout << map[y][x];
            }
        }
        std::cout << "\n";
    }
    std::cout << "--------------------" << std::endl;
}

void part_1(
    vector<vector<char>> map, const vector<Vec2>& movement_list, Vec2 char_pos)
{
    for (auto movement : movement_list) {
        auto next_char_pos = char_pos + movement;
        auto c = map[static_cast<size_t>(next_char_pos.y)][static_cast<size_t>(next_char_pos.x)];
        if (c == '.') {
            char_pos = next_char_pos;
        } else if (c == 'O') {
            auto next_pos = next_char_pos;
            while (true) {
                next_pos = next_pos + movement;
                c = map[static_cast<size_t>(next_pos.y)][static_cast<size_t>(next_pos.x)];
                if (c == '.') {
                    char_pos = next_char_pos;
                    map[static_cast<size_t>(next_char_pos.y)]
                       [static_cast<size_t>(next_char_pos.x)]
                        = '.';
                    map[static_cast<size_t>(next_pos.y)]
                       [static_cast<size_t>(next_pos.x)]
                        = 'O';
                    break;
                }
                if (c == '#') {
                    break;
                }
            }

        } // else, dont change the position, its a wall #

#ifdef ANIMATE
        print_map(map, char_pos);
        std::this_thread::sleep_for(std::chrono::milliseconds(ANIMATE));
#endif
    }

    long long total = 0;
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (map[y][x] == 'O') {
                total += y * 100 + x;
            }
        }
    }

    std::cout << "Result: " << total << std::endl;
}

vector<vector<char>> expand_map(const vector<vector<char>>& map)
{
    vector<vector<char>> expanded_map;
    for (size_t y = 0; y < map.size(); y++) {
        vector<char> row;
        for (size_t x = 0; x < map[0].size(); x++) {
            char c = map[y][x];
            switch (c) {
            case '@':
                row.push_back('@');
                row.push_back('.');
                break;
            case 'O':
                row.push_back('[');
                row.push_back(']');
                break;
            default:
                row.push_back(c);
                row.push_back(c);
                break;
            }
        }
        expanded_map.push_back(row);
    }
    return expanded_map;
}

void push_char(
    vector<vector<char>>& map, Vec2 push_pos, Vec2 push_dir, char pushed_char)
{
    map[static_cast<size_t>(push_pos.y - push_dir.y)]
       [static_cast<size_t>(push_pos.x - push_dir.x)]
        = '.';
    map[static_cast<size_t>(push_pos.y)][static_cast<size_t>(push_pos.x)]
        = pushed_char;
}

bool push(vector<vector<char>>& map, Vec2 push_pos, Vec2 push_dir,
    set<Vec2> pushed, char pushed_char)
{
    if (pushed.find(push_pos) != pushed.end()) {
        return true;
    }
    pushed.insert(push_pos);
    auto c
        = map[static_cast<size_t>(push_pos.y)][static_cast<size_t>(push_pos.x)];

    if (c == '.') {
        push_char(map, push_pos, push_dir, pushed_char);
        return true;
    }
    if (c == '#') {
        return false;
    }
    if (c == '[' || c == ']') {
        if (push_dir.x != 0) {
            auto next_push_pos = push_pos + push_dir;
            if (push(map, next_push_pos, push_dir, pushed, c)) {
                push_char(map, push_pos, push_dir, pushed_char);
            } else {
                return false;
            }
        } else {
            Vec2 pair_pos = push_pos;
            pair_pos.x += c == '[' ? 1 : -1;
            pushed.insert(pair_pos);

            auto next_push_pos = push_pos + push_dir;
            auto next_pair_pos = pair_pos + push_dir;

            if (push(map, next_push_pos, push_dir, pushed, c)
                && push(map, next_pair_pos, push_dir, pushed,
                    map[static_cast<size_t>(pair_pos.y)]
                       [static_cast<size_t>(pair_pos.x)])) {
                push_char(map, push_pos, push_dir, pushed_char);
                return true;
            } else {
                return false;
            }
        }
    }
    return true;
}

void part_2(
    vector<vector<char>> map, const vector<Vec2>& movement_list, Vec2 char_pos)
{
    for (auto movement : movement_list) {
        auto next_char_pos = char_pos + movement;
        set<Vec2> pushed;
        auto map_copy = map;

        if (push(map, next_char_pos, movement, pushed, '.')) {
            char_pos = next_char_pos;
        } else {
            map = map_copy;
        }

#ifdef ANIMATE
        print_map(map, char_pos);
        std::this_thread::sleep_for(std::chrono::milliseconds(ANIMATE));
#endif
    }

    long long total = 0;
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (map[y][x] == '[') {
                total += y * 100 + x;
            }
        }
    }

    std::cout << "Result: " << total << std::endl;
}

} // namespace

void day_15(const char* input)
{
    std::cout << "Day 15:" << std::endl;
    auto [map, movements, char_pos] = parse_input(input);

    auto expanded_map = expand_map(map);
    auto expanded_char_pos = char_pos;
    expanded_char_pos.x *= 2;

    // part_1(map, movements, char_pos);
    part_2(expanded_map, movements, expanded_char_pos);
}
