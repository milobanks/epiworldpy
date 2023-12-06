#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h> // for py::array_t
#include "epiworld-common.hpp"

using namespace epiworld;

namespace py = pybind11;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


std::shared_ptr< epimodels::ModelSEIRCONN<int> > ModelSEIR(
    std::string name,
    int n,
    double prevalence,
    double contact_rate,
    double transmission_rate,
    double incubation_days,
    double recovery_rate
    ) {

    std::shared_ptr< epimodels::ModelSEIRCONN<int> > object(
        new epimodels::ModelSEIRCONN<int>(
            name,
            n,
            prevalence,
            contact_rate,
            transmission_rate,
            incubation_days,
            recovery_rate
        )
        );

    return object;
}


PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
        epiworldpy wrapper
        -----------------------

        .. currentmodule:: epiworldpy

        .. autosummary::
           :toctree: _generate

           ModelSEIR
    )pbdoc";

    // Only this is necesary to expose the class
    py::class_<epimodels::ModelSEIRCONN<int>, std::shared_ptr<epimodels::ModelSEIRCONN<int>>>(m, "ModelSEIRCONN")
        // .def(py::init<>())
        .def("print", &epimodels::ModelSEIRCONN<int>::print,
            // py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>(),
            R"pbdoc(
            Print the object

            Some other explanation about the print function.)
        )pbdoc",
            py::arg("summary") = true
            )
        .def("run", &epimodels::ModelSEIRCONN<int>::run, R"pbdoc(
            Run the model
        )pbdoc",
            py::arg("ndays"),
            py::arg("seed") = 1u
            );

    // Example with shared_ptr
    m.def("ModelSEIR", &ModelSEIR, R"pbdoc(
        Create an SEIR model
    )pbdoc",
        py::arg("name"),
        py::arg("n"),
        py::arg("prevalence"),
        py::arg("contact_rate"),
        py::arg("transmission_rate"),
        py::arg("incubation_days"),
        py::arg("recovery_rate")
        );

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
