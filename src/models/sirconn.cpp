#include "sirconn.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor. */
std::shared_ptr<epimodels::ModelSIRCONN<int>>
ModelSIRCONN(std::string name, int n, double prevalence, double contact_rate,
			 double transmission_rate, double recovery_rate) {

	std::shared_ptr<epimodels::ModelSIRCONN<int>> object(
		new epimodels::ModelSIRCONN<int>(name, n, prevalence, contact_rate,
										 transmission_rate, recovery_rate));

	return object;
}

static std::shared_ptr<DataBase<int>>
get_db(epimodels::ModelSIRCONN<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_sirconn(
	pybind11::class_<epiworld::epimodels::ModelSIRCONN<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIRCONN<int>>>
		&c) {
	c.def("print", &epimodels::ModelSIRCONN<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelSIRCONN<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_sirconn_constructor(pybind11::module &m) {
	m.def("ModelSIRCONN", &ModelSIRCONN, "Create a new SIRCONN model.",
		  py::arg("name"), py::arg("n"), py::arg("prevalence"),
		  py::arg("contact_rate"), py::arg("transmission_rate"),
		  py::arg("recovery_rate"));
}
