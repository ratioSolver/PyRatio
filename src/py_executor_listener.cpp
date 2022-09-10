#include "py_executor_listener.h"
#include "item.h"

namespace ratio::python
{
    py_executor_listener::py_executor_listener(ratio::executor::executor &exec, pybind11::object &py_exec) : executor_listener(exec), py_exec(py_exec) {}
    py_executor_listener::~py_executor_listener() {}

    void py_executor_listener::tick(const semitone::rational &time) { py_exec("fire_tick")(pybind11::module_::import("oRatio.rational").attr("Rational")(time.numerator(), time.denominator())); }

    void py_executor_listener::starting(const std::unordered_set<ratio::core::atom *> &atoms)
    {
        pybind11::set atms;
        for (const auto &atm : atoms)
            atms.add(py_exec.attr("solver").attr("atoms")[std::to_string(get_id(*atm)).c_str()]);
        py_exec("fire_starting")(atms);
    }
    void py_executor_listener::start(const std::unordered_set<ratio::core::atom *> &atoms)
    {
        pybind11::set atms;
        for (const auto &atm : atoms)
            atms.add(py_exec.attr("solver").attr("atoms")[std::to_string(get_id(*atm)).c_str()]);
        py_exec("fire_start")(atms);
    }

    void py_executor_listener::ending(const std::unordered_set<ratio::core::atom *> &atoms)
    {
        pybind11::set atms;
        for (const auto &atm : atoms)
            atms.add(py_exec.attr("solver").attr("atoms")[std::to_string(get_id(*atm)).c_str()]);
        py_exec("fire_ending")(atms);
    }
    void py_executor_listener::end(const std::unordered_set<ratio::core::atom *> &atoms)
    {
        pybind11::set atms;
        for (const auto &atm : atoms)
            atms.add(py_exec.attr("solver").attr("atoms")[std::to_string(get_id(*atm)).c_str()]);
        py_exec("fire_end")(atms);
    }
} // namespace ratio::python