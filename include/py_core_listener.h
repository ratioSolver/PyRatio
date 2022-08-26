#pragma once
#include "core_listener.h"
#include <Python.h>

namespace ratio::python
{
  class py_core_listener : public ratio::core::core_listener
  {
  public:
    py_core_listener(ratio::core::core &cr, PyObject *py_slv);
    ~py_core_listener();

  private:
    PyObject *py_slv;
  };
} // namespace ratio::python
