#include <pybind11/pybind11.h>

#include "pillar_state/pillar_state.hpp"

PYBIND11_MODULE(pillar_state_py, m)
{
  namespace py = pybind11;
  namespace P = Pillar;

  py::class_<P::State>(m, "State")
    .def(py::init<>())
    .def("num_properties", &P::State::num_properties)
    .def_static("create_from_yaml_file", &P::State::create_from_yaml_file, py::arg("yaml_path"));
}