#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <queue>
#include <string>

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

        int process_inputs(const std::vector<float>& inputs, long long timestep, int num_inputs);
        int get_num_neurons();
        std::string get_outputs();
        std::string print_network();

        //Will retrieve an action based on the lowest interspike interval of the outputs.
        int get_action();
        //friend std::ostream& operator<<(std::ostream& os, const Network& network);
        void process_training(int reward);

    private:

        std::vector<Neuron*> inputs;
        std::vector<Neuron*> outputs;
        std::vector<std::vector<Neuron*>> hidden_layers;
        std::vector<Neuron*> create_layer(int num_in_layer);

    };
}

#endif
