#ifndef NEURON_H
#define NEURON_H

namespace SNN {
  class Neuron {
  public:
    Neuron();
    Neuron(float threshold, float leak_resistance, float resting_potential,
            float refractory_potential, float refractory_period);
    ~Neuron();

    //Will update the neuron potential due to leak based off how long it
    //has been since this neuron has been processed.
    void update_neuron_potential_leak(longlong timestep);
    //Will update the neuron potential based off an incoming signal
    void adjust_neuron_potential(float incoming_potential);
    //Will set this neuron to be in a refractory_period
    void set_refractory(longlong timestep);

  private:
    //KEEPS TRACK OF CURRENT NEURON POTENTIAL OF NEURON
    float potential;
    longlong last_update_timestep;
    longlong refractory_start_timestep;

    //HYPERPARAMETERS PER NEURON
    float threshold;
    float leak_resistance;
    float resting_potential;
    int refractory_period;
    float refractory_potential;

  }
}

#endif
