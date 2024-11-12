//
//  HPWSketch.cpp
//  Set Reconciliation
//
//  Implementation of https://epubs.siam.org/doi/10.1137/1.9781611977585.ch21
//
//  Created by Lucas Kellar on 11/7/24.
//

#include "HPWSketch.hpp"
#include <stdexcept>
#include <algorithm>
#include <deque>
#include <unordered_set>
#include <optional>

HPWSketch::HPWSketch(size_t sketch_size, size_t hash_count) : hasher(MultiHash(sketch_size, hash_count)) {
    this->sketch = std::vector<int>(sketch_size, 0);
}

void HPWSketch::toggle(int x) {
    std::vector<int> hashes = this->hasher.hash(x);
    for (int hash : hashes) {
        this->sketch[hash] = this->sketch[hash] ^ x;
    }
}

void HPWSketch::merge(const HPWSketch& other) {
    if (other.sketch.size() != this->sketch.size()) {
        throw std::invalid_argument( "Sketches must be same size to merge" );
    }
    for (size_t i = 0; i < this->sketch.size(); i++) {
        this->sketch[i] = this->sketch[i] ^ other.sketch[i];
    }
}

bool HPWSketch::looksPure(size_t index) {
    int datum = this->sketch[index];
    if (datum == 0) {
        return false;
    }
    
    std::vector<int> hashes = this->hasher.hash(datum);
    return std::find(hashes.begin(), hashes.end(), index) != hashes.end();
}

// returns -1 if not found
int HPWSketch::findNextPureIndex(int lastPureIndex = -1) {
    // start from where the last one was found, loop til we reach the whole array
    for (size_t i = (lastPureIndex + 1) % this->sketch.size(); i <= lastPureIndex; i = (i + 1) % this->sketch.size()) {
        if (looksPure(i)) {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

std::optional<std::unordered_set<int>> HPWSketch::decode() {
    std::unordered_set<int> result;
    std::deque<size_t> pureIndexes;
    int iterations = 0;
    for (size_t i = 0; i < this->sketch.size(); i++) {
        if (this->looksPure(i)) {
            pureIndexes.push_back(i);
        }
    }
    while (!pureIndexes.empty()) {
        size_t index = pureIndexes.front();
        if (this->looksPure(index)) {
            int x = this->sketch[index];
            this->toggle(x);
            // if x present in results, remove it, if not, add in
            // this lets anomalies self correct.
            auto it = result.find(x);
            if (it != result.end()) {
                result.erase(it);
            } else {
                result.insert(x);
            }
            
            // we're already generating this above? should we deduplicate?
            std::vector<int> hashes = this->hasher.hash(x);
            for (auto hash : hashes) {
                if (looksPure(hash)) {
                    pureIndexes.push_back(hash);
                }
            }
        }
        pureIndexes.pop_front();
        
        // If we're in an probably infinite loop, just fail
        if (iterations == this->sketch.size() * 3) {
            return std::nullopt;
        }
        iterations++;
    }
    
    // we're finished decoding but 1s still remain, decoding failed
    if (std::any_of(this->sketch.begin(), this->sketch.end(), [](int x) {return x != 0; })) {
        return std::nullopt;
    }
    
    return result;
}
