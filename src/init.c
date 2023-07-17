#include <Python.h>

static const char moduledocstring[] = "GPIO functionality of allwinner chips using Python";

PyMethodDef aw_gpio_methods[] = {
    {NULL, NULL, 0, NULL},
};
static struct PyModuleDef rpigpiomodule = {
    PyModuleDef_HEAD_INIT,
    "aw._gpio",      // name of module
    moduledocstring, // module documentation, may be NULL
    -1,              // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    aw_gpio_methods};

// 在import时会被调出来执行
PyMODINIT_FUNC PyInit__gpio(void)
{
    printf("PyInit__GPIO\r\n");
    PyObject *module = NULL;

    if ((module = PyModule_Create(&rpigpiomodule)) == NULL)
        return NULL;

    return module;
}