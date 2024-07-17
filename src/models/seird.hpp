#ifndef EPIWORLDPY_SEIRD_HPP
#define EPIWORLDPY_SEIRD_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_seird(
	pybind11::class_<epiworld::epimodels::ModelSEIRD<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSEIRD<int>>> &c);
void export_seird_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SEIRD_HPP */
