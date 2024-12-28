#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../common/vector2.h"

namespace {
using std::map;
using std::pair;
using std::string;
using std::vector;
using schlong = long long;
using Vec2 = Vector2<int>;


// clang-format off
map<char, Vec2> DIR_map = { 
    { '^', { 0, -1 } }, { '>', { 1, 0 } },
    { '<', { -1, 0 } }, { 'V', { 0, 1 } } };

const char NUMPAD[4][3] = { 
    { '7', '8', '9' }, 
    { '4', '5', '6' },
    { '1', '2', '3' }, 
    { ' ', '0', 'A' } };

const char KEYPAD[2][3] = { 
    { ' ', '^', 'A' }, 
    { '<', 'V', '>' } };

const char DIR_PRIORITY[4] = { '<', '^', 'V', '>' };
const char DIR_PRIORITY_REVERSED[4] = { '>', 'V', '^', '<' };
// clang-format on

const map<char, Vec2> get_number_map()
{
    map<char, Vec2> number_map;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            number_map[NUMPAD[y][x]] = Vec2(x, y);
        }
    }

    return number_map;
}

const map<char, Vec2> get_keypad_map()
{
    map<char, Vec2> keypad_map;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            keypad_map[KEYPAD[y][x]] = Vec2(x, y);
        }
    }

    return keypad_map;
}

const map<char, Vec2> NUMPAD_MAP = get_number_map();
const map<char, Vec2> KEYPAD_MAP = get_keypad_map();

int sign(int value)
{
    if (value > 0)
        return 1;
    if (value < 0)
        return -1;
    return 0;
}

vector<string> parse_input(const char* input)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return {};
    }

    vector<string> codes;
    string line;
    while (std::getline(file, line)) {
        codes.push_back(line);
    }

    return codes;
}

void add_sequence(string& sequence, Vec2 pos, Vec2 next_pos, Vec2 death_button,
    bool reversed = false)
{
    string addition = "";
    Vec2 shift = next_pos - pos;
    Vec2 current_pos = pos;
    for (char dir_char : reversed ? DIR_PRIORITY_REVERSED : DIR_PRIORITY) {
        Vec2 dir = DIR_map.at(dir_char);

        if (dir.x != 0 && dir.x == sign(shift.x)) {
            current_pos.x = next_pos.x;
            for (int i = 0; i < abs(shift.x); i++) {
                addition.push_back(dir_char);
            }
        }
        if (dir.y != 0 && dir.y == sign(shift.y)) {
            current_pos.y = next_pos.y;
            for (int i = 0; i < abs(shift.y); i++) {
                addition.push_back(dir_char);
            }
        }
        if (current_pos == death_button) {
            add_sequence(sequence, pos, next_pos, death_button, true);
            return;
        }
    }

    sequence += addition;
}

string get_numpad_sequence(string code)
{
    string sequence = "";
    Vec2 pos = NUMPAD_MAP.at('A');

    for (char c : code) {
        Vec2 next_pos = NUMPAD_MAP.at(c);

        add_sequence(sequence, pos, next_pos, Vec2(0, 3));

        pos = next_pos;
        sequence.push_back('A');
    }

    return sequence;
}

// [sequence, depth] -> sequence length
map<pair<string, int>, schlong> memo;
schlong keypad_sequence_length(string sequence, int depth)
{
    if (depth == 0) {
        return static_cast<schlong>(sequence.size());
    }

    if (memo.find({ sequence, depth }) != memo.end()) {
        return memo.at({ sequence, depth });
    }

    schlong total = 0;

    Vec2 pos = KEYPAD_MAP.at('A');
    for (char c : sequence) {
        string new_sequence = "";
        Vec2 next_pos = KEYPAD_MAP.at(c);

        add_sequence(new_sequence, pos, next_pos, Vec2(0,0));

        pos = next_pos;
        new_sequence.push_back('A');
        total += keypad_sequence_length(new_sequence, depth - 1);
    }

    memo[{ sequence, depth }] = total;

    return total;
}

schlong solve(const vector<string>& codes, int depth) {
    schlong total = 0;
    for (const auto& code : codes) {
        auto sequence = get_numpad_sequence(code);
        auto num = std::atoll(code.substr(0, 3).c_str());
        auto seq_len = keypad_sequence_length(sequence, depth);

        total += num * seq_len;
    }
    return total;
}

} // namespace

void day_21(const char* input)
{
    std::cout << "Day 21: " << std::endl;

    auto codes = parse_input(input);

    std::cout << "Part 1: " << solve(codes, 2) << std::endl;
    std::cout << "Part 2: " << solve(codes, 25) << std::endl;
}
