#ifndef _COMMON_H
#define _COMMON_H
#include <Python.h>


#define	INPUT			 0
#define	OUTPUT			 1

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

extern void define_commons(PyObject *module);

#endif