//
//  knuthhash.hpp
//  Set Reconciliation
//
//  Riff on Knuth's Multiplicative Hash
//  a * x >> (w-b) % array_size
//  where w is word size and b is number of bits in array size
//
//  Created by Lucas Kellar on 11/12/24.
//

#ifndef knuthhash_hpp
#define knuthhash_hpp

#include <cstdio>
#include <vector>
#include <limits>
#include "multihash.hpp"

const int WORD_SIZE = std::numeric_limits<int>::digits;

class KnuthHash : public MultiHash {
public:
    KnuthHash(size_t array_size, size_t hash_count);
    std::vector<int> hash(int input) const;
private:
    std::vector<int> a;
    const int shift;
    size_t hash_count;
    size_t array_size;
};

#endif /* knuthhash_hpp */
