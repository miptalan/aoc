#include <iostream>
#include <fstream>
#include <string>

constexpr int mod(const int a, const int m) noexcept {
    return (m + a % m) % m;
}

int part1(const int start, const std::vector<int>& ticks) {
    int num_zeroes = 0, curr = start;
    for (const int tick : ticks) {
        curr = mod(curr+tick, 100);
        if (curr == 0) {
            ++num_zeroes;
        }
    }
    return num_zeroes;
}

int part2(const int start, const std::vector<int>& ticks) {
    int num_zeroes = 0, curr = start, prev = start;
    for (int tick : ticks) {
        if (tick <-99 || tick > 99) {
            num_zeroes += abs(tick / 100);
            tick %= 100;
        }

        curr += tick;
        if (prev!= 0 && (curr > 100 || curr < 0)) {
            ++num_zeroes;
        }
        curr = mod(curr, 100);
        if (curr == 0) {
            ++num_zeroes;
        }
        prev = curr;
    }
    return num_zeroes;
}

int main() {
    const std::string file_name = "test.txt";
    constexpr int start = 50;
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error opening file " << file_name << std::endl;
        return 1;
    }

    std::vector<int> ticks;
    std::string line;
    while (std::getline(file, line)) {
        int tick = std::stoi(line.substr(1));
        if (line[0] == 'L') {
            tick *= -1;
        }
        ticks.push_back(tick);
    }

    std::cout << "======= Part 1 ======\n";
    std::cout << part1(start, ticks) << "\n\n";
    std::cout << "======= Part 2 ======\n";
    std::cout << part2(start, ticks) << "\n\n";

    return 0;
}