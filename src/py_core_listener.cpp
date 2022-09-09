#include "py_core_listener.h"
#include "predicate.h"
#include "field.h"
#include "constructor.h"
#include "method.h"
#include "item.h"
#include "solver.h"
#include <queue>

namespace ratio::python
{
    inline bool is_core(const ratio::core::scope &scp) noexcept { return &scp == &scp.get_core(); }

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

        // we revise the types..
        for (const auto &t : new_types)
        {
            revise_type(*t);

            // we add the predicates..
            for (const auto &[pred_name, pred] : t->get_predicates())
            {
                new_predicates.insert(pred.get());
                new_predicate(*pred);
            }
        }

        // we add the predicates..
        for (const auto &[pred_name, pred] : cr.get_predicates())
            if (const auto &p_it = all_types.find(pred.get()); p_it == all_types.cend())
            {
                new_predicates.insert(pred.get());
                new_predicate(*pred);
            }

        // we revise the predicates..
        for (const auto &pred : new_predicates)
            revise_predicate(*pred);

        // we add items and atoms..
        std::unordered_set<ratio::core::item *> c_items;
        for (const auto &[pred_name, pred] : cr.get_predicates())
            for (const auto &a : pred->get_instances())
            {
                auto &atm = static_cast<ratio::core::atom &>(*a);
                c_items.insert(&atm);
                const auto &i_it = all_items.find(&atm);
                if (i_it == all_items.cend())
                    new_atom(atm);
            }
        for (const auto &[tp_name, tp] : cr.get_types())
            if (!tp->is_primitive())
                q.push(tp.get());
        while (!q.empty())
        {
            for (const auto &i : q.front()->get_instances())
            {
                auto &itm = *i;
                c_items.insert(&itm);
                if (const auto &i_it = all_items.find(&itm); i_it == all_items.cend())
                    new_item(itm);
            }
            for (const auto &[pred_name, pred] : q.front()->get_predicates())
                for (const auto &a : pred->get_instances())
                {
                    auto &atm = static_cast<ratio::core::atom &>(*a);
                    c_items.insert(&atm);
                    if (const auto &a_it = all_items.find(&atm); a_it == all_items.cend())
                        new_atom(atm);
                }
            for (const auto &[tp_name, tp] : q.front()->get_types())
                q.push(tp.get());
            q.pop();
        }

        for (auto &[itm, py_itm] : std::unordered_map<const ratio::core::item *, pybind11::object>(all_items))
        {
            if (const auto citm = dynamic_cast<const ratio::core::complex_item *>(itm))
                for (const auto &[xpr_name, xpr] : citm->get_vars())
                    set(py_itm, xpr_name, *xpr);

            if (const auto a = dynamic_cast<const ratio::core::atom *>(itm))
            {
                auto &slv = static_cast<ratio::solver::solver &>(cr);
                py_itm.attr("state") = slv.get_sat_core()->value(get_sigma(slv, *a));
            }
        }

        for (const auto &[xpr_name, xpr] : cr.get_vars())
            set(py_slv, xpr_name, *xpr);

        py_slv.attr("fire_state_changed")();
    }

    void py_core_listener::started_solving() { py_slv.attr("fire_started_solving")(); }
    void py_core_listener::solution_found() { py_slv.attr("fire_solution_found")(); }
    void py_core_listener::inconsistent_problem() { py_slv.attr("fire_inconsistent_problem")(); }

    void py_core_listener::new_type(const ratio::core::type &t)
    {
        auto tp = pybind11::module_::import("oRatio.type").attr("Type")(t.get_name(), t.is_primitive());
        all_types.emplace(&t, tp);

        auto &c_scp = t.get_scope();
        if (is_core(c_scp))
            py_slv.attr("types")[t.get_name().c_str()] = tp;
        else
            all_types.at(static_cast<ratio::core::type *>(&c_scp)).attr("types")[t.get_name().c_str()] = tp;
    }

    void py_core_listener::revise_type(const ratio::core::type &t)
    {
        auto &tp = all_types.at(&t);
        // we add the type fields..
        for (const auto &[f_name, f] : t.get_fields())
            tp.attr("fields")[f_name.c_str()] = pybind11::module_::import("oRatio.field").attr("Field")(f_name, all_types.at(&f->get_type()));

        // we add the supertypes..
        pybind11::list l = tp.attr("supertypes");
        for (const auto &st : t.get_supertypes())
            l.append(all_types.at(st));

        // we add the constructors..
        pybind11::list ctrs = tp.attr("supertypes");
        for (const auto &ctr : t.get_constructors())
        {
            pybind11::list pars;
            for (const auto &arg : ctr->get_args())
                pars.append(pybind11::module_::import("oRatio.field").attr("Field")(arg->get_name(), all_types.at(&arg->get_type())));
            ctrs.append(pybind11::module_::import("oRatio.constructor").attr("Constructor")(pars));
        }

        // we add the methods..
        pybind11::dict mthds = tp.attr("methods");
        for (const auto &[mthd_name, c_mthds] : cr.get_methods())
        {
            pybind11::list n_mthds;
            for (const auto &mthd : c_mthds)
            {
                pybind11::list pars;
                for (const auto &arg : mthd->get_args())
                    pars.append(pybind11::module_::import("oRatio.field").attr("Field")(arg->get_name(), all_types.at(&arg->get_type())));
                n_mthds.append(pybind11::module_::import("oRatio.method").attr("Method")(mthd_name, pars, mthd->get_return_type() ? all_types.at(mthd->get_return_type()) : pybind11::none()));
            }
            mthds[mthd_name.c_str()] = n_mthds;
        }
    }

    void py_core_listener::new_predicate(const ratio::core::predicate &p)
    {
        auto pred = pybind11::module_::import("oRatio.type").attr("Predicate")(p.get_name());
        for (const auto &arg : p.get_args())
            pred.attr("fields")[arg->get_name().c_str()] = pybind11::module_::import("oRatio.field").attr("Field")(arg->get_name(), all_types.at(&arg->get_type()));
        all_types.emplace(&p, pred);

        auto &c_scp = p.get_scope();
        if (is_core(c_scp))
            py_slv.attr("predicates")[p.get_name().c_str()] = pred;
        else
            all_types.at(static_cast<ratio::core::type *>(&c_scp)).attr("predicates")[p.get_name().c_str()] = pred;
    }
    void py_core_listener::revise_predicate(const ratio::core::predicate &p)
    {
        // we add the supertypes..
        pybind11::list l = all_types.at(&p).attr("supertypes");
        for (const auto &st : p.get_supertypes())
            l.append(all_types.at(st));
    }

    void py_core_listener::new_item(const ratio::core::item &itm)
    {
        auto &c_type = all_types.at(&itm.get_type());
        auto c_item = pybind11::module_::import("oRatio.item").attr("Item")(py_slv, c_type, get_id(itm));
        c_item.attr("name") = cr.guess_name(itm);
        all_items.emplace(&itm, c_item);

        c_type.attr("add_instance")(c_item);
    }

    void py_core_listener::new_atom(const ratio::core::atom &atm)
    {
        auto &c_pred = all_types.at(&atm.get_type());
        auto c_atm = pybind11::module_::import("oRatio.item").attr("Item")(py_slv, c_pred, get_id(atm));
        all_items.emplace(&atm, c_atm);

        c_pred.attr("add_instance")(c_atm);
    }

    void py_core_listener::set(pybind11::object &c_obj, const std::string &name, const ratio::core::item &itm)
    {
        auto &slv = static_cast<ratio::solver::solver &>(cr);
        const auto &i_it = all_items.find(&itm);
        pybind11::object new_obj;
        if (const auto bi = dynamic_cast<const ratio::core::bool_item *>(&itm))
        { // the expression represents a primitive bool type..
            const auto &c_val = slv.get_sat_core()->value(bi->get_value());
            if (i_it == all_items.cend())
            { // we create a new boolean..
                new_obj = pybind11::module_::import("oRatio.item").attr("BoolItem")(py_slv, get_id(*bi), ((sign(bi->get_value()) ? "b" : "!b") + std::to_string(variable(bi->get_value()))), c_val);
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(bi, new_obj);
            }
            else // we update the value..
                i_it->second.attr("val") = c_val;
        }
        else if (const auto ai = dynamic_cast<const ratio::core::arith_item *>(&itm))
        { // the expression represents a primitive arithmetic type..
            const auto lb = slv.get_lra_theory().lb(ai->get_value());
            const auto ub = slv.get_lra_theory().ub(ai->get_value());
            const auto val = slv.get_lra_theory().value(ai->get_value());
            pybind11::object py_lb = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(lb.get_rational().numerator(), lb.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(lb.get_infinitesimal().numerator(), lb.get_infinitesimal().denominator()));
            pybind11::object py_ub = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(ub.get_rational().numerator(), ub.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(ub.get_infinitesimal().numerator(), ub.get_infinitesimal().denominator()));
            pybind11::object py_val = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(val.get_rational().numerator(), val.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(val.get_infinitesimal().numerator(), val.get_infinitesimal().denominator()));

            if (i_it == all_items.cend())
            { // we create a new arith..
                new_obj = pybind11::module_::import("oRatio.item").attr("ArithItem")(py_slv, all_types.at(&itm.get_type()), get_id(*ai), to_string(ai->get_value()), py_lb, py_ub, py_val);
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(ai, new_obj);
            }
            else
            { // we update the value..
                i_it->second.attr("lb") = py_lb;
                i_it->second.attr("ub") = py_ub;
                i_it->second.attr("val") = py_val;
            }
        }
        else if (const auto ei = dynamic_cast<const ratio::core::enum_item *>(&itm))
        { // the expression represents an enum type..
            const auto vals = slv.get_ov_theory().value(ei->get_var());
            pybind11::set py_vals;
            for (const auto &v : vals)
                py_vals.add(all_items.at(static_cast<const ratio::core::item *>(v)));

            if (i_it == all_items.cend())
            { // we create a new enum..
                new_obj = pybind11::module_::import("oRatio.item").attr("EnumItem")(py_slv, all_types.at(&itm.get_type()), get_id(*ei), "e" + std::to_string(ei->get_var()), py_vals);
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(ai, new_obj);
            }
            else // we update the value..
                i_it->second.attr("vals") = py_vals;
        }
        else if (const auto si = dynamic_cast<const ratio::core::string_item *>(&itm))
        { // the expression represents a primitive string type..
            if (i_it == all_items.cend())
            {
                new_obj = pybind11::module_::import("oRatio.item").attr("StringItem")(py_slv, get_id(*si), si->get_value());
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(ai, new_obj);
            }
            else // we update the value..
                i_it->second.attr("val") = si->get_value();
        }
        else // the expression represents an item..
            c_obj.attr("exprs")[name.c_str()] = i_it->second;
    }
} // namespace ratio::python
