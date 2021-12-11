#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class Board {
  public:
    std::vector<std::vector<unsigned>> numbers;
    std::vector<std::vector<bool>> marked;
    Board() {}
    ~Board() {}
    void add_row(const std::vector<unsigned> row) {
        numbers.push_back(row);
        std::vector<bool> marked_row(row.size(), false);
        marked.push_back(marked_row);
    }

    void add_drawn_number(unsigned number) {
        for (int i = 0; i < numbers.size(); i++) {
            for (int j = 0; j < numbers[0].size(); j++) {
                if (numbers[i][j] == number) {
                    marked[i][j] = true;
                }
            }
        }
    }

    bool is_done() {
        for (int i = 0; i < numbers.size(); i++) {
            bool all_row = true;
            bool all_col = true;
            for (int j = 0; j < numbers[0].size(); j++) {
                all_row &= marked[i][j];
                all_col &= marked[j][i];
            }
            if (all_row || all_col) {
                return true;
            }
        }
        return false;
    }

    unsigned sum_unmarked() {
        unsigned sum = 0;
        for (int i = 0; i < numbers.size(); i++) {
            for (int j = 0; j < numbers[0].size(); j++) {
                if (!marked[i][j]) {
                    sum += numbers[i][j];
                }
            }
        }
        return sum;
    }

    void print() {
        for (int i = 0; i < numbers.size(); i++) {
            for (int j = 0; j < numbers[0].size(); j++) {
                std::cout << numbers[i][j] << "/" << marked[i][j] << ", ";
            }
            std::cout << std::endl;
        }
    }
};

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

std::vector<Board> parse_boards(std::vector<std::string>::const_iterator iter,
                                std::vector<std::string>::const_iterator end) {
    std::vector<Board> boards;
    std::string line;
    Board board = Board();
    for (; iter != end; iter++) {
        line = *iter;
        if (line == "") {
            boards.push_back(board);
            board = Board();
        } else {
            std::vector<unsigned> numbers = parse_numbers(line);
            board.add_row(numbers);
        }
    }
    boards.push_back(board);
    return boards;
}

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day4/input.txt");
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

void print_boards(std::vector<Board>& boards) {
    for (Board& board : boards) {
        std::cout << std::endl;
        board.print();
        std::cout << std::endl;
    }
}

unsigned first_solution(const std::vector<std::string>& lines) {
    std::vector<unsigned> drawn_numbers = parse_numbers(lines[0]);
    std::vector<Board> boards = parse_boards(lines.begin() + 2, lines.end());

    for (unsigned drawn_number : drawn_numbers) {
        for (Board& board : boards) {
            board.add_drawn_number(drawn_number);
            if (board.is_done()) {
                unsigned sum = board.sum_unmarked();
                return sum * drawn_number;
            }
        }
    }
    return 0;
}

bool all(std::vector<bool> vec) {
    bool all = true;
    for (bool elem : vec) {
        all &= elem;
    }
    return all;
}

unsigned second_solution(const std::vector<std::string>& lines) {
    std::vector<unsigned> drawn_numbers = parse_numbers(lines[0]);
    std::vector<Board> boards = parse_boards(lines.begin() + 2, lines.end());
    std::vector<bool> is_done(boards.size(), false);

    for (unsigned drawn_number : drawn_numbers) {
        for (int i = 0; i < boards.size(); i++) {
            boards[i].add_drawn_number(drawn_number);
            if (boards[i].is_done()) {
                is_done[i] = true;
                if (all(is_done)) {
                    unsigned sum = boards[i].sum_unmarked();
                    return sum * drawn_number;
                }
            }
        }
    }
    return 0;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}