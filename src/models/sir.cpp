#include "sir.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor. */
std::shared_ptr<epimodels::ModelSIR<int>> ModelSIR(std::string name,
												   double prevalence,
												   double transmission_rate,
												   double recovery_rate) {

	std::shared_ptr<epimodels::ModelSIR<int>> object(
		new epimodels::ModelSIR<int>(name, prevalence, transmission_rate,
									 recovery_rate));

	return object;
}

static std::shared_ptr<DataBase<int>> get_db(epimodels::ModelSIR<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_sir(
	pybind11::class_<epiworld::epimodels::ModelSIR<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIR<int>>> &c) {
	c.def("print", &epimodels::ModelSIR<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelSIR<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_sir_constructor(pybind11::module &m) {
	m.def("ModelSIR", &ModelSIR, "Create a new SIR model.", py::arg("name"),
		  py::arg("prevalence"), py::arg("transmission_rate"),
		  py::arg("recovery_rate"));
}
