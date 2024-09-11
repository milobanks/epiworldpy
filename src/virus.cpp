#include "virus.hpp"
#include "virus-distribute-meat.hpp"

using namespace epiworld;
using namespace epiworldpy;
namespace py = pybind11;

static epiworld::Virus<int> new_virus(std::string name, double prevalence,
									  bool as_proportion, double prob_infecting,
									  double prob_recovery, double prob_death,
									  double post_immunity, double incubation) {
	Virus<int> virus(name, prevalence, as_proportion);

	virus.set_prob_infecting(prob_infecting);
	virus.set_prob_recovery(prob_recovery);
	virus.set_prob_death(prob_death);
	virus.set_incubation(incubation);

	if (post_immunity > 0.0) {
		virus.set_post_immunity(post_immunity);
	}

	return virus;
}

static std::string get_name(epiworld::Virus<int> &self) {
	return self.get_name();
}

static void set_name(epiworld::Virus<int> &self, std::string name) {
	self.set_name(name);
}

static void set_prob_infecting(epiworld::Virus<int> virus,
							   double prob_infecting) {
	virus.set_prob_infecting(prob_infecting);
}

static void set_prob_recovery(epiworld::Virus<int> virus,
							  double prob_recovery) {
	virus.set_prob_recovery(prob_recovery);
}

static void set_prob_death(epiworld::Virus<int> virus, double prob_death) {
	virus.set_prob_death(prob_death);
}

static void set_incubation(epiworld::Virus<int> virus, double incubation) {
	virus.set_prob_infecting(incubation);
}

static void set_state(epiworld::Virus<int> virus, size_t init, size_t end,
					  size_t removed) {
	virus.set_state(init, end, removed);
}

static void set_distribution(epiworld::Virus<int> virus,
							 VirusToAgentFun<int> fun) {
	virus.set_distribution(fun);
}

static void print_virus(epiworld::Virus<int> virus) { virus.print(); }

void epiworldpy::export_virus(
	pybind11::class_<epiworld::Virus<int>,
					 std::shared_ptr<epiworld::Virus<int>>> &c) {
	c.def(py::init(&new_virus), "Create a new virus (evil...)", py::arg("name"),
		  py::arg("prevalence"), py::arg("as_proportion"),
		  py::arg("prob_infecting"), py::arg("prob_recovery"),
		  py::arg("prob_death"), py::arg("post_immunity"),
		  py::arg("incubation"))
		.def("get_name", &get_name, "Get the tool name.")
		.def("set_name", &set_name, "Set the tool name.", py::arg("name"))
		.def("set_state", &set_state, "Set some state.", py::arg("init"),
			 py::arg("end"), py::arg("removed"))
		.def("set_prob_infecting", &set_prob_infecting,
			 "Set the probability of infection.", py::arg("prob_infecting"))
		.def("set_prob_recovery", &set_prob_recovery,
			 "Set the probability for recovery.", py::arg("prob_recovery"))
		.def("set_prob_death", &set_prob_death,
			 "Set the probability for mortality.", py::arg("prob_death"))
		.def("set_incubation", &set_incubation, "Set the incubation period.",
			 py::arg("incubation"))
		.def("set_distribution", &set_distribution,
			 "Set the distribution function.", py::arg("fun"))
		.def("print", &print_virus, "Print information about this virus.");
}

static VirusToAgentFun<int> new_distribution_fun(
	const std::function<void(Virus<int> &, Model<int> *)> &fun) {
	return VirusToAgentFun<int>(fun);
}

static VirusToAgentFun<int> new_random_distribution(double prevalence,
													bool as_proportion) {
	return distribute_virus_randomly(prevalence);
}

static VirusToAgentFun<int> new_distribute_to_set(std::vector<size_t> ids) {
	/* epiworldr does a negative check, do we need to do this? */
	return distribute_virus_to_set(ids);
}

void epiworldpy::export_virus_to_agent_fun(
	pybind11::class_<epiworld::VirusToAgentFun<int>,
					 std::shared_ptr<epiworld::VirusToAgentFun<int>>> &c) {
	c.def_static("new_distribution_fun", &new_distribution_fun,
				 "Create a new distribution function based off a lambda.",
				 py::arg("fun"))
		.def_static("new_random_distribution", &new_random_distribution,
					"Randomly infect agents in the model.")
		.def_static("new_distribute_to_set", &new_distribute_to_set,
					"Infect a set of agents in the model.", py::arg("ids"));
}
