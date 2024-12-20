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
#include <bit>
#include "dualhash.hpp"
#include "knuthhash.hpp"
#include "tabularhash.hpp"

#include "OriginalSketch.hpp"
#include "StackedSketch.hpp"
#include <unordered_map>


bool test_hpwsketch() {
    const int M = 1000000;
    const int N = 1024;
    const double space_ratio = 1.29;
    std::vector<int> numbers;
    numbers.reserve(M);
    std::unordered_set<int> outliers;
    outliers.reserve(N);
    
    std::random_device dev;
    std::mt19937 mt{ dev() };
    std::bernoulli_distribution threshold(static_cast<double>(N)/static_cast<double>(M));
    
    for (int i = 0; i < M; i++) {
        numbers.push_back(i);
        if (threshold(mt)) {
            outliers.insert(i);
        }
    }
    
    const int sketch_size = outliers.size() * space_ratio ;
    
    //TabularHash hasher(sketch_size, 3, std::bit_width(static_cast<size_t>(M)));
    KnuthHash hasher(sketch_size, 3);
    HPWSketch sketch(sketch_size, hasher);
    
    std::shuffle(numbers.begin(), numbers.end(), dev);
    
    for (int i : numbers) {
        sketch.toggle(i);
    }
    
    std::shuffle(numbers.begin(), numbers.end(), dev);
    
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

// puts in the same key and value
bool test_originalsketch() {
    const int M = 1000000;
    const int N = 1024;
    const double space_ratio = 1.38;
    std::unordered_map<int, int> numbers;
    numbers.reserve(M);
    std::vector<int> keys;
    keys.reserve(M);
    std::unordered_map<int, int> outliers;
    outliers.reserve(N);
    std::random_device dev;
    std::mt19937 mt{ dev() };
    std::bernoulli_distribution threshold(static_cast<double>(N)/static_cast<double>(M));
    std::uniform_int_distribution int_gen(0, M);
    
    for (int i = 0; i < M; i++) {
        int value = int_gen(mt);
        numbers.insert({i, value});
        keys.push_back(i);
        if (threshold(mt)) {
            outliers.insert({i, value});
        }
    }
    
    const int sketch_size = outliers.size() * space_ratio;
    
    //DualHash hasher(sketch_size, 3);
    KnuthHash hasher(sketch_size, 3);
    //TabularHash hasher(sketch_size, 3, std::bit_width(static_cast<size_t>(M)));
    OriginalSketch sketch(sketch_size, hasher);
    
    std::shuffle(keys.begin(), keys.end(), dev);
    
    for (int key : keys) {
        int value = numbers.at(key);
        sketch.insert(key, value);
    }
    
    std::shuffle(keys.begin(), keys.end(), dev);
    
    for (int key : keys) {
        if (!outliers.contains(key)) {
            int value = numbers.at(key);
            sketch.remove(key, value);
        }
    }
    
    auto out = sketch.decode();
    if (out.second) {
        std::cerr << "Decode Incomplete - ";
    }
    
    // could be interesting to see if it ever succeeds when fail is set to true
    if (out.first != outliers) {
        std::cerr << "Failed - Expected Length: " << outliers.size() << " | Actual Length: " << out.first.size() << std::endl;
        return false;
    }
    std::cout << "Decode Succeeded - Length: " << outliers.size() << std::endl;
    return true;
}

bool test_stackedsketch() {
    const int M = 1000000;
    const int N = 1000;
    
    std::unordered_map<int, int> numbers;
    numbers.reserve(M);
    std::vector<int> keys;
    keys.reserve(M);
    std::unordered_map<int, int> outliers;
    outliers.reserve(N);
    
    std::random_device dev;
    std::mt19937 mt{ dev() };
    std::bernoulli_distribution threshold(static_cast<double>(N)/static_cast<double>(M));
    std::uniform_int_distribution int_gen(0, M);
    
    for (int i = 0; i < M; i++) {
        int value = int_gen(mt);
        numbers.insert({i, value});
        keys.push_back(i);
        if (threshold(mt)) {
            outliers.insert({i, value});
        }
    }
    
    int array_size = static_cast<int>(outliers.size());
    double error_rate = 0.05;
    StackedSketch sketch(array_size, error_rate, HashType::Knuth);
    
    std::shuffle(keys.begin(), keys.end(), dev);
    
    for (int key : keys) {
        int value = numbers.at(key);
        sketch.insert(key, value);
    }
    
    std::shuffle(keys.begin(), keys.end(), dev);
    
    for (int key : keys) {
        if (!outliers.contains(key)) {
            int value = numbers.at(key);
            sketch.remove(key, value);
        }
    }
    
    auto out = sketch.decode();
    // could be interesting to see if it ever succeeds when fail is set to true
    if (out != outliers) {
        std::cerr << "Failed - Expected Length: " << outliers.size() << " | Actual Length: " << out.size() << std::endl;
        return false;
    }
    std::cout << "Decode Succeeded - Length: " << outliers.size() << std::endl;
    return true;
}

int main(int argc, const char * argv[]) {
    /*DualHash hasher(10, 3);
    OriginalSketch sketch(10, hasher);
    sketch.insert(1, 2);
    sketch.printSketch();
    auto x = sketch.decode();*/
    const int attempts = 1;
    int success = 0;
    for (int i = 0; i < attempts; i++) {
        if (i % 10 == 0) {
            std::cout << i << '\n';
        }
        if (test_stackedsketch()) {
            success++;
        }
    }
    std::cout << '\n' << "Successes: " << success << " | Attempts: " << attempts << " | Success Rate: " << static_cast<double>(success) / static_cast<double>(attempts) << std::endl;
    return 0;
}
