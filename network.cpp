#include <vector>
#include <queue>
#include <assert.h>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <iostream>
#include <string>

#include "network.h"

using namespace SNN;

//Network::Network() { }
Network::Network(const int num_layers, const std::vector<int>& neurons_per_layer) {

    //Seed randomness when creating network
    srand(static_cast<unsigned>(time(0)));

    //Check that the network has an input, at least one hidden and an output layer
    //i.e. that the num layers is greater or equal to 3
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
int Network::process_inputs(const std::vector<float>& input_values, long long timestep, int num_inputs) {
    //Create an unordered map to keep track of all of the neurons added to the process queue
    std::unordered_map<Neuron*, bool> already_added_to_queue;
    //Keep track of all the neurons needed to be processed
    std::queue<Neuron*> to_process;
    //Keep track of how many neurons have been processed
    int neurons_processed = 0;
    //Check that the amount of input values matches the number of input neurons
    //assert(num_inputs == inputs.size());
    //Give the input neurons the input values
    bool process_fire = false;
    for (int i = 0; i < num_inputs; i++) {
        process_fire = inputs[i]->adjust_neuron_potential(input_values[i], timestep);

        //Queue up all the inputs that need to be processed for firing
        if (process_fire) {
            //Check to make sure it has not already been added to the queue to be processed
            if (already_added_to_queue.find(inputs[i]) == already_added_to_queue.end()) {
                to_process.push(inputs[i]);
                already_added_to_queue.insert({inputs[i], true});
            }
        } else {
            //All of the inputs will need to be looked at. The other neurons processed
            //that need to be fired will be added to this counter when they are processed
            //in the queue
            neurons_processed++;
        }

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
            if (already_added_to_queue.find(neuron) == already_added_to_queue.end()) {
                to_process.push(neuron);
                already_added_to_queue.insert({neuron, true});
            }
        }
    }

    //Need to process all the output neurons every step
    for (auto it = outputs.begin(); it != outputs.end(); ++it) {
        (*it)->update_neuron_potential_leak(timestep);
    }

    return neurons_processed;
}

//Will return the output layer
std::string Network::get_outputs() {
    std::string output_fire = "";

    for (auto it = outputs.begin(); it != outputs.end(); ++it) {
        //std::cout << (*it)->get_interspike_interval() << " ";
        output_fire += (*it)->get_interspike_interval();
        output_fire += "  ";
    }

    /*for (unsigned int i = 0; i < hidden_layers[0].size(); i++) {
      if (hidden_layers[0][i]->get_potential() < float(0)) {
        output_fire += " 1";
      } else {
        output_fire += " 0";
      }
    }*/
    return output_fire;
}

std::string Network::print_network() {
    std::string network_string = "Inputs ";
    for (unsigned int i = 0; i < inputs.size(); i++) {
        if (inputs[i]->get_potential() < float(0)) {
            network_string += " 1";
        } else {
            network_string += " 0";
        }
    }
    network_string += "\n";
    for (unsigned int i = 0; i < hidden_layers.size(); i++) {
        network_string += "HL " + std::to_string(i);
        for (unsigned int j = 0; j < hidden_layers[i].size(); j++) {
            if (hidden_layers[i][j]->get_potential() < float(0)) {
                network_string += " 1";
            } else {
                network_string += " 0";
            }
        }
        network_string += "\n";
    }

    network_string += "Outputs ";
    for (unsigned int i = 0; i < outputs.size(); i++) {
        if (outputs[i]->get_potential() < float(0)) {
            network_string += " 1";
        } else {
            network_string += " 0";
        }
    }
    network_string += "\n";
    return network_string;
}

int Network::get_num_neurons() {
    int num_neurons = 0;
    num_neurons += inputs.size();
    for (std::vector<Neuron*> layer : hidden_layers) {
        num_neurons += layer.size();
    }
    num_neurons += outputs.size();
    return num_neurons;
}

//Overload to the << operator for printing individual neuron information
/*std::ostream& operator<<(std::ostream& os, const Neuron& neuron) {
    os << "Neuron Potential: " << neuron.potential;
    return os;
}*/

int Network::get_action() {
    //Find the lowest interspike interval
    int action = 0;
    float curr_interval = 0;
    //Essentially set it to infinity
    float lowest_interval = 1000;
    for (int i = 0; i < outputs.size(); i++) {
        curr_interval = outputs[i]->get_interspike_interval();
        if (curr_interval < lowest_interval) {
            action = i;
            lowest_interval = curr_interval;
        }
    }

    return action;
}

void Network::process_training(int reward) {
    //Go through all layers and pass on the reward info
    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        (*it)->process_training(reward);
    }
    //For all the hidden layers
    for (auto it = hidden_layers.begin(); it != hidden_layers.end(); ++it) {
        for (auto n_it = (*it).begin(); n_it != (*it).end(); ++it) {
            (*n_it)->process_training(reward);
        }
    }
    //Dont need to deal with the outputs as they have no outgoing connections
}
