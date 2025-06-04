// bindings/min_heap_py.c

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../src/min_heap.h"

typedef struct {
    PyObject_HEAD;
    MinHeap* min_heap;
} PyMinHeapObject;

static PyTypeObject PyMinHeapType;

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

static PyObject* PyMinHeap_cls_heapify(PyObject* cls, PyObject* args) {
    if (!PyType_Check(cls)) {
        PyErr_SetString(PyExc_TypeError, "First argument must be a class");
        return NULL;
    }

    PyObject* input_list;

    if(!PyArg_ParseTuple(args, "O!", &PyList_Type, &input_list)) return NULL;

    int len = PyList_Size(input_list);
    int* arr = malloc(sizeof(int) * len);
    if(!arr) return PyErr_NoMemory();

    for (int i = 0; i < len; i++) {
        PyObject* item = PyList_GetItem(input_list, i);

        long val = PyLong_AsLong(item);
        if (PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError, "List items must be integers");
            free(arr);
            return NULL;
        }
        arr[i] = (int) val;
    }

    MinHeap* heap = min_heap_heapify(arr, len);
    free(arr);

    PyMinHeapObject* obj = PyObject_New(PyMinHeapObject, (PyTypeObject*)cls);
    if(!obj) {
        min_heap_free(heap);
        return PyErr_NoMemory();
    }

    obj->min_heap = heap;
    return (PyObject*)obj;
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

static PyObject* PyMinHeap_pushpop(PyMinHeapObject* self, PyObject* args) {
    int value;
    int removed;

    if(!PyArg_ParseTuple(args, "i", &value)) return NULL;
    if (!min_heap_pushpop(self->min_heap, value, &removed)) {
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(removed);
}

static PyObject* PyMinHeap_peek(PyMinHeapObject* self, PyObject* args) {
    int peek;

    if (!min_heap_peek(self->min_heap, &peek)) {
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(peek);
}

static Py_ssize_t PyMinHeap_length(PyMinHeapObject* self) {
    return (Py_ssize_t)min_heap_length(self->min_heap);
}

static PyMethodDef PyMinHeap_methods[] = {
    {"heapify", (PyCFunction)PyMinHeap_cls_heapify, METH_VARARGS | METH_CLASS, "Receives an integer array and returns a minimum heap."},
    {"insert", (PyCFunction)PyMinHeap_insert, METH_VARARGS, "Add a value to the minimum heap."},
    {"remove", (PyCFunction)PyMinHeap_remove, METH_NOARGS, "Remove the minimum value from the minimum heap."},
    {"peek", (PyCFunction)PyMinHeap_peek, METH_NOARGS, "Peek the minimum value from the minimum heap."},
    {"pushpop", (PyCFunction)PyMinHeap_pushpop, METH_VARARGS, "Insert a value into the minimum heap and remove the current minimum in a single operation."},
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
    "- heapify(array: list[int]) - Receives an array and returns a minimum heap.\n"
    "- insert(value: int) - Add a value to the minimum heap.\n"
    "- remove() -> Optional[int] - Remove the minimum value from the minimum heap.\n"
    "- peek() -> Optional[int] - Peek the minimum value from the minimum heap.\n"
    "- pushpop(value: int) -> int - Insert a value into the minimum heap and remove the current minimum in a single operation.\n",
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

