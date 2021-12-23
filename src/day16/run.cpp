#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <regex>
#include <string>
#include <vector>

struct Header {
    int version;
    int type_id;
};

class Package {
    Header header;
    long value;
    std::vector<Package> subpackages;

  public:
    void set_header(Header new_header) { header = new_header; }
    int get_version() const { return header.version; }
    int get_type() const { return header.type_id; }
    void set_value(long new_value) { value = new_value; }
    long get_value() const { return value; }
    void add_subpackage(Package subpackage) {
        subpackages.push_back(subpackage);
    }
    bool is_literal() const { return subpackages.size() == 0; }
    const std::vector<Package>& get_subpackages() const { return subpackages; }
    ~Package(){};
};

std::vector<std::string> read_lines() {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream myfile("src/day16/input.txt");
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

std::string convert_to_binary(const std::string& message) {
    std::string output;
    // 0 = 0000
    // 1 = 0001
    // 2 = 0010
    // 3 = 0011
    // 4 = 0100
    // 5 = 0101
    // 6 = 0110
    // 7 = 0111
    // 8 = 1000
    // 9 = 1001
    // A = 1010
    // B = 1011
    // C = 1100
    // D = 1101
    // E = 1110
    // F = 1111
    for (int i = 0; i < message.length(); i++) {
        char current = message[i];
        switch (current) {
        case '0':
            output.append("0000");
            break;
        case '1':
            output.append("0001");
            break;
        case '2':
            output.append("0010");
            break;
        case '3':
            output.append("0011");
            break;
        case '4':
            output.append("0100");
            break;
        case '5':
            output.append("0101");
            break;
        case '6':
            output.append("0110");
            break;
        case '7':
            output.append("0111");
            break;
        case '8':
            output.append("1000");
            break;
        case '9':
            output.append("1001");
            break;
        case 'A':
            output.append("1010");
            break;
        case 'B':
            output.append("1011");
            break;
        case 'C':
            output.append("1100");
            break;
        case 'D':
            output.append("1101");
            break;
        case 'E':
            output.append("1110");
            break;
        case 'F':
            output.append("1111");
            break;
        }
    }
    return output;
}

long get_number(const std::string& s) { return std::stol(s, nullptr, 2); }

long parse_value(std::string message, int& offset) {
    std::string number;
    bool last = false;
    while (not last) {
        last = message[offset] == '0';
        number += message.substr(offset + 1, 4);
        offset += 5;
    }
    std::cout << "literal value str: " << number << std::endl;
    long num = get_number(number);
    return num;
}

Package parse(std::string message, int& offset) {
    std::string version_str = message.substr(offset, 3);
    offset += 3;
    std::string type_str = message.substr(offset, 3);
    offset += 3;
    int version = get_number(version_str);
    int type = get_number(type_str);
    Header header{version, type};

    if (type == 4) {
        // literal
        long value = parse_value(message, offset);
        std::cout << "literal with version: " << version
                  << " and value: " << value << std::endl;
        Package literal;
        literal.set_header(header);
        literal.set_value(value);
        return literal;
    } else {
        // operator
        std::cout << "operator with version: " << version
                  << " and type: " << type << std::endl;
        Package op;
        op.set_header(header);
        if (message[offset++] == '1') {
            // number of sub-packets immediately contained
            std::string number_packages_str = message.substr(offset, 11);
            offset += 11;
            unsigned int number_packages = get_number(number_packages_str);
            std::cout << "number_packages: " << number_packages << std::endl;
            for (int i = 0; i < number_packages; i++) {
                Package pck = parse(message, offset);
                op.add_subpackage(pck);
            }
        } else {
            // total length in bits
            std::string packet_length_str = message.substr(offset, 15);
            offset += 15;
            unsigned int packet_length = get_number(packet_length_str);
            std::cout << "packet_length: " << packet_length << std::endl;
            int packacke_end = offset + packet_length;
            while (offset < packacke_end) {
                Package pck = parse(message, offset);
                op.add_subpackage(pck);
            }
        }
        return op;
    }
}

unsigned int sum_versions(const Package& package) {
    int sum = package.get_version();
    for (const Package& p : package.get_subpackages()) {
        sum += sum_versions(p);
    }
    return sum;
}

unsigned int first_solution(const std::vector<std::string>& lines) {
    std::string binary_message = convert_to_binary(lines[0]);

    std::cout << binary_message << std::endl;
    int offset = 0;
    Package p = parse(binary_message, offset);
    int sum = sum_versions(p);
    return sum;
}

long get_value(const Package& package) {
    switch (package.get_type()) {
    case 0: {
        long sum = 0;
        for (const Package& p : package.get_subpackages()) {
            sum += get_value(p);
        }
        return sum;
    }
    case 1: {
        long prod = 1;
        for (const Package& p : package.get_subpackages()) {
            prod *= get_value(p);
        }
        return prod;
    }
    case 2: {
        long min = LONG_MAX;
        for (const Package& p : package.get_subpackages()) {
            long val = get_value(p);
            if (val < min) {
                min = val;
            }
        }
        return min;
    }
    case 3: {
        long max = LONG_MIN;
        for (const Package& p : package.get_subpackages()) {
            long val = get_value(p);
            if (val > max) {
                max = val;
            }
        }
        return max;
    }
    case 4:
        return package.get_value();
    case 5: {
        auto subs = package.get_subpackages();
        return get_value(subs[0]) > get_value(subs[1]);
    }
    case 6: {
        auto subs = package.get_subpackages();
        return get_value(subs[0]) < get_value(subs[1]);
    }
    case 7: {
        auto subs = package.get_subpackages();
        return get_value(subs[0]) == get_value(subs[1]);
    }
    }
}

long second_solution(const std::vector<std::string>& lines) {
    std::string binary_message = convert_to_binary(lines[0]);
    int offset = 0;
    Package p = parse(binary_message, offset);
    long value = get_value(p);
    return value;
}

int main() {
    std::vector<std::string> lines = read_lines();
    unsigned first = first_solution(lines);
    long second = second_solution(lines);
    std::cout << "first: " << first << std::endl;
    std::cout << "second: " << second << std::endl;
    return 0;
}
