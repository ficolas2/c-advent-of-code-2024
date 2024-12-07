#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::vector;

struct Equation {
    long result;
    vector<long> numbers;
};

// result: number1 number2 number3
// Ej:
// 24: 2 3 4
vector<Equation> parse_input(const char* input_file)
{
    vector<Equation> equations;

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Could not open file " << input_file << std::endl;
        return equations;
    }

    std::string line;
    vector<std::string> tokens;
    while (std::getline(file, line)) {
        boost::split(
            tokens, line, boost::is_any_of(": "), boost::token_compress_on);

        long result = std::stol(tokens[0]);

        vector<long> numbers;
        for (size_t i = 1; i < tokens.size(); i++) {
            numbers.push_back(std::stol(tokens[i]));
        }

        equations.push_back({ result, numbers });
    }

    return equations;
}

long concat_long(long a, long b)
{
    assert (a > 0 && b > 0);
    if (b == 0) {
        return a * 10;
    }

    long digits = static_cast<long>((log10(static_cast<double>(b))) + 1);

    return a * static_cast<long>(pow(10, static_cast<double>(digits))) + b;
}

bool check_equation(vector<long>::const_iterator current,
    const vector<long>::const_iterator end, long current_total, long result, bool concat)
{
    // The result only goes up, so simple way to cut the calculations. 
    if (current_total > result) {
        return false;
    }

    auto next = std::next(current);
    if (next == end) {
        if (current_total == result) {
            return true;
        }
        return false;
    }

    if (check_equation(next, end, current_total + *next, result, concat)) {
        return true;
    }
    if (check_equation(next, end, current_total * *next, result, concat)) {
        return true;
    }
    if (concat && check_equation(next, end, concat_long(current_total, *next), result, concat)) {
        return true;
    }

    return false;
}

long solve(const vector<Equation>& equations, bool part_2 = false)
{
    long total { 0 };

    for (const auto& equation : equations) {
        if (check_equation(equation.numbers.begin(), equation.numbers.end(),
                equation.numbers[0], equation.result, part_2)) {
            total += equation.result;
        }
    }

    return total;
}
}

void day_07(const char* input_file)
{
    std::cout << "Day 07" << std::endl;
    auto equations = parse_input(input_file);

    std::cout<< "Part 1: " << solve(equations) << std::endl;
    std::cout<< "Part 2: " << solve(equations, true) << std::endl;
}
