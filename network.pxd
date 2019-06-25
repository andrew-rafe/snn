from libcpp.vector cimport vector
from libcpp cimport bool
from neuron cimport Neuron

cdef extern from "network.cpp":
  pass

# Declare the class with cdef - A bit like a .h file
cdef extern from "network.h" namespace "SNN":
  cdef cppclass Network:
    Network(int, vector[int]&) except +
    int process_inputs(float [], long long)
    int get_num_neurons()
    void get_outputs()
