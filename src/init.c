#include <Python.h>

#include "h616_py.h"


static PyObject *py_check(PyObject *self, PyObject *args)
{
    printf("输出\r\n");
    Py_RETURN_NONE;
}



static const char moduledocstring[] = "GPIO functionality of allwinner chips using Python";

PyMethodDef aw_gpio_methods[] = {
    {"go", py_check, METH_VARARGS, "check"},
    {NULL, NULL, 0, NULL},
};
static struct PyModuleDef awgpiomodule = {
    PyModuleDef_HEAD_INIT,
    "aw._gpio",      // name of module
    moduledocstring, // module documentation, may be NULL
    -1,              // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    aw_gpio_methods
};


// 在import时会被调出来执行
PyMODINIT_FUNC PyInit__gpio(void)
{
    printf("PyInit__GPIO\r\n");
    PyObject *module = NULL;

    if ((module = PyModule_Create(&awgpiomodule)) == NULL)
        return NULL;
    // PyModule_AddObject(module, "PWM", (PyObject*)&PWMType);
    PyObject *obj_p;

    obj_p = h616_get_pobj_oper();
    Py_INCREF(obj_p);
    PyModule_AddObject(module, "h616_gpio", obj_p);

    return module;
}