//
//  BasicSketch.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/21/24.
//

#include "BasicSketch.hpp"
#include <stdexcept>

BasicSketch::BasicSketch(size_t rows, size_t columns, const MultiHash& hasher) : hasher(hasher), row_count(rows), column_count(columns) {
    if (hasher.hash_count != rows) {
        throw std::invalid_argument( "Must have equal number of rows and hash functions" );
    }
    this->keys = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
    this->values = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
    this->count = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
}

void BasicSketch::insert(int key, int value) {
    std::vector<int> hash_vector = this->hasher.hash(key);
    for (size_t index = 0; index < hash_vector.size(); index++) {
        int hash = hash_vector[index];
        this->keys[index][hash] += key;
        this->values[index][hash] += value;
        this->count[index][hash]++;
    }
}

void BasicSketch::remove(int key, int value) {
    std::vector<int> hash_vector = this->hasher.hash(key);
    for (size_t index = 0; index < hash_vector.size(); index++) {
        int hash = hash_vector[index];
        this->keys[index][hash] -= key;
        this->values[index][hash] -= value;
        this->count[index][hash]--;
    }
}

std::unordered_map<int, int> BasicSketch::decode() {
    std::unordered_map<int, int> result;
    for (size_t row = 0; row < this->row_count; row++) {
        for (size_t column = 0; column < this->column_count; column++) {
            if (this->count[row][column] == 1) {
                result.insert({this->keys[row][column], this->values[row][column]});
            }
        }
    }
    return result;
}
