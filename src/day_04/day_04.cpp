#include <fstream>
#include <iostream>
#include <vector>

namespace {
using std::size_t;

using InputType = std::vector<std::vector<char>>;

const char word[] = { 'X', 'M', 'A', 'S' };
const int word_size = sizeof(word) / sizeof(word[0]);

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
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        input.push_back(row);
    }

    return input;
}

bool check_dir(int x, int y, int dir_x, int dir_y, const InputType& input)
{
    for (int i = 1; i < word_size; i++) {
        int checking_x = x + dir_x * i;
        int checking_y = y + dir_y * i;

        if (checking_x < 0 || checking_y < 0
            || static_cast<size_t>(checking_x) >= input[0].size()
            || static_cast<size_t>(checking_y) >= input.size()) {
            return false;
        }

        if (input[static_cast<size_t>(checking_y)]
                 [static_cast<size_t>(checking_x)]
            != word[i]) {
            return false;
        }
    }
    return true;
}

int check_cell(size_t x, size_t y, const InputType& input)
{
    int total { 0 };
    for (int dir_x = -1; dir_x <= 1; ++dir_x) {
        for (int dir_y = -1; dir_y <= 1; ++dir_y) {
            if (dir_x == 0 && dir_y == 0) {
                continue;
            }

            if (check_dir(static_cast<int>(x), static_cast<int>(y), dir_x,
                    dir_y, input)) {
                total++;
            }
        }
    }
    return total;
}

void part_1(InputType input)
{
    int total { 0 };

    for (size_t y = 0; y < input.size(); ++y) {
        for (size_t x = 0; x < input[y].size(); ++x) {
            if (input[y][x] != word[0]) {
                continue;
            }

            total += check_cell(x, y, input);
        }
    }

    std::cout << "Part 1: " << total << std::endl;
}

void part_2(InputType input)
{
    int total { 0 };

    for (size_t y = 1; y < input.size() - 1; ++y) {
        for (size_t x = 1; x < input[y].size() - 1; ++x) {
            if (input[y][x] != 'A') {
                continue;
            }

            char c1 = input[y + 1][x - 1]; // c1  c2
            char c2 = input[y + 1][x + 1]; //   --
            char c3 = input[y - 1][x - 1]; // c3  c4
            char c4 = input[y - 1][x + 1]; // c1 with c4, c2 with c3

            if ((c1 != 'M' && c1 != 'S') || (c2 != 'M' && c2 != 'S')
                || (c3 != 'M' && c3 != 'S') || (c4 != 'M' && c4 != 'S')) {
                continue;
            }

            if (c1 != c4 && c2 != c3) {
                total++;
            }
        }
    }
    std::cout << "Part 2: " << total << std::endl;
}
}

void day_04(const char* input_file)
{
    std::cout << "Day 04: " << std::endl;
    auto input = read_input(input_file);
    part_1(input);
    part_2(input);
}
