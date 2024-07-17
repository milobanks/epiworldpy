#include "diffnet.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor. */
std::shared_ptr<epimodels::ModelDiffNet<int>>
ModelDiffNet(std::string name, double prevalence, double prob_adopt,
			 bool normalize_exposure, double *data, int data_ncols,
			 std::vector<size_t> data_cols, std::vector<double> params) {

	std::shared_ptr<epimodels::ModelDiffNet<int>> object(
		new epimodels::ModelDiffNet<int>(name, prevalence, prob_adopt,
										 normalize_exposure, data, data_ncols,
										 data_cols, params));

	return object;
}

static std::shared_ptr<DataBase<int>>
get_db(epimodels::ModelDiffNet<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_diffnet(
	pybind11::class_<epiworld::epimodels::ModelDiffNet<int>,
					 std::shared_ptr<epiworld::epimodels::ModelDiffNet<int>>>
		&c) {
	c.def("print", &epimodels::ModelDiffNet<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelDiffNet<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_diffnet_constructor(pybind11::module &m) {
	m.def("ModelDiffNet", &ModelDiffNet, "Create a new DiffNet model.",
		  py::arg("name"), py::arg("prevalence"), py::arg("prob_adopt"),
		  py::arg("normalize_exposure"), py::arg("data"), py::arg("data_ncols"),
		  py::arg("data_cols"), py::arg("params"));
}
