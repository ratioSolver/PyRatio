#pragma once
#include "solver_listener.h"
#include <Python.h>

namespace ratio::python
{
  class py_solver_listener : public ratio::solver::solver_listener
  {
  public:
    py_solver_listener(ratio::solver::solver &slv, PyObject *py_slv);
    ~py_solver_listener();

  private:
    PyObject *py_slv;
  };
} // namespace ratio::python
