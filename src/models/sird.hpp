#ifndef EPIWORLDPY_SIRD_HPP
#define EPIWORLDPY_SIRD_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sird(
	pybind11::class_<epiworld::epimodels::ModelSIRD<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIRD<int>>> &c);
void export_sird_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SIRD_HPP */
