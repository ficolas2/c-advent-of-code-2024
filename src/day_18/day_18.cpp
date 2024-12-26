#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

#include "../common/vector2.h"

namespace {
using std::vector;
using Vec2 = Vector2<int>;
using std::priority_queue;

const int SIZE = 71;

const Vec2 DIRECTIONS[4] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };

vector<Vec2> parse_input(const char* input)
{
    std::ifstream file(input);
    vector<Vec2> falling_bytes;

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return falling_bytes;
    }

    std::string line;
    while (std::getline(file, line)) {
        auto pos = line.find(',');
        int x = std::stoi(line.substr(0, pos));
        int y = std::stoi(line.substr(pos + 1));
        falling_bytes.push_back({ x, y });
    }

    return falling_bytes;
}

struct PathHead {
    Vec2 pos;
    int cost;
    Vec2 dest;

    PathHead(Vec2 p, int c, Vec2 d)
        : pos(p)
        , cost(c)
        , dest(d)
    {
    }

    bool operator<(const PathHead& other) const { return cost > other.cost; }
};

int get_cost(const vector<vector<bool>>& grid)
{
    Vec2 dest = { 70, 70 };
    PathHead path = { { 0, 0 }, 0, dest };

    vector<vector<bool>> visited(SIZE, vector(SIZE, false));
    priority_queue<PathHead, std::vector<PathHead>, std::less<PathHead>> pq;
    pq.push({ { 0, 0 }, 0, dest });

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        if (current.pos == dest) {
            path = current;
            break;
        }

        for (const auto& dir : DIRECTIONS) {
            auto next = current.pos + dir;
            if (next.x < 0 || next.x >= SIZE || next.y < 0 || next.y >= SIZE) {
                continue;
            }
            if (visited[static_cast<size_t>(next.y)]
                       [static_cast<size_t>(next.x)]) {
                continue;
            }
            if (grid[static_cast<size_t>(next.y)]
                    [static_cast<size_t>(next.x)]) {
                pq.push({ next, current.cost + 1, dest });
                visited[static_cast<size_t>(next.y)]
                       [static_cast<size_t>(next.x)]
                    = true;
            }
        }
    }

    return path.cost;
}

void part_1(const vector<Vec2>& falling_bytes)
{
    vector<vector<bool>> grid = vector(SIZE, vector(SIZE, true));
    size_t i = 0;
    for (; i < 1024; i++) {
        Vec2 fb = falling_bytes[i];
        grid[static_cast<size_t>(fb.y)][static_cast<size_t>(fb.x)] = false;
    }

    std::cout << "Part 1: " << get_cost(grid) << std::endl;
}

void part_2(const vector<Vec2>& falling_bytes)
{
    vector<vector<bool>> grid = vector(SIZE, vector(SIZE, true));
    size_t i = 0;
    while (true) {
        Vec2 fb = falling_bytes[i];
        grid[static_cast<size_t>(fb.y)][static_cast<size_t>(fb.x)] = false;
        if (get_cost(grid) == 0) {
            break;
        }
        i++;
    }

    std::cout << "Part 2: " << falling_bytes[i].x << "," << falling_bytes[i].y
              << std::endl;
}

} // namespace

void day_18(const char* input)
{
    std::cout << "Day 18: " << std::endl;
    auto falling_bytes = parse_input(input);

    part_1(falling_bytes);
    part_2(falling_bytes);
}
