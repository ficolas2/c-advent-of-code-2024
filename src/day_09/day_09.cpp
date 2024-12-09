#include <cassert>
#include <fstream>
#include <iostream>
#include <list>

namespace {
using std::list;
using std::size_t;

class Entry {

public:
    int id;
    int length;
    Entry(int id, int length)
        : id(id)
        , length(length)
    {
    }
    Entry(int length)
        : id(-1)
        , length(length)
    {
    }

    bool is_empty() const { return id == -1; }
    void set_empty() { id = -1; }
};

list<Entry> parse_input(const char* input_file)
{
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << input_file << std::endl;
        return {};
    }

    list<Entry> entries;
    std::string line;

    std::getline(file, line);
    int id = 0;
    bool empty = false;
    for (char c : line) {
        int length = std::atoi(&c);
        if (empty) {
            entries.push_back({ length });
            empty = false;
        } else {
            entries.push_back({ id, length });
            id++;
            empty = true;
        }
    }

    return entries;
}

long calculate_score(const list<Entry>& entries)
{
    long score = 0;
    long index = -1;
    for (const auto& entry : entries) {
        for (int entry_index = 0; entry_index < entry.length; entry_index++) {
            ++index;
            if (entry.is_empty())
                continue;
            score += index * entry.id;
        }
    }
    return score;
}

int sum_all_entries(const list<Entry>& entries)
{
    int sum = 0;
    for (const auto& entry : entries) {
        if (entry.is_empty())
            continue;
        sum += entry.id * entry.length;
    }
    return sum;
}

[[maybe_unused]] void print_entries(const list<Entry>& entries)
{
    for (const auto& entry : entries) {
        for (int i = 0; i < entry.length; i++) {
            if (entry.is_empty())
                std::cout << ".";
            else
                std::cout << entry.id;
        }
    }
    std::cout << std::endl;
}

//  Iterate though the list in both directions
//  When the iterators hit eachother, stop
//  When the begin_it direction hits an empty entry, insert an entry from the
//  end_it:
//      - If the begin_it entry is bigger, insert the end_it entry before the
//      begin_it entry,
//        and subtract the end_it entry length from the begin_it entry
//      - If they are equal, set the begin_it entry to the end_it entry id, and
//      set the
//        end_it to empty
//      - If the end_it entry is bigger, set the begin_it entry to the end_it
//      id, and subtract
//        the begin_it entry length from the end_it entry
void part_1(list<Entry> entries)
{
    auto begin_it = entries.begin();
    auto end_it = std::prev(entries.end());

    int sum = sum_all_entries(entries);
    // print_entries(entries);
    while (begin_it != end_it) {
        while (!begin_it->is_empty()) {
            begin_it = std::next(begin_it);
            if (begin_it == end_it) {
                goto end;
            }
        }
        while (end_it->is_empty()) {
            end_it = std::prev(end_it);
            if (begin_it == end_it) {
                goto end;
            }
        }

        if (begin_it->length <= end_it->length) {
            begin_it->id = end_it->id;
            // Length reduction w/o insertion, means the total size changes
            end_it->length -= begin_it->length;
            if (end_it->length == 0)
                end_it->set_empty();
        } else {
            begin_it->length -= end_it->length;
            auto new_entry = Entry(end_it->id, end_it->length);
            entries.insert(begin_it, new_entry);
            end_it->set_empty();
        }
        assert(sum_all_entries(entries) == sum);
        // print_entries(entries);
    end:;
    }

    std::cout << "Result: " << calculate_score(entries) << std::endl;
}

void part_2(list<Entry> entries)
{
    auto end_it = entries.end();
    int prev_id = end_it->id;
    while (end_it != entries.begin()) {
        end_it = std::prev(end_it);
        if (end_it->is_empty() || prev_id < end_it->id) {
            continue;
        }

        for (auto begin_it = entries.begin(); begin_it != end_it; begin_it++) {
            if (!begin_it->is_empty()) {
                continue;
            }
            if (begin_it->length >= end_it->length) {
                begin_it->length -= end_it->length;
                auto new_entry = Entry(end_it->id, end_it->length);
                entries.insert(begin_it, new_entry);
                end_it->set_empty();

                break;
            }
        }

        // print_entries(entries);
    }

    std::cout << "Result: " << calculate_score(entries) << std::endl;
}
} // namespace

void day_09(const char* input_file)
{
    std::cout << "Day 09: " << std::endl;

    auto entries = parse_input(input_file);

    part_1(entries);
    part_2(entries);
}
