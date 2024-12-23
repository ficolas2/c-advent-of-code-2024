#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

#include "../common/vector2.h"

namespace {
using std::tuple;
using std::vector;
using Vec2 = Vector2<int>;
using std::priority_queue;

const vector<Vec2> DIRECTIONS = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
const int MOVEMENT_COST = 1;
const int ROTATION_COST = 1000;

tuple<vector<vector<bool>>, Vec2, Vec2> parse_input(const char* input)
{
    std::ifstream file(input);

    vector<vector<bool>> map;
    Vec2 start = { 0, 0 };
    Vec2 end = { 0, 0 };

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return std::make_tuple(map, start, end);
    }

    std::string line;
    while (std::getline(file, line)) {
        vector<bool> row;
        for (char c : line) {
            switch (c) {
            case '#':
                row.push_back(true);
                break;
            case '.':
                row.push_back(false);
                break;
            case 'S':
                start = { static_cast<int>(row.size()),
                    static_cast<int>(map.size()) };
                row.push_back(false);
                break;
            case 'E':
                end = { static_cast<int>(row.size()),
                    static_cast<int>(map.size()) };
                row.push_back(false);
                break;
            }
        }
        map.push_back(row);
    }

    return std::make_tuple(map, start, end);
}

struct PathHead {
    Vec2 pos;
    Vec2 prev_pos;
    int cost;
    vector<vector<bool>> visited;

    PathHead(Vec2 p, Vec2 pp, int c, vector<vector<bool>> v)
        : pos(p)
        , prev_pos(pp)
        , cost(c)
        , visited(v)
    {
    }

    bool operator>(const PathHead& other) const { return cost > other.cost; }
};

void print_map(vector<vector<bool>> map, vector<vector<bool>> visited)
{

    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            if (visited[y][x]) {
                std::cout << 'O';
            } else if (map[y][x]) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << std::endl;
    }
}

void part_1(vector<vector<bool>> map, Vec2 start, Vec2 end)
{
    priority_queue<PathHead, std::vector<PathHead>, std::greater<PathHead>> pq;

    // Facing east, so thats equivalent to the previous possition being west.
    pq.push(PathHead(start, start - Vec2 { 1, 0 }, 0,
        vector<vector<bool>>(map.size(), vector<bool>(map[0].size(), false))));

    int best_path_cost = -1;
    vector<PathHead> best_paths;

    auto visited_cost = vector<vector<int>>(map.size(), vector<int>(map[0].size(), -1));

    while (pq.size() != 0) {
        auto pathHead = pq.top();
        auto [pos, prev_pos, cost, visited] = pathHead;
        pq.pop();
        visited[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)] = true;


        if (pos == end && (best_path_cost < 0 || cost == best_path_cost)) {
            best_path_cost = cost;
            best_paths.push_back(
                PathHead(pos, prev_pos, cost, visited));
            continue;
        }

        vector<PathHead> to_push;
        for (auto dir : DIRECTIONS) {
            auto next_cost = cost;
            auto next_pos = pos + dir;

            if (map[static_cast<size_t>(next_pos.y)][static_cast<size_t>(next_pos.x)])
                continue; // Wall
            if (visited[static_cast<size_t>(next_pos.y)][static_cast<size_t>(next_pos.x)])
                continue; // Visited
            if (best_path_cost > 0 && next_cost > best_path_cost)
                continue; // Not one of the best paths

            auto v = next_pos - prev_pos;
            if (v.x != 0 && v.y != 0) {
                next_cost += ROTATION_COST;
            } else {
            }
            next_cost += MOVEMENT_COST;

            to_push.push_back(PathHead(next_pos, pos, next_cost, visited));
        }

        if (to_push.size() > 1) {
            for (auto path : to_push) {
                pq.push(path);
            }
        } else if (to_push.size() == 1) {
            // Only checking in straight paths, because curves can have different costs depending
            // on what the previous path is. A bit botchy, but it works.
            auto& c = visited_cost[static_cast<size_t>(pos.y)][static_cast<size_t>(pos.x)];
            if (c == -1) {
                c = cost;
            } else if (c < cost) {
                continue; // Not one of the best paths
            }
            pq.push(to_push[0]);
        }
    }

    vector<vector<bool>> combined_visited (map.size(), vector<bool>(map[0].size(), false));
    int resting_spaces = 0;
    for (size_t y = 0; y < map.size(); y++) {
        for (size_t x = 0; x < map[0].size(); x++) {
            for (auto path : best_paths) {
                if (path.visited[y][x]) {
                    resting_spaces++;
                    combined_visited[y][x] = true;
                    break;
                }
            }
        }
    }



    print_map(map, combined_visited);
    std::cout << "Best paths: " << best_paths.size() << std::endl;
    std::cout << "Part 1: " << best_path_cost << std::endl;
    std::cout << "Part 2: " << resting_spaces << std::endl;
}

} // namespace

void day_16(const char* input)
{
    std::cout << "Day 15:" << std::endl;
    auto [map, start, end] = parse_input(input);

    part_1(map, start, end);
}
