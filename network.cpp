#include <vector>
#include <queue>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <unordered_map>

#include "network.h"

using namespace SNN;

//Network::Network() { }
Network::Network(const int num_layers, const std::vector<int>& neurons_per_layer) { 
    
    //Seed randomness when creating network
    srand(static_cast<unsigned>(time(0)));
    
    //Check that the network has an input, at least one hidden and an output layer
    //i.e. that the num layers is greater or equal to 3
    assert(num_layers >= 3);
    //Then need to check whether the number of layers matches the vector size
    //storing the neurons per layer
    assert(neurons_per_layer.size() == num_layers);
    
    //If the above asserts pass then we need to start creating the layers
    //First create the input layer
    inputs = create_layer(neurons_per_layer[0]);
    std::vector<Neuron*> prev_layer, next_layer;
    prev_layer = inputs;
    for (int i = 1; i < num_layers-1; i++) {
        next_layer = create_layer(neurons_per_layer[i]);
        hidden_layers.push_back(next_layer);
        //Loop through all of the neurons in the prev layer and
        //apply the connections to the neurons in the next layer
        for (int j = 0; j < prev_layer.size(); j++) {
            prev_layer[j]->set_neuron_connections(next_layer);
        }
        prev_layer = next_layer;
    }
    
    //Finalise the output layer
    //Create the output layer
    outputs = create_layer(neurons_per_layer[neurons_per_layer.size() - 1]);
    //Link the prev layer to the output layer
    for (int j = 0; j < prev_layer.size(); j++) {
            prev_layer[j]->set_neuron_connections(outputs);
    }
}

//Need to propagate through the network and delete all of the neurons
Network::~Network() {
    //First delete all of the input layer neurons
    for (int i = 0; i < inputs.size(); i++) {
        delete inputs[i];
    }
    //Then delete all of the hidden layer neurons
    for (int i = 0; i < hidden_layers.size(); i++) {
        for (int j = 0; j < hidden_layers[i].size(); j++) {
            delete hidden_layers[i][j];
        }
    }
    //Then delete the ouput layer neurons
    for (int i = 0; i < outputs.size(); i++) {
        delete outputs[i];
    }
}

//A helper function to create a single layer
std::vector<Neuron*> Network::create_layer(int num_in_layer) {
    std::vector<Neuron*> layer;
    for (int i = 0; i < num_in_layer; i++) {
        layer.push_back(new Neuron());
    }
    return layer;
}

//Send the input values to the input layer which will propogate everything
//through the network. It will return the number of neurons that have been processed
//in this timestep
int Network::process_inputs(std::vector<float> input_values, const long long& timestep) { 
    //Create an unordered map to keep track of all of the neurons added to the process queue
    std::unordered_map<Neuron*, bool> already_added_to_queue;
    //Keep track of all the neurons needed to be processed
    std::queue<Neuron*> to_process;
    //Keep track of how many neurons have been processed
    int neurons_processed;
    //Check that the amount of input values matches the number of input neurons
    assert(input_values.size() == inputs.size());
    //Give the input neurons the input values
    bool process_fire = false;
    for (int i = 0; i < inputs.size(); i++) {
        process_fire = inputs[i]->adjust_neuron_potential(input_values[i], timestep);
        //Queue up all the inputs that need to be processed for firing
        if (process_fire) {
            //Check to make sure it has not already been added to the queue to be processed
            if (already_added_to_queue.find(inputs[i]) != already_added_to_queue.end()) {
                to_process.push(inputs[i]);
                already_added_to_queue.insert({inputs[i], true});
            }
        } else {
            //All of the inputs will need to be looked at. The other neurons processed
            //that need to be fired will be added to this counter when they are processed
            //in the queue
            neurons_processed++;
        }
        
        return neurons_processed;
    }
    
    std::vector<Neuron*> add_to_queue;
    Neuron* next;
    //Now process all of the neurons in the queue until all necessary neurons are processed
    while (!to_process.empty()) {
        //Get the next in the queue before removing the neuron from the queue
        next = to_process.front();
        to_process.pop();
        //Increment the number of neurons processed
        neurons_processed++;
        //Get all the neurons to add into the queue
        add_to_queue = next->process_firing(timestep);
        //Check whether the neuron has already been added to the queue and push it if it hasnt
        //CAN I DO THIS MORE EFFICIENTLY??
        for (Neuron* neuron : add_to_queue) {
            if (already_added_to_queue.find(neuron) != already_added_to_queue.end()) {
                to_process.push(neuron);
                already_added_to_queue.insert({neuron, true});
            }
        }
    }
}

//Will return the output layer
std::vector<Neuron*> Network::get_outputs() { 
    return outputs;
}

//Overload to the << operator for printing individual neuron information
/*std::ostream& operator<<(std::ostream& os, const Neuron& neuron) {
    os << "Neuron Potential: " << neuron.potential;
    return os;
}*/
    