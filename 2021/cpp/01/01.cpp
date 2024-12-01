#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::cout;

int part1(const std::vector<long>& data) {
    long current, previous {-1};
    int counter {0};
    for (auto& current : data) {
        if (previous != -1 && current > previous) {
            ++counter;
        }
        previous = current;
    }
    return counter;
}

int part2(const std::vector<long>& data) {
    int counter {0};
    long window[3] {-1, -1, -1};
    long current {0}, previous {0};
    long long running_sum {0}, previous_running_sum {0};
    
    for (int i = 0; i < data.size(); ++i) {
        current = data[i];
        if (window[i % 3] == -1) {
            window[i % 3] = current;
        } else {
            previous = window[i % 3];
            window[i % 3] = current;
            
            previous_running_sum = running_sum;
            
            running_sum += current;
            running_sum -= previous;
            
            if (running_sum > previous_running_sum) {
                ++counter;
            }
        }
    }
    
    return counter;
}

int main(int argc, const char** argv) {
    std::string file_name = argv[1];
    std::ifstream file(file_name);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << file_name << std::endl;
        return 1;
    }
    
    std::vector<long> data;
    for (long current = 0; file >> current;) {
        data.push_back(current);
    }
    file.close();
    
    int r1 = part1(data);
    int r2 = part2(data);
    
    cout << "Part 1: " << r1 << "\n";
    cout << "Part 2: " << r2 << std::endl;
    
    return 0;
}
