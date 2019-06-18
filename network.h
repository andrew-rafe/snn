#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <queue>

#include "neuron.h"

namespace SNN {
    class Network {
    public:

        //CONSTRUCTOR AND DESTRUCTdwOR
        //The constructors will populate the inputs and output layers
        //and then deal with the connections between all of the hidden
        //layer neurons
        //Network();
        Network(const int num_layers, const std::vector<int>& neurons_per_layer);
        ~Network();
        
        int process_inputs(std::vector<float> input_values, long long timestep);
        std::vector<Neuron*> get_outputs();
        
        //friend std::ostream& operator<<(std::ostream& os, const Network& network);

    private:
        
        std::vector<Neuron*> inputs;
        std::vector<Neuron*> outputs;
        std::vector<std::vector<Neuron*>> hidden_layers;
        std::vector<Neuron*> create_layer(int num_in_layer);
        
    };
}

#endif
