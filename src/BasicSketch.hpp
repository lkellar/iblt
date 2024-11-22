//
//  BasicSketch.hpp
//  Set Reconciliation
//
//  Intended to be used as a component in StackedSketch
//
//  Created by Lucas Kellar on 11/21/24.
//

#ifndef BasicSketch_hpp
#define BasicSketch_hpp

#include <vector>
#include "multihash.hpp"
#include <unordered_map>

// row count must equal number of hashes
class BasicSketch {
public:
    BasicSketch(size_t rows, size_t columns, const MultiHash& hasher);
    void insert(int key, int value);
    void remove(int key, int value);
    std::unordered_map<int, int> decode();
private:
    std::vector<std::vector<int>> keys;
    std::vector<std::vector<int>> values;
    std::vector<std::vector<int>> count;
    const size_t row_count;
    const size_t column_count;
    const MultiHash& hasher;
};

#endif /* BasicSketch_hpp */
