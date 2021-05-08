#include <pybind11/pybind11.h>

#include "pillar_state/pillar_state.hpp"

PYBIND11_MODULE(pillar_state_py, m)
{
  namespace py = pybind11;
  namespace P = Pillar;

  py::class_<P::State>(m, "State")
    .def(py::init<>())
    .def("copy", &P::State::copy)
    
    .def_static("create_from_yaml_file", &P::State::create_from_yaml_file, py::arg("yaml_path"))
    .def_static("create_from_serialized_string", &P::State::create_from_serialized_string, py::arg("ser"))

    .def("update_property", (void (P::State::*)(const std::string&, double)) 
        &P::State::update_property, py::arg("property_name"), py::arg("value"))
    .def("update_property", (void (P::State::*)(const std::string&, double, double)) 
        &P::State::update_property, py::arg("property_name"), py::arg("value"), py::arg("variance"))

    .def("update_property", (void (P::State::*)(const std::string&, const std::vector<double>&))
        &P::State::update_property, py::arg("property_name"), py::arg("values"))
    .def("update_property", (void (P::State::*)(const std::string&, const std::vector<double>&, const std::vector<double>&))
        &P::State::update_property, py::arg("property_name"), py::arg("values"), py::arg("variances"))
    .def("update_property", (void (P::State::*)(const std::string&, const std::vector<double>&, const std::vector<std::string>&))
        &P::State::update_property, py::arg("property_name"), py::arg("values"), py::arg("value_names"))
    .def("update_property", (void (P::State::*)(const std::string&, const std::vector<double>&, const std::vector<std::string>&, const std::vector<double>&))
        &P::State::update_property, py::arg("property_name"), py::arg("values"), py::arg("value_names"), py::arg("variances"))

    .def("clear", &P::State::clear)

    .def_property_readonly("num_properties", &P::State::num_properties)
    .def_property_readonly("num_dimensions", &P::State::num_dimensions)
    .def_property_readonly("size", &P::State::size)

    .def("literals", &P::State::literals)
    .def("get_literal_prop_names", &P::State::get_literal_prop_names, py::arg("literal"))

    .def("root_nodes", &P::State::root_nodes)
    .def("get_prop_names", &P::State::get_prop_names)

    .def("has_prop", &P::State::has_prop)

    .def("get_prop_size", &P::State::get_prop_size, py::arg("prop_name"))
    .def("get_prop_sizes", &P::State::get_prop_sizes, py::arg("prop_names")) 
    .def("get_vec_idxs", &P::State::get_vec_idxs, py::arg("prop_names"))
    .def("get_total_prop_sizes", &P::State::get_total_prop_sizes, py::arg("prop_names"))
    .def("get_values_as_vec", (std::vector<double> (P::State::*)(const std::vector<std::string>) const)
        &P::State::get_values_as_vec, py::arg("prop_names"))
    .def("get_value_names_as_vec", &P::State::get_value_names_as_vec, py::arg("prop_names"))
    .def("set_values_from_vec", &P::State::set_values_from_vec, py::arg("prop_names"), py::arg("values"))

    .def("get_serialized_string", &P::State::get_serialized_string)
    .def("update_from_yaml_file", &P::State::update_from_yaml_file, py::arg("yaml_path"))

    .def("__str__", &P::State::get_print_str, py::arg("format")=1)
    
    .def("__getitem__", (std::vector<double> (P::State::*)(const std::string&) const)
        &P::State::get_values_as_vec, py::arg("prop_name"))
    .def("__setitem__", (void (P::State::*)(const std::string&, const std::vector<double>&))
        &P::State::update_property, py::arg("property_name"), py::arg("values"))
  ;

}