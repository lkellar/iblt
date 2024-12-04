//
//  StackedSketch.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#include "StackedSketch.hpp"
#include <stdexcept>
#include <cmath>
#include "knuthhash.hpp"
#include "dualhash.hpp"

StackedSketch::StackedSketch(double threshold, double error_rate, HashType hashType) : hashType(hashType) {
    int sketchCount = std::ceil(std::log2(threshold));
    this->sketches.reserve(sketchCount);
    double tau = C_0 * log2(1/error_rate);
    int logtau = std::ceil(log2(tau));
    int singlerow_count = sketchCount - logtau;
    
    // Single Row Columns
    for (int index = 0; index < singlerow_count; index++) {
        int columns = std::ceil(C * threshold * std::pow(2, -index));
        this->addNextSketch(1, columns);
    }
    
    // Multirow Columns
    for (int index = 0; index < logtau; index++) {
        int columns = std::ceil(C * tau * std::pow(2, -index));
        int rows = std::pow(2, index);
        
        this->addNextSketch(rows, columns);
    }
}

void StackedSketch::addNextSketch(size_t rows, size_t columns) {
    // # rows = number of hash functions needed
    switch (this->hashType) {
        case HashType::Knuth: {
            this->sketches.emplace_back(rows, columns, KnuthHash::createHash(columns, rows));
            return;
        }
        case HashType::Dual: {
            this->sketches.emplace_back(rows, columns, DualHash::createHash(columns, rows));
            return;
        }
    }
}
void StackedSketch::insert(int key, int value) {
    for (BasicSketch& sketch : this->sketches) {
        sketch.insert(key, value);
    }
}

void StackedSketch::remove(int key, int value) {
    for (BasicSketch& sketch : this->sketches) {
        sketch.remove(key, value);
    }
}

std::unordered_map<int, int> StackedSketch::decode() {
    std::unordered_map<int, int> result;
    for (BasicSketch& sketch : sketches) {
        for (const auto & [key, value] : result ) {
            sketch.remove(key, value);
        }
        std::unordered_map<int, int> decoded = sketch.decode();
        result.insert(decoded.begin(), decoded.end());
    }
    return result;
}
