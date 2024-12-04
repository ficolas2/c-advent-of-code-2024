#include <fstream>
#include <iostream>
#include <regex>

namespace {
using InputType = std::vector<std::string>;

InputType read_input(const char* input_file)
{
    InputType input;

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cout << "File could not be opened" << input_file << std::endl;
        return input;
    }

    std::string line;
    while (std::getline(file, line)) {
        input.push_back(line);
    }

    return input;
}

void part_1(InputType input)
{
    std::regex pattern(R"_(mul\((\d+),(\d+)\))_");
    int total { 0 };

    for (const std::string& line : input) {
        for (std::sregex_iterator it(line.begin(), line.end(), pattern), end_it;
            it != end_it; ++it) {

            const std::smatch& match = *it;
            total += std::stoi(match[1].str()) * std::stoi(match[2].str());
        }
    }

    std::cout << "Part 1: " << total << std::endl;
}

void part_2(InputType input)
{
    std::regex pattern(R"_((do(?:n't)?)|mul\((\d+),(\d+)\))_");

    bool enabled { true };
    int total { 0 };

    for (const std::string& line : input) {

        for (std::sregex_iterator it(line.begin(), line.end(), pattern), end_it;
            it != end_it; ++it) {

            const std::smatch& match = *it;

            if (match[1].str() == "do") {
                enabled = true;
            } else if (match[1].str() == "don't") {
                enabled = false;
            } else if (enabled) {
                total += std::stoi(match[2].str()) * std::stoi(match[3].str());
            }
        }
    }
    std::cout << "Part 2: " << total << std::endl;
}

} // namespace

void day_03(const char* input_file)
{
    std::cout << "Day 02: " << std::endl;
    auto input = read_input(input_file);
    part_1(input);
    part_2(input);
}
