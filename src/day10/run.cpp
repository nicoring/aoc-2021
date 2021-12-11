#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day10/input.txt");
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

char closing(char c) {
    switch (c) {
    case '(':
        return ')';
        break;
    case '[':
        return ']';
        break;
    case '{':
        return '}';;
        break;
    case '<':
        return '>';
        break;
    }
}

int first_solution(const std::vector<std::string>& lines) {
    int error_sum = 0;

    for (const std::string& line : lines) {
        std::stack<char> stack;
        for (char c : line) {
            if (c == '(' || c == '[' || c == '<' || c == '{') {
                stack.push(c);
            } else if (stack.empty()) {
                break;
            } else if (closing(stack.top()) == c) {
                stack.pop();
            } else {
                switch (c) {
                case ')':
                    error_sum += 3;
                    break;
                case ']':
                    error_sum += 57;
                    break;
                case '}':
                    error_sum += 1197;
                    break;
                case '>':
                    error_sum += 25137;
                    break;
                }
                break;
            }
        }
    }
    return error_sum;
}

unsigned long complete_score(std::stack<char>& stack) {
    unsigned long score = 0;
    while (!stack.empty()) {
        score *= 5;
        switch (stack.top()) {
        case '(':
            score += 1;
            break;
        case '[':
            score += 2;
            break;
        case '{':
            score += 3;
            break;
        case '<':
            score += 4;
            break;
        }
        stack.pop();
    }
    return score;
}

unsigned long middle(std::vector<unsigned long> scores) {
    unsigned middle_index = scores.size() / 2;
    std::sort(scores.begin(), scores.end());
    return scores[middle_index];
}

unsigned long second_solution(const std::vector<std::string>& lines) {
    std::vector<unsigned long> scores;
    for (const std::string& line : lines) {
        std::stack<char> stack;
        bool error = false;
        for (char c : line) {
            if (c == '(' || c == '[' || c == '<' || c == '{') {
                stack.push(c);
            } else if (stack.empty()) {
                error = true;
                break;
            } else if (closing(stack.top()) == c) {
                stack.pop();
            } else {
                error = true;
                break;
            }
        }
        if (!error) {
            unsigned long score = complete_score(stack);
            scores.push_back(score);
        }
    }
    return middle(scores);
}

int main() {
    std::vector<std::string> lines = read_lines();
    int first = first_solution(lines);
    unsigned long second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}