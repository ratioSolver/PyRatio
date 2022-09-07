#pragma once
#include "solver_listener.h"
#include <pybind11/pybind11.h>

namespace ratio::python
{
  class py_solver_listener : public ratio::solver::solver_listener
  {
  public:
    py_solver_listener(ratio::solver::solver &slv, pybind11::object &py_slv);
    ~py_solver_listener();

  private:
    pybind11::object py_slv;
  };
} // namespace ratio::python
