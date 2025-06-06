// bindings/linked_list_py.c

#define PY_SSYZE_T_CLEAN
#include <Python.h>
#include "../src/linked_list.h"

typedef struct {
    PyObject_HEAD;
    LinkedList* ll;
} PyLinkedListObject;

static void PyLinkedList_dealloc(PyLinkedListObject* self) {
    linked_list_free(self->ll);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* PyLinkedList_new(PyTypeObject* type, PyObject* args, PyObject* kwargs) {
    PyLinkedListObject* self;
    self = (PyLinkedListObject*)type->tp_alloc(type, 0);
    self->ll = linked_list_create();

    return (PyObject*)self;
}

static PyObject* PyLinkedList_prepend(PyLinkedListObject* self, PyObject* args) {
    int value;

    if(!PyArg_ParseTuple(args, "i", &value)) return NULL;
    linked_list_prepend(self->ll, value);

    Py_RETURN_NONE;
}

static PyObject* PyLinkedList_pop(PyLinkedListObject* self) {
    int removed;

    if(!linked_list_pop(self->ll, &removed)) {
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(removed);
}

static PyObject* PyLinkedList_peek(PyLinkedListObject* self) {
    int peeked;

    if(!linked_list_peek(self->ll, &peeked)) {
        Py_RETURN_NONE;
    }

    return PyLong_FromLong(peeked);
}


static Py_ssize_t PyLinkedList_length(PyLinkedListObject* self) {
    return (Py_ssize_t)linked_list_length(self->ll);
}

static PyMethodDef PyLinkedList_methods[] = {
    {"prepend", (PyCFunction)PyLinkedList_prepend, METH_VARARGS, "Add a value at the beginning."},
    {"pop", (PyCFunction)PyLinkedList_pop, METH_VARARGS, "Remove the first element. Returns the value removed. If there is no element, return None."},
    {"peek", (PyCFunction)PyLinkedList_peek, METH_VARARGS, "Peek the first element. If there is no element, return None."},
    {NULL}
};

static PySequenceMethods PyLinkedList_sequence_methods = {
    .sq_length = (lenfunc)PyLinkedList_length,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

static PyTypeObject PyLinkedListType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "linked_list.LinkedList",
    .tp_basicsize = sizeof(PyLinkedListObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Linked List\n"
    "\n"
    "- prepend(value: int) - Add a value at the beginning."
    "- pop() -> Optional[int] - Remove the first element. Returns the value removed. If there is no element, return None."
    "- peek() -> Optional[int] - Peek the first element. If there is no element, return None.",
    .tp_methods = PyLinkedList_methods,
    .tp_new = PyLinkedList_new,
    .tp_dealloc = (destructor)PyLinkedList_dealloc,
    .tp_as_sequence = &PyLinkedList_sequence_methods,
};

static PyModuleDef linked_list_module = {
    PyModuleDef_HEAD_INIT,
    "linked_list",
    "This module exposes a high-performance, memory-efficient linked list "
    "implementation written in C to Python via the C API",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_linked_list(void) {
    PyObject *m;

    if (PyType_Ready(&PyLinkedListType) < 0) return NULL;

    m = PyModule_Create(&linked_list_module);
    if (!m) return NULL;

    Py_INCREF(&PyLinkedListType);
    PyModule_AddObject(m, "LinkedList", (PyObject*)&PyLinkedListType);
    return m;
}