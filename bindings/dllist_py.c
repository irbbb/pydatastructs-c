// bindings/dllist_py.c

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../src/dllist.h"

typedef struct {
    PyObject_HEAD;
    DoublyLinkedList* list;
} PyDLLObject;

static void PyDLL_dealloc(PyDLLObject* self) {
    dll_free(self->list);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyDLL_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    PyDLLObject* self;
    self = (PyDLLObject*)type->tp_alloc(type, 0);
    self->list = dll_create();

    return (PyObject*)self;
}

static PyObject* PyDLL_append(PyDLLObject* self, PyObject* args) {
    int value;

    if (!PyArg_ParseTuple(args, "i", &value)) return NULL;
    dll_append((DoublyLinkedList*)self->list, value);

    Py_RETURN_NONE;
}

static PyObject* PyDLL_prepend(PyDLLObject* self, PyObject* args) {
    int value;

    if (!PyArg_ParseTuple(args, "i", &value)) return NULL;
    dll_prepend((DoublyLinkedList*)self->list, value);

    Py_RETURN_NONE;
}

static PyObject* PyDLL_remove(PyDLLObject* self, PyObject* args) {
    int value;

    if (!PyArg_ParseTuple(args, "i", &value)) return NULL;

    int removed;
    if (!dll_remove((DoublyLinkedList*)self->list, value, &removed))
        Py_RETURN_NONE;

    return PyLong_FromLong(removed);
}

static PyObject* PyDLL_pop(PyDLLObject* self, PyObject* args) {
    int index;

    if (!PyArg_ParseTuple(args, "i", &index)) return NULL;

    int removed;
    if (!dll_pop((DoublyLinkedList*)self->list, index, &removed))
        Py_RETURN_NONE;

    return PyLong_FromLong(removed);
}

static PyObject* PyDLL_to_list(PyDLLObject* self, PyObject* Py_UNUSED(ignored)) {
    int size = dll_length((DoublyLinkedList*)self->list);
    int* values = dll_to_array((DoublyLinkedList*)self->list);
    PyObject* pylist = PyList_New(size);

    for (int i = 0; i < size; i++) {
        PyList_SetItem(pylist, i, PyLong_FromLong(values[i]));
    }

    return pylist;
}

static Py_ssize_t PyDLL_length(PyDLLObject* self) {
    return (Py_ssize_t)dll_length((DoublyLinkedList*)self->list);
}

static PyMethodDef PyDLL_methods[] = {
    {"append", (PyCFunction)PyDLL_append, METH_VARARGS, "Add a value at the end."},
    {"prepend", (PyCFunction)PyDLL_prepend, METH_VARARGS, "Add a value at the beginning."},
    {"pop", (PyCFunction)PyDLL_pop, METH_VARARGS, "Remove a node by index. Returns the value removed."},
    {"remove", (PyCFunction)PyDLL_remove, METH_VARARGS, "Remove a node by value. Returns the value removed."},
    {"to_list", (PyCFunction)PyDLL_to_list, METH_NOARGS, "Convert to Python list."},
    {NULL}
};

static PySequenceMethods PYDLL_sequence_methods = {
    .sq_length = (lenfunc)PyDLL_length,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static PyTypeObject PyDLLType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dllist.DoublyLinkedList",
    .tp_basicsize = sizeof(PyDLLObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Doubly Linked List\n"
    "\n"
    "- append(value: int) - Add a value at the end.\n"
    "- prepend(value: int) - Add a value at the beginning.\n"
    "- remove(value: int) -> Optional[int] - Remove a node by value. Returns the value removed.\n"
    "- pop(index: int) -> Optional[int] - Remove a node by index. Returns the value removed.\n"
    "- to_list() -> list[int] - Convert to Python list.\n"
    "- len(list) -> int - Get number of items.\n",
    .tp_methods = PyDLL_methods,
    .tp_new = PyDLL_new,
    .tp_dealloc = (destructor)PyDLL_dealloc,
    .tp_as_sequence = &PYDLL_sequence_methods,
};

static PyModuleDef dllist_module = {
    PyModuleDef_HEAD_INIT,
    "dllist",
    "This module exposes a high-performance, memory-efficient doubly linked list "
    "implementation written in C to Python via the C API"
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_dllist(void) {
    PyObject* m;
    if (PyType_Ready(&PyDLLType) < 0) return NULL;

    m = PyModule_Create(&dllist_module);
    if (!m) return NULL;

    Py_INCREF(&PyDLLType);
    PyModule_AddObject(m, "DoublyLinkedList", (PyObject*)&PyDLLType);
    return m;
}