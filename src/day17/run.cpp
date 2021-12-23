#include <cmath>
#include <iostream>
#include <string>
#include <vector>

struct vector {
    int x, y;
    vector operator+(vector a) const { return {a.x + x, a.y + y}; }
};

struct plane {
    vector top_left, bottom_right;

    bool contains(vector v) const {
        bool in_x_bounds = v.x >= top_left.x and v.x <= bottom_right.x;
        bool in_y_bounds = v.y <= top_left.y and v.y >= bottom_right.y;
        return in_x_bounds and in_y_bounds;
    }

    bool is_above(vector v) const { return v.y < bottom_right.y; }
};

std::vector<vector> simulate(plane target, vector velocity) {
    std::vector<vector> trajectory;
    vector pos{0, 0};
    trajectory.push_back(pos);
    while (not target.is_above(pos)) {    
        pos = pos + velocity;
        trajectory.push_back(pos);
        if (target.contains(pos))
            break;
        if (velocity.x > 0)
            velocity.x--;
        if (velocity.x < 0)
            velocity.x++;
        velocity.y--;
    }
    return trajectory;
}

int max_height(std::vector<vector> trajectory) {
    int max = INT_MIN;
    for (vector v : trajectory) {
        if (v.y > max) {
            max = v.y;
        }
    }
    return max;
}

void print(std::vector<vector> trajectory) {
    for (vector v : trajectory) {
        std::cout << "x: " << v.x << " y: " << v.y << std::endl;
    }
}

int main() {
    // x=20..30, y=-10..-5
    // plane target{{20, -5}, {30, -10}};
    // target area: x=56..76, y=-162..-134
    plane target{{56, -134}, {76, -162}};

    int valid_shots = 0;

    int best_height = INT_MIN;
    vector best_vel;
    for (int vx = 1; vx < 100; vx++) {
        for (int vy = -1000; vy < 1000; vy++) {
            vector velocity{vx, vy};
            std::vector<vector> t = simulate(target, velocity);
            if (target.contains(t.back())) {
                valid_shots++;
                int height = max_height(t);
                if (height > best_height) {
                    best_height = height;
                    best_vel = velocity;
                }
            }
        }
    }
    std::cout << "best vel x: " << best_vel.x << " y: " << best_vel.y << std::endl;
    std::cout << "best_height: " << best_height << std::endl;
    std::cout << "valid_shots: " << valid_shots << std::endl;
    return 0;
}
