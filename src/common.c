#include <Python.h>
#include "common.h"

#define	INPUT			 0
#define	OUTPUT			 1

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

void define_commons(PyObject *module)
{

   PyModule_AddObject(module, "INPUT", Py_BuildValue("i", INPUT));
   PyModule_AddObject(module, "OUTPUT", Py_BuildValue("i", OUTPUT));

   PyModule_AddObject(module, "PUD_OFF", Py_BuildValue("i", PUD_OFF));
   PyModule_AddObject(module, "PUD_DOWN", Py_BuildValue("i", PUD_DOWN));
   PyModule_AddObject(module, "PUD_UP", Py_BuildValue("i", PUD_UP));
}