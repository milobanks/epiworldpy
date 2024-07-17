#ifndef EPIWORLDPY_SIR_HPP
#define EPIWORLDPY_SIR_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sir(
	pybind11::class_<epiworld::epimodels::ModelSIR<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIR<int>>> &c);
void export_sir_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SIR_HPP */
