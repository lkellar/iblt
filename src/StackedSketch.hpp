//
//  StackedSketch.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#ifndef StackedSketch_hpp
#define StackedSketch_hpp

#include <iostream>
#include "BasicSketch.hpp"
#include "multihash.hpp"
#include <numbers>
#include <functional>

class StackedSketch {
public:
    StackedSketch(double threshold, double error_rate, HashType hashType);
    void insert(int key, int value);
    std::unordered_map<int, int> decode();
    void remove(int key, int value);
private:
    void addNextSketch(size_t rows, size_t columns);
    const HashType hashType;
    constexpr static double C = 4/std::numbers::e;
    // fairly large constant
    constexpr static double C_0 = 10;
    std::vector<BasicSketch> sketches;
    std::vector<MultiHash> hashes;
};

#endif /* StackedSketch_hpp */
