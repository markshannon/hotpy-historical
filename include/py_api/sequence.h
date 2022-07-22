
#ifndef HOTPY_PyAPI_SEQUENCE_H
#define HOTPY_PyAPI_SEQUENCE_H

#define PyByteArray_GET_SIZE(ob) Py_SIZE(ob)
#define PyBytes_GET_SIZE(ob) Py_SIZE(ob)
#define PyTuple_GET_SIZE(ob) Py_SIZE(ob)
#define PyUnicode_GET_SIZE(ob) Py_SIZE(ob)
                  
#endif // HOTPY_PyAPI_SEQUENCE_H

