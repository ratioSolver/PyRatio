#include "solver.h"
#include "py_core_listener.h"
#include "py_solver_listener.h"
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

        auto s = new ratio::solver::solver();
        PyObject_SetAttr(py_s, PyUnicode_InternFromString("native_handler"), PyLong_FromLong(reinterpret_cast<uintptr_t>(s)));

        auto cl = new ratio::python::py_core_listener(*s, py_s);
        PyObject_SetAttr(py_s, PyUnicode_InternFromString("core_listener_native_handler"), PyLong_FromLong(reinterpret_cast<uintptr_t>(cl)));

        auto sl = new ratio::python::py_solver_listener(*s, py_s);
        PyObject_SetAttr(py_s, PyUnicode_InternFromString("solver_listener_native_handler"), PyLong_FromLong(reinterpret_cast<uintptr_t>(sl)));

        return PyLong_FromLong(reinterpret_cast<uintptr_t>(s));
    }

    static PyObject *delete_solver_instance(PyObject *self, PyObject *args)
    {
        PyObject *py_s;
        if (!PyArg_ParseTuple(args, "O", &py_s))
            return NULL;

        delete reinterpret_cast<ratio::python::py_solver_listener *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("solver_listener_native_handler"))));
        PyObject_DelAttr(py_s, PyUnicode_InternFromString("solver_listener_native_handler"));

        delete reinterpret_cast<ratio::python::py_core_listener *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("core_listener_native_handler"))));
        PyObject_DelAttr(py_s, PyUnicode_InternFromString("core_listener_native_handler"));

        delete reinterpret_cast<ratio::solver::solver *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("native_handler"))));
        PyObject_DelAttr(py_s, PyUnicode_InternFromString("native_handler"));

        return PyBool_FromLong(1);
    }

    static PyObject *read_riddle(PyObject *self, PyObject *args)
    {
        PyObject *py_s;
        PyObject *py_args;
        if (!PyArg_ParseTuple(args, "OO", &py_s, &py_args))
            return NULL;

        auto s = reinterpret_cast<ratio::solver::solver *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("native_handler"))));

        if (PyList_Check(py_args))
        {
            std::cout << "a list..\n";
            PyObject *iter = PyObject_GetIter(py_args);
            if (!iter)
                return NULL;

            std::vector<std::string> c_files;
            while (true)
            {
                PyObject *next = PyIter_Next(iter);
                if (next)
                    c_files.push_back(PyBytes_AsString(PyUnicode_AsEncodedString(next, "utf-8", "~E~")));
                else
                    break;
            }

            try
            {
                s->read(c_files);
            }
            catch (const std::exception &e)
            {
                return PyBool_FromLong(0);
            }
        }
        else
            try
            {
                s->read(PyBytes_AsString(PyUnicode_AsEncodedString(py_args, "utf-8", "~E~")));
            }
            catch (const std::exception &e)
            {
                return PyBool_FromLong(0);
            }
        return PyBool_FromLong(1);
    }

    static PyObject *solve_problem(PyObject *self, PyObject *args)
    {
        PyObject *py_s;
        if (!PyArg_ParseTuple(args, "O", &py_s))
            return NULL;

        auto s = reinterpret_cast<ratio::solver::solver *>(PyLong_AsLong(PyObject_GetAttr(py_s, PyUnicode_InternFromString("native_handler"))));

        try
        {
            s->solve();
        }
        catch (const std::exception &e)
        {
            return PyBool_FromLong(0);
        }
        return PyBool_FromLong(1);
    }

    static PyMethodDef oRatioMethods[] = {
        {"new_instance", new_solver_instance, METH_VARARGS, "Creates a new solver instance."},
        {"delete_instance", delete_solver_instance, METH_VARARGS, "Deletes an existing solver instance."},
        {"read_riddle", read_riddle, METH_VARARGS, "Reads a RiDDLe script or a list of RiDDLe files."},
        {"solve_problem", solve_problem, METH_VARARGS, "Solves the given problem."},
        {NULL, NULL, 0, NULL}};

    static struct PyModuleDef oRatioModule = {PyModuleDef_HEAD_INIT, "oRatioNative", NULL, -1, oRatioMethods};

    PyMODINIT_FUNC PyInit_oRatioNative(void) { return PyModule_Create(&oRatioModule); }
#ifdef __cplusplus
}
#endif