#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day6/input.txt");
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

unsigned long simulate(const std::vector<std::string>& lines,
                       unsigned num_rounds) {
    std::vector<unsigned> population = parse_numbers(lines[0]);
    int new_count;
    for (int round = 0; round < num_rounds; round++) {
        new_count = 0;
        for (int i = 0; i < population.size(); i++) {
            if (population[i] == 0) {
                new_count++;
                population[i] = 6;
            } else {
                population[i]--;
            }
        }
        for (int i = 0; i < new_count; i++) {
            population.push_back(8);
        }
    }
    return population.size();
}

unsigned long long simulate2(const std::vector<std::string>& lines,
                             unsigned num_rounds) {
    std::vector<unsigned> population = parse_numbers(lines[0]);
    unsigned long long age_counts[9]{0};

    for (unsigned age : population) {
        age_counts[age]++;
    }

    for (int round = 0; round < num_rounds; round++) {
        unsigned long long new_count = age_counts[0];
        for (int i = 1; i < 9; i++) {
            age_counts[i - 1] = age_counts[i];
        }
        age_counts[8] = new_count;
        age_counts[6] += new_count;
    }

    unsigned long long sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += age_counts[i];
    }
    return sum;
}

unsigned long long first_solution(const std::vector<std::string>& lines) {
    return simulate2(lines, 80);
}

unsigned long long second_solution(const std::vector<std::string>& lines) {
    return simulate2(lines, 256);
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned long long second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}