#include "py_solver_listener.h"

namespace ratio::python
{
    py_solver_listener::py_solver_listener(ratio::solver &slv, pybind11::object &py_slv) : solver_listener(slv), py_slv(py_slv) {}
    py_solver_listener::~py_solver_listener() {}

    void py_solver_listener::flaw_created(const ratio::flaw &f)
    {
        // the flaw's cause..
        pybind11::list causes;
        for (const auto &c : f.get_causes())
            causes.append(get_id(c.get()));

        // the flaw's position..
        const auto [lb, ub] = slv.get_idl_theory().bounds(f.get_position());

        py_slv.attr("fire_flaw_created")(get_id(f), causes, f.get_data().to_string(), slv.get_sat_core().value(f.get_phi()), pybind11::module_::import("oRatio.solver_listener").attr("Bound")(lb, ub));
    }
    void py_solver_listener::flaw_state_changed(const ratio::flaw &f) { py_slv.attr("fire_flaw_state_changed")(get_id(f), slv.get_sat_core().value(f.get_phi())); }
    void py_solver_listener::flaw_cost_changed(const ratio::flaw &f)
    {
        // the flaw's current estimated cost..
        const auto est_cost = f.get_estimated_cost();

        py_slv.attr("fire_flaw_cost_changed")(get_id(f), pybind11::module_::import("oRatio.rational").attr("Rational")(est_cost.numerator(), est_cost.denominator()));
    }
    void py_solver_listener::flaw_position_changed(const ratio::flaw &f)
    {
        // the flaw's position..
        const auto [lb, ub] = slv.get_idl_theory().bounds(f.get_position());

        py_slv.attr("fire_flaw_position_changed")(get_id(f), pybind11::module_::import("oRatio.solver_listener").attr("Bound")(lb, ub));
    }
    void py_solver_listener::current_flaw(const ratio::flaw &f) { py_slv.attr("fire_current_flaw")(get_id(f)); }

    void py_solver_listener::resolver_created(const ratio::resolver &r)
    {
        // the resolver's intrinsic cost..
        const auto intr_cost = r.get_intrinsic_cost();

        py_slv.attr("fire_resolver_created")(get_id(r), get_id(r.get_flaw()), r.get_data().to_string(), pybind11::module_::import("oRatio.rational").attr("Rational")(intr_cost.numerator(), intr_cost.denominator()), slv.get_sat_core().value(r.get_rho()));
    }
    void py_solver_listener::resolver_state_changed(const ratio::resolver &r) { py_slv.attr("fire_resolver_state_changed")(get_id(r), slv.get_sat_core().value(r.get_rho())); }
    void py_solver_listener::current_resolver(const ratio::resolver &r) { py_slv.attr("fire_current_resolver")(get_id(r)); }

    void py_solver_listener::causal_link_added(const ratio::flaw &f, const ratio::resolver &r) { py_slv.attr("fire_causal_link_added")(get_id(f), get_id(r)); }
} // namespace ratio::python
