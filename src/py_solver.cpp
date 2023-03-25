#include "solver.h"
#include "py_core_listener.h"
#include "py_solver_listener.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define NATIVE_HANDLER "__native_handler"
#define CORE_LISTENER_NATIVE_HANDLER "__core_listener_native_handler"
#define SOLVER_LISTENER_NATIVE_HANDLER "__solver_listener_native_handler"

static ratio::solver *get_solver(const pybind11::object &py_slv) { return reinterpret_cast<ratio::solver *>(py_slv.attr(NATIVE_HANDLER).cast<uintptr_t>()); }

void new_instance(pybind11::object &py_slv)
{
    auto s = new ratio::solver(false);
    py_slv.attr(NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(s);

    auto cl = new ratio::python::py_core_listener(*s, py_slv);
    py_slv.attr(CORE_LISTENER_NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(cl);

    auto sl = new ratio::python::py_solver_listener(*s, py_slv);
    py_slv.attr(SOLVER_LISTENER_NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(sl);

    s->init();
}
void delete_instance(pybind11::object &py_slv)
{
    delete reinterpret_cast<ratio::python::py_solver_listener *>(py_slv.attr(SOLVER_LISTENER_NATIVE_HANDLER).cast<uintptr_t>());
    delete reinterpret_cast<ratio::python::py_core_listener *>(py_slv.attr(CORE_LISTENER_NATIVE_HANDLER).cast<uintptr_t>());
    delete get_solver(py_slv);
}
pybind11::bool_ read_script(pybind11::object &py_slv, const pybind11::str &riddle)
{
    try
    {
        get_solver(py_slv)->read(riddle.cast<std::string>());
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
pybind11::bool_ read_files(pybind11::object &py_slv, const std::vector<pybind11::str> &files)
{
    std::vector<std::string> c_files;
    for (const auto &file : files)
        c_files.push_back(file.cast<std::string>());
    try
    {
        get_solver(py_slv)->read(c_files);
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
pybind11::bool_ solve_problem(pybind11::object &py_slv)
{
    try
    {
        get_solver(py_slv)->solve();
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}
pybind11::str extract_timelines(pybind11::object &py_slv) { return to_timelines(*get_solver(py_slv)).to_string(); }

PYBIND11_MODULE(oRatioSolverNative, m)
{
    m.doc() = "Python API for the oRatio solver";

    m.def("new_instance", &new_instance, "Creates a new solver instance");
    m.def("delete_instance", &delete_instance, "Deletes an existing solver instance");
    m.def("read_script", &read_script, "Reads a RiDDLe script");
    m.def("read_files", &read_files, "Reads a list of RiDDLe files");
    m.def("solve_problem", &solve_problem, "Solves the given problem");
    m.def("extract_solver_timelines", &extract_timelines, "Extracts the timelines from the given solver");
}