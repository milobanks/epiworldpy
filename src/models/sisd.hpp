#ifndef EPIWORLDPY_SISD_HPP
#define EPIWORLDPY_SISD_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sisd(
	pybind11::class_<epiworld::epimodels::ModelSISD<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSISD<int>>> &c);
void export_sisd_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SISD_HPP */
