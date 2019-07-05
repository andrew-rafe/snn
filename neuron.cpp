#include <iostream>
#include "neuron.h"
#include "hyperparams_neuron.hpp"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace SNN;

//DEFAULT CONSTRUCTOR
Neuron::Neuron() {
    potential = 0;
    last_update_timestep = 0;
    refractory_start_timestep = 0 ;
    num_connections = 0;
    firing_buffer.resize(Hyperparams_Neuron::BUFFER_SIZE, false);
}

/*Neuron::Neuron(float threshold, float leak_resistance, float resting_potential,
    float refractory_potential, int refractory_period) {
    this.threshold = threshold;
    this.leak_resistance = leak_resistance;
    this.resting_potential = resting_potential;
    this.refractory_potential = refractory_potential;
    this.refractory_period = refractory_period;

    potential = 0;
    last_update_timestep = 0;
    refractory_start_timestep = 0 - refractory_period;
}*/

Neuron::~Neuron() {

} // DONT NEED ANYTHING ELSE HERE AS EVERYTHING IS PRIMITIVE

//Given a vector of all of the neurons in the next layer, this method will
//populate this neurons connections member variable with random weights between
//0.0 and 1.0 inclusive. This method should only be called after a seed has been
//created.
void Neuron::set_neuron_connections(const std::vector<Neuron*>& neurons_in_next) {

    //Will add all of the connections from this neuron to every neuron in the
    //next layer
    for (int i = 0; i < neurons_in_next.size(); i++) {
        //Will add one of the connections with a random weight
        connections.push_back({neurons_in_next[i], ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX))*2 - 1)});
        num_connections++;
    }

}

//Will update the neuron potential due to leak based off how long it
//has been since this neuron has been processed.
void Neuron::update_neuron_potential_leak(long long timestep) {
    //Check how many timesteps it has been since the last update
    int steps_since_last_update = static_cast<unsigned int>(timestep) - last_update_timestep;
    //Loop through and apply the update according to equation
    //leak(t) = -1/leak_resistance * (potential - resting_potential)
    //TODO:Is there another way to calculate the leak in one step instead
    // of looping through all of the steps sinces last update

    //This method only works with resting potential = 0. Need to figure out
    //a different method for resting_potental != 0
    if (steps_since_last_update > 0) {
        potential = potential * pow((1-1/Hyperparams_Neuron::LEAK_RESISTANCE), static_cast<float>(steps_since_last_update));
    }

    //Update the firing buffer to remove true fires from outside the buffer size
    if (steps_since_last_update >= Hyperparams_Neuron::BUFFER_SIZE) {
        std::fill(firing_buffer.begin(), firing_buffer.end(), false);
    } else {
        //Otherwise we need to set the oldest number of buffer positions back to false
        //TODO: Figure out a more efficient way to do this. CAN I USE FILL LIKE ABOVE
        for (int i = 1; i < steps_since_last_update; i++) {
            firing_buffer[(timestep + i) % Hyperparams_Neuron::BUFFER_SIZE] = false;
        }
    }

    /*
    while (steps_since_last_update > 0) {
        potential -= (1/leak_resistance) * (potential - resting_potential);
        //Break if the potential drops to 0
        if (potential < 0) {
            potential = 0;
            break;
        } else {
            //Otherwise we need to step through each leak timestep
            steps_since_last_update -= 1;
        }
    }*/

}

//Will update the neuron potential based off an incoming signal
//Need to make sure not to do anything to neurons within refractory_periods
bool Neuron::adjust_neuron_potential(float incoming_potential, long long timestep) {
    //Check that this neuron is not in a period of refraction
    //if it is we don't want to do anything to its neuron potential
    if (static_cast<unsigned int>(timestep) - Hyperparams_Neuron::REFRACTORY_PERIOD >= refractory_start_timestep) {
        //Check that the neuron potential if was in a period of refractory
        //is now not less than the resting potential
        if (potential < Hyperparams_Neuron::RESTING_POTENTIAL) {
            potential = Hyperparams_Neuron::RESTING_POTENTIAL;
        } else {
            //First we need to apply the leak to this neuron
            update_neuron_potential_leak(timestep);
        }

        //Then we need to add the incoming potential to this neuron
        potential += incoming_potential;
        //std::cout<<potential<<std::endl;

        //We need to set leak update step to be the current time step to prevent
        //leaking too much
        last_update_timestep = timestep;
        //Set for firing if neuron potential is greater than threshold and put
        //this neuron into a period of refraction
        if (potential >= Hyperparams_Neuron::THRESHOLD) {
            set_refractory(timestep);
            return true;
        }

    }

    //The neuron has not exceeded its threshold or is still in its refractory period
    return false;

}

//Will process the firing of this neuron and return a vector of all of the
//next layer neurons that need to be processed for firing
std::vector<Neuron*> Neuron::process_firing(long long timestep) {
    //Will only reach this function when it has been queued to be processed for firing
    //the potential will have already have been reset and sent into a refractory period
    //when this neuron is queued for processing the firing.
    //Therefore all we have to do is adjust the neuron potential of all of its connections
    std::vector<Neuron*> new_to_process;

    for (int i = 0; i < num_connections; i++) {
        //If the post synaptic neuron is ready to fire then we need to add it to the new_process
        //vector which will be returned
        if (connections[i].first->adjust_neuron_potential(connections[i].second * Hyperparams_Neuron::THRESHOLD, timestep)) {
            new_to_process.push_back(connections[i].first);
        }
    }

    //Add this timestep to the firing buffer
    firing_buffer[timestep % Hyperparams_Neuron::BUFFER_SIZE] = true;

    return new_to_process;
}

//Will set this neuron to be in a refractory_period
void Neuron::set_refractory(long long timestep) {
    refractory_start_timestep = static_cast<unsigned int>(timestep);
    potential = Hyperparams_Neuron::REFRACTORY_POTENTIAL;
}

float Neuron::get_potential() {
    return potential;
}

float Neuron::get_interspike_interval() {
    int num_fires = std::count(firing_buffer.begin(), firing_buffer.end(), true);
    //std::cout << num_fires;
    return (num_fires != 0) ? (float)Hyperparams_Neuron::BUFFER_SIZE/(float)num_fires : 0;
}


//TRAINING HAPPENS AT INDIVIDUAL NEURON LEVEL
//GETS THE REWARD AND CHANGES ITS WEIGHTS ACCORDINGLY
void Neuron::process_training(const int& reward) {
    //if the reward is positive then we want to strengthen connections with those that fired together
    //if the reward is negative we want to weaken connections with those that fired together
    //We need some probing of the post_synaptic neuron to see if it fired at that same time
    //So pass in a buffer index and ask whether it fired as well
    for (int i = 0; i < Hyperparams_Neuron::BUFFER_SIZE; i++) {
        //THIS NEEDS TO BE MADE MORE EFFICIENT
        //Look through all of the connections
        if (firing_buffer[i] == true) {
            for (auto it = connections.begin(); it != connections.end(); ++it) {
                if ((*it).first->did_it_fire(i)) {
                    //Strengthen or weaken based on reward
                    (*it).second += (reward > 0) ? 0.01 : -0.01;
                    
                    if ((*it).second < -1) {
                        (*it).second = -1;
                    } else if ((*it).second > 1) {
                        (*it).second = 1;
                    }
                    std::cout << "Changing weight to: " << (*it).second << std::endl;
                } else if (!(*it).first->did_it_fire(i)) {
                    //Strengthen or weaken based on reward
                    (*it).second += (reward > 0) ? -0.01 : 0.01;
                    
                    if ((*it).second < -1) {
                        (*it).second = -1;
                    } else if ((*it).second > 1) {
                        (*it).second = 1;
                    }
                    std::cout << "Changing weight to: " << (*it).second << std::endl;
                }
                //MIGHT NEED TO CLAMP THE WEIGHTS
            }
        }
        
    }
}

bool Neuron::did_it_fire(const int& buffer_index) {
    return firing_buffer[buffer_index];
}