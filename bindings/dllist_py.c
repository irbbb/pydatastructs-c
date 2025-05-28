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

static PyObject* PyDLL_to_list(PyDLLObject* self, PyObject* Py_UNUSED(ignored)) {
    int size = dll_length((DoublyLinkedList*)self->list);
    int* values = dll_to_array((DoublyLinkedList*)self->list);
    PyObject* pylist = PyList_New(size);

    for (int i = 0; i < size; i++) {
        PyList_SetItem(pylist, i, PyLong_FromLong(values[i]));
    }

    return pylist;
}

static PyMethodDef PyDLL_methods[] = {
    {"append", (PyCFunction)PyDLL_append, METH_VARARGS, "Append value"},
    {"to_list", (PyCFunction)PyDLL_to_list, METH_NOARGS, "Convert to Python list"},
    {NULL}
};

static PyTypeObject PyDLLType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "dllist.DoublyLinkedList",
    .tp_basicsize = sizeof(PyDLLObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Doubly Linked List",
    .tp_methods = PyDLL_methods,
    .tp_new = PyDLL_new,
    .tp_dealloc = (destructor)PyDLL_dealloc,
};

static PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "dllist",
    "Python bindings for data structures in C",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_dllist(void) {
    PyObject* m;
    if (PyType_Ready(&PyDLLType) < 0) return NULL;

    m = PyModule_Create(&module);
    if (!m) return NULL;

    Py_INCREF(&PyDLLType);
    PyModule_AddObject(m, "DoublyLinkedList", (PyObject*)&PyDLLType);
    return m;
}