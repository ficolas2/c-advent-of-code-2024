#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "../common/vector2.h"

namespace {
using std::tuple;
using std::vector;
using Vec2 = Vector2<double>;

const int A_PRICE = 3;
const int B_PRICE = 1;
const double PART_2_OFFSET = 10000000000000;

vector<tuple<Vec2, Vec2, Vec2>> read_input(const char* input)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return {};
    }

    vector<tuple<Vec2, Vec2, Vec2>> result;

    std::string line;
    while (std::getline(file, line)) {
        // Button A: X+79, Y+26
        // Button B: X+61, Y+99
        // Prize: X=7544, Y=9586
        line = line.substr(12);
        auto pos = line.find(", Y+");
        auto x = line.substr(0, pos);
        auto y = line.substr(pos + 4);
        auto a = Vec2(std::stod(x), std::stod(y));

        std::getline(file, line);
        line = line.substr(12);
        pos = line.find(", Y+");
        x = line.substr(0, pos);
        y = line.substr(pos + 4);
        auto b = Vec2(std::stod(x), std::stod(y));

        std::getline(file, line);
        line = line.substr(9);
        pos = line.find(", Y=");
        x = line.substr(0, pos);
        y = line.substr(pos + 4);
        auto p = Vec2(std::stod(x), std::stod(y));

        std::getline(file, line);

        result.push_back(std::make_tuple(a, b, p));
    }

    return result;
}

// Three vectors, A, B and P
// A.x * a + b.x * b = P.x
// A.y * a + b.y * b = P.y
//
// Solving for a and b:
//
// b = (A.x / A.y * P.y - P.x) / (A.x / A.y * B.y - B.x)
// a = (B.x / B.y * P.y - P.x) / (B.x / B.y * A.y - A.x)
void solve(vector<tuple<Vec2, Vec2, Vec2>>& input, bool part_2)
{
    long total = 0;
    for (auto& [a, b, p] : input) {
        if (part_2) {
            p.x += PART_2_OFFSET;
            p.y += PART_2_OFFSET;
        }
        auto b_val = (static_cast<double>(a.x) / a.y * p.y - p.x)
            / (static_cast<double>(a.x) / a.y * b.y - b.x);
        auto a_val = (static_cast<double>(b.x) / b.y * p.y - p.x)
            / (static_cast<double>(b.x) / b.y * a.y - a.x);

        if (std::abs(a_val - std::round(a_val)) > 0.0001) {
            continue;
        }
        if (std::abs(b_val - std::round(b_val)) > 0.0001) {
            continue;
        }

        total += static_cast<long>(std::round(a_val)) * A_PRICE + static_cast<long>(std::round(b_val)) * B_PRICE;
    }

    std::cout << "Total: " << total << std::endl;
}

} // namespace

void day_13(const char* input_file) {
    std::cout << "Day 13:" << std::endl;

    auto input = read_input(input_file);

    solve(input, false);
    solve(input, true);
    
}
