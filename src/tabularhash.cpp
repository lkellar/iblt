//
//  tabularhash.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 12/1/24.
//

#include "tabularhash.hpp"
#include <random>
#include <algorithm>
#include <bit>

// if we have an upper bound on max_bits, we can ignore the front
TabularHash::TabularHash(size_t array_size, size_t hash_count, size_t max_bits): MultiHash(hash_count), array_size(array_size)  {
    
    std::mt19937 mt{ std::random_device{}() };
    
    this->rows = std::ceil(max_bits/static_cast<double>(this->BLOCK_SIZE));
    std::uniform_int_distribution<int> dist {0, static_cast<int>(std::bit_ceil(array_size) - 1)};
    
    auto gen = [&](){ return dist(mt); };
    
    
    // this line is horrible, but it creates hash_count tables with #rows rows and 2^BLOCk_SIZE cols
    this->tables = std::vector<std::vector<std::vector<int>>>(hash_count, std::vector<std::vector<int>>(this->rows, std::vector<int>(this->BLOCK_SIZE + 1)));
    
    for (size_t table_index = 0; table_index < hash_count; table_index++) {
        for (size_t row_index = 0; row_index < this->rows; row_index++) {
            std::generate(this->tables[table_index][row_index].begin(), this->tables[table_index][row_index].end(), gen);
        }
    }
}

std::vector<int> TabularHash::hash(int input) const {
    std::vector<int> result = std::vector<int>();
    result.reserve(this->hash_count);
    
    for (size_t table_index = 0; table_index < this->hash_count; table_index++) {
        int h = 0;
        for (size_t row_index = 0; row_index < this->rows; row_index++) {
            h ^= this->tables[table_index][row_index][(input >> row_index*this->BLOCK_BITS) & this->BLOCK_SIZE];
        }
        result.push_back(h % this->array_size);
    }
    
    return result;
}

std::unique_ptr<TabularHash> TabularHash::createHash(size_t array_size, size_t hash_count, size_t max_bits) {
    return std::make_unique<TabularHash>(TabularHash(array_size, hash_count, max_bits));
}
