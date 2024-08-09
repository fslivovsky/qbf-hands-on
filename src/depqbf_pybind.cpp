#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "depqbf.h"

namespace py = pybind11;

PYBIND11_MODULE(depqbf_module, m) {
py::class_<DepQBF::DepQBF_exception>(m, "DepQBFException")
  .def(py::init<const std::string&>())
  .def("__str__", [](const DepQBF::DepQBF_exception &e) {
    return e.what();
  });

py::class_<DepQBF>(m, "DepQBF")
  .def(py::init<>())
  .def("configure", &DepQBF::configure)
  .def("add_quantifier_block", &DepQBF::add_quantifier_block)
  .def("add_clause", &DepQBF::add_clause)
  .def("solve", &DepQBF::solve)
  .def("get_outermost_model", &DepQBF::get_outermost_model)
  .def("get_core", &DepQBF::get_core)
  .def("reset", &DepQBF::reset);
}