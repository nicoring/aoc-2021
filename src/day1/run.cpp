// reading a text file
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> read_lines() {
    std::vector<int> lines;
    std::string line;
    std::ifstream myfile("src/day1/input.txt");
    if (myfile.is_open()) {
        while (getline(myfile, line, ' ')) {
            lines.push_back(stoi(line));
        }
        myfile.close();
    } else {
        std::cout << "Unable to open file";
    }
    return lines;
}

unsigned count_increments(std::vector<int>& lines) {
    unsigned count = 0;
    for (unsigned i = 1; i < lines.size(); i++) {
        if (lines[i] > lines[i - 1]) {
            count += 1;
        }
    }
    return count;
}

unsigned count_increments_window(std::vector<int>& lines) {
    unsigned count = 0;
    int sum_prev, sum_now;
    for (unsigned i = 4; i < lines.size(); i++) {
        sum_prev = lines[i - 3] + lines[i - 2] + lines[i - 1];
        sum_now = lines[i - 2] + lines[i - 1] + lines[i];
        if (sum_now > sum_prev) {
            count += 1;
        }
    }
    return count;
}

int main() {
    std::vector<int> lines = read_lines();
    unsigned single_count = count_increments(lines);
    std::cout << "single: " << single_count << std::endl;
    unsigned window_count = count_increments_window(lines);
    std::cout << "window: " << window_count << std::endl;
    return 0;
}