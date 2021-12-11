#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day5/input.txt");
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

bool all(std::vector<bool> vec) {
    bool all = true;
    for (bool elem : vec) {
        all &= elem;
    }
    return all;
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

std::vector<std::vector<unsigned>>
parse_lines(const std::vector<std::string>& lines) {
    std::vector<std::vector<unsigned>> parsed;
    for (std::string line : lines) {
        parsed.push_back(parse_numbers(line));
    }
    return parsed;
}

std::pair<unsigned, unsigned>
get_dims(std::vector<std::vector<unsigned>> parsed_lines) {
    unsigned max_x = 0;
    unsigned max_y = 0;
    for (auto& nums : parsed_lines) {
        if (nums[0] > max_x) {
            max_x = nums[0];
        }
        if (nums[1] > max_y) {
            max_y = nums[1];
        }
        if (nums[2] > max_x) {
            max_x = nums[2];
        }
        if (nums[3] > max_y) {
            max_y = nums[3];
        }
    }
    return {max_x, max_y};
}

std::vector<std::vector<unsigned>>
build_map(std::vector<std::vector<unsigned>>& parsed_lines, bool diagonal) {
    auto dims = get_dims(parsed_lines);
    std::vector<std::vector<unsigned>> map;
    map.resize(dims.first + 1, std::vector<unsigned>(dims.second + 1, 0));

    int x1, y1, x2, y2;
    for (std::vector<unsigned>& nums : parsed_lines) {
        x1 = nums[0];
        y1 = nums[1];
        x2 = nums[2];
        y2 = nums[3];

        if (x1 == x2) {
            if (y1 > y2)
                std::swap(y1, y2);
            for (int i = y1; i <= y2; i++) {
                map[x1][i]++;
            }
        } else if (y1 == y2) {
            if (x1 > x2)
                std::swap(x1, x2);
            for (int i = x1; i <= x2; i++) {
                map[i][y1]++;
            }
        } else if (diagonal) {
            int dx, dy;
            if ((x2 - x1) > 0) {
                dx = 1;
            } else {
                dx = -1;
            }
            if ((y2 - y1) > 0) {
                dy = 1;
            } else {
                dy = -1;
            }
            for (int x = x1, y = y1; (x != x2 + dx); x += dx, y += dy) {
                map[x][y]++;
            }
        }
    }
    return map;
}

unsigned count_overlaps(std::vector<std::vector<unsigned>>& map) {
    unsigned count = 0;
    for (auto row : map) {
        for (unsigned elem : row) {
            if (elem >= 2)
                count++;
        }
    }
    return count;
}

unsigned first_solution(const std::vector<std::string>& lines) {
    std::vector<std::vector<unsigned>> parsed_lines = parse_lines(lines);
    std::vector<std::vector<unsigned>> map = build_map(parsed_lines, false);
    unsigned count = count_overlaps(map);
    return count;
}

unsigned second_solution(const std::vector<std::string>& lines) {
    std::vector<std::vector<unsigned>> parsed_lines = parse_lines(lines);
    std::vector<std::vector<unsigned>> map = build_map(parsed_lines, true);
    unsigned count = count_overlaps(map);
    return count;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}