#include "model.hpp"

using namespace epiworldpy;
using namespace epiworld;

void epiworldpy::export_model(
	pybind11::class_<epiworld::Model<int>,
					 std::shared_ptr<epiworld::Model<int>>> &c) {
	c.def("get_name", &Model<int>::get_name,
		  "Get the name of the type of model.");
}
