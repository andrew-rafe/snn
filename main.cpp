#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "network.h"

using namespace SNN;

float* generate_rand_inputs(int num_of_inputs);

int main() {

    const int NUM_LAYERS = 3;
    std::cout << "Generating Network" << std::endl;
    std::vector<int> NEURONS_PER_LAYER {25, 10, 4};

    Network* network = new Network(NUM_LAYERS, NEURONS_PER_LAYER);

    long long timestep = 1;

    for (int i = 0; i < 10; i++) {
        float* inputs = generate_rand_inputs(NEURONS_PER_LAYER[0]);
        //network->process_inputs(inputs, timestep, NEURONS_PER_LAYER[0]);
        timestep++;
        std::cout << network->print_network() << std::endl;
        delete(inputs);
    }

    delete network;

    return 0;
}

//Generate some random inputs
float* generate_rand_inputs(int num_of_inputs) {

    float* rand_inputs = new float[num_of_inputs];

    for (int i = 0; i < num_of_inputs; i++) {
        rand_inputs[i] = static_cast<float>((rand() / static_cast<float>(RAND_MAX)));
    }

    return rand_inputs;

}
