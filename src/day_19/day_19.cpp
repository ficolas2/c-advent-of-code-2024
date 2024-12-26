#include <boost/algorithm/string/compare.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/mpl/assert.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::map;
using std::string;
using std::vector;

void parse_input(
    const char* input, vector<string>& towels, vector<string>& dessigns)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return;
    }

    string line;
    std::getline(file, line);
    boost::split(
        towels, line, boost::is_any_of(", "), boost::token_compress_on);

    std::getline(file, line);

    while (std::getline(file, line)) {
        dessigns.push_back(line);
    }

    file.close();
}

map<string, map<size_t, long>> memo;
long check_dessign(
    const vector<string>& towels, const string& dessign, size_t pos = 0)
{
    if (memo[dessign].find(pos) != memo[dessign].end()) {
        return memo[dessign][pos];
    }
    long total = 0;
    for (const auto& towel : towels) {
        if (towel.size() + pos > dessign.size()) {
            continue;
        }

        if (dessign.compare(pos, towel.size(), towel) == 0) {
            auto next_pos = pos + towel.size();
            if (next_pos == dessign.size()) {
                total += 1;
            } else {
                total += check_dessign(towels, dessign, next_pos);
            }
        }
    }
    memo[dessign][pos] = total;
    return total;
}

} // namespace

void day_19(const char* input)
{
    std::cout << "Day 19: " << std::endl;

    vector<string> towels;
    vector<string> dessigns;
    parse_input(input, towels, dessigns);

    long total_possibilities = 0;
    long total_dessigns = 0;
    for (const auto& dessign : dessigns) {
        auto amount = check_dessign(towels, dessign);
        if (amount > 0) {
            total_possibilities += amount;
            ++total_dessigns;
        }
    }

    std::cout << "Part 1: " << total_dessigns << std::endl;
    std::cout << "Part 2: " << total_possibilities << std::endl;
}
