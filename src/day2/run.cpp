// reading a text file
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day2/input.txt");
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

unsigned first_solution(std::vector<std::string>& lines) {
    unsigned depth = 0;
    unsigned pos = 0;
    const std::regex regex("([a-z]+).([0-9]+)");
    std::smatch match;
    for (const auto& line : lines) {
        std::regex_match(line, match, regex);

        // command
        std::ssub_match command_match = match[1];
        std::string command = command_match.str();

        // number
        std::ssub_match number_match = match[2];
        unsigned number = stoi(number_match.str());

        if (command == "up") {
            depth -= number;
        } else if (command == "down") {
            depth += number;
        } else {
            pos += number;
        }
    }
    return depth * pos;
}

unsigned second_solution(std::vector<std::string>& lines) {
    unsigned depth = 0;
    unsigned pos = 0;
    int aim = 0;
    const std::regex regex("([a-z]+).([0-9]+)");
    std::smatch match;
    for (const auto& line : lines) {
        std::regex_match(line, match, regex);

        // command
        std::ssub_match command_match = match[1];
        std::string command = command_match.str();

        // number
        std::ssub_match number_match = match[2];
        unsigned number = stoi(number_match.str());

        if (command == "up") {
            aim -= number;
        } else if (command == "down") {
            aim += number;
        } else {
            pos += number;
            depth += aim * number;
        }
    }
    return depth * pos;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}