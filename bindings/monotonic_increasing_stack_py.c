// bindings/monotonic_stack_py.c

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../src/monotonic_increasing_stack.h"

typedef struct {
    PyObject_HEAD;
    monotonic_increasing_stack_t* stack;
} PyMonotonicIncreasingStackObject;

static void PyMonotonicIncreasingStack_dealloc(PyMonotonicIncreasingStackObject* self) {
    monotonic_increasing_stack_destroy(&(self->stack));
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyMonotonicIncreasingStack_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    PyMonotonicIncreasingStackObject* self;
    self = (PyMonotonicIncreasingStackObject*)type->tp_alloc(type, 0);
    if (!self) return NULL;

    if (monotonic_increasing_stack_create(&(self->stack)) != MIS_SUCCESS) {
        Py_DECREF(self);
        PyErr_SetString(PyExc_RuntimeError, "Failed to create MonotonicIncreasingStack");
        return NULL;
    }

    return (PyObject*)self;
}

static PyObject* PyMonotonicIncreasingStack_push(PyMonotonicIncreasingStackObject* self, PyObject* args) {
    int32_t value;

    if (!PyArg_ParseTuple(args, "i", &value)) return NULL;
    if (monotonic_increasing_stack_push(self->stack, value) != MIS_SUCCESS) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to push MonotonicIncreasingStack");
    }

    Py_RETURN_NONE;
}

static PyObject* PyMonotonicIncreasingStack_pop(PyMonotonicIncreasingStackObject* self) {
    int32_t removed;

    mis_status_t pop_status = monotonic_increasing_stack_pop(self->stack, &removed);
    if (pop_status == MIS_ERROR_EMPTY) {
        Py_RETURN_NONE;

    } else if (pop_status != MIS_SUCCESS) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to pop MonotonicIncreasingStack");
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(removed);
}

static PyObject* PyMonotonicIncreasingStack_top(PyMonotonicIncreasingStackObject* self) {
    int32_t top;

    mis_status_t pop_status = monotonic_increasing_stack_top(self->stack, &top);
    if (pop_status == MIS_ERROR_EMPTY) {
        Py_RETURN_NONE;

    } else if (pop_status != MIS_SUCCESS) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to top MonotonicIncreasingStack");
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(top);
}

static Py_ssize_t PyMonotonicIncreasingStack_length(PyMonotonicIncreasingStackObject* self) {
    uint32_t size = 0;
    if (monotonic_increasing_stack_size(self->stack, &size) != MIS_SUCCESS) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to get size for MonotonicIncreasingStack");
        return -1;
    }
    return (Py_ssize_t)size;
}

static PyMethodDef PyMonotonicIncreasingStack_methods[] = {
    {"push", (PyCFunction)PyMonotonicIncreasingStack_push, METH_VARARGS, "Add a value to the monotonic increasing stack."},
    {"pop", (PyCFunction)PyMonotonicIncreasingStack_pop, METH_NOARGS, "Remove a value from the monotonic increasing stack."},
    {"top", (PyCFunction)PyMonotonicIncreasingStack_top, METH_VARARGS, "Get top a value from the monotonic increasing stack."},
    {NULL}
};

static PySequenceMethods PyMonotonicIncreasingStack_sequence_methods = {
    .sq_length = (lenfunc)PyMonotonicIncreasingStack_length,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static PyTypeObject PyMonotonicIncreasingStackType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "monotonic_increasing_stack.MonotonicIncreasingStack",
    .tp_basicsize = sizeof(PyMonotonicIncreasingStackObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Monotonic Increasing Stack\n"
    "\n"
    "- push(value: int) - Add a value to the monotonic increasing stack.\n"
    "- pop() -> Optional[int] - Remove a value from the monotonic increasing stack.\n"
    "- top() -> Optional[int] - Get top a value from the monotonic increasing stack.\n",
    .tp_methods = PyMonotonicIncreasingStack_methods,
    .tp_new = PyMonotonicIncreasingStack_new,
    .tp_dealloc = (destructor)PyMonotonicIncreasingStack_dealloc,
    .tp_as_sequence = &PyMonotonicIncreasingStack_sequence_methods,
};

static PyModuleDef monotonic_increasing_stack_module = {
    PyModuleDef_HEAD_INIT,
    "monotonic_increasing_stack",
    "This module exposes a high-performance, memory-efficient monotonic increasing stack "
    "implementation written in C to Python via the C API",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_monotonic_increasing_stack(void) {
    PyObject* m;

    if (PyType_Ready(&PyMonotonicIncreasingStackType) < 0) return NULL;

    m = PyModule_Create(&monotonic_increasing_stack_module);
    if (!m) return NULL;

    Py_INCREF(&PyMonotonicIncreasingStackType);
    PyModule_AddObject(m, "MonotonicIncreasingStack", (PyObject*)&PyMonotonicIncreasingStackType);
    return m;
}