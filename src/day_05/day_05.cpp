
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::list;
using std::map;
using std::size_t;
using std::vector;

using PageRule = map<int, vector<int>>;

void read_input(
    const char* input_file, PageRule& rules, vector<list<int>>& input)
{
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cout << "File could not be opened" << input_file << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> split;
    while (std::getline(file, line) && line != "") {
        boost::split(split, line, boost::is_any_of("|"));

        int left_page = std::stoi(split[0]);
        int right_page = std::stoi(split[1]);

        rules[left_page].push_back(right_page);
    }

    while (std::getline(file, line)) {
        boost::split(split, line, boost::is_any_of(","));

        list<int> row;
        for (const auto& token : split) {
            row.push_back(std::stoi(token));
        }

        input.push_back(row);
    }
}

int get_middle(const list<int>& list)
{
    auto slow = list.begin();
    auto fast = list.begin();

    while (fast != list.end() && ++fast != list.end()) {
        ++slow;
        ++fast;
    }

    return *slow;
}

// A|B
//
// If A and B are on the same list, that means B must be printed after A.
// In other words, when finding A, there cannot be any B before it.
//
// To fix an incorrect one, for example
// A|B
// B,A,C
// We need to take the wrong one (prev_page) and place it after the current one
void part_1(PageRule& rules, vector<list<int>> input)
{

    int correct_total = { 0 };
    int wrong_total = { 0 };
    for (auto list : input) {
        bool correct { true };

        for (auto it = list.begin(); it != list.end(); it++) {
            int page = *it;

            if (rules[page].size() > 0) {
                for (const auto prohibited_prev_page : rules[page]) {
                    for (auto prev_it = list.begin(); prev_it != it;) {
                        int prev_page = *prev_it;
                        if (prev_page == prohibited_prev_page) {
                            correct = false;

                            auto next_prev_it = std::next(prev_it);
                            list.splice(std::next(it), list, prev_it);
                            prev_it = next_prev_it;
                        } else {
                            ++prev_it;
                        }
                    }
                }
            }
        }

        if (correct) {
            correct_total += get_middle(list);
        } else {
            wrong_total += get_middle(list);
        }
    }

    std::cout << "Result: " << correct_total << std::endl;
    std::cout << "Result: " << wrong_total << std::endl;
}

} // namespace

void day_05(const char* input_file)
{
    std::cout << "Day 5" << std::endl;

    PageRule rules;
    vector<list<int>> input;
    read_input(input_file, rules, input);

    part_1(rules, input);
}
