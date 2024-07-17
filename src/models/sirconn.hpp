#ifndef EPIWORLDPY_SIRCONN_HPP
#define EPIWORLDPY_SIRCONN_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sirconn(
	pybind11::class_<epiworld::epimodels::ModelSIRCONN<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIRCONN<int>>>
		&c);
void export_sirconn_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SIRCONN_HPP */
