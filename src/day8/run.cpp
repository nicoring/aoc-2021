#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day8/input.txt");
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

struct line_pair {
    std::vector<std::string> signal_pattern;
    std::vector<std::string> output;
};

line_pair parse_line(const std::string& line) {
    std::vector<std::string> signal_pattern;
    std::vector<std::string> output;
    std::regex word_regex("([^\\s]+)");
    auto words_begin =
        std::sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = std::sregex_iterator();

    std::smatch match = *words_begin;
    std::string match_str = match.str();
    while (match_str != "|") {
        signal_pattern.push_back(match_str);
        words_begin++;
        match = *words_begin;
        match_str = match.str();
    }

    // skip |
    words_begin++;

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        output.push_back(match_str);
    }
    return line_pair{signal_pattern, output};
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    unsigned count = 0;

    for (const std::string& line : lines) {
        line_pair pair = parse_line(line);
        for (const std::string& word : pair.output) {
            unsigned len = word.size();
            if (len == 2 || len == 4 || len == 3 || len == 7) {
                count++;
            }
        }
    }
    return count;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    unsigned sum = 0;

    unsigned found;
    for (const std::string& line : lines) {
        found = 0;
        line_pair pair = parse_line(line);
        char mapping[7];
        // while (found != 7) {
        //     for ()
        // }
    }

    return sum;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}