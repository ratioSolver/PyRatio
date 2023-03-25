#include "py_core_listener.h"
#include "type.h"
#include "field.h"
#include "constructor.h"
#include "method.h"
#include "solver.h"
#include <queue>
#include <pybind11/stl.h>

namespace ratio::python
{
    py_core_listener::py_core_listener(riddle::core &cr, pybind11::object &py_slv) : core_listener(cr), py_slv(py_slv) {}
    py_core_listener::~py_core_listener() {}

    void py_core_listener::log(const std::string &msg) { py_slv.attr("fire_log")(msg); }
    void py_core_listener::read(const std::string &script) { py_slv.attr("fire_read")(script); }
    void py_core_listener::read(const std::vector<std::string> &files) { py_slv.attr("fire_read")(files); }

    void py_core_listener::state_changed()
    {
        std::unordered_set<riddle::type *> new_types;
        std::unordered_set<riddle::predicate *> new_predicates;
        std::queue<riddle::type *> q;
        for (const auto &tp : cr.get_types())
            q.push(&tp.get());
        while (!q.empty())
        {
            riddle::type &t = *q.front();
            q.pop();

            if (const auto &t_it = all_types.find(&t); t_it == all_types.cend())
            { // we have a new type..
                if (!t.is_primitive())
                    new_types.insert(&t);
                new_type(t);
            }

            // we add the types..
            if (auto ct = dynamic_cast<riddle::complex_type *>(&t); ct)
                for (const auto &tp : ct->get_types())
                    q.push(&tp.get());
        }

        // we revise the types..
        for (const auto &t : new_types)
        {
            revise_type(*t);

            // we add the predicates..
            if (auto ct = dynamic_cast<riddle::complex_type *>(t); ct)
                for (const auto &pred : ct->get_predicates())
                {
                    new_predicates.insert(&pred.get());
                    new_predicate(pred.get());
                }
        }

        // we add the predicates..
        for (const auto &pred : cr.get_predicates())
            if (const auto &p_it = all_types.find(&pred.get()); p_it == all_types.cend())
            {
                new_predicates.insert(&pred.get());
                new_predicate(pred.get());
            }

        // we revise the predicates..
        for (const auto &pred : new_predicates)
            revise_predicate(*pred);

        // we add items and atoms..
        std::unordered_set<riddle::item *> c_items;
        for (const auto &pred : cr.get_predicates())
            for (const auto &a : pred.get().get_instances())
            {
                auto &atm = static_cast<ratio::atom &>(*a);
                c_items.insert(&atm);
                const auto &i_it = all_items.find(&atm);
                if (i_it == all_items.cend())
                    new_atom(atm);
            }
        for (const auto &tp : cr.get_types())
            if (!tp.get().is_primitive())
                q.push(&tp.get());
        while (!q.empty())
        {
            if (auto ct = dynamic_cast<riddle::complex_type *>(q.front()); ct)
            {
                for (const auto &i : ct->get_instances())
                {
                    auto &itm = *i;
                    c_items.insert(&itm);
                    if (const auto &i_it = all_items.find(&itm); i_it == all_items.cend())
                        new_item(itm);
                }
                for (const auto &pred : ct->get_predicates())
                    for (const auto &a : pred.get().get_instances())
                    {
                        auto &atm = static_cast<ratio::atom &>(*a);
                        c_items.insert(&atm);
                        if (const auto &a_it = all_items.find(&atm); a_it == all_items.cend())
                            new_atom(atm);
                    }
                for (const auto &tp : ct->get_types())
                    q.push(&tp.get());
            }
            q.pop();
        }

        for (auto &[itm, py_itm] : std::unordered_map<const riddle::item *, pybind11::object>(all_items))
        {
            if (const auto citm = dynamic_cast<const riddle::complex_item *>(itm))
                for (const auto &[xpr_name, xpr] : citm->get_vars())
                    set(py_itm, xpr_name, *xpr);

            if (const auto a = dynamic_cast<const ratio::atom *>(itm))
            {
                auto &slv = static_cast<ratio::solver &>(cr);
                py_itm.attr("state") = slv.get_sat_core().value(a->get_sigma());
            }
        }

        for (const auto &[xpr_name, xpr] : cr.get_vars())
            set(py_slv, xpr_name, *xpr);

        py_slv.attr("fire_state_changed")();
    }

    void py_core_listener::started_solving() { py_slv.attr("fire_started_solving")(); }
    void py_core_listener::solution_found() { py_slv.attr("fire_solution_found")(); }
    void py_core_listener::inconsistent_problem() { py_slv.attr("fire_inconsistent_problem")(); }

    void py_core_listener::new_type(const riddle::type &t)
    {
        auto tp = pybind11::module_::import("oRatio.type").attr("Type")(t.get_name(), t.is_primitive());
        all_types.emplace(&t, tp);

        auto &c_scp = t.get_scope();
        if (is_core(c_scp))
            py_slv.attr("types")[t.get_name().c_str()] = tp;
        else if (auto ct = dynamic_cast<const riddle::type *>(&c_scp); ct)
            all_types.at(ct).attr("types")[t.get_name().c_str()] = tp;
    }

    void py_core_listener::revise_type(const riddle::type &t)
    {
        auto &tp = all_types.at(&t);
        if (auto ct = dynamic_cast<const riddle::complex_type *>(&t); ct)
        {
            // we add the type fields..
            for (const auto &[f_name, f] : ct->get_fields())
                tp.attr("fields")[f_name.c_str()] = pybind11::module_::import("oRatio.field").attr("Field")(all_types.at(&f->get_type()), f_name);

            // we add the parents..
            pybind11::list pars = tp.attr("parents");
            for (const auto &st : ct->get_parents())
                pars.append(all_types.at(&st.get()));

            // we add the types..
            pybind11::list tps = tp.attr("types");
            for (const auto &st : ct->get_types())
                tps.append(all_types.at(&st.get()));

            // we add the constructors..
            pybind11::list ctrs = tp.attr("constructors");
            for (const auto &ctr : ct->get_constructors())
            {
                pybind11::list pars;
                for (const auto &arg : ctr.get().get_args())
                    pars.append(pybind11::module_::import("oRatio.field").attr("Field")(all_types.at(&arg.get().get_type()), arg.get().get_name()));
                ctrs.append(pybind11::module_::import("oRatio.constructor").attr("Constructor")(pars));
            }

            // we add the methods..
            std::map<std::string, std::vector<std::reference_wrapper<riddle::method>>> methods;
            for (const auto &mthd : ct->get_methods())
                methods[mthd.get().get_name()].push_back(mthd);
            pybind11::dict mthds = tp.attr("methods");
            for (const auto &[mthd_name, c_mthds] : methods)
            {
                pybind11::list n_mthds;
                for (const auto &mthd : c_mthds)
                {
                    pybind11::list pars;
                    for (const auto &arg : mthd.get().get_args())
                        pars.append(pybind11::module_::import("oRatio.field").attr("Field")(all_types.at(&arg.get().get_type()), arg.get().get_name()));
                    n_mthds.append(pybind11::module_::import("oRatio.method").attr("Method")(mthd_name, pars, mthd.get().get_return_type() ? all_types.at(mthd.get().get_return_type()) : pybind11::none()));
                }
                mthds[mthd_name.c_str()] = n_mthds;
            }
        }
    }

    void py_core_listener::new_predicate(const riddle::predicate &p)
    {
        auto pred = pybind11::module_::import("oRatio.type").attr("Predicate")(p.get_name());
        for (const auto &arg : p.get_args())
            pred.attr("fields")[arg.get().get_name().c_str()] = pybind11::module_::import("oRatio.field").attr("Field")(all_types.at(&arg.get().get_type()), arg.get().get_name());
        all_types.emplace(&p, pred);

        auto &c_scp = p.type::get_scope();
        if (riddle::is_core(c_scp))
            py_slv.attr("predicates")[p.get_name().c_str()] = pred;
        else if (auto ct = dynamic_cast<const riddle::type *>(&c_scp); ct)
            all_types.at(ct).attr("predicates")[p.get_name().c_str()] = pred;
    }
    void py_core_listener::revise_predicate(const riddle::predicate &p)
    {
        // we add the parents..
        pybind11::list l = all_types.at(&p).attr("parents");
        for (const auto &st : p.get_parents())
            l.append(all_types.at(&st.get()));
    }

    void py_core_listener::new_item(const riddle::item &itm)
    {
        auto &c_type = all_types.at(&itm.get_type());
        auto c_item = pybind11::module_::import("oRatio.item").attr("ComplexItem")(py_slv, c_type, get_id(itm));
        c_item.attr("name") = cr.guess_name(itm);
        all_items.emplace(&itm, c_item);

        c_type.attr("add_instance")(c_item);
        py_slv.attr("items")[std::to_string(get_id(itm)).c_str()] = c_item;
    }

    void py_core_listener::new_atom(const riddle::atom &atm)
    {
        auto &c_pred = all_types.at(&atm.get_type());
        auto c_atm = pybind11::module_::import("oRatio.item").attr("Atom")(py_slv, c_pred, get_id(atm));
        all_items.emplace(&atm, c_atm);

        c_pred.attr("add_instance")(c_atm);
        py_slv.attr("atoms")[std::to_string(get_id(atm)).c_str()] = c_atm;
    }

    void py_core_listener::set(pybind11::object &c_obj, const std::string &name, const riddle::item &itm)
    {
        auto &slv = static_cast<ratio::solver &>(cr);
        const auto &i_it = all_items.find(&itm);
        pybind11::object new_obj;
        if (const auto bi = dynamic_cast<const ratio::bool_item *>(&itm))
        { // the expression represents a primitive bool type..
            const auto &c_val = slv.get_sat_core().value(bi->get_lit());
            if (i_it == all_items.cend())
            { // we create a new boolean..
                new_obj = pybind11::module_::import("oRatio.item").attr("BoolItem")(py_slv, get_id(*bi), ((sign(bi->get_lit()) ? "b" : "!b") + std::to_string(variable(bi->get_lit()))), c_val);
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(bi, new_obj);
            }
            else // we update the value..
                i_it->second.attr("val") = c_val;
        }
        else if (const auto ai = dynamic_cast<const ratio::arith_item *>(&itm))
        { // the expression represents a primitive arithmetic type..
            const auto lb = slv.get_lra_theory().lb(ai->get_lin());
            const auto ub = slv.get_lra_theory().ub(ai->get_lin());
            const auto val = slv.get_lra_theory().value(ai->get_lin());
            pybind11::object py_lb = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(lb.get_rational().numerator(), lb.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(lb.get_infinitesimal().numerator(), lb.get_infinitesimal().denominator()));
            pybind11::object py_ub = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(ub.get_rational().numerator(), ub.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(ub.get_infinitesimal().numerator(), ub.get_infinitesimal().denominator()));
            pybind11::object py_val = pybind11::module_::import("oRatio.rational").attr("InfRational")(pybind11::module_::import("oRatio.rational").attr("Rational")(val.get_rational().numerator(), val.get_rational().denominator()), pybind11::module_::import("oRatio.rational").attr("Rational")(val.get_infinitesimal().numerator(), val.get_infinitesimal().denominator()));

            if (i_it == all_items.cend())
            { // we create a new arith..
                new_obj = pybind11::module_::import("oRatio.item").attr("ArithItem")(py_slv, all_types.at(&itm.get_type()), get_id(*ai), to_string(ai->get_lin()), py_lb, py_ub, py_val);
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
        else if (const auto ei = dynamic_cast<const ratio::enum_item *>(&itm))
        { // the expression represents an enum type..
            const auto vals = slv.get_ov_theory().value(ei->get_var());
            pybind11::set py_vals;
            for (const auto &v : vals)
                py_vals.add(all_items.at(dynamic_cast<const riddle::item *>(v)));

            if (i_it == all_items.cend())
            { // we create a new enum..
                new_obj = pybind11::module_::import("oRatio.item").attr("EnumItem")(py_slv, all_types.at(&itm.get_type()), get_id(*ei), "e" + std::to_string(ei->get_var()), py_vals);
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(ai, new_obj);
            }
            else // we update the value..
                i_it->second.attr("vals") = py_vals;
        }
        else if (const auto si = dynamic_cast<const ratio::string_item *>(&itm))
        { // the expression represents a primitive string type..
            if (i_it == all_items.cend())
            {
                new_obj = pybind11::module_::import("oRatio.item").attr("StringItem")(py_slv, get_id(*si), si->get_string());
                c_obj.attr("exprs")[name.c_str()] = new_obj;
                all_items.emplace(ai, new_obj);
            }
            else // we update the value..
                i_it->second.attr("val") = si->get_string();
        }
        else // the expression represents an item..
            c_obj.attr("exprs")[name.c_str()] = i_it->second;

        if (c_obj.is(py_slv)) // since the object is the solver, we also add a field..
            c_obj.attr("fields")[name.c_str()] = pybind11::module_::import("oRatio.field").attr("Field")(all_types.at(&itm.get_type()), name);
    }
} // namespace ratio::python
