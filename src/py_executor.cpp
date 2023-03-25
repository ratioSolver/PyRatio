#include "executor.h"
#include "py_executor_listener.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define NATIVE_HANDLER "__native_handler"
#define EXECUTOR_LISTENER_NATIVE_HANDLER "__executor_listener_native_handler"

static ratio::solver *get_solver(const pybind11::object &py_exec) { return reinterpret_cast<ratio::solver *>(py_exec.attr("solver").attr(NATIVE_HANDLER).cast<uintptr_t>()); }
static ratio::executor::executor *get_executor(const pybind11::object &py_exec) { return reinterpret_cast<ratio::executor::executor *>(py_exec.attr(NATIVE_HANDLER).cast<uintptr_t>()); }
static ratio::atom *get_atom(const pybind11::handle &py_atm) { return reinterpret_cast<ratio::atom *>(py_atm.cast<uintptr_t>()); }

void new_instance(pybind11::object &py_exec, pybind11::object &units_per_tick)
{
    auto s = get_solver(py_exec);
    auto *exec = new ratio::executor::executor(*s, utils::rational(static_cast<utils::I>(units_per_tick.attr("numerator").cast<utils::I>()), static_cast<utils::I>(units_per_tick.attr("denominator").cast<utils::I>())));
    py_exec.attr(NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(exec);

    auto *el = new ratio::python::py_executor_listener(*exec, py_exec);
    py_exec.attr(EXECUTOR_LISTENER_NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(el);
}
void delete_instance(pybind11::object &py_exec)
{
    delete reinterpret_cast<ratio::python::py_executor_listener *>(py_exec.attr(EXECUTOR_LISTENER_NATIVE_HANDLER).cast<uintptr_t>());
    delete get_executor(py_exec);
}

pybind11::bool_ is_plan_executing(pybind11::object &py_exec) { return get_executor(py_exec)->is_executing(); }

void start_plan_execution(pybind11::object &py_exec) { get_executor(py_exec)->start_execution(); }
void pause_plan_execution(pybind11::object &py_exec) { get_executor(py_exec)->pause_execution(); }

pybind11::bool_ is_plan_finished(pybind11::object &py_exec) { return get_executor(py_exec)->is_finished(); }

pybind11::bool_ adapt_script(pybind11::object &py_exec, const pybind11::str &riddle)
{
    try
    {
        get_executor(py_exec)->adapt(riddle.cast<std::string>());
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
pybind11::bool_ adapt_files(pybind11::object &py_exec, const std::vector<pybind11::str> &files)
{
    std::vector<std::string> c_files;
    for (const auto &file : files)
        c_files.push_back(file.cast<std::string>());
    try
    {
        get_executor(py_exec)->adapt(c_files);
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

void tick(pybind11::object &py_exec) { get_executor(py_exec)->tick(); }

void dont_start_yet(pybind11::object &py_exec, pybind11::list &atoms)
{
    std::unordered_map<const ratio::atom *, utils::rational> c_atoms;
    for (const auto &atm : atoms)
    {
        pybind11::tuple atm_delay = atm.cast<pybind11::tuple>();
        c_atoms.emplace(get_atom(atm_delay[0]), utils::rational(atm_delay[1].attr("numerator").cast<utils::I>(), atm_delay[1].attr("denominator").cast<utils::I>()));
    }
    get_executor(py_exec)->dont_start_yet(c_atoms);
}

void dont_end_yet(pybind11::object &py_exec, pybind11::list &atoms)
{
    std::unordered_map<const ratio::atom *, utils::rational> c_atoms;
    for (const auto &atm : atoms)
    {
        pybind11::tuple atm_delay = atm.cast<pybind11::tuple>();
        c_atoms.emplace(get_atom(atm_delay[0]), utils::rational(atm_delay[1].attr("numerator").cast<utils::I>(), atm_delay[1].attr("denominator").cast<utils::I>()));
    }
    get_executor(py_exec)->dont_end_yet(c_atoms);
}

void failure(pybind11::object &py_exec, pybind11::list &atoms)
{
    std::unordered_set<const ratio::atom *> c_atoms;
    for (const auto &atm : atoms)
        c_atoms.emplace(get_atom(atm));
    get_executor(py_exec)->failure(c_atoms);
}

PYBIND11_MODULE(oRatioExecutorNative, m)
{
    m.doc() = "Python API for the oRatio executor";

    m.def("new_instance", &new_instance, "Creates a new executor instance");
    m.def("delete_instance", &delete_instance, "Deletes an existing executor instance");
    m.def("is_plan_executing", &is_plan_executing, "Checks whether the current solution is being executed");
    m.def("start_plan_execution", &start_plan_execution, "Starts the execution of the current solution");
    m.def("pause_plan_execution", &pause_plan_execution, "Pauses the execution of the current solution");
    m.def("is_plan_finished", &is_plan_finished, "Checks whether there are task to be executed in the future");
    m.def("adapt_script", &adapt_script, "Adapts the current solution a RiDDLe script");
    m.def("adapt_files", &adapt_files, "Adapts the current solution to a list of RiDDLe files");
    m.def("exec_tick", &tick, "Executes one tick");
    m.def("dont_start_tasks_yet", &dont_start_yet, "Delays the start of the execution of the given tasks adapting the solution accordingly");
    m.def("dont_end_tasks_yet", &dont_end_yet, "Delays the end of the execution of the given tasks adapting the solution accordingly");
    m.def("failed_tasks", &failure, "Notifies the failure of the given tasks adapting the solution accordingly");
}