#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <queue>

nampespace SNN {
  class Network {
  public:

    //CONSTRUCTOR AND DESTRUCTOR
    Network();
    Network(int num_layers, std::vector<int> neurons_per_layer);
    ~Network();

    

  private:

    std::vector<std::vector<Neuron*>> neurons;
    std::vector<std::vector<float>> weights;

    std::queue<Neuron*> unprocessed;

  }
}
