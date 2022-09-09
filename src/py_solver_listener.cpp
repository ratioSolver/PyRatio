#include "py_solver_listener.h"

namespace ratio::python
{
    py_solver_listener::py_solver_listener(ratio::solver::solver &slv, pybind11::object &py_slv) : solver_listener(slv), py_slv(py_slv) {}
    py_solver_listener::~py_solver_listener() {}

    void py_solver_listener::flaw_created(const ratio::solver::flaw &f)
    {
        // the flaw's cause..
        pybind11::list causes;
        for (const auto &c : f.get_causes())
            causes.append(get_id(*c));

        // the flaw's position..
        const auto [lb, ub] = slv.get_idl_theory().bounds(f.get_position());

        py_slv.attr("fire_flaw_created")(get_id(f), causes, f.get_data().dump(), slv.get_sat_core()->value(f.get_phi()), lb, ub);
    }
    void py_solver_listener::flaw_state_changed(const ratio::solver::flaw &f) { py_slv.attr("fire_flaw_state_changed")(get_id(f), slv.get_sat_core()->value(f.get_phi())); }
    void py_solver_listener::flaw_cost_changed(const ratio::solver::flaw &f)
    {
        // the flaw's current estimated cost..
        const auto est_cost = f.get_estimated_cost();

        py_slv.attr("fire_flaw_cost_changed")(get_id(f), slv.get_sat_core()->value(f.get_phi()), est_cost.numerator(), est_cost.denominator());
    }
    void py_solver_listener::flaw_position_changed(const ratio::solver::flaw &f)
    {
        // the flaw's position..
        const auto [lb, ub] = slv.get_idl_theory().bounds(f.get_position());

        py_slv.attr("fire_flaw_position_changed")(get_id(f), lb, ub);
    }
    void py_solver_listener::current_flaw(const ratio::solver::flaw &f) { py_slv.attr("fire_current_flaw")(get_id(f)); }

    void py_solver_listener::resolver_created(const ratio::solver::resolver &r)
    {
        // the resolver's intrinsic cost..
        const auto intr_cost = r.get_intrinsic_cost();

        py_slv.attr("fire_resolver_created")(get_id(r), get_id(r.get_effect()), r.get_data().dump(), intr_cost.numerator(), intr_cost.denominator(), slv.get_sat_core()->value(r.get_rho()));
    }
    void py_solver_listener::resolver_state_changed(const ratio::solver::resolver &r) { py_slv.attr("fire_resolver_state_changed")(get_id(r), slv.get_sat_core()->value(r.get_rho())); }
    void py_solver_listener::current_resolver(const ratio::solver::resolver &r) { py_slv.attr("fire_current_resolver")(get_id(r)); }

    void py_solver_listener::causal_link_added(const ratio::solver::flaw &f, const ratio::solver::resolver &r) { py_slv.attr("fire_causal_link_added")(get_id(f), get_id(r)); }
} // namespace ratio::python
