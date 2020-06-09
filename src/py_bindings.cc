#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>

#include "pillar_state/pillar_state.h"

PYBIND11_MODULE(pillar_state_py, m)
{
  namespace py = pybind11;
  namespace P = Pillar;

  py::class_<P::State>(m, "State")
    .def(py::init<>())
    .def("has_high_ground", &P::State::has_high_ground);
}
