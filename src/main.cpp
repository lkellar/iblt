//
//  main.cpp
//  Set Reconciliation
//
//  Created by Lucas Kellar on 11/7/24.
//

#include <iostream>
#include "HPWSketch.hpp"

int main(int argc, const char * argv[]) {
    HPWSketch sketch = HPWSketch(2000, 4);
    int value;
    while (std::cin >> value) {
        sketch.toggle(value);
    }
    
    if (auto out = sketch.decode()) {
        for (auto item : *out) {
            std::cout << item << '\n';
        }
    } else {
        std::cerr << "Decode failed" << std::endl;
    }
    return 0;
}
