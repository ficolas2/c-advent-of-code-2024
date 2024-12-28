#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace {
using std::map;
using std::set;
using std::string;
using std::vector;

vector<long> parse_input(const char* input)
{
    vector<long> numbers;
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return numbers;
    }

    string line;
    while (std::getline(file, line)) {
        numbers.push_back(std::stol(line));
    }

    file.close();
    return numbers;
}

long mix(long a, long b) { return a ^ b; }

long prune(long num) { return num % 16777216; }

long step(long num)
{
    num = mix(num, num * 64);
    num = prune(num);
    num = mix(num, num / 32);
    num = prune(num);
    num = mix(num, num * 2048);
    num = prune(num);

    return num;
}

// Using a long instead of a string to store the sequence, because its faster on hashmaps.
// Every step is a number from -9 to 9, so we can store it in two digits.
//
//    negative
//   V  V  V  V  
//   00 00 00 00
//    ^  ^  ^  ^
//     positive
long step_sequence(long sequence, long diff)
{
    sequence *= 100;
    sequence %= 100000000;
    if (diff < 0) {
        sequence += 10 * std::abs(diff);
    } else {
        sequence += diff;
    }

    return sequence;
}

void part_1(const vector<long>& numbers)
{
    long total = 0;
    for (auto starting_number : numbers) {
        auto number = starting_number;
        for (int i = 0; i < 2000; i++) {
            number = step(number);
        }

        total += number;
    }

    std::cout << "Part 1: " << total << std::endl;
}

void part_2(const vector<long>& numbers)
{
    vector<map<long, long>> sequence_results_list;
    map<long, long> sequence_banana_map;

    for (size_t num_i = 0; num_i < numbers.size(); num_i++) {
        set<long> read_sequences;
        auto num = numbers[num_i];
        auto prev_num = num;
        long sequence = 0;

        for (int i = 0; i < 2000; i++) {
            num = step(prev_num);
            sequence = step_sequence(sequence, num % 10 - prev_num % 10);

            // sequence is long enough and its the first appeareance
            if (i >= 3) {
                if (read_sequences.find(sequence) == read_sequences.end()) {
                    sequence_banana_map[sequence] += num % 10;
                }
                read_sequences.insert(sequence);
            }
            prev_num = num;
        }
    }

    long max_bananas = 0;
    for (auto [_, bananas] : sequence_banana_map) {
        if (bananas > max_bananas) {
            max_bananas = bananas;
        }
    }

    std::cout << "Part 2: " << max_bananas << std::endl;
}

} // namespace

void day_22(const char* input)
{
    std::cout << "Day 22: " << std::endl;

    auto numbers = parse_input(input);

    part_1(numbers);
    part_2(numbers);
}
