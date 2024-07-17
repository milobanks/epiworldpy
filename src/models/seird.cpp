#include "seird.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor. */
std::shared_ptr<epimodels::ModelSEIRD<int>>
ModelSEIRD(std::string name, double prevalence, double transmission_rate,
		   double incubation_days, double recovery_rate, double death_rate) {

	std::shared_ptr<epimodels::ModelSEIRD<int>> object(
		new epimodels::ModelSEIRD<int>(name, prevalence, transmission_rate,
									   incubation_days, recovery_rate,
									   death_rate));

	return object;
}

static std::shared_ptr<DataBase<int>> get_db(epimodels::ModelSEIRD<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_seird(
	pybind11::class_<epiworld::epimodels::ModelSEIRD<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSEIRD<int>>>
		&c) {
	c.def("print", &epimodels::ModelSEIRD<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelSEIRD<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_seird_constructor(pybind11::module &m) {
	m.def("ModelSEIRD", &ModelSEIRD, "Create a new SEIRD model.",
		  py::arg("name"), py::arg("prevalence"), py::arg("transmission_rate"),
		  py::arg("incubation_days"), py::arg("recovery_rate"),
		  py::arg("death_rate"));
}
