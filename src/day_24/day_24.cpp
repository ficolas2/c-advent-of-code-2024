#include <fstream>
#include <iostream>
#include <map>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

namespace {
using std::map;
using std::string;
using std::vector;
using schlong = long long;

struct gate {
    string op;
    string input1;
    string input2;
    string output;
};

const int INPUT_BITS = 45;
const int OUTPUT_BITS = INPUT_BITS + 1;

void parse_input(const char* input, map<string, gate>& gates,
    map<string, bool>& wires)
{
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return;
    }

    string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }

        auto wire = line.substr(0, 3);
        auto val = stoi(line.substr(5));

        wires[wire] = val;
    }

    vector<string> tokens;
    while (std::getline(file, line)) {
        boost::split(tokens, line, boost::is_space());

        auto input1 = tokens[0];
        auto op = tokens[1];
        auto input2 = tokens[2];

        auto output = tokens[4];

        gates[output] = { op, input1, input2, output };
    }

}

bool calculate(const map<string, gate>& gates, map<string, bool>& wires,
    const string& output)
{
    if (wires.find(output) != wires.end()) {
        return wires[output];
    }

    auto& gate = gates.at(output);

    bool input1 = calculate(gates, wires, gate.input1);
    bool input2 = calculate(gates, wires, gate.input2);

    if (gate.op == "AND") {
        wires[output] = input1 & input2;
    } else if (gate.op == "OR") {
        wires[output] = input1 | input2;
    } else if (gate.op == "XOR") {
        wires[output] = input1 ^ input2;
    }

    return wires[output];
}

void part_1(const map<string, gate>& gates, map<string, bool> wires)
{
    schlong result = 0;
    for (int i = 0; i < OUTPUT_BITS; i++) {
        string output = "z";
        if (i < 10) {
            output += "0";
        }
        output += std::to_string(i);
        schlong val = calculate(gates, wires, output);

        result += val << std::stoll(output.substr(1));
    }
    std::cout << "Part 1: " << result << std::endl;
}

bool find_gate_with_input(string op, string gate_id, map<string, gate>& gates)
{
    for (auto [_, next_gate] : gates) {
        if (next_gate.op == op
            && (next_gate.input1 == gate_id || next_gate.input2 == gate_id)) {
            return true;
        }
    }
    return false;
}

bool is_circuit_input(gate& gate)
{
    return (gate.input1[0] == 'x' && gate.input2[0] == 'y')
        || (gate.input1[0] == 'y' && gate.input2[0] == 'x');
}

void part_2(map<string, gate>& gates)
{
    vector<string> faulty_gates;
    for (auto [gate_id, gate] : gates) {
        // circuit output, other than the last bit, should come from a XOR
        // (Last bit comes from the carry, so its an OR)
        // These are all the rules for circuit output.
        if (gate_id[0] == 'z') {
            if (gate_id != "z45") {
                if (gate.op != "XOR") {
                    faulty_gates.push_back(gate_id);
                }
            }
            continue;
        }
        // XOR gates that arent circuit inputs, and arent outputs should not
        // exist
        if (gate.op == "XOR") {
            if (gate.input1[0] != 'x' && gate.input1[0] != 'y'
                && gate.input2[0] != 'x' && gate.input2[0] != 'y') {
                faulty_gates.push_back(gate_id);
            }
        }
        // XOR gates with inputs x, y, have to output into a XOR gate
        if (gate.op == "XOR" && is_circuit_input(gate)) {
            if (!find_gate_with_input("XOR", gate_id, gates)) {
                faulty_gates.push_back(gate_id);
            }
        }
        // AND gates other than x00 AND y00, should output into an OR gate
        if (gate.op == "AND"
            && ((gate.input1 != "x00" && gate.input1 != "y00")
                || (gate.input2 != "x00" && gate.input2 != "y00"))) {
            if (!find_gate_with_input("OR", gate_id, gates)) {
                faulty_gates.push_back(gate_id);
            }
        }
    }

    std::sort(faulty_gates.begin(), faulty_gates.end());
    for (size_t i = 0; i < faulty_gates.size(); i++) {
        if (i != 0) {
            std::cout << ",";
        }
        std::cout << "Part 2: "<< faulty_gates[i];
    }
}

} // namespace

void day_24(const char* input)
{
    map<string, gate> gates;
    map<string, bool> wires;
    vector<string> output_list;

    parse_input(input, gates, wires);

    part_1(gates, wires);
    part_2(gates);
}
