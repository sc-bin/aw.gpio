#include <Python.h>

#include "common.h"
static PyObject *py_check(PyObject *self, PyObject *args)
{
    printf("输出\r\n");
    Py_RETURN_NONE;
}



static const char moduledocstring[] = "GPIO functionality of allwinner chips using Python";

PyMethodDef h616_methods[] = {
    {"go", py_check, METH_VARARGS, "check"},
    {NULL, NULL, 0, NULL},
};
static struct PyModuleDef awgpiomodule = {
    PyModuleDef_HEAD_INIT,
    "aw._gpio",      // name of module
    moduledocstring, // module documentation, may be NULL
    -1,              // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    h616_methods
};


// 在import时会被调出来执行
PyMODINIT_FUNC PyInit__gpio(void)
{
    printf("PyInit__GPIO\r\n");
    PyObject *module = NULL;

    if ((module = PyModule_Create(&awgpiomodule)) == NULL)
        return NULL;
    define_commons(module);

    PyModule_AddObject(module, "num", Py_BuildValue("i", 666));   // deprecated
    return module;
}