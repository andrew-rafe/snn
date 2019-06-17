#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <pair>

namespace SNN {
    
    class Neuron {
        
    public:
        
        Neuron();
        Neuron(float threshold, float leak_resistance, float resting_potential,
            float refractory_potential, float refractory_period);
        ~Neuron();
        
        //A setter method to add all of the connections between this neuron and the next
        //layer of neurons
        void set_neuron_connections(const std::vector<std::pair<Neuron*, float>>& connections);
        
        //Will update the neuron potential due to leak based off how long it
        //has been since this neuron has been processed.
        void update_neuron_potential_leak(const longlong& timestep);
        
        //Will update the neuron potential based off an incoming signal'
        //This will return true if the neuron potential exceeds its threshold
        //and will set this neuron into a period of refraction
        bool adjust_neuron_potential(float incoming_potential, const longlong& timestep);
        
        //This method will process the firing of this neuron to all of its connected neurons
        std::vector<Neuron*> process_firing(const longlong& timestep);
        
        //Will set this neuron to be in a refractory_period
        void set_refractory(const longlong& timestep);
        
        friend std::ostream& operator<<(std::ostream& os, const Neuron& neuron);

    private:
        
        //HOLDS ALL OF THE NEURONS AND THE WEIGHTS BETWEEN THIS
        //NEURON AND ALL THE CONNECTED NEURONS IN THE NEXT LAYER
        std::vector<std::pair<Neuron*, float>> connections;
        //Will keep track of this int separately as using .size() on the vector
        //uses a counting algorithm instead of a set value. Doing this for increased
        //efficiency
        int num_connections;
            
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
