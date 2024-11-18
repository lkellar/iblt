//
//  OriginalSketch.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#include "OriginalSketch.hpp"
#include <queue>
#include <algorithm>

OriginalSketch::OriginalSketch(size_t sketch_size, const MultiHash& hasher) : hasher(hasher) {
    this->sketch = std::vector<OriginalCell>(sketch_size);
}

// return distinct hashes for key
std::unordered_set<int> OriginalSketch::hash_distinct(int key) {
    std::vector<int> hash_vector = this->hasher.hash(key);
    return std::unordered_set<int>(hash_vector.begin(), hash_vector.end());
}

void OriginalSketch::insert(int key, int value) {
    std::unordered_set<int> hashes = this->hash_distinct(key);
    for (int hash : hashes) {
        this->sketch[hash].count++;
        this->sketch[hash].keySum += key;
        this->sketch[hash].valueSum += value;
    }
}

// returns indexes of buckets that we removed key/value from
std::unordered_set<int> OriginalSketch::remove(int key, int value) {
    std::unordered_set<int> hashes = this->hash_distinct(key);
    for (int hash : hashes) {
        this->sketch[hash].count--;
        this->sketch[hash].keySum -= key;
        this->sketch[hash].valueSum -= value;
    }
    return hashes;
}

std::expected<int, OriginalGetError> OriginalSketch::get(int key) {
    std::unordered_set<int> hashes = this->hash_distinct(key);
    for (int hash : hashes) {
        OriginalCell cell = this->sketch[hash];
        if (cell.count == 0) {
            // finding an empty cell means key can't be in here
            return std::unexpected(OriginalGetError::not_present);
        } else if (cell.count == 1) {
            if (cell.keySum == key) {
                return cell.valueSum;
            } else {
                // finding a cell with count 1 and NOT our key means key can't be in here
                return std::unexpected(OriginalGetError::not_present);
            }
        }
    }
    
    // if we can't find a cell with count 0 or 1, we can't determine if key is in ehre
    return std::unexpected(OriginalGetError::failed);
}


struct DecodeComparator {
    bool operator() (const std::pair<int, int> lhs, const std::pair<int, int> rhs) const {
        return lhs.second > rhs.second;
    }
};


std::pair<std::unordered_map<int, int>, bool> OriginalSketch::decode() {
    std::unordered_map<int, int> result;
    
    // holds a pair of index and count
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, DecodeComparator> queue;
    for (int i = 0; i < this->sketch.size(); i++) {
        int count = this->sketch[i].count;
        
        queue.push({i, count});
    }
    
    while (!queue.empty()) {
        auto next = queue.top();
        int index = next.first;
        OriginalCell cell = this->sketch[index];
        if (cell.count != 1) {
            queue.pop();
            continue;
        }
        result.insert({cell.keySum, cell.valueSum});
        std::unordered_set<int> indexes = this->remove(cell.keySum, cell.valueSum);
        for (int deleted_index : indexes) {
            if (deleted_index == index) {
                continue;
            }
            queue.push({deleted_index, this->sketch[deleted_index].count});
        }
        queue.pop();
    }
    // check if any of the sketch still has anything in it
    if (std::any_of(this->sketch.begin(), this->sketch.end(), [](OriginalCell x) {return !x.empty(); })) {
        return {result, true};
    }
    
    return {result, false};
}
