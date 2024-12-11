#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::list;
using std::unordered_map;
using std::vector;

// just in case I have to replace it to a larger type
using int_t = long;

list<int_t> read_input(const char* input)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return {};
    }

    list<int_t> result;

    std::string line;
    vector<std::string> tokens;

    std::getline(file, line);
    boost::split(tokens, line, boost::is_any_of(" "));

    for (const auto& token : tokens) {
        result.push_back(std::stol(token));
    }

    return result;
}

// stone number, step
std::unordered_map<int_t, std::unordered_map<int, int_t>> memo;
int_t get_stones_at_step(int_t stone, int step)
{
    if (step == 0) {
        return 1;
    }

    if (memo[stone].find(step) != memo[stone].end()) {
        return memo[stone][step];
    }

    if (stone == 0) {
        return memo[stone][step] = get_stones_at_step(1, step - 1);
    }

    auto digits = static_cast<int_t>((log10(static_cast<double>(stone))) + 1);
    if (digits % 2 == 0) {
        auto half_order_of_magnitude = static_cast<int_t>(
            round(pow(10, static_cast<double>(digits) / 2)));
        auto left = stone / half_order_of_magnitude;
        auto right = stone % half_order_of_magnitude;

        return memo[stone][step] = get_stones_at_step(left, step - 1)
            + get_stones_at_step(right, step - 1);
    }

    return memo[stone][step] = get_stones_at_step(stone * 2024, step - 1);
}

void part_2(list<int_t> stones, int interactions)
{
    int_t total { 0 };

    for (auto stone : stones) {
        total += get_stones_at_step(stone, interactions);
    }

    std::cout << "Total: " << total << std::endl;
}

} // namespace

void day_11(const char* input)
{
    std::cout << "Day 11: " << std::endl;

    list<int_t> input_data = read_input(input);

    // part_1(input_data, 25);
    part_2(input_data, 25);
    part_2(input_data, 75);
}
