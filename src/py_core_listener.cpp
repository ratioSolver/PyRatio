#include "py_core_listener.h"

namespace ratio::python
{
    py_core_listener::py_core_listener(ratio::core::core &cr, pybind11::object &py_slv) : core_listener(cr), py_slv(py_slv) {}
    py_core_listener::~py_core_listener() {}

    void py_core_listener::log(const std::string &msg) { py_slv.attr("fire_log")(msg); }
    void py_core_listener::read(const std::string &script) { py_slv.attr("fire_read")(script); }
    void py_core_listener::read(const std::vector<std::string> &files) { py_slv.attr("fire_read")(files); }

    void py_core_listener::state_changed() { py_slv.attr("fire_state_changed")(); }

    void py_core_listener::started_solving() { py_slv.attr("fire_started_solving")(); }
    void py_core_listener::solution_found() { py_slv.attr("fire_solution_found")(); }
    void py_core_listener::inconsistent_problem() { py_slv.attr("fire_inconsistent_problem")(); }
} // namespace ratio::python
