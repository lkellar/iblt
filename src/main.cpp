//
//  main.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/7/24.
//

#include <iostream>
#include "HPWSketch.hpp"
#include <unordered_set>
#include <algorithm>
#include <random>
#include "dualhash.hpp"


bool test_hpwsketch() {
    const int M = 1000000;
    const int N = 1024;
    const double space_ratio = 2;
    const int sketch_size = N * space_ratio;
    std::vector<int> numbers;
    numbers.reserve(M);
    std::unordered_set<int> outliers;
    outliers.reserve(N);
    
    DualHash hasher(sketch_size, 3);
    HPWSketch sketch(sketch_size, hasher);
    std::random_device dev;
    std::mt19937 mt{ dev() };
    std::bernoulli_distribution threshold(static_cast<double>(N)/static_cast<double>(M));
    
    for (int i = 0; i < M; i++) {
        numbers.push_back(i);
        if (threshold(mt)) {
            outliers.insert(i);
        }
    }
    
    std::shuffle(numbers.begin(), numbers.end(), dev);
    
    for (int i : numbers) {
        sketch.toggle(i);
    }
    
    for (int i : numbers) {
        if (!outliers.contains(i)) {
            sketch.toggle(i);
        }
    }
    
    if (auto out = sketch.decode()) {
        if (out == outliers) {
            std::cout << "Decode Succeeded - Length: " << outliers.size() << std::endl;
            return true;
        } else {
            std::cerr << "Decode Failed - Expected Length: " << outliers.size() << " | Actual Length: " << out->size() << std::endl;
            return false;
        }
    } else {
        std::cerr << "Decode failed" << std::endl;
        return false;
    }
}

int main(int argc, const char * argv[]) {
    const int attempts = 100;
    int success = 0;
    for (int i = 0; i < attempts; i++) {
        if (test_hpwsketch()) {
            success++;
        }
    }
    std::cout << '\n' << "Successes: " << success << " | Attempts: " << attempts << "| Success Rate: " << static_cast<double>(success) / static_cast<double>(attempts) << std::endl;
    return 0;
}
