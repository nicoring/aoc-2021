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

typedef std::map<std::string, std::vector<std::string>> adjacency_map;

typedef std::vector<std::string> trail;

struct trail_with_state {
    trail trail;
    std::string double_cave;
    bool visited;
};

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day12/input.txt");
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

std::pair<std::string, std::string> parse_line(const std::string& line) {
    /*
    examples:

    end-LJ
    start-he
    JK-end
    pc-wy
    LJ-pc
    */
    std::regex nodes_regex("([\\w]+)");
    std::sregex_iterator it =
        std::sregex_iterator(line.begin(), line.end(), nodes_regex);

    std::string from = (*it).str();
    it++;
    std::string to = (*it).str();
    return {from, to};
}

adjacency_map create_graph(const std::vector<std::string>& lines) {
    adjacency_map map;

    for (const std::string& line : lines) {
        std::pair<std::string, std::string> pair = parse_line(line);
        if (map.find(pair.first) == map.end()) {
            map[pair.first] = std::vector<std::string>();
        }
        if (map.find(pair.second) == map.end()) {
            map[pair.second] = std::vector<std::string>();
        }
        map[pair.first].push_back(pair.second);
        map[pair.second].push_back(pair.first);
    }
    return map;
}

void print_graph(adjacency_map map) {
    adjacency_map::iterator it;
    for (it = map.begin(); it != map.end(); it++) {
        std::cout << it->first << "-> [ ";
        for (auto el : it->second) {
            std::cout << el << " ";
        }
        std::cout << " ]" << std::endl;
    }
}

bool contains(trail trail, std::string node) {
    return std::find(trail.begin(), trail.end(), node) != trail.end();
}

void print_trail(trail trail) {
    for (auto el : trail) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    adjacency_map map = create_graph(lines);
    print_graph(map);

    std::stack<trail> stack;
    stack.push(trail{"start"});

    int count = 0;
    while (not stack.empty()) {
        trail current_trail = stack.top();
        stack.pop();
        std::string current_node = current_trail.back();
        if (current_node == "end") {
            print_trail(current_trail);
            count++;
        } else {
            for (const std::string& neighbour : map[current_node]) {
                if (isupper(neighbour[0]) or
                    not contains(current_trail, neighbour)) {
                    trail new_trail(current_trail);
                    new_trail.push_back(neighbour);
                    stack.push(new_trail);
                }
            }
        }
    }

    return count;
}

unsigned int second_solution(const std::vector<std::string>& lines) {
    adjacency_map map = create_graph(lines);
    print_graph(map);

    std::stack<trail_with_state> stack;
    stack.push(trail_with_state{trail{"start"}, "", false});

    std::set<std::string> final_trails;

    int count = 0;
    while (not stack.empty()) {
        trail_with_state current_trail = stack.top();
        stack.pop();
        std::string current_node = current_trail.trail.back();
        if (current_node == "end") {
            print_trail(current_trail.trail);
            std::string trail_string = "";
            for (std::string node : current_trail.trail) {
                trail_string += node;
            }
            final_trails.insert(trail_string);
            count++;
        } else {
            for (const std::string& neighbour : map[current_node]) {
                if (isupper(neighbour[0])) {
                    trail new_trail(current_trail.trail);
                    new_trail.push_back(neighbour);
                    trail_with_state new_trail_with_state{
                        new_trail, current_trail.double_cave,
                        current_trail.visited};
                    stack.push(new_trail_with_state);
                    continue;
                }

                if (neighbour == "start")
                    continue;
                if (neighbour == "end") {
                    trail new_trail(current_trail.trail);
                    new_trail.push_back(neighbour);
                    trail_with_state new_trail_with_state{
                        new_trail, current_trail.double_cave,
                        current_trail.visited};
                    stack.push(new_trail_with_state);
                    continue;
                }

                if (current_trail.double_cave == "" and
                    not contains(current_trail.trail, neighbour)) {
                    // start two new trails
                    // one using double mode
                    trail new_trail1(current_trail.trail);
                    new_trail1.push_back(neighbour);
                    trail_with_state new_trail_with_state1{new_trail1,
                                                           neighbour, false};
                    stack.push(new_trail_with_state1);

                    // one not using double mode
                    trail new_trail2(current_trail.trail);
                    new_trail2.push_back(neighbour);
                    trail_with_state new_trail_with_state2{new_trail2, "",
                                                           false};
                    stack.push(new_trail_with_state2);
                    continue;
                }

                if (current_trail.double_cave == neighbour and
                    not current_trail.visited) {
                    // use up double mode
                    trail new_trail(current_trail.trail);
                    new_trail.push_back(neighbour);
                    trail_with_state new_trail_with_state{
                        new_trail, current_trail.double_cave, true};
                    stack.push(new_trail_with_state);
                } else if (not contains(current_trail.trail, neighbour)) {
                    trail new_trail(current_trail.trail);
                    new_trail.push_back(neighbour);
                    trail_with_state new_trail_with_state{
                        new_trail, current_trail.double_cave,
                        current_trail.visited};
                    stack.push(new_trail_with_state);
                }
            }
        }
    }

    return final_trails.size();
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    unsigned second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
