#include "surv.hpp"

#include <pybind11/numpy.h>
#include <pybind11/stl.h>

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

/* Proxy constructor.
 *
 * TODO: I noticed that epiworld::epimodels::ModelSURV is overloaded to accept a
 * variable number of arguments, should this be expressed on the Python side? It
 * isn't in R, as far as I can tell, and I don't know how concerning symmetry
 * is.
 */
std::shared_ptr<epimodels::ModelSURV<int>>
ModelSURV(std::string name, double prevalence, double efficacy_vax,
		  double latent_period, double prob_symptoms, double prop_vaccinated,
		  double prop_vax_redux_transm, double infect_period,
		  double prop_vax_redux_infect, double surveillance_prob,
		  double transmission_rate, double prob_death, double prob_noreinfect) {

	std::shared_ptr<epimodels::ModelSURV<int>> object(
		new epimodels::ModelSURV<int>(
			name, prevalence, efficacy_vax, latent_period, prob_symptoms,
			prop_vaccinated, prop_vax_redux_transm, infect_period,
			prop_vax_redux_infect, surveillance_prob, transmission_rate,
			prob_death, prob_noreinfect));

	return object;
}

static std::shared_ptr<DataBase<int>> get_db(epimodels::ModelSURV<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

void epiworldpy::export_surv(
	pybind11::class_<epiworld::epimodels::ModelSURV<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSURV<int>>> &c) {
	c.def("print", &epimodels::ModelSURV<int>::print,
		  "Print a summary of the model run.", py::arg("summary") = true)
		.def("run", &epimodels::ModelSURV<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference);
}

void epiworldpy::export_surv_constructor(pybind11::module &m) {
	m.def("ModelSURV", &ModelSURV, "Create a new SURV model.", py::arg("name"),
		  py::arg("prevalence"), py::arg("efficacy_vax"),
		  py::arg("latent_period"), py::arg("prob_symptoms"),
		  py::arg("prop_vaccinated"), py::arg("prop_vax_redux_transm"),
		  py::arg("infect_period"), py::arg("prop_vax_redux_infect"),
		  py::arg("surveillance_prob"), py::arg("transmission_rate"),
		  py::arg("prob_death"), py::arg("prob_noreinfect"));
}
