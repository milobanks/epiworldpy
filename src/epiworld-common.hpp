#ifndef DEFM_COMMON_H
#define DEFM_COMMON_H

using namespace pybind11::literals;

inline void pyprinter(const char * fmt, ...) 
{

  // Creating a buffer
  char buffer[1024];

  va_list args;
  va_start(args, fmt);
  vsprintf(&buffer[0], fmt, args);
  va_end(args);

  // Passing to pyprint
  pybind11::print(std::string(buffer), pybind11::arg("end") = "");

}

#define printf_epiworld pyprinter

#include "epiworld.hpp"
// #include "models/defm.hpp"

// inline void check_covar(
//   int & idx_,
//   std::string & idx,
//   std::shared_ptr< defm::DEFM > & ptr
// ) {

//   // Retrieving the matching covariate
//   if (idx != "")
//   {

//     // Getting the covariate names
//     auto cnames = ptr->get_X_names();

//     // Can we find it?
//     for (size_t i = 0u; i < cnames.size(); ++i) {
//       if (cnames[i] == idx)
//       {
//         idx_ = i;
//         break;
//       }
//     }

//     if (idx_ < 0)
//       throw std::range_error(
//         "The variable " + idx + "does not exists."
//         );

//   }

// }

// #define DEFM_DEFINE_ACCESS(object) \
//     std::function<size_t(size_t,size_t,size_t,size_t)> element_access; \
//     if ((object)->get_column_major()) \
//     { \
//         element_access = [](size_t i, size_t j, size_t nrow, size_t) -> size_t { \
//             return i + j * nrow; \
//         }; \
//     } else { \
//         element_access = [](size_t i, size_t j, size_t, size_t ncol) -> size_t { \
//             return j + i * ncol; \
//         }; \
//     } 


// /**
//  * @brief Create a numpy array from a pointer
//  * @param res The numpy array
//  * @param ptr The pointer
//  * @param nrows The number of rows
//  * @param ncols The number of columns
//  * @param type_ The type of the array
// */
// #define DEFM_WRAP_NUMPY(var_res, var_ptr, nrows, ncols, type_) \
//   py::array_t< type_ > var_res ({nrows, ncols}); \
//     auto res_buff = var_res .request(); \
//     type_ * var_ptr = static_cast< type_ * >(res_buff.ptr);


#endif

