#include "solver.h"
#include "py_core_listener.h"
#include "solver.h"
#include "item.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#ifdef __cplusplus
extern "C"
{
#endif
    static PyObject *new_solver_instance(PyObject *self, PyObject *args)
    {
        PyObject *py_s;
        if (!PyArg_ParseTuple(args, "O", &py_s))
            return NULL;

        auto *s = new ratio::solver::solver();
        PyObject_SetAttr(py_s, PyUnicode_InternFromString("native_handler"), PyLong_FromLong(reinterpret_cast<uintptr_t>(s)));
        auto *cl = new ratio::python::py_core_listener(*s);
        PyObject_SetAttr(py_s, PyUnicode_InternFromString("core_listener_native_handler"), PyLong_FromLong(reinterpret_cast<uintptr_t>(cl)));
        return PyLong_FromLong(reinterpret_cast<uintptr_t>(s));
    }

    static PyObject *delete_solver_instance(PyObject *self, PyObject *args)
    {
        PyObject *py_s;
        if (!PyArg_ParseTuple(args, "O", &py_s))
            return NULL;

        delete reinterpret_cast<ratio::python::py_core_listener *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("core_listener_native_handler"))));
        delete reinterpret_cast<ratio::solver::solver *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("native_handler"))));
        return PyBool_FromLong(1);
    }

    static PyMethodDef oRatioMethods[] = {
        {"new_instance", new_solver_instance, METH_VARARGS, "Creates a new solver instance."},
        {"delete_instance", delete_solver_instance, METH_VARARGS, "Deletes an existing solver instance."},
        {NULL, NULL, 0, NULL}};

    static struct PyModuleDef oRatioModule = {PyModuleDef_HEAD_INIT, "oRatioNative", NULL, -1, oRatioMethods};

    PyMODINIT_FUNC PyInit_oRatioNative(void) { return PyModule_Create(&oRatioModule); }
#ifdef __cplusplus
}
#endif