//
//  multihash.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/12/24.
//

#ifndef multihash_hpp
#define multihash_hpp

#include <vector>

class MultiHash {
public:
    virtual std::vector<int> hash(int input) const = 0;
};

#endif /* multihash_hpp */
