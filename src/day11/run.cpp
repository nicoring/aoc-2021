#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

template <class T> using Grid = std::vector<std::vector<T>>;

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day11/input.txt");
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

std::vector<int> parse_numbers(std::string line) {
    std::vector<int> numbers;
    std::regex numbers_regex("([0-9])");
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

Grid<int> parse_grid(const std::vector<std::string>& lines) {
    Grid<int> grid;
    for (auto& line : lines) {
        grid.push_back(parse_numbers(line));
    }
    return grid;
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    Grid<int> grid = parse_grid(lines);
    const int rows = grid.size();
    const int cols = grid[0].size();
    int num_flashes = 0;
    for (int round = 0; round < 100; round++) {
        Grid<bool> flashed(rows, std::vector<bool>(cols, false));
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                grid[row][col]++;
            }
        }

        bool has_flashed;
        do {
            has_flashed = false;
            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    if (flashed[row][col])
                        continue;
                    if (grid[row][col] > 9) {
                        has_flashed = true;
                        flashed[row][col] = true;
                        num_flashes++;
                        for (int drow = -1; drow < 2; drow++) {
                            for (int dcol = -1; dcol < 2; dcol++) {
                                int _row = row + drow;
                                int _col = col + dcol;
                                if (_row >= 0 && _row < rows && _col >= 0 &&
                                    _col < cols) {
                                    grid[_row][_col]++;
                                }
                            }
                        }
                    }
                }
            }
        } while (has_flashed);

        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (flashed[row][col]) {
                    grid[row][col] = 0;
                }
            }
        }
    }
    return num_flashes;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    Grid<int> grid = parse_grid(lines);
    const int rows = grid.size();
    const int cols = grid[0].size();

    unsigned int round = 0;
    while (true) {
        round++;
        Grid<bool> flashed(rows, std::vector<bool>(cols, false));
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                grid[row][col]++;
            }
        }

        bool has_flashed;
        do {
            has_flashed = false;
            for (int row = 0; row < rows; row++) {
                for (int col = 0; col < cols; col++) {
                    if (flashed[row][col])
                        continue;
                    if (grid[row][col] > 9) {
                        has_flashed = true;
                        flashed[row][col] = true;
                        for (int drow = -1; drow < 2; drow++) {
                            for (int dcol = -1; dcol < 2; dcol++) {
                                int _row = row + drow;
                                int _col = col + dcol;
                                if (_row >= 0 && _row < rows && _col >= 0 &&
                                    _col < cols) {
                                    grid[_row][_col]++;
                                }
                            }
                        }
                    }
                }
            }
        } while (has_flashed);

        bool all_flashed = true;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                if (flashed[row][col]) {
                    grid[row][col] = 0;
                } else {
                    all_flashed = false;
                }
            }
        }
        if (all_flashed)
            break;
    }
    return round;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
