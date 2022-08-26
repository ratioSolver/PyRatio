#include "py_core_listener.h"

namespace ratio::python
{
    py_core_listener::py_core_listener(ratio::core::core &cr, PyObject *py_slv) : core_listener(cr), py_slv(py_slv) {}
    py_core_listener::~py_core_listener() {}
} // namespace ratio::python
