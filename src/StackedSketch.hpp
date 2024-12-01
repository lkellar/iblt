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
    StackedSketch(int threshold, double error_rate, HashType hashType);
    void insert(int key, int value);
    std::unordered_map<int, int> decode();
    void remove(int key, int value);
    // returns how many multihashers will be needed. this will be enforced
    static int multihashCountNeeded(int threshold);
private:
    void addNextSketch(size_t rows, size_t columns, size_t hash_count);
    const HashType hashType;
    constexpr static double C = 8/std::numbers::e;
    // fairly large constant
    constexpr static double C_0 = 20;
    std::vector<BasicSketch> sketches;
    std::vector<MultiHash> hashes;
};

#endif /* StackedSketch_hpp */
