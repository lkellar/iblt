//
//  multihash.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/9/24.
//

#ifndef multihash_hpp
#define multihash_hpp

#include <cstdio>
#include <vector>

const int LARGE_PRIME = 2147483647;

class MultiHash {
public:
    MultiHash(size_t array_size, size_t hash_count);
    std::vector<int> hash(int input);
private:
    std::vector<int> a;
    std::vector<int> b;
    size_t hash_count;
    size_t array_size;
};

#endif /* multihash_hpp */
