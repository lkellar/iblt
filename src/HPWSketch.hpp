//
//  HPWSketch.hpp
//  Set Reconciliation
//
//  Implementation of https://epubs.siam.org/doi/10.1137/1.9781611977585.ch21
//
//  Created by Lucas Kellar on 11/7/24.
//

#ifndef HPWSketch_hpp
#define HPWSketch_hpp

#include <vector>
#include <unordered_set>
#include <optional>
#include "multihash.hpp"

class HPWSketch {
public:
    HPWSketch(size_t size, const MultiHash& hasher);
    void toggle(int x);
    void merge(const HPWSketch& other);
    std::optional<std::unordered_set<int>> decode();
private:
    bool looksPure(size_t index);
    int findNextPureIndex(int lastPureIndex);
    std::vector<int> sketch;
    const MultiHash& hasher;
};

#endif /* HPWSketch_hpp */
