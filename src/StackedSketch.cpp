//
//  StackedSketch.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#include "StackedSketch.hpp"
#include <stdexcept>
#include <cmath>

StackedSketch::StackedSketch(int threshold, double error_rate, const std::vector<MultiHash>& hashers) {
    int sketchCount = this->multihashCountNeeded(threshold);
    if (sketchCount != hashers.size()) {
        throw std::invalid_argument( "Incorrect number of hashers provided. Use multihashCountNeeded" );
    }
    this->sketches.reserve(sketchCount);
    double tau = C_0 * log2(1/error_rate);
    int logtau = std::ceil(log2(tau));
    int singlerow_count = sketchCount - logtau;
    
    for (int index = 0; index < singlerow_count; index++) {
        int columns = std::ceil(C * threshold * std::pow(2, -index));
        this->sketches.emplace_back(1, columns, hashers[index]);
    }
    
    for (int index = 0; index < logtau; index++) {
        int columns = std::ceil(C * tau * std::pow(2, -index));
        this->sketches.emplace_back(std::pow(2, index), columns, hashers[index + singlerow_count]);
    }
}

int StackedSketch::multihashCountNeeded(int threshold) {
    return std::ceil(std::log2(static_cast<double>(threshold)));
}

void StackedSketch::insert(int key, int value) {
    for (BasicSketch sketch : this->sketches) {
        sketch.insert(key, value);
    }
}

void StackedSketch::remove(int key, int value) {
    for (BasicSketch sketch : this->sketches) {
        sketch.remove(key, value);
    }
}

std::unordered_map<int, int> StackedSketch::decode() {
    std::unordered_map<int, int> result;
    for (BasicSketch sketch : sketches) {
        for (const auto & [key, value] : result ) {
            sketch.remove(key, value);
        }
        std::unordered_map<int, int> decoded = sketch.decode();
        result.insert(decoded.begin(), decoded.end());
    }
    return result;
}
