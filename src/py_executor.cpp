#include "executor.h"
#include "py_executor_listener.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#define NATIVE_HANDLER "__native_handler"
#define EXECUTOR_LISTENER_NATIVE_HANDLER "__executor_listener_native_handler"

static ratio::solver::solver *get_solver(const pybind11::object &py_exec) { return reinterpret_cast<ratio::solver::solver *>(py_exec.attr("solver").attr(NATIVE_HANDLER).cast<uintptr_t>()); }
static ratio::executor::executor *get_executor(const pybind11::object &py_exec) { return reinterpret_cast<ratio::executor::executor *>(py_exec.attr(NATIVE_HANDLER).cast<uintptr_t>()); }

void new_instance(pybind11::object &py_exec, pybind11::object &units_per_tick)
{
    auto s = get_solver(py_exec);
    auto *exec = new ratio::executor::executor(*s, semitone::rational(static_cast<semitone::I>(units_per_tick.attr("numerator").cast<semitone::I>()), static_cast<semitone::I>(units_per_tick.attr("denominator").cast<semitone::I>())));
    py_exec.attr(NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(exec);

    auto *el = new ratio::python::py_executor_listener(*exec, py_exec);
    py_exec.attr(EXECUTOR_LISTENER_NATIVE_HANDLER) = reinterpret_cast<uintptr_t>(el);
}
void delete_instance(pybind11::object &py_exec)
{
    delete reinterpret_cast<ratio::python::py_executor_listener *>(py_exec.attr(EXECUTOR_LISTENER_NATIVE_HANDLER).cast<uintptr_t>());
    delete get_executor(py_exec);
}

void tick(pybind11::object &py_exec) { get_executor(py_exec)->tick(); }

PYBIND11_MODULE(oRatioExecutorNative, m)
{
    m.doc() = "Python API for the oRatio executor";

    m.def("new_instance", &new_instance, "Creates a new executor instance");
    m.def("delete_instance", &delete_instance, "Deletes an existing executor instance");
    m.def("exec_tick", &tick, "Executes one tick");
}