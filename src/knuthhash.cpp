//
//  knuthhash.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/12/24.
//

#include "knuthhash.hpp"
#include <random>
#include <bit>

KnuthHash::KnuthHash(size_t array_size, size_t hash_count): MultiHash(hash_count), array_size(array_size), shift(WORD_SIZE - std::countl_zero(array_size)){
    std::mt19937 mt{ std::random_device{}() };
    this->a.reserve(hash_count);
    
    // generate random(ish) As and Bs for our hash functions
    for (size_t i = 0; i < hash_count; i++) {
        this->a.push_back(mt());
    }
}


std::vector<int> KnuthHash::hash(int input) const {
    std::vector<int> result = std::vector<int>();
    result.reserve(this->hash_count);
    
    for (size_t i = 0; i < this->hash_count; i++) {
        int h = (this->a[i] * input >> this->shift) % this->array_size;
        result.push_back(h);
    }
    
    return result;
}

std::unique_ptr<KnuthHash> KnuthHash::createHash(size_t array_size, size_t hash_count) {
    return std::make_unique<KnuthHash>(KnuthHash(array_size, hash_count));
}
