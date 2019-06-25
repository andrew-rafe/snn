# distutils: language = c++

from neuron cimport Neuron
from network cimport Network

cdef class PyNeuron:
  cdef Neuron* c_neuron

  def get_potential(self):
    return self.c_neuron.get_potential()

cdef class PyNetwork:
  cdef Network* c_net

  def __cinit__(self, num_layers, neurons_per_layer):
    self.c_net = new Network(num_layers, neurons_per_layer)

  def __dealloc__(self):
    del self.c_net

  def process_inputs(self, input_values, timestep):
    return self.c_net.process_inputs(input_values, timestep)

  def get_num_neurons(self):
    return self.c_net.get_num_neurons()

  def get_outputs(self):
    self.c_net.get_outputs()
