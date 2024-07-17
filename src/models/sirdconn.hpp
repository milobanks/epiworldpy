#ifndef EPIWORLDPY_SIRDCONN_HPP
#define EPIWORLDPY_SIRDCONN_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sirdconn(
	pybind11::class_<epiworld::epimodels::ModelSIRDCONN<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIRDCONN<int>>>
		&c);
void export_sirdconn_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SIRDCONN_HPP */
