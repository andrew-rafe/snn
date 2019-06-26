# distutils: language = c++

from neuron cimport Neuron
from network cimport Network
from libc.stdlib cimport malloc, free

import numpy as np
cimport numpy as np
import ctypes

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

  def process_inputs(self, np.ndarray[np.float_t,ndim=1,mode='c']input_values, timestep, num_inputs):
    cdef np.ndarray[float, ndim=1, mode="c"] c_inputs = np.ascontiguousarray(input_values, dtype = ctypes.c_float)
    cdef float * input_pointer = <float *>malloc(input_values.size * sizeof(float))
    if not input_pointer:
      raise MemoryError
    try:
      return self.c_net.process_inputs(input_pointer, timestep, num_inputs)
    finally:
      free(input_pointer)

  def get_num_neurons(self):
    return self.c_net.get_num_neurons()

  def get_outputs(self):
    return self.c_net.get_outputs()
