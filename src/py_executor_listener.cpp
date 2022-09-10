#include "py_executor_listener.h"

namespace ratio::python
{
    py_executor_listener::py_executor_listener(ratio::executor::executor &exec, pybind11::object &py_exec) : executor_listener(exec), py_exec(py_exec) {}
    py_executor_listener::~py_executor_listener() {}
} // namespace ratio::python