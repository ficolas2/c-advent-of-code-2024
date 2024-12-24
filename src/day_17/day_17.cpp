#include <cmath>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <sys/types.h>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::vector;
using number_t = long long;

const size_t A = 0;
const size_t B = 1;
const size_t C = 2;

struct Computer {
    number_t registers[3];
    size_t pointer;
    vector<int> program;
    std::string program_str;
};

Computer parse_input(const char* input)
{
    std::ifstream file(input);

    Computer computer = { {}, 0, {}, "" };

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return computer;
    }

    std::string line;

    for (size_t i = 0; i < 3; i++) {
        std::getline(file, line);
        computer.registers[i] = std::stoi(line.substr(12));
    }
    std::getline(file, line);
    std::getline(file, line);
    computer.program_str = line.substr(9);

    vector<std::string> tokens;
    boost::split(tokens, computer.program_str, boost::is_any_of(","));
    for (const auto& token : tokens) {
        computer.program.push_back(std::stoi(token));
    }

    return computer;
}

std::string execute(Computer computer, bool debug = false)
{
    computer.pointer = 0;
    std::string output = "";
    do {

        auto opcode = computer.program[computer.pointer];
        auto literal = computer.program[computer.pointer + 1];
        number_t combo;

        if (literal <= 3) {
            combo = literal;
        } else {
            combo = computer.registers[literal - 4];
        }

        if (debug) {
            std::cout << "--------------------" << std::endl;
            std::cout << "Pointer: " << computer.pointer << std::endl;
            std::cout << "A: " << computer.registers[A] << std::endl;
            std::cout << "B: " << computer.registers[B] << std::endl;
            std::cout << "C: " << computer.registers[C] << std::endl;

            std::cout << "Instruction: " << opcode << std::endl;
            std::cout << "Literal: " << literal << std::endl;
            std::cout << "Combo: " << combo << std::endl;
        }

        assert(opcode <= 7 && opcode >= 0);
        switch (opcode) {
        case 0: // adv
            computer.registers[A] = computer.registers[A]
                / static_cast<number_t>(std::pow(2, combo));
            break;
        case 1: // bxl
            computer.registers[B] = computer.registers[B] ^ literal;
            break;
        case 2: // bst
            computer.registers[B] = combo % 8;
            break;
        case 3: // jnz
            if (computer.registers[A] != 0)
                computer.pointer = static_cast<size_t>(literal - 2);
            break;
        case 4: // bxc
            computer.registers[B]
                = computer.registers[B] ^ computer.registers[C];
            break;
        case 5: // out
            if (output.size() != 0) {
                output += ",";
            }
            output += std::to_string(combo % 8);
            if (output.size() > computer.program_str.size()) {
                return output;
            }
            if (debug) {
                std::cout << "----> Output: " << output << std::endl;
            }
            break;
        case 6: // bdv
            computer.registers[B] = computer.registers[A]
                / static_cast<number_t>(std::pow(2, combo));
            break;
        case 7:
            computer.registers[C] = computer.registers[A]
                / static_cast<number_t>(std::pow(2, combo));
            break;
        }

        computer.pointer += 2;
    } while (computer.pointer < computer.program.size());

    return output;
}

// This function makes several assumptions from the dataset:
// - There is a loop that runs from IP=0 to IP=14, whenever A>0.
// - A gets divided by 8 every loop.
number_t reverse_run(Computer computer, size_t iteration = 0)
{
    // Peform the reverse IP=14 operation, which is the one that changes A.
    auto denominator = 8; // Hardcoded from IP = 10
    auto prev_A = computer.registers[A];
    // Run through all possible values of A that would result in that
    // integer division
    for (int i = 0; i < denominator; i++) {
        computer.registers[A] = prev_A * denominator + i;
        // Run the program forwards. If the output is the wanted one, continue
        // running backwards
        auto out = execute(computer);
        if (out.size() != iteration * 2 + 1) {
            continue;
        }
        if (out == computer.program_str) {
            return computer.registers[A];
        }
        auto program_ending = computer.program_str.substr(
            computer.program_str.length() - (iteration * 2 + 1));
        if (out == program_ending) {
            auto res = reverse_run(computer, iteration + 1);
            if (res != 0) {
                return res;
            }
        }
    }
    return 0;
}

} // namespace

void day_17(const char* input)
{
    std::cout << "Day 16: " << std::endl;
    auto computer = parse_input(input);

    auto out = execute(computer);
    std::cout << "Part 1: " << out << std::endl;

    computer.registers[A] = 0;
    std::cout << "Part 2: " << reverse_run(computer) << std::endl;
}
