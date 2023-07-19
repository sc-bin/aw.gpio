#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

static PyObject *py_select(PyObject *self, PyObject *args)
{
    FILE *fp;
    char buffer[1024];
    char *compatible;
    int i;

    fp = fopen("/proc/device-tree/compatible", "r");
    if (fp == NULL)
    {
        printf("Failed to open /proc/device-tree/compatible \n");
        return -1;
    }

    fgets(buffer, 100, fp);
    fclose(fp);

    compatible = strtok(buffer, "\n");

    for (i = 0; i < (int)sizeof(chips_compatible); i++)
    {
        if (strcmp(compatible, chips_compatible[i]) == 0)
        {
            return Py_BuildValue("i", i);
        }
    }
    return Py_BuildValue("i", -1);
}

static const char moduledocstring[] = "GPIO functionality of allwinner chips ";

PyMethodDef methods[] = {
    {"select", py_select, METH_VARARGS, "select now chip by device-tree"},
    {NULL, NULL, 0, NULL},
};
static struct PyModuleDef awgpiomodule = {
    PyModuleDef_HEAD_INIT,
    "aw._gpio",      // name of module
    moduledocstring, // module documentation, may be NULL
    -1,              // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    methods};

// 在import时会被调出来执行
PyMODINIT_FUNC PyInit__gpio(void)
{
    PyObject *module = NULL;

    if ((module = PyModule_Create(&awgpiomodule)) == NULL)
        return NULL;

    return module;
}