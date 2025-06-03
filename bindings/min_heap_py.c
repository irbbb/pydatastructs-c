// bindings/min_heap_py.c

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../src/min_heap.h"

typedef struct {
    PyObject_HEAD;
    MinHeap* min_heap;
} PyMinHeapObject;

static void PyMinHeap_dealloc(PyMinHeapObject* self) {
    min_heap_free(self->min_heap);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyMinHeap_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    PyMinHeapObject* self;
    self = (PyMinHeapObject*)type->tp_alloc(type, 0);
    self->min_heap = min_heap_create();

    return (PyObject*)self;
}

static PyObject* PyMinHeap_insert(PyMinHeapObject* self, PyObject* args) {
    int value;

    if(!PyArg_ParseTuple(args, "i", &value)) return NULL;
    min_heap_insert(self->min_heap, value);

    Py_RETURN_NONE;
}

static PyObject* PyMinHeap_remove(PyMinHeapObject* self, PyObject* args) {
    int removed;

    if (!min_heap_remove(self->min_heap, &removed)) {
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(removed);
}

static Py_ssize_t PyMinHeap_length(PyMinHeapObject* self) {
    return (Py_ssize_t)min_heap_length(self->min_heap);
}

static PyMethodDef PyMinHeap_methods[] = {
    {"insert", (PyCFunction)PyMinHeap_insert, METH_VARARGS, "Add a value to the minimum heap."},
    {"remove", (PyCFunction)PyMinHeap_remove, METH_NOARGS, "Remove the minimum value from the minimum heap."},
    {NULL}
};

static PySequenceMethods PyMinHeap_sequence_methods = {
    .sq_length = (lenfunc)PyMinHeap_length,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static PyTypeObject PyMinHeapType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "min_heap.MinHeap",
    .tp_basicsize = sizeof(PyMinHeapObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Minimum Heap\n"
    "\n"
    "- insert(value: int) - Add a value to the minimum heap.\n"
    "- remove() -> Optional[int] - Remove the minimum value from the minimum heap.\n",
    .tp_methods = PyMinHeap_methods,
    .tp_new = PyMinHeap_new,
    .tp_dealloc = (destructor)PyMinHeap_dealloc,
    .tp_as_sequence = &PyMinHeap_sequence_methods,
};

static PyModuleDef min_heap_module = {
    PyModuleDef_HEAD_INIT,
    "min_heap",
    "This module exposes a high-performance, memory-efficient minimum heap "
    "implementation written in C to Python via the C API",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_min_heap(void) {
    PyObject *m;

    if (PyType_Ready(&PyMinHeapType) < 0) return NULL;

    m = PyModule_Create(&min_heap_module);
    if (!m) return NULL;

    Py_INCREF(&PyMinHeapType);
    PyModule_AddObject(m, "MinHeap", (PyObject*)&PyMinHeapType);
    return m;
}

