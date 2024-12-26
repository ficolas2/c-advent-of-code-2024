#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace {
using std::map;
using std::set;
using std::string;
using std::vector;

map<string, set<string>> parse_input(const char* input)
{
    map<string, set<string>> connections;
    std::ifstream file(input);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input << std::endl;
        return connections;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Ej:
        // aa-bb
        auto a = line.substr(0, 2);
        auto b = line.substr(3);

        connections[a].insert(b);
        connections[b].insert(a);
    }

    file.close();
    return connections;
}
set<string> set_intersection(set<string> a, set<string> b)
{
    set<string> result;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
        std::inserter(result, result.begin()));
    return result;
}


void part_1(map<string, set<string>>& connections)
{
    set<set<string>> cliques;
    for (const auto& [me, peer_list] : connections) {
        if (me[0] == 't') {

            for (const string& peer : peer_list) {
                auto peer_peers_list = connections[peer];

                set<string> intersection = set_intersection(peer_list, peer_peers_list);


                for (const string& intersection_peer : intersection) {
                    cliques.insert({ me, peer, intersection_peer });
                }
            }
        }
    }

    std::cout << "Part 1: " << cliques.size() << std::endl;
}

// R is the current clique
// P are the vertices that can still be added to the R clique
// X are the vertices that have already been added to the R clique
void BronKerbosch(set<string> R, set<string> P, set<string> X,
    const map<string, set<string>>& connections, set<set<string>>& cliques)
{
    if (P.empty() && X.empty()) {
        cliques.insert(R);
        return;
    }

    for (const string& v : P) {
        set<string> new_R = R;
        new_R.insert(v);
        auto neighbors = connections.at(v);

        BronKerbosch(new_R, set_intersection(P, neighbors),
            set_intersection(X, neighbors), connections, cliques);

        P.erase(v);
        X.insert(v);
    }
}

void part_2(map<string, set<string>>& connections)
{
    set<string> P;
    for (const auto& [node, _] : connections) {
        P.insert(node);
    }

    set<set<string>> cliques;
    BronKerbosch({}, P, {}, connections, cliques);

    size_t max_size = 0;
    set<string> largest_clique = {};
    for (const auto& clique : cliques) {
        if (clique.size() > max_size) {
            max_size = clique.size();
            largest_clique = clique;
        }
    }

    std::cout << "Part 2: ";

    vector<string> sorted_clique(largest_clique.begin(), largest_clique.end());
    std::sort(sorted_clique.begin(), sorted_clique.end());
    bool begin = true;
    for (const auto& node : sorted_clique) {
        if (begin) {
            begin = false;
        } else {
            std::cout << ",";
        }
        std::cout << node;
    }
}

} // namespace

void day_23(const char* input)
{
    std::cout << "Day 23: " << std::endl;

    auto connections = parse_input(input);

    part_1(connections);
    part_2(connections);
}
