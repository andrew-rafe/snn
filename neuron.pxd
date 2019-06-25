from libcpp.vector cimport vector
from libcpp cimport bool

cdef extern from "neuron.cpp":
  pass

cdef extern from "neuron.h" namespace "SNN":
  cdef cppclass Neuron:
    float get_potential()
