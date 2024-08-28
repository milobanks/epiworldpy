#include "model.hpp"

#define MODEL_OF(m, model, description)                                        \
	py::class_<epimodels::Model##model<int>, Model<int>>(m, "Model" #model,    \
														 description)

using namespace epiworldpy;
using namespace epiworld;
namespace py = pybind11;

static std::shared_ptr<DataBase<int>> get_db(Model<int> &self) {
	return std::shared_ptr<DataBase<int>>(
		&self.get_db(), [](DataBase<int> *) { /* do nothing, no delete */ });
}

static size_t size(Model<int> &self) { return self.size(); }

static void add_virus(Model<int> &self, Virus<int> &virus) {
	// TODO
	virus.set_state(1, 2);

	self.add_virus(virus);
}

static void add_tool(Model<int> &self, Tool<int> &tool) { self.add_tool(tool); }

static void add_state_1(Model<int> &self, std::string name) {
	self.add_state(name);
}

static void add_state_2(Model<int> &self, std::string name,
						UpdateFun<int> fun) {
	self.add_state(name, fun);
}

static void add_param(Model<int> &self, double val, std::string name) {
	self.add_param(val, name);
}

static size_t get_n_replicates(Model<int> &self) {
	return self.get_n_replicates();
}

static size_t get_n_viruses(Model<int> &self) { return self.get_n_viruses(); }

static size_t get_n_tools(Model<int> &self) { return self.get_n_tools(); }

static size_t get_ndays(Model<int> &self) { return self.get_ndays(); }

static std::vector<std::string> get_states(Model<int> &self) {
	return self.get_states();
}

static float get_param(Model<int> &self, std::string name) {
	return self.get_param(name);
}

static void set_param(Model<int> &self, std::string name, float val) {
	self.set_param(name, val);
}

static std::string get_name(Model<int> &self) { return self.get_name(); }

static void set_name(Model<int> &self, std::string name) {
	self.set_name(name);
}

static void queueing_on(Model<int> &self) { self.queuing_on(); }
static void queueing_off(Model<int> &self) { self.queuing_off(); }
static void verbose_on(Model<int> &self) { self.verbose_on(); }
static void verbose_off(Model<int> &self) { self.verbose_off(); }

void epiworldpy::export_model(py::class_<epiworld::Model<int>> &c) {
	c.def(py::init<>())
		.def("get_name", &Model<int>::get_name,
			 "Get the name of the type of model.")
		.def("agents_smallworld", &Model<int>::agents_smallworld,
			 "Populatates the model's agents from a small world.", py::arg("n"),
			 py::arg("k"), py::arg("d"), py::arg("p"))
		.def("agents_from_edgelist", &Model<int>::agents_from_edgelist,
			 "Populatates the model's agents from an edge list.",
			 py::arg("source"), py::arg("target"), py::arg("size"),
			 py::arg("directed"))
		.def("print", &Model<int>::print, "Print a summary of the model run.",
			 py::arg("summary") = true)
		.def("run", &Model<int>::run,
			 "Run the model according to the previously specific parameters.",
			 py::arg("ndays"), py::arg("seed") = 1u)
		.def("get_db", &get_db,
			 "Get the data from the model run, which may then be queried with "
			 "associated methods.",
			 py::return_value_policy::reference)
		.def("size", &size, "Get the number of agents in the model.")
		.def("add_virus", &add_virus, "Adds a virus to this model.",
			 py::arg("virus"))
		.def("add_tool", &add_tool, "Adds a tool to this model.",
			 py::arg("tool"))
		.def("add_state", &add_state_1, "Adds a state to this model.",
			 py::arg("name"))
		.def("add_state", &add_state_2, "Adds a state to this model.",
			 py::arg("name"), py::arg("fun"))
		.def("add_param", &add_param, "Adds a parameter to this model.",
			 py::arg("value"), py::arg("name"))
		.def("get_n_replicates", &get_n_replicates,
			 "Get the number of replicates.")
		.def("get_n_viruses", &get_n_viruses, "Get the number of viruses.")
		.def("get_n_tools", &get_n_tools, "Get the number of tools.")
		.def("get_ndays", &get_ndays, "Get the number of days in the model.")
		.def("get_states", &get_states,
			 "Get the states associated with this model.")
		.def("get_param", &get_param, "Get a model parameter from its name.",
			 py::arg("name"))
		.def("set_param", &set_param, "Set a model parameter from its name.",
			 py::arg("name"), py::arg("val"))
		.def("get_name", &get_name, "Get the name of the model.")
		.def("set_name", &set_name, "Set the name of the model.",
			 py::arg("name"))
		.def("queuing_on", &verbose_on, "Enabling queuing,")
		.def("queuing_off", &verbose_off, "Disabling queuing.")
		.def("verbose_on", &verbose_on, "Turn up the verbosity.")
		.def("verbose_off", &verbose_off, "Turn off the verbosity.");
}

void epiworldpy::export_all_models(pybind11::module &m) {
	/* Export models.
	 *
	 * TODO: Make Python model proxys inhereit from an interface and do this in
	 * a loop.
	 */
	auto diffnet = MODEL_OF(m, DiffNet, "A network diffusion model.");
	auto seir = MODEL_OF(m, SEIR,
						 "A model with four compartments: susceptible, "
						 "exposed, infectious, and recovered.");
	auto seirconn = MODEL_OF(
		m, SEIRCONN,
		"A model with four compartments: susceptible, exposed, infectious, and "
		"recovered.");
	auto seird = MODEL_OF(m, SEIRD,
						  "A model with five compartments: susceptible, "
						  "exposed, infectious, recovered, and dead.");
	auto sir = MODEL_OF(m, SIR,
						"A model with three compartments: "
						"susceptible, infectious, and recovered.");
	auto sirconn = MODEL_OF(m, SIRCONN,
							"A model with three compartments: susceptible, "
							"infectious, and recovered.");
	auto sird = MODEL_OF(m, SIRD,
						 "A model with four compartments: susceptible, "
						 "infectious, recovered, and dead.");
	auto sirdconn = MODEL_OF(m, SIRDCONN,
							 "A model with four compartments: susceptible, "
							 "infectious, recovered, and dead.");
	auto sis = MODEL_OF(
		m, SIS, "A model wth two compartments: susceptible and infectious.");
	auto sisd = MODEL_OF(
		m, SISD,
		"A model wth three compartments: susceptible, infectious, and death.");
	auto surv = MODEL_OF(
		m, SURV,
		"A model where agents may be isolated, even when asymptomatic.");

	epiworldpy::export_diffnet(diffnet);
	epiworldpy::export_seir(seir);
	epiworldpy::export_seirconn(seirconn);
	epiworldpy::export_seird(seird);
	epiworldpy::export_sir(sir);
	epiworldpy::export_sirconn(sirconn);
	epiworldpy::export_sird(sird);
	epiworldpy::export_sirdconn(sirdconn);
	epiworldpy::export_sis(sis);
	epiworldpy::export_sisd(sisd);
	epiworldpy::export_surv(surv);
}

void epiworldpy::export_diffnet(MODEL_CHILD_TYPE(DiffNet) & c) {
	c.def(py::init<std::string, double, double, bool, double *, int,
				   std::vector<size_t>, std::vector<double>>(),
		  "Create a new DiffNet model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("prob_adopt"), py::arg("normalize_exposure"), py::arg("data"),
		  py::arg("data_ncols"), py::arg("data_cols"), py::arg("params"));
}

void epiworldpy::export_seir(MODEL_CHILD_TYPE(SEIR) & c) {
	c.def(py::init<std::string, double, double, double, double>(),
		  "Create a new SEIR model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("incubation_days"),
		  py::arg("recovery_rate"));
}

void epiworldpy::export_seirconn(MODEL_CHILD_TYPE(SEIRCONN) & c) {
	c.def(py::init<std::string, int, double, double, double, double, double>(),
		  "Create a new SEIRCONN model.", py::arg("name"), py::arg("n"),
		  py::arg("prevalence"), py::arg("contact_rate"),
		  py::arg("transmission_rate"), py::arg("incubation_days"),
		  py::arg("recovery_rate"));
}

void epiworldpy::export_seird(MODEL_CHILD_TYPE(SEIRD) & c) {
	c.def(py::init<std::string, double, double, double, double, double>(),
		  "Create a new SEIRD model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("incubation_days"),
		  py::arg("recovery_rate"), py::arg("death_rate"));
}

void epiworldpy::export_sir(MODEL_CHILD_TYPE(SIR) & c) {
	c.def(py::init<std::string, double, double, double>(),
		  "Create a new SIR model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"));
}

void epiworldpy::export_sirconn(MODEL_CHILD_TYPE(SIRCONN) & c) {
	c.def(py::init<std::string, int, double, double, double, double>(),
		  "Create a new SIRCONN model.", py::arg("name"), py::arg("n"),
		  py::arg("prevalence"), py::arg("contact_rate"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"));
}

void epiworldpy::export_sird(MODEL_CHILD_TYPE(SIRD) & c) {
	c.def(py::init<std::string, double, double, double, double>(),
		  "Create a new SIRD model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"),
		  py::arg("death_rate"));
}

void epiworldpy::export_sirdconn(MODEL_CHILD_TYPE(SIRDCONN) & c) {
	c.def(py::init<std::string, int, double, double, double, double, double>(),
		  "Create a new SIRDCONN model.", py::arg("name"), py::arg("n"),
		  py::arg("prevalence"), py::arg("contact_rate"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"),
		  py::arg("death_rate"));
}

void epiworldpy::export_sis(MODEL_CHILD_TYPE(SIS) & c) {
	c.def(py::init<std::string, double, double, double>(),
		  "Create a new SIS model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"));
}

void epiworldpy::export_sisd(MODEL_CHILD_TYPE(SISD) & c) {
	c.def(py::init<std::string, double, double, double, double>(),
		  "Create a new SISD model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("transmission_rate"), py::arg("recovery_rate"),
		  py::arg("death_rate"));
}

void epiworldpy::export_surv(MODEL_CHILD_TYPE(SURV) & c) {
	/*
	 * TODO: I noticed that epiworld::epimodels::ModelSURV is overloaded to
	 * accept a variable number of arguments, should this be expressed on the
	 * Python side? It isn't in R, as far as I can tell, and I don't know how
	 * concerning symmetry is.
	 */
	c.def(py::init<std::string, double, double, double, double, double, double,
				   double, double, double, double, double, double>(),
		  "Create a new SURV model.", py::arg("name"), py::arg("prevalence"),
		  py::arg("efficacy_vax"), py::arg("latent_period"),
		  py::arg("prob_symptoms"), py::arg("prop_vaccinated"),
		  py::arg("prop_vax_redux_transm"), py::arg("infect_period"),
		  py::arg("prop_vax_redux_infect"), py::arg("surveillance_prob"),
		  py::arg("transmission_rate"), py::arg("prob_death"),
		  py::arg("prob_noreinfect"));
}

static UpdateFun<int>
new_update_fun(const std::function<void(Agent<int> *, Model<int> *)> &fun) {
	return UpdateFun<int>(fun);
}

static UpdateFun<int> default_update_susceptible_cpp() {
	return UpdateFun<int>(default_update_susceptible<int>);
}

static UpdateFun<int> default_update_exposed_cpp() {
	return UpdateFun<int>(default_update_exposed<int>);
}

void epiworldpy::export_update_fun(
	pybind11::class_<epiworld::UpdateFun<int>,
					 std::shared_ptr<epiworld::UpdateFun<int>>> &c) {
	c.def_static("new_update_fun", &new_update_fun,
				 "Create a new compartment update function based off a lambda.",
				 py::arg("fun"))
		.def_static("default_update_susceptible",
					&default_update_susceptible_cpp,
					"Update the susceptible compartment automatically.")
		.def_static("default_update_exposed", &default_update_exposed_cpp,
					"Update the exposed compartment automatically.");
}
