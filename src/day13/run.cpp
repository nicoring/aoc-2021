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

typedef std::vector<std::vector<bool>> grid;

struct point {
    int x;
    int y;
};
struct fold {
    char axis;
    int num;
};

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day13/input.txt");
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

point parse_point(const std::string& line) {
    std::regex nodes_regex("([0-9]+)");
    std::sregex_iterator it =
        std::sregex_iterator(line.begin(), line.end(), nodes_regex);

    int x = std::stoi((*it).str());
    it++;
    int y = std::stoi((*it).str());
    return {x, y};
}

fold parse_fold(const std::string& line) {
    // fold along y=447
    const std::regex regex("fold along ([a-z])=([0-9]+)");
    std::smatch match;
    std::regex_match(line, match, regex);

    // axis
    std::ssub_match axis_match = match[1];
    char axis = axis_match.str()[0];

    // number
    std::ssub_match number_match = match[2];
    int number = std::stoi(number_match.str());
    return {axis, number};
}

grid parse_grid(const std::vector<std::string>& lines) {
    grid grid;
    std::vector<point> points;
    int max_x = 0;
    int max_y = 0;
    for (auto line : lines) {
        if (line == "")
            break;
        point point = parse_point(line);
        points.push_back(point);
        max_x = std::max(max_x, point.x);
        max_y = std::max(max_y, point.y);
    }
    grid.resize(max_y + 1, std::vector(max_x + 1, false));
    for (auto point : points) {
        grid[point.y][point.x] = true;
    }
    return grid;
}

std::vector<fold> parse_folds(const std::vector<std::string>& lines) {
    std::vector<fold> folds;
    int i = 0;
    for (; i < lines.size(); i++) {
        if (lines[i] == "") {
            i++;
            break;
        }
    }
    for (; i < lines.size(); i++) {
        folds.push_back(parse_fold(lines[i]));
    }
    return folds;
}

void print_grid(grid grid) {
    for (auto row : grid) {
        for (bool el : row) {
            std::cout << (el ? "# " : ". ");
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

void do_fold(grid& grid, fold& fold) {
    int axis_size = fold.axis == 'y' ? grid.size() : grid[0].size();
    for (int dist = 1; (fold.num - dist) >= 0 and (fold.num + dist) < axis_size; dist++) {
        if (fold.axis == 'x') {
            for (int row = 0; row < grid.size(); row++) {
                grid[row][fold.num - dist] = grid[row][fold.num - dist] or grid[row][fold.num + dist];
                grid[row][fold.num + dist] = false;
            }
        } else {
            for (int col = 0; col < grid[0].size(); col++) {
                grid[fold.num - dist][col] = grid[fold.num - dist][col] or grid[fold.num + dist][col];
                grid[fold.num + dist][col] = false;
            }
        }
    }

    if (fold.axis == 'y') {
        grid.resize(fold.num);
    } else {
        for (auto& row : grid) {
            row.resize(fold.num);
        }
    }
}

int sum(grid& grid) {
    int count = 0;
    for (auto row: grid) {
        for (bool el : row) {
            if (el) count++;
        }
    }
    return count;
}

int first_solution(const std::vector<std::string>& lines) {
    grid grid = parse_grid(lines);
    std::vector<fold> folds = parse_folds(lines);
    // print_grid(grid);
    do_fold(grid, folds[0]);
    // print_grid(grid);
    // do_fold(grid, folds[1]);
    // print_grid(grid);
    return sum(grid);
}

int second_solution(const std::vector<std::string>& lines) {
    grid grid = parse_grid(lines);
    std::vector<fold> folds = parse_folds(lines);
    for (auto& fold : folds) {
        do_fold(grid, fold);
    }
    print_grid(grid);
    int count = 0;
    return count;
}

int main() {
    std::vector<std::string> lines = read_lines();
    int first = first_solution(lines);
    int second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
