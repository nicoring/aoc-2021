#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day3/input.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line, ' ')) {
            lines.push_back(line);
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }
    return lines;
}

unsigned first_solution(const std::vector<std::string>& lines) {
    unsigned gamma_rate = 0;
    unsigned epsilon_rate = 0;
    unsigned count;
    for (unsigned i = 0; i < lines[0].size(); i++) {
        count = 0;
        for (const std::string& line : lines) {
            if (line[line.size() - i - 1] == '1') {
                count += 1;
            }
        }
        if (count > (lines.size() / 2)) {
            gamma_rate += 1 << i;
        } else {
            epsilon_rate += 1 << i;
        }
    }
    return gamma_rate * epsilon_rate;
}

enum rating_type { CO2, OXYGEN };

unsigned get_rating(std::vector<std::string> lines, rating_type type) {
    unsigned pos = 0;
    unsigned count;
    char filter;
    while (lines.size() > 1) {
        count = 0;
        for (const auto& line : lines) {
            if (line[pos] == '1') {
                count += 1;
            }
        }
        if (type == OXYGEN) {
            if (count >= (lines.size() - count)) {
                filter = '1';
            } else {
                filter = '0';
            }
        } else {
            if (count < (lines.size() - count)) {
                filter = '1';
            } else {
                filter = '0';
            }
        }
        std::vector<std::string> new_lines;
        for (const std::string& line : lines) {
            if (line[pos] == filter) {
                new_lines.push_back(line);
            }
            lines = new_lines;
        }

        pos++;
    }
    assert(lines.size() == 1);
    unsigned i = std::stoi(lines[0], nullptr, 2);
    return i;
}

unsigned second_solution(const std::vector<std::string>& lines) {
    unsigned oxygen_rating = get_rating(lines, OXYGEN);
    unsigned co2_rating = get_rating(lines, CO2);
    return oxygen_rating * co2_rating;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}