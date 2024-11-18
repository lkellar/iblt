//
//  OriginalSketch.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#include "OriginalSketch.hpp"
#include <deque>
#include <algorithm>

OriginalSketch::OriginalSketch(size_t sketch_size, const MultiHash& hasher) : hasher(hasher) {
    this->sketch = std::vector<OriginalCell>(sketch_size);
}

// return distinct hashes for key
std::vector<int> OriginalSketch::hash_distinct(int key) {
    std::vector<int> hash_vector = this->hasher.hash(key);
    // remove duplicates
    // used to use an unordered_set here but the overhead was insane.
    std::sort(hash_vector.begin(), hash_vector.end());
    auto last = std::unique(hash_vector.begin(), hash_vector.end());
    hash_vector.erase(last, hash_vector.end());
    return hash_vector;
}

void OriginalSketch::insert(int key, int value) {
    std::vector<int> hashes = this->hash_distinct(key);
    for (int hash : hashes) {
        this->sketch[hash].count++;
        this->sketch[hash].keySum += key;
        this->sketch[hash].valueSum += value;
    }
}

// returns indexes of buckets that we removed key/value from
std::vector<int> OriginalSketch::remove(int key, int value) {
    std::vector<int> hashes = this->hash_distinct(key);
    for (int hash : hashes) {
        this->sketch[hash].count--;
        this->sketch[hash].keySum -= key;
        this->sketch[hash].valueSum -= value;
    }
    return hashes;
}

std::expected<int, OriginalGetError> OriginalSketch::get(int key) {
    std::vector<int> hashes = this->hash_distinct(key);
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

std::pair<std::unordered_map<int, int>, bool> OriginalSketch::decode() {
    std::unordered_map<int, int> result;
    
    // holds indexes with known count = 1
    std::deque<size_t> singletons;
    for (int i = 0; i < this->sketch.size(); i++) {
        int count = this->sketch[i].count;
        
        if (count == 1) {
            singletons.push_back(i);
        }
    }
    
    while (!singletons.empty()) {
        size_t index = singletons.front();
        singletons.pop_front();
        OriginalCell cell = this->sketch[index];
        // should only be 0 if not 1, since it should've been 1 if included in the singletons
        if (cell.count != 1) {
            continue;
        }
        result.insert({cell.keySum, cell.valueSum});
        std::vector<int> indexes = this->remove(cell.keySum, cell.valueSum);
        for (int deleted_index : indexes) {
            if (deleted_index == index) {
                continue;
            }
            OriginalCell deleted_cell = this->sketch[deleted_index];
            if (deleted_cell.count == 1) {
                singletons.push_back(deleted_index);
            }
        }
    }
    // check if any of the sketch still has anything in it
    if (std::any_of(this->sketch.begin(), this->sketch.end(), [](OriginalCell x) {return !x.empty(); })) {
        return {result, true};
    }
    
    return {result, false};
}
