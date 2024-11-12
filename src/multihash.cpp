//
//  multihash.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/9/24.
//

#include "multihash.hpp"

#include <random>

MultiHash::MultiHash(size_t array_size, size_t hash_count): hash_count(hash_count), array_size(array_size) {
    std::mt19937 mt{ std::random_device{}() };
    this->a.reserve(hash_count);
    this->b.reserve(hash_count);
    
    // generate random(ish) As and Bs for our hash functions
    for (size_t i = 0; i < hash_count; i++) {
        this->a.push_back(mt());
        this->b.push_back(mt());
    }
}


// returns hash_count instances of (ax + b) % LARGE_PRIME % array_size
std::vector<int> MultiHash::hash(int input) {
    std::vector<int> result = std::vector<int>();
    result.reserve(this->hash_count);
    
    for (size_t i = 0; i < this->hash_count; i++) {
        int h = (((this->a[i] * input) + this->b[i]) % LARGE_PRIME) % this->array_size;
        result.push_back(h);
    }
    
    return result;
}
