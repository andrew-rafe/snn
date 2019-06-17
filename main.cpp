#include <iostream>
#include <vector>

#include "network.h"

using namespace SNN;

int main() {
    
    const int NUM_LAYERS = 5;
    std::vector<int> NEURONS_PER_LAYER {10, 20, 10, 8, 5};
    
    Network* network = new Network(NUM_LAYERS, NEURONS_PER_LAYER);
    
    delete network;
    
    return 0;
}

