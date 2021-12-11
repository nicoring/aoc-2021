#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef std::vector<std::vector<unsigned>> grid;

typedef struct {
    int row, col;
} point;

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day9/input.txt");
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

grid parse_grid(const std::vector<std::string>& lines) {
    grid grid;
    for (auto& line : lines) {
        grid.push_back(parse_numbers(line));
    }
    return grid;
}

std::vector<point> get_low_points(grid grid) {
    std::vector<point> low_points;
    for (int row = 0; row < grid.size(); row++) {
        for (int col = 0; col < grid[0].size(); col++) {
            unsigned current = grid[row][col];
            if (row - 1 >= 0 && grid[row - 1][col] <= current)
                continue;
            if (row + 1 < grid.size() && grid[row + 1][col] <= current)
                continue;
            if (col - 1 >= 0 && grid[row][col - 1] <= current)
                continue;
            if (col + 1 < grid[0].size() && grid[row][col + 1] <= current)
                continue;
            low_points.push_back({row, col});
        }
    }
    return low_points;
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    grid grid = parse_grid(lines);
    std::vector<point> low_points = get_low_points(grid);
    unsigned risk_level_sum = 0;

    for (point& low_point : low_points) {
        unsigned height = grid[low_point.row][low_point.col];
        risk_level_sum += height + 1;
    }

    return risk_level_sum;
}

unsigned basin_size(const grid& grid, point low_point) {
    int size = 0;

    const int rows = grid.size();
    const int cols = grid[0].size();
    std::vector<std::vector<bool>> seen(rows, std::vector<bool>(cols, false));
    seen[low_point.row][low_point.col] = true;
    std::stack<const point> stack;
    stack.push(low_point);
    while (!stack.empty()) {
        const point current = stack.top();
        stack.pop();
        size++;

        const int row = current.row;
        const int col = current.col;

        if (row - 1 >= 0 && !seen[row - 1][col] && grid[row - 1][col] < 9) {
            seen[row - 1][col] = true;
            stack.push({row - 1, col});
        }
        if (row + 1 < rows && !seen[row + 1][col] && grid[row + 1][col] < 9) {
            seen[row + 1][col] = true;
            stack.push({row + 1, col});
        }
        if (col - 1 >= 0 && !seen[row][col - 1] && grid[row][col - 1] < 9) {
            seen[row][col - 1] = true;
            stack.push({row, col - 1});
        }
        if (col + 1 < cols && !seen[row][col + 1] && grid[row][col + 1] < 9) {
            seen[row][col + 1] = true;
            stack.push({row, col + 1});
        }
    }

    return size;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    std::vector<int> sizes;
    grid grid = parse_grid(lines);
    std::vector<point> low_points = get_low_points(grid);
    for (const point& low_point : low_points) {
        int size = basin_size(grid, low_point);
        sizes.push_back(size);
    }
    std::sort(sizes.begin(), sizes.end());
    assert(sizes.size() > 3);
    int last_index = sizes.size() - 1;
    return sizes[last_index] * sizes[last_index - 1] * sizes[last_index - 2];
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
