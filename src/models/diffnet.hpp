#ifndef EPIWORLDPY_DIFFNET_HPP
#define EPIWORLDPY_DIFFNET_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_diffnet(
	pybind11::class_<epiworld::epimodels::ModelDiffNet<int>,
					 std::shared_ptr<epiworld::epimodels::ModelDiffNet<int>>>
		&c);
void export_diffnet_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_DIFFNET_HPP */
