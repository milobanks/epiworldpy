#ifndef EPIWORLDPY_MODEL_HPP
#define EPIWORLDPY_MODEL_HPP

#include "../interface.hpp"
#include <pybind11/pybind11.h>

namespace epiworldpy {
void export_model(pybind11::class_<epiworld::Model<int>,
								   std::shared_ptr<epiworld::Model<int>>> &c);
}

#endif /* EPIWORLDPY_MODEL_HPP */
