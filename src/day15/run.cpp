#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <vector>

int MAX_INT = 2147483647;

template <class T> using Grid = std::vector<std::vector<T>>;

struct Point {
    int row, col;
};

struct QueueElem {
    int cost;
    Point point;
};

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day15/input.txt");
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

std::vector<Point> neighbours(Point point, int rows, int cols) {
    std::vector<Point> points;
    int row = point.row;
    int col = point.col;
    if (row - 1 >= 0) {
        points.push_back(Point{row - 1, col});
    }
    if (row + 1 < rows) {
        points.push_back(Point{row + 1, col});
    }
    if (col - 1 >= 0) {
        points.push_back(Point{row, col - 1});
    }
    if (col + 1 < cols) {
        points.push_back(Point{row, col + 1});
    }
    return points;
};

unsigned int shortest_path(Grid<int> grid) {
    const int rows = grid.size();
    const int cols = grid[0].size();

    Grid<int> dist(rows, std::vector<int>(cols, MAX_INT));

    Grid<bool> visited(rows, std::vector<bool>(cols, false));
    dist[0][0] = 0;
    auto cmp = [](QueueElem left, QueueElem right) {
        return left.cost > right.cost;
    };
    std::priority_queue<QueueElem, std::vector<QueueElem>, decltype(cmp)>
        prio_queue(cmp);

    prio_queue.push(QueueElem{0, Point{0, 0}});

    while (not prio_queue.empty()) {
        QueueElem current = prio_queue.top();
        prio_queue.pop();
        if (visited[current.point.row][current.point.col])
            continue;
        visited[current.point.row][current.point.col] = true;
        int current_dist = dist[current.point.row][current.point.col];
        for (Point neighbour : neighbours(current.point, rows, cols)) {
            int new_dist = current_dist + grid[neighbour.row][neighbour.col];
            if (new_dist < dist[neighbour.row][neighbour.col]) {
                dist[neighbour.row][neighbour.col] = new_dist;
                prio_queue.push(QueueElem{new_dist, neighbour});
            }
        }
    }

    return dist[rows - 1][cols - 1];
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    Grid<int> grid = parse_grid(lines);
    return shortest_path(grid);
}

Grid<int> expand_grid(Grid<int> grid) {
    const int factor = 5;
    Grid<int> new_grid;
    new_grid.resize(grid.size() * factor,
                    std::vector<int>(grid[0].size() * factor));
    for (int i = 0; i < factor; i++) {
        for (int j = 0; j < factor; j++) {
            for (int row = 0; row < grid.size(); row++) {
                for (int col = 0; col < grid[0].size(); col++) {
                    int add = i + j;
                    int new_value = (grid[row][col] + add) > 9
                                        ? grid[row][col] + add - 9
                                        : grid[row][col] + add;
                    new_grid[row + i * grid.size()][col + j * grid[0].size()] =
                        new_value;
                }
            }
        }
    }
    return new_grid;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    Grid<int> grid = parse_grid(lines);
    grid = expand_grid(grid);
    return shortest_path(grid);
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
