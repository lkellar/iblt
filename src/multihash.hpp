//
//  multihash.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/12/24.
//

#ifndef multihash_hpp
#define multihash_hpp

#include <vector>

enum HashType {
    Knuth,
    Dual,
    Tabular
};

class MultiHash {
public:
    MultiHash(size_t hash_count): hash_count(hash_count) {}
    virtual std::vector<int> hash(int input) const = 0;
    const size_t hash_count;
};

#endif /* multihash_hpp */
