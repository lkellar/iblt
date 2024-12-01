//
//  dualhash.hpp
//  Set Reconciliation
//
//  Simple ax+b hash where a & b are random numbers
//
//  Created by Lucas Kellar on 11/9/24.
//

#ifndef dualhash_hpp
#define dualhash_hpp

#include <cstdio>
#include <vector>
#include "multihash.hpp"
#include <memory>

// TODO just shift instead of the modulo
// or is it doing that under the hood maybe
const int LARGE_PRIME = 2147483647;

class DualHash : public MultiHash {
public:
    DualHash(size_t array_size, size_t hash_count);
    std::vector<int> hash(int input) const;
    static std::unique_ptr<DualHash> createHash(size_t array_size, size_t hash_count);
private:
    std::vector<int> a;
    std::vector<int> b;
    size_t array_size;
};

#endif /* dualhash_hpp */
