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
    .def(py::init([](py::object bootstrap_with = py::none()) {
      auto instance = new DepQBF();
      if (!bootstrap_with.is_none()) {
        py::list clauses = py::cast<py::list>(bootstrap_with);
        for (auto clause : clauses) {
          instance->add_clause(py::cast<std::vector<int>>(clause));
        }
      }
      return instance;
    }), py::arg("bootstrap_with") = py::none())

    .def("configure", &DepQBF::configure)
    .def("add_quantifier_block", &DepQBF::add_quantifier_block)
    .def("add_clause", &DepQBF::add_clause)
    
    .def("append_formula", [](DepQBF &self, py::iterable clauses) {
      for (auto clause : clauses) {
        self.add_clause(py::cast<std::vector<int>>(clause));
      }
    })
    
    .def("solve", [](DepQBF &self, py::list assumptions = py::list()) {
      self.reset();  // Automatically reset before solving
      return self.solve(py::cast<std::vector<int>>(assumptions));
    }, py::arg("assumptions") = py::list())

    .def("get_outermost_model", &DepQBF::get_outermost_model)
    .def("get_core", &DepQBF::get_core)
    .def("reset", &DepQBF::reset);
}
