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

class StackedSketch {
    StackedSketch(int threshold, double error_rate, const std::vector<MultiHash>& hashers);
    void insert(int key, int value);
    std::unordered_map<int, int> decode();
    void remove(int key, int value);
    // returns how many multihashers will be needed. this will be enforced
    static int multihashCountNeeded(int threshold);
private:
    constexpr static double C = 8/std::numbers::e;
    // fairly large constant
    constexpr static double C_0 = 20;
    std::vector<BasicSketch> sketches;
};

#endif /* StackedSketch_hpp */
