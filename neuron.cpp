#include <iostream>
#include "Neuron.h"

#include <vector>

namespace SNN {

//DEFAULT CONSTRUCTOR
    Neuron::Neuron() {
        threshold = 1;
        leak_resistance = 5;
        resting_potential = 0;
        refractory_period = 5;
        refractory_potential = -1;

        potential = 0;
        last_update_timestep = 0;
        refractory_start_timestep = 0 - refractory_period;

    }

    Neuron::Neuron(float threshold, float leak_resistance, float resting_potential,
        float refractory_potential, int refractory_period) {
        this.threshold = threshold;
        this.leak_resistance = leak_resistance;
        this.resting_potential = resting_potential;
        this.refractory_potential = refractory_potential;
        this.refractory_period = refractory_period;

        potential = 0;
        last_update_timestep = 0;
        refractory_start_timestep = 0 - refractory_period;
    }

    Neuron::~Neuron() { } // DONT NEED ANYTHING HERE AS EVERYTHING IS PRIMITIVE

    //Will update the neuron potential due to leak based off how long it
    //has been since this neuron has been processed.
    void Neuron::update_neuron_potential_leak(longlong timestep) {
        //Check how many timesteps it has been since the last update
        int steps_since_last_update = timestep - last_update_timestep;
        //Loop through and apply the update according to equation
        //leak(t) = -1/leak_resistance * (potential - resting_potential)
        //TODO:Is there another way to calculate the leak in one step instead
        // of looping through all of the steps sinces last update
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
        }

        //We need to set leak update step to be the current time step to prevent
        //leaking too much
        last_update_timestep = timestep;
    }

    //Will update the neuron potential based off an incoming signal
    //Need to make sure not to do anything to neurons within refractory_periods
    bool Neuron::adjust_neuron_potential(float incoming_potential, longlong timestep) {
        //Check that this neuron is not in a period of refraction
        //if it is we don't want to do anything to its neuron potential
        if (timestep - refractory_period < refractory_start_timestep) return;
        //First we need to apply the leak to this neuron
        update_neuron_potential_leak(timestep);
        //Then we need to add the incoming potential to this neuron
        potential += incoming_potential;
        //Set for firing if neuron potential is greater than threshold and put
        //this neuron into a period of refraction
        if (potential >= threshold) {
            set_refractory(timestep);
            return true;
        }
        //The neuron has not exceeded its threshold and has not fired
        //therefore return false
        return false;
    }

    //Will process the firing of this neuron and return a vector of all of the
    //next layer neurons that need to be processed for firing
    std::vector<Neuron*> Neuron::process_firing(const longlong& timestep) {
        //Will only reach this function when it has been queued to be processed for firing
        //the potential will have already have been reset and sent into a refractory period
        //when this neuron is queued for processing the firing.
        //Therefore all we have to do is adjust the neuron potential of all of its connections
        for (int i = 0; i < num_connections; i++) {
            connections[i].first->adjust_neuron_potential(connections[i].second * threshold);
        }
    }
    
    //Will set this neuron to be in a refractory_period
    void Neuron::set_refractory(longlong timestep) {
        refractory_start_timestep = timestep;
        potential = refractory_potential;
    }



}
