#include "day_01.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <csetjmp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {
using InputType = std::pair<std::vector<int>, std::vector<int>>;

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

        input.first.push_back(std::atoi(tokens[0].c_str()));
        input.second.push_back(std::atoi(tokens[1].c_str()));
    }

    return input;
}

void part_1(const InputType& input)
{
    int sum { 0 };
    for (std::size_t i = 0; i < input.first.size(); i++) {
        sum += std::abs(input.first[i] - input.second[i]);
    }

    std::cout << "Result: " << sum << std::endl;
}

void part_2(const InputType& input)
{
    int similarity { 0 };
    std::size_t i { 0 };
    std::size_t j { 0 };

    int current_matches { 0 };

    while (i < input.first.size() && j < input.second.size()) {
        int a = input.first[i];
        int b = input.second[j];

        if (a == b) {
            current_matches++;
            j++;
        } else if (b < a) {
            j++;
        } else {
            similarity += current_matches * a;
            i++;
            if (a != input.first[i])
                current_matches = 0;
        }
    }

    std::cout << "Result: " << similarity << std::endl;
}
} // namespace

void day_01(const char* input_file)
{
    std::cout << "Day 01:" << std::endl;
    InputType input = read_input(input_file);

    std::sort(input.first.begin(), input.first.end());
    std::sort(input.second.begin(), input.second.end());

    part_1(input);
    part_2(input);
}
