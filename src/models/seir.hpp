#ifndef EPIWORLDPY_SEIR_HPP
#define EPIWORLDPY_SEIR_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_seir(
	pybind11::class_<epiworld::epimodels::ModelSEIR<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSEIR<int>>> &c);
void export_seir_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SEIR_HPP */
