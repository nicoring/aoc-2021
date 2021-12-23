#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day14/input.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            lines.push_back(line);
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }
    return lines;
}

std::pair<std::string, std::string> parse_pair(const std::string& line) {
    // CS -> C
    const std::regex regex("(\\w+) -> (\\w+)");
    std::smatch match;
    std::regex_match(line, match, regex);

    std::string from = match[1].str();
    std::string to = match[2].str();
    return {from, to};
}

std::vector<std::pair<std::string, std::string>>
parse_pairs(const std::vector<std::string>& lines) {
    std::vector<std::pair<std::string, std::string>> pairs;
    for (int i = 2; i < lines.size(); i++) {
        pairs.push_back(parse_pair(lines[i]));
    }
    return pairs;
}

std::map<std::string, std::string>
build_map(const std::vector<std::string>& lines) {
    auto pairs = parse_pairs(lines);
    std::map<std::string, std::string> mapping(pairs.begin(), pairs.end());
    return mapping;
}

unsigned long run(const std::vector<std::string>& lines, int steps) {
    std::map<std::string, std::string> mapping = build_map(lines);
    std::string polymer = lines[0];

    for (int step = 0; step < steps; step++) {
        std::string new_polymer(polymer);
        for (int i = 0; i < polymer.size() - 1; i++) {
            std::string substring = polymer.substr(i, 2);
            std::string new_elem = mapping[substring];
            new_polymer.insert(2 * i + 1, new_elem);
        }
        // std::cout << new_polymer << std::endl;
        std::cout << step << " length: " << new_polymer.size() << std::endl;
        polymer = new_polymer;
    }

    std::set<char> chars(polymer.begin(), polymer.end());
    std::map<unsigned long, char> counts;
    for (char c : chars) {
        unsigned long count = std::count(polymer.begin(), polymer.end(), c);
        counts[count] = c;
    }

    return counts.rbegin()->first - counts.begin()->first;
}

unsigned long first_solution(const std::vector<std::string>& lines) {
    return run(lines, 10);
}

unsigned long second_solution(const std::vector<std::string>& lines) {
    return run(lines, 40);
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned long first = first_solution(lines);
    unsigned long second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
