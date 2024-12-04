//
//  tabularhash.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 12/1/24.
//

#ifndef tabularhash_hpp
#define tabularhash_hpp

#include <cstdio>
#include <vector>
#include "multihash.hpp"
#include <memory>
#include <bit>

class TabularHash : public MultiHash {
public:
    TabularHash(size_t array_size, size_t hash_count, size_t max_bits);
    std::vector<int> hash(int input) const;
    static std::unique_ptr<TabularHash> createHash(size_t array_size, size_t hash_count, size_t max_bits);
private:
    std::vector<std::vector<std::vector<int>>> tables;
    size_t array_size;
    // number of rows present in each table, the middle index in tables
    int rows;
    static constexpr int BLOCK_SIZE = 0b1;
    static constexpr int BLOCK_BITS = std::bit_width(static_cast<size_t>(BLOCK_SIZE));
};

#endif /* tabularhash_hpp */
