#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day7/input.txt");
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

std::vector<unsigned> parse_numbers(std::string line) {
    std::vector<unsigned> numbers;
    std::regex numbers_regex("([0-9]+)");
    auto numbers_begin =
        std::sregex_iterator(line.begin(), line.end(), numbers_regex);
    auto numbers_end = std::sregex_iterator();
    std::smatch match;
    for (std::sregex_iterator i = numbers_begin; i != numbers_end; i++) {
        match = *i;
        std::string match_str = match.str();
        numbers.push_back(std::stoi(match_str));
    }
    return numbers;
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    std::vector<unsigned> numbers = parse_numbers(lines[0]);
    // todo find median
    return 0;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    std::vector<unsigned> numbers = parse_numbers(lines[0]);
    unsigned sum = 0;
    for (unsigned num : numbers) {
        sum += num;
    }
    int average = std::floor((float)sum / numbers.size());
    std::cout << "average: " << (float)sum / numbers.size()
              << " rounded: " << average << std::endl;
    unsigned dist_sum = 0;
    for (int num : numbers) {
        unsigned dist = std::abs(num - average);
        dist_sum += dist * (dist + 1) / 2;
    }
    return dist_sum;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}