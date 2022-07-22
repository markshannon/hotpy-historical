#ifndef HOTPY_PyAPI_ALLOCATE_H
#define HOTPY_PyAPI_ALLOCATE_H

#include "hotpy_types.h"

#define PyObject_NewVar(TYPE, type, size) ((TYPE*)_PyObject_NewVar(type, size))

#define PyObject_New(TYPE, type) ((TYPE*)_PyObject_New(type))

#endif // HOTPY_PyAPI_OBJECT_H

