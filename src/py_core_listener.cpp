#include "py_core_listener.h"
#include "predicate.h"
#include <queue>

namespace ratio::python
{
    py_core_listener::py_core_listener(ratio::core::core &cr, pybind11::object &py_slv) : core_listener(cr), py_slv(py_slv) {}
    py_core_listener::~py_core_listener() {}

    void py_core_listener::log(const std::string &msg) { py_slv.attr("fire_log")(msg); }
    void py_core_listener::read(const std::string &script) { py_slv.attr("fire_read")(script); }
    void py_core_listener::read(const std::vector<std::string> &files) { py_slv.attr("fire_read")(files); }

    void py_core_listener::state_changed()
    {
        std::unordered_set<ratio::core::type *> new_types;
        std::unordered_set<ratio::core::predicate *> new_predicates;
        std::queue<ratio::core::type *> q;
        for (const auto &[tp_name, tp] : cr.get_types())
            q.push(tp.get());
        while (!q.empty())
        {
            ratio::core::type &t = *q.front();
            q.pop();

            if (const auto &t_it = all_types.find(&t); t_it == all_types.cend())
            { // we have a new type..
                if (!t.is_primitive())
                    new_types.insert(&t);
                new_type(t);
            }

            for (const auto &[tp_name, tp] : t.get_types())
                q.push(tp.get());
        }

        py_slv.attr("fire_state_changed")();
    }

    void py_core_listener::started_solving() { py_slv.attr("fire_started_solving")(); }
    void py_core_listener::solution_found() { py_slv.attr("fire_solution_found")(); }
    void py_core_listener::inconsistent_problem() { py_slv.attr("fire_inconsistent_problem")(); }

    void py_core_listener::new_type(const ratio::core::type &t)
    {
        auto tp = pybind11::module_::import("oRatio.type").attr("Type")(t.get_name(), t.is_primitive());
        all_types.emplace(&t, tp);
    }
} // namespace ratio::python
