#include "sirdconn.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor. */
std::shared_ptr<epimodels::ModelSIRDCONN<int>>
ModelSIRDCONN(std::string name, int n, double prevalence, double contact_rate,
			  double transmission_rate, double recovery_rate,
			  double death_rate) {

	std::shared_ptr<epimodels::ModelSIRDCONN<int>> object(
		new epimodels::ModelSIRDCONN<int>(name, n, prevalence, contact_rate,
										  transmission_rate, recovery_rate,
										  death_rate));

	return object;
}

static std::shared_ptr<DataBase<int>>
get_db(epimodels::ModelSIRDCONN<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_sirdconn(
	pybind11::class_<epiworld::epimodels::ModelSIRDCONN<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIRDCONN<int>>>
		&c) {
	c.def("print", &epimodels::ModelSIRDCONN<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelSIRDCONN<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_sirdconn_constructor(pybind11::module &m) {
	m.def("ModelSIRDCONN", &ModelSIRDCONN, "Create a new SIRDCONN model.",
		  py::arg("name"), py::arg("n"), py::arg("prevalence"),
		  py::arg("contact_rate"), py::arg("transmission_rate"),
		  py::arg("recovery_rate"), py::arg("death_rate"));
}
