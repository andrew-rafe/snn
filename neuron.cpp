#include <iostream>
#include "Neuron.h"

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
  void Neuron::adjust_neuron_potential(float incoming_potential, longlong timestep) {
    //Check that this neuron is not in a period of refraction
    //if it is we don't want to do anything to its neuron potential
    if (timestep - refractory_period < refractory_start_timestep) return;
    //First we need to apply the leak to this neuron
    update_neuron_potential_leak(timestep);
    //Then we need to add the incoming potential to this neuron
    potential += incoming_potential;
  }

  //Will set this neuron to be in a refractory_period
  void Neuron::set_refractory(longlong timestep) {
    refractory_start_timestep = timestep;
    potential = refractory_potential;
  }



}
