#include "py_solver_listener.h"

namespace ratio::python
{
    py_solver_listener::py_solver_listener(ratio::solver::solver &slv, PyObject *py_slv) : solver_listener(slv), py_slv(py_slv) {}
    py_solver_listener::~py_solver_listener() {}
} // namespace ratio::python
