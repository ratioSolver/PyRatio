#include "solver.h"
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
        return PyLong_FromLong(reinterpret_cast<uintptr_t>(s));
    }

    static PyObject *delete_solver_instance(PyObject *self, PyObject *args)
    {
        long c_slv = PyLong_AsLong(PyObject_GetAttr(self, PyUnicode_InternFromString("native_handler")));
        ratio::solver::solver *slv = reinterpret_cast<ratio::solver::solver *>(c_slv);
        delete slv;
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