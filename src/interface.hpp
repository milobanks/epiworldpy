#ifndef EPIWORLDPY_INTERFACE_HPP
#define EPIWORLDPY_INTERFACE_HPP

#include <pybind11/pybind11.h>

using namespace pybind11::literals;

inline void pyprinter(const char *fmt, ...) {
	char buffer[1024];

	va_list args;
	va_start(args, fmt);
	vsnprintf(&buffer[0], sizeof(buffer), fmt, args);
	va_end(args);

	pybind11::print(std::string(buffer), pybind11::arg("end") = "");
}

#define printf_epiworld pyprinter

/* Keep me at the bottom! */
#include "epiworld.hpp"

#endif /* EPIWORLPY_INTERFACE_HPP */
