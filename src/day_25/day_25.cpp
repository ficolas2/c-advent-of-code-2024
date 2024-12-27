#include <fstream>
#include <iostream>
#include <vector>
#include <array>

namespace {
using std::vector;
using std::array;

void parse_input(const char* input, vector<array<int, 5>>& keys, vector<array<int, 5>>& locks)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        bool key = line[0] == '.';
        array<int, 5> arr { 5, 5, 5, 5, 5 };

        for (size_t y = 0; y < 5; y++) {
            std::getline(file, line);
            for (size_t x = 0; x < 5; x++) {
                if (line[x] == '.') {
                    --arr[x];
                }
            }
        }

        if (key) {
            keys.push_back(arr);
        } else {
            locks.push_back(arr);
        }
        std::getline(file, line);
        std::getline(file, line);
    }

    file.close();
}

void part_1(vector<array<int, 5>>& keys, vector<array<int, 5>>& locks)
{
    int total = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            for (size_t i = 0; i < 5; i++) {
                if (key[i] + lock[i] > 5) {
                    goto next;
                }
            }
            total++;

        next:;
        }
    }

    std::cout << "Part 1: " << total << std::endl;
}

} // namespace

void day_25(const char* input) { 
    std::cout << "Day 25: " << std::endl;

    vector<array<int, 5>> keys;
    vector<array<int, 5>> locks;
    parse_input(input, keys, locks);

    part_1(keys, locks);

}
