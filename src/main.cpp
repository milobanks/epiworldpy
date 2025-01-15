#include <sys/stat.h>

#include <pybind11/iostream.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // silently fails when removed.

#include "database.hpp"
#include "misc.hpp"
#include "model-bones.hpp"
#include "model.hpp"
#include "entity.hpp"
#include "saver.hpp"
#include "tool.hpp"
#include "virus.hpp"

namespace py = pybind11;
using namespace epiworld;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

PYBIND11_MODULE(_core, m) {
  m.doc() = R"pbdoc(
        Epiworld Python Wrapper
        -----------------------

        .. currentmodule:: epiworldpy

        .. autosummary::
		   :recursive:
           :toctree: generated

		   ModelDiffNet
		   ModelSEIR
		   ModelSEIRCONN
    	   ModelSEIRD
		   ModelSIR
		   ModelSIRCONN
		   ModelSIRD
		   ModelSIRDCONN
		   ModelSIS
    	  ModelSISD
		  ModelSURV
		  Saver
    )pbdoc";

  auto update_fun =
      py::class_<UpdateFun<int>>(m, "UpdateFun", "Model update functions.");
  auto model = py::class_<Model<int>>(
      m, "Model", "A generic model of some kind; a parent class.");
  auto database = py::class_<DataBase<int>, std::shared_ptr<DataBase<int>>>(
      m, "DataBase", "A container for data generated by a model run.");
  auto entity = py::class_<Entity<int>, std::shared_ptr<Entity<int>>>(
      m, "Entity", "Unknown.");
  auto saver =
      py::class_<epiworldpy::Saver, std::shared_ptr<epiworldpy::Saver>>(
          m, "Saver", "Saves the result of multiple runs.");
  auto tool =
      py::class_<Tool<int>>(m, "Tool", "A tool for modifying virus spread.");
  auto virus = py::class_<Virus<int>>(m, "Virus", "A virus.");

  epiworldpy::export_update_fun(update_fun);
  epiworldpy::export_model(model);
  epiworldpy::export_all_models(m);
  epiworldpy::export_database(database);
  epiworldpy::export_entity(entity);
  epiworldpy::export_saver(saver);
  epiworldpy::export_tool(tool);
  epiworldpy::export_virus(virus);

#ifdef VERSION_INFO
  /* Give the real version. */
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  /* Also give the real version, but prefix with 'dev'. */
  m.attr("__version__") = "dev-" MACRO_STRINGIFY(VERSION_INFO);
#endif
}
