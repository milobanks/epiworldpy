#include <cstring>
#include <stdexcept>
#include <sys/stat.h>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h> // for py::array_t
#include "epiworld-common.hpp"
#include "model-bones.hpp"

using namespace epiworld;
using namespace pybind11::literals;

namespace py = pybind11;

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

std::shared_ptr< epimodels::ModelSEIRCONN<int> > ModelSEIR(
    std::string name,
    int n,
    double prevalence,
    double contact_rate,
    double transmission_rate,
    double incubation_days,
    double recovery_rate
    ) {

    std::shared_ptr< epimodels::ModelSEIRCONN<int> > object(
        new epimodels::ModelSEIRCONN<int>(
            name,
            n,
            prevalence,
            contact_rate,
            transmission_rate,
            incubation_days,
            recovery_rate
        )
        );

    return object;
}

PYBIND11_MODULE(_core, m) {
    m.doc() = R"pbdoc(
        epiworldpy wrapper
        -----------------------

        .. currentmodule:: epiworldpy

        .. autosummary::
           :toctree: _generate

           ModelSEIR
    )pbdoc";

    py::class_<Model<int>, std::shared_ptr<Model<int>>>(m, "Model")
        .def("get_name", &Model<int>::get_name);

    py::class_<epiworld::Saver>(m, "Saver")
        .def(py::init([](py::args args, const py::kwargs& kwargs) {
            /* TODO: Verify that this has the same effect as `make_saver` in:
             *  https://github.com/UofUEpiBio/epiworldR/blob/main/R/make_saver.R
             */

            bool file_output = true;
            struct stat sinfo;
            std::string fn = parse_kwarg_string(kwargs, "fn", "");
            std::string id = temp_id(5);
            std::vector<std::string> whats;
            std::vector<std::string> valid_whats = {
                "total_hist",
                "virus_info",
                "virus_hist",
                "tool_info",
                "tool_hist",
                "transmission",
                "transition",
                "reproductive",
                "generation"
            };

            /* Make sure valid arguments are passed into this constructor, and marshall
             * things out all the same. */
            for (auto arg : args) {
                std::string whatum = arg.cast<std::string>();

                if (std::find(valid_whats.begin(), valid_whats.end(), whatum) == valid_whats.end()) {
                    throw std::invalid_argument("What '" + whatum + "' is not supported.");
                }

                whats.push_back(whatum);
            }

            /* Handle the filename. If only we have C++17's std::filesystem... */
            if (fn.empty()) {
                int error = 0;
                std::string norm = temp_directory_path() + EPIWORLD_OS_PATHSEP + "epiworldpy-" + id;

#ifdef EPIWORLD_PLATFORM_WINDOWS
                error = _mkdir(norm.c_str());
#else 
                error = mkdir(norm.c_str(), 0733);
#endif

                if (error != 0) {
                    throw std::runtime_error(strerror(error));
                }

                fn = norm + EPIWORLD_OS_PATHSEP + "%05lu-episimulation.csv";
                file_output = false;
#if EPIWORLD_PLATFORM_WINDOWS
            } else if (_stat(fn.c_str(), &sinfo) != 0) {
#else
            } else if (stat(fn.c_str(), &sinfo) != 0) {
#endif
                throw std::runtime_error("The directory \"" + fn + "\" does not exist.");
            }

            return epiworld::Saver(whats, fn, id, file_output);
        }))
        .def("run_multiple", [](
            Saver &self,
            Model<int> &model,
            int ndays,
            int nsims,
            const py::kwargs& kwargs) {
            int seed = parse_kwarg_int(kwargs, "seed", std::time(0));
            int nthreads = parse_kwarg_int(kwargs, "nthreads", 1);
            bool reset = parse_kwarg_bool(kwargs, "reset", true);
            bool verbose = parse_kwarg_bool(kwargs, "verbose", true);

            /* Do we have previously saved files? */
            self.unlink_siblings();

            /* Dispatch! */
            model.run_multiple(ndays, nsims, seed, *self, reset, verbose, nthreads);

            /* EpiworldR does this so we do too. */
            return model;
        });

    py::class_<DataBase<int>, std::shared_ptr<DataBase<int>>>(m, "DataBase")
        .def("get_hist_total", [](DataBase<int> &self) {
            std::vector<std::string> states;
            std::vector<int> dates;
            std::vector<int> counts;

            self.get_hist_total(&dates, &states, &counts);

            py::array py_states = py::array(py::cast(states));
            py::array_t<int> py_dates(dates.size(), dates.data());
            py::array_t<int> py_counts(counts.size(), counts.data());

            py::dict ret(
                "dates"_a=py_dates,
                "states"_a=py_states,
                "counts"_a=py_counts);

            return ret;
        });

    // Only this is necessary to expose the class
    py::class_<epimodels::ModelSEIRCONN<int>, std::shared_ptr<epimodels::ModelSEIRCONN<int>>, Model<int>>(m, "ModelSEIRCONN")
        // .def(py::init<>())
        .def("print", &epimodels::ModelSEIRCONN<int>::print,
            // py::call_guard<py::scoped_ostream_redirect, py::scoped_estream_redirect>(),
            R"pbdoc(
            Print the object

            Some other explanation about the print function.)
        )pbdoc",
            py::arg("summary") = true
            )
        .def("run", &epimodels::ModelSEIRCONN<int>::run, R"pbdoc(
            Run the model
        )pbdoc",
            py::arg("ndays"),
            py::arg("seed") = 1u
            )
        .def("get_db", [](epimodels::ModelSEIRCONN<int> &self) {
            return std::shared_ptr<DataBase<int>>(&self.get_db(), [](DataBase<int>*){ /* do nothing, no delete */ });
        }, py::return_value_policy::reference);

    // Example with shared_ptr
    m.def("ModelSEIR", &ModelSEIR, R"pbdoc(
        Create an SEIR model
    )pbdoc",
        py::arg("name"),
        py::arg("n"),
        py::arg("prevalence"),
        py::arg("contact_rate"),
        py::arg("transmission_rate"),
        py::arg("incubation_days"),
        py::arg("recovery_rate")
        );

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
