#ifndef HYPERPARAMS_NEURON_HPP
#define HYPERPARAMS_NEURON_HPP

namespace SNN {
    class Hyperparams_Neuron {
        public:

        constexpr static int THRESHOLD = 1;
        constexpr static int LEAK_RESISTANCE = 4;
        constexpr static float RESTING_POTENTIAL = 0;
        constexpr static int REFRACTORY_PERIOD = 5;
        constexpr static float REFRACTORY_POTENTIAL = -1;

        constexpr static int BUFFER_SIZE = 20;

        private:
        Hyperparams_Neuron() {}

    };
}

#endif