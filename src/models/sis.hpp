#ifndef EPIWORLDPY_SIS_HPP
#define EPIWORLDPY_SIS_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_sis(
	pybind11::class_<epiworld::epimodels::ModelSIS<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSIS<int>>> &c);
void export_sis_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SIS_HPP */
