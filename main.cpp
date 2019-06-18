#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "network.h"

using namespace SNN;

std::vector<float> generate_rand_inputs(int num_of_inputs);

int main() {
    
    const int NUM_LAYERS = 3;
    std::vector<int> NEURONS_PER_LAYER {10000, 5000, 10};
    
    Network* network = new Network(NUM_LAYERS, NEURONS_PER_LAYER);
    
    long long timestep = 1;
    
    for (int i = 0; i < 100; i++) {
        std::vector<float> inputs = generate_rand_inputs(NEURONS_PER_LAYER[0]);
        //std::cout << "Neurons Processed: " << network->process_inputs(inputs, timestep) << " Timestep: " << timestep << std::endl;
        timestep++;
    }
    
    delete network;
    
    return 0;
}

//Generate some random inputs
std::vector<float> generate_rand_inputs(int num_of_inputs) {
    
    std::vector<float> rand_inputs;
    
    for (int i = 0; i < num_of_inputs; i++) {
        rand_inputs.push_back(static_cast<float>((rand() / static_cast<float>(RAND_MAX))/4));
    }
    
    return rand_inputs;

}

