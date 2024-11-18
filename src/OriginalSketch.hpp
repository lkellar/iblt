//
//  OriginalSketch.hpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/14/24.
//

#ifndef OriginalSketch_hpp
#define OriginalSketch_hpp

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <expected>
#include "multihash.hpp"
#include <utility>

#include <iostream>

/* two types of issues can occur in a GET lookup
    - not_present means it's just not in the set
    - failed means that it could be in the set, but we weren't able to extract it IF it was in there
 */
enum OriginalGetError {not_present, failed};

struct OriginalCell {
    int count = 0;
    int keySum = 0;
    int valueSum = 0;
    
    bool empty() {
        return count == 0 && keySum == 0 && valueSum == 0;
    }
};

class OriginalSketch {
public:
    OriginalSketch(size_t sketch_size, const MultiHash& hasher);
    void insert(int key, int value);
    std::unordered_set<int> remove(int key, int value);
    // merge must use same hash funcitions probably
    //void merge(const OriginalSketch& other);
    std::expected<int, OriginalGetError> get(int x);
    // returns a list and an "incomplete" flag
    // this is desctructive
    std::pair<std::unordered_map<int, int>, bool> decode();
    void printSketch() {
        for (auto cell : this->sketch) {
            std::cout << "Count: " << cell.count << " | KeySum: " << cell.keySum << " | ValueSum: " << cell.valueSum << '\n';
        }
    }
private:
    std::unordered_set<int> hash_distinct(int key);
    std::vector<OriginalCell> sketch;
    const MultiHash& hasher;
};

#endif /* OriginalSketch_hpp */
