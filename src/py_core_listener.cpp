#include "py_core_listener.h"

namespace ratio::python
{
    py_core_listener::py_core_listener(ratio::core::core &cr, PyObject *py_slv) : core_listener(cr), py_slv(py_slv) {}
    py_core_listener::~py_core_listener() {}

    void py_core_listener::log(const std::string &msg) {}
    void py_core_listener::read(const std::string &script) {}
    void py_core_listener::read(const std::vector<std::string> &files) {}

    void py_core_listener::state_changed() {}

    void py_core_listener::started_solving() {}
    void py_core_listener::solution_found() {}
    void py_core_listener::inconsistent_problem() {}
} // namespace ratio::python
