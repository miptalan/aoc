#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <unordered_map>

struct Interval {
    std::uint64_t l;
    std::uint64_t r;

    Interval(const std::uint64_t l, const std::uint64_t r) : l(l), r(r) {}
    explicit Interval(const std::string& str) {
        std::istringstream iss(str);
        std::int64_t tmp;
        iss >> tmp; l = tmp;
        iss >> tmp; r = abs(tmp);
    }

    friend std::ostream & operator<<(std::ostream &os, const Interval &obj) {
        return os << obj.l << "-" << obj.r;
    }
};

std::vector<Interval> parse_input(std::ifstream& file) {
    std::vector<Interval> result;
    std::string line;
    while (std::getline(file, line, ',')) {
        result.emplace_back(line);
    }
    return result;
}

std::uint64_t pow10(int p) {
    static std::uint64_t pre[20] = {
        1,
        10,
        100,
        1000,
        10000,
        100000,
        1000000,
        10000000,
        100000000,
        1000000000,
        10000000000,
        100000000000,
        1000000000000,
        10000000000000,
        100000000000000,
        1000000000000000,
        10000000000000000,
        100000000000000000,
        1000000000000000000,
        10000000000000000000u
    };

    [[likely]]
    if (p < 20) {
        return pre[p];
    }
    return static_cast<std::uint64_t>(std::pow(10, p));
}

int main() {
    std::cout << "Advent of Code 2025 - Day 02" << std::endl;

    const std::string file_name = "../input.txt";
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error opening file " << file_name << std::endl;
        return 1;
    }

    const auto input = parse_input(file);

    std::cout << "======= Part 1 ======\n";
    std::uint64_t result1 {0};
    for (const auto& interval : input) {
        std::cout << "Traversing " << interval << "\n";
        // determine number of digits
        auto num_digits_left = static_cast<int>(std::ceil(std::log10(interval.l)));
        if (num_digits_left % 2 != 0) {
            ++num_digits_left;
        }
        auto num_digits_right = static_cast<int>(std::ceil(std::log10(interval.r)));
        if (num_digits_right % 2 != 0) {
            --num_digits_right;
        }
        const auto start = std::max(interval.l, pow10(num_digits_left - 1));
        const auto end = std::min(interval.r, pow10(num_digits_right + 1) - 1);

        for (auto i = start; i <= end; i++) {
            if (const auto num_digits = static_cast<int>(std::ceil(std::log10(i)));
                num_digits % 2 == 0 && i % (pow10(num_digits/2) + 1) == 0) {
                std::cout << i << " ";
                result1 += i;
            }
        }
        std::cout << '\n';
    }
    std::cout << "Part 1: " << result1 << "\n";

    std::cout << "======= Part 2 ======\n";
    std::uint64_t result2 {0};
    const auto max_interval = *std::ranges::max_element(input, [](const auto& lhs, const auto& rhs) {return lhs.r < rhs.r;});
    const auto max_divisor_pow = static_cast<int>(std::ceil(std::log10(max_interval.r)));
    std::unordered_map<int, std::vector<std::uint64_t>> divisors;
    for (auto i = 2; i <= max_divisor_pow; ++i) {
        for (auto p = 1; p < i; ++p) {
            if (i % p == 0) {
                auto d = i / p;
                auto p10 = pow10(p);
                auto result {1u};
                for (; d > 1; --d) {
                    result = result * p10 + 1;
                }
                divisors[i].push_back(result);
            }
        }
    }

    for (const auto& divisor : divisors) {
        std::cout << "divisor[" << divisor.first << "] = ";
        for (const auto& d : divisor.second) {
            std::cout << d << " ";
        }
        std::cout << '\n';
    }

    for (const auto& interval : input) {
        std::cout << "Traversing " << interval << "\n";
        for (auto i = interval.l; i <= interval.r; i++) {
            for (const auto d : divisors[static_cast<int>(std::ceil(std::log10(i)))]) {
                if (i % d == 0) {
                    std::cout << i << " ";
                    result2 += i;
                    break;
                }
            }
        }
        std::cout << '\n';
    }

    std::cout << "Part 2: " << result2 << "\n";

    return 0;
}
