#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

namespace {
using InputType = std::vector<std::vector<int>>;

InputType read_input(const char* input_file)
{
    InputType input;

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cout << "File could not be opened" << input_file << std::endl;
        return input;
    }

    std::string line;
    std::vector<std::string> tokens;
    while (std::getline(file, line)) {
        boost::split(tokens, line, boost::is_space(), boost::token_compress_on);

        std::vector<int> row;
        for (const auto& token : tokens) {
            row.push_back(std::atoi(token.c_str()));
        }

        input.push_back(row);
    }

    return input;
}

bool check_row(const std::vector<int>& row, int skip)
{
    int prev { 0 };
    bool increasing;
    bool first { true };
    for (std::size_t i = 0; i < row.size(); i++) {
        if (static_cast<int>(i) == skip) {
            continue;
        }

        const int current = row[i];

        if (prev == 0 && first) {
            prev = current;
            continue;
        }
        if (first) {
            increasing = current > prev;
            first = false;
        }

        if ((increasing && current <= prev) || (!increasing && current >= prev)
            || std::abs(current - prev) > 3) {
            return false;
        }
        prev = current;
    }
    return true;
}

int solve(const InputType& input, bool tolerance)
{
    int safe { 0 };

    for (const std::vector<int>& row : input) {

        if (tolerance) {
            for (std::size_t i = 0; i < row.size(); i++) {
                if (check_row(row, static_cast<int>(i))) {
                    safe++;
                    break;
                }
            }
        } else {
            if (check_row(row, -1)) {
                safe += check_row(row, -1);
            }
        }
    }

    return safe;
}
}

void day_02(const char* input_file)
{
    std::cout << "Day 02: " << std::endl;
    auto input = read_input(input_file);

    std::cout << "Result: " << solve(input, false) << std::endl;
    std::cout << "Result: " << solve(input, true) << std::endl;
}
