#ifndef EPIWORLDPY_SURV_HPP
#define EPIWORLDPY_SURV_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_surv(
	pybind11::class_<epiworld::epimodels::ModelSURV<int>,
					 std::shared_ptr<epiworld::epimodels::ModelSURV<int>>> &c);
void export_surv_constructor(pybind11::module &module);
} // namespace epiworldpy

#endif /* EPIWORLDPY_SURV_HPP */
