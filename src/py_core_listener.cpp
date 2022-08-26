#include "py_core_listener.h"

namespace ratio::python
{
    py_core_listener::py_core_listener(ratio::core::core &cr) : core_listener(cr) {}
    py_core_listener::~py_core_listener() {}
} // namespace ratio::python
