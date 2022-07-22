
#ifndef HOTPY_PyAPI_BOOL_H
#define HOTPY_PyAPI_BOOL_H

extern PyObject* Py_False;
extern PyObject* Py_True;

#define PyBool_Check(ob) ((ob) == Py_False || (ob) == Py_True)
#define PyBool_FromLong(v) ((v) ? Py_True : Py_False)

#endif // HOTPY_PyAPI_SEQUENCE_H

