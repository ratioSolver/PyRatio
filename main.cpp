#include "solver.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *new_solver_instance(PyObject *self, PyObject *args)
{
    auto *s = new ratio::solver::solver();
    return PyLong_FromLong(reinterpret_cast<uintptr_t>(s));
}

static PyMethodDef oRatioMethods[] = {
    {"new_instance", new_solver_instance, METH_VARARGS, "Creates a new solver instance."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef oRatioModule = {PyModuleDef_HEAD_INIT, "oRatio", NULL, -1, oRatioMethods};

PyMODINIT_FUNC PyInit_oRatio(void) { return PyModule_Create(&oRatioModule); }