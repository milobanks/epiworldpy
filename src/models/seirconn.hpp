#ifndef EPIWORLDPY_SEIRCONN_HPP
#define EPIWORLDPY_SEIRCONN_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_seirconn(
	pybind11::class_<epiworld::epimodels::ModelSEIRCONN<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSEIRCONN<int>>>
		&c);
void export_sierconn_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SEIRCONN_HPP */
