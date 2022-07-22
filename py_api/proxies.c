/** This file is auto-generated */
#include "hotpy.h"
#include "operators.h"


/* /home/mark/Python-3.2b2/Doc/c-api/allocation.rst */
R_object _hotpy_proxy_PyObject_Init(R_object op, R_type type) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_Init(op, type);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_InitVar(R_object op, R_type type, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_InitVar(op, type, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/arg.rst */
R_object _hotpy_proxy_Py_BuildValue(const char* format, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPy_BuildValue(format, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_Py_VaBuildValue(const char* format, va_list  vargs) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPy_VaBuildValue(format, vargs);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytearray.rst */
R_object _hotpy_proxy_PyByteArray_FromObject(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyByteArray_FromObject(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyByteArray_FromStringAndSize(const char* string, Py_ssize_t  len) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyByteArray_FromStringAndSize(string, len);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyByteArray_Concat(R_object a, R_object b) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyByteArray_Concat(a, b);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytes.rst */
R_object _hotpy_proxy_PyBytes_FromString(const char* v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyBytes_FromString(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyBytes_FromStringAndSize(const char* v, Py_ssize_t  len) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyBytes_FromStringAndSize(v, len);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyBytes_FromFormat(const char* format, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyBytes_FromFormat(format, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyBytes_FromFormatV(const char* format, va_list  vargs) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyBytes_FromFormatV(format, vargs);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyBytes_FromObject(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyBytes_FromObject(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/capsule.rst */
R_object _hotpy_proxy_PyCapsule_New(void* pointer, const char* name, PyCapsule_Destructor  destructor) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyCapsule_New(pointer, name, destructor);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/complex.rst */
R_object _hotpy_proxy_PyComplex_FromDoubles(double  real, double  imag) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyComplex_FromDoubles(real, imag);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/dict.rst */
R_object _hotpy_proxy_PyDictProxy_New(R_object dict) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDictProxy_New(dict);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_Copy(R_object p) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_Copy(p);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_GetItem(R_object p, R_object key) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_GetItem(p, key);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_GetItemWithError(R_object p, R_object key) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_GetItemWithError(p, key);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_GetItemString(R_object p, const char* key) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_GetItemString(p, key);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_Items(R_object p) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_Items(p);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_Keys(R_object p) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_Keys(p);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyDict_Values(R_object p) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyDict_Values(p);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/exceptions.rst */
R_object _hotpy_proxy_PyErr_SetFromErrno(R_object type) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetFromErrno(type);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_SetFromErrnoWithFilename(R_object type, const char* filename) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetFromErrnoWithFilename(type, filename);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_SetFromWindowsErr(int  ierr) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetFromWindowsErr(ierr);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_SetExcFromWindowsErr(R_object type, int  ierr) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetExcFromWindowsErr(type, ierr);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_SetFromWindowsErrWithFilename(int  ierr, const char* filename) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetFromWindowsErrWithFilename(ierr, filename);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_SetExcFromWindowsErrWithFilename(R_object type, int  ierr, char* filename) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_SetExcFromWindowsErrWithFilename(type, ierr, filename);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_NewException(char* name, R_object base, R_object dict) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_NewException(name, base, dict);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyErr_NewExceptionWithDoc(char* name, char* doc, R_object base, R_object dict) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyErr_NewExceptionWithDoc(name, doc, base, dict);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyException_GetTraceback(R_object ex) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyException_GetTraceback(ex);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyException_GetContext(R_object ex) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyException_GetContext(ex);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyException_GetCause(R_object ex) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyException_GetCause(ex);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicodeEncodeError_Create(const char* encoding, const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicodeEncodeError_Create(encoding, object, length, start, end, reason);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicodeTranslateError_Create(const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicodeTranslateError_Create(object, length, start, end, reason);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/file.rst */
R_object _hotpy_proxy_PyFile_GetLine(R_object p, int  n) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyFile_GetLine(p, n);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/float.rst */
R_object _hotpy_proxy_PyFloat_FromString(R_object str) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyFloat_FromString(str);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyFloat_FromDouble(double  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyFloat_FromDouble(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/gen.rst */
R_object _hotpy_proxy_PyGen_New(R_frame frame) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyGen_New(frame);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/import.rst */
R_object _hotpy_proxy_PyImport_ImportModule(const char* name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ImportModule(name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ImportModuleNoBlock(const char* name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ImportModuleNoBlock(name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ImportModuleEx(char* name, R_object globals, R_object locals, R_object fromlist) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ImportModuleEx(name, globals, locals, fromlist);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ImportModuleLevel(char* name, R_object globals, R_object locals, R_object fromlist, int  level) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ImportModuleLevel(name, globals, locals, fromlist, level);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_Import(R_object name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_Import(name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ReloadModule(R_object m) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ReloadModule(m);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_AddModule(const char* name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_AddModule(name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ExecCodeModule(char* name, R_object co) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ExecCodeModule(name, co);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ExecCodeModuleEx(char* name, R_object co, char* pathname) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ExecCodeModuleEx(name, co, pathname);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_ExecCodeModuleWithPathnames(char* name, R_object co, char* pathname, char* cpathname) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_ExecCodeModuleWithPathnames(name, co, pathname, cpathname);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyImport_GetImporter(R_object path) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyImport_GetImporter(path);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/init.rst */
R_object _hotpy_proxy_PyEval_GetCallStats(R_object self) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyEval_GetCallStats(self);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/iterator.rst */
R_object _hotpy_proxy_PySeqIter_New(R_object seq) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySeqIter_New(seq);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyCallIter_New(R_object callable, R_object sentinel) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyCallIter_New(callable, sentinel);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/iter.rst */
R_object _hotpy_proxy_PyIter_Next(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyIter_Next(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/list.rst */
R_object _hotpy_proxy_PyList_New(Py_ssize_t  len) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyList_New(len);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyList_GetItem(R_object list, Py_ssize_t  index) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyList_GetItem(list, index);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyList_GET_ITEM(R_object list, Py_ssize_t  i) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyList_GET_ITEM(list, i);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyList_GetSlice(R_object list, Py_ssize_t  low, Py_ssize_t  high) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyList_GetSlice(list, low, high);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyList_AsTuple(R_object list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyList_AsTuple(list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/long.rst */
R_object _hotpy_proxy_PyLong_FromLong(long  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromLong(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromUnsignedLong(unsigned long p0) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromUnsignedLong(p0);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromSsize_t(Py_ssize_t  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromSsize_t(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromSize_t(size_t  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromSize_t(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromLongLong(PY_LONG_LONG  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromLongLong(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromUnsignedLongLong(unsigned  PY_LONG_LONG) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromUnsignedLongLong(PY_LONG_LONG);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromDouble(double  v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromDouble(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromString(char* str, char** pend, int  base) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromString(str, pend, base);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromUnicode(Py_UNICODE* u, Py_ssize_t  length, int  base) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromUnicode(u, length, base);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyLong_FromVoidPtr(void* p) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyLong_FromVoidPtr(p);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/mapping.rst */
R_object _hotpy_proxy_PyMapping_Keys(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMapping_Keys(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMapping_Values(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMapping_Values(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMapping_Items(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMapping_Items(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMapping_GetItemString(R_object o, char* key) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMapping_GetItemString(o, key);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/marshal.rst */
R_object _hotpy_proxy_PyMarshal_WriteObjectToString(R_object value, int  version) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMarshal_WriteObjectToString(value, version);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMarshal_ReadObjectFromFile(FILE* file) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMarshal_ReadObjectFromFile(file);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMarshal_ReadLastObjectFromFile(FILE* file) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMarshal_ReadLastObjectFromFile(file);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMarshal_ReadObjectFromString(char* string, Py_ssize_t  len) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMarshal_ReadObjectFromString(string, len);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/method.rst */
R_object _hotpy_proxy_PyInstanceMethod_New(R_object func) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyInstanceMethod_New(func);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyInstanceMethod_Function(R_object im) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyInstanceMethod_Function(im);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyInstanceMethod_GET_FUNCTION(R_object im) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyInstanceMethod_GET_FUNCTION(im);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMethod_New(R_object func, R_object self) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMethod_New(func, self);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMethod_Function(R_object meth) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMethod_Function(meth);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMethod_GET_FUNCTION(R_object meth) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMethod_GET_FUNCTION(meth);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMethod_Self(R_object meth) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMethod_Self(meth);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyMethod_GET_SELF(R_object meth) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyMethod_GET_SELF(meth);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/module.rst */
R_object _hotpy_proxy_PyModule_New(const char* name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyModule_New(name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyModule_GetDict(R_object module) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyModule_GetDict(module);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyModule_GetFilenameObject(R_object module) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyModule_GetFilenameObject(module);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyModule_Create(PyModuleDef* module) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyModule_Create(module);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyModule_Create2(PyModuleDef* module, int  module_api_version) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyModule_Create2(module, module_api_version);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/number.rst */
R_object _hotpy_proxy_PyNumber_Add(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Add(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Subtract(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Subtract(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Multiply(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Multiply(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_FloorDivide(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_FloorDivide(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_TrueDivide(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_TrueDivide(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Remainder(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Remainder(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Divmod(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Divmod(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Power(R_object o1, R_object o2, R_object o3) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Power(o1, o2, o3);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Negative(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Negative(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Positive(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Positive(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Absolute(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Absolute(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Invert(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Invert(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Lshift(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Lshift(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Rshift(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Rshift(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_And(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_And(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Xor(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Xor(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Or(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Or(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceAdd(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceAdd(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceSubtract(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceSubtract(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceMultiply(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceMultiply(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceFloorDivide(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceFloorDivide(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceTrueDivide(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceTrueDivide(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceRemainder(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceRemainder(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlacePower(R_object o1, R_object o2, R_object o3) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlacePower(o1, o2, o3);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceLshift(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceLshift(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceRshift(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceRshift(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceAnd(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceAnd(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceXor(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceXor(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_InPlaceOr(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_InPlaceOr(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Long(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Long(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Float(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Float(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_Index(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_Index(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyNumber_ToBase(R_object n, int  base) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyNumber_ToBase(n, base);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/object.rst */
R_object _hotpy_proxy_PyObject_GetAttr(R_object o, R_object attr_name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_GetAttr(o, attr_name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_GetAttrString(R_object o, const char* attr_name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_GetAttrString(o, attr_name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_GenericGetAttr(R_object o, R_object name) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_GenericGetAttr(o, name);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_RichCompare(R_object o1, R_object o2, int  opid) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_RichCompare(o1, o2, opid);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_Repr(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_Repr(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_Str(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_Str(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_Call(R_object callable_object, R_object args, R_object kw) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_Call(callable_object, args, kw);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_CallObject(R_object callable_object, R_object args) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_CallObject(callable_object, args);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_CallFunction(R_object callable, char* format, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_CallFunction(callable, format, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_CallMethod(R_object o, char* method, char* format, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_CallMethod(o, method, format, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_CallFunctionObjArgs(R_object callable, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_CallFunctionObjArgs(callable, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_CallMethodObjArgs(R_object o, R_object name, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_CallMethodObjArgs(o, name, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_GetItem(R_object o, R_object key) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_GetItem(o, key);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyObject_GetIter(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyObject_GetIter(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/sequence.rst */
R_object _hotpy_proxy_PySequence_Concat(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_Concat(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_Repeat(R_object o, Py_ssize_t  count) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_Repeat(o, count);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_InPlaceConcat(R_object o1, R_object o2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_InPlaceConcat(o1, o2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_InPlaceRepeat(R_object o, Py_ssize_t  count) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_InPlaceRepeat(o, count);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_GetItem(R_object o, Py_ssize_t  i) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_GetItem(o, i);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_GetSlice(R_object o, Py_ssize_t  i1, Py_ssize_t  i2) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_GetSlice(o, i1, i2);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_List(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_List(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_Tuple(R_object o) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_Tuple(o);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_Fast(R_object o, const char* m) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_Fast(o, m);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySequence_ITEM(R_object o, Py_ssize_t  i) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySequence_ITEM(o, i);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/set.rst */
R_object _hotpy_proxy_PySet_New(R_object iterable) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySet_New(iterable);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyFrozenSet_New(R_object iterable) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyFrozenSet_New(iterable);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PySet_Pop(R_object set) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySet_Pop(set);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/slice.rst */
R_object _hotpy_proxy_PySlice_New(R_object start, R_object stop, R_object step) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPySlice_New(start, stop, step);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/sys.rst */


/* /home/mark/Python-3.2b2/Doc/c-api/tuple.rst */
R_object _hotpy_proxy_PyTuple_New(Py_ssize_t  len) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyTuple_New(len);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyTuple_Pack(Py_ssize_t  n, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyTuple_Pack(n, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyTuple_GetItem(R_object p, Py_ssize_t  pos) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyTuple_GetItem(p, pos);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyTuple_GET_ITEM(R_object p, Py_ssize_t  pos) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyTuple_GET_ITEM(p, pos);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyTuple_GetSlice(R_object p, Py_ssize_t  low, Py_ssize_t  high) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyTuple_GetSlice(p, low, high);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/type.rst */
R_object _hotpy_proxy_PyType_GenericAlloc(R_type type, Py_ssize_t  nitems) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyType_GenericAlloc(type, nitems);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyType_GenericNew(R_type type, R_object args, R_object kwds) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyType_GenericNew(type, args, kwds);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/unicode.rst */
R_object _hotpy_proxy_PyUnicode_FromUnicode(const Py_UNICODE* u, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromUnicode(u, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromStringAndSize(const char* u, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromStringAndSize(u, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromFormat(const char* format, va_list  _ap_list) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromFormat(format, _ap_list);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromFormatV(const char* format, va_list  vargs) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromFormatV(format, vargs);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_TransformDecimalToASCII(Py_UNICODE* s, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_TransformDecimalToASCII(s, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromEncodedObject(R_object obj, const char* encoding, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromEncodedObject(obj, encoding, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromObject(R_object obj) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromObject(obj);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeFSDefault(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeFSDefault(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_FromWideChar(const wchar_t* w, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_FromWideChar(w, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Encode(const Py_UNICODE* s, Py_ssize_t  size, const char* encoding, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Encode(s, size, encoding, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsEncodedString(R_object unicode, const char* encoding, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsEncodedString(unicode, encoding, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeUTF8(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeUTF8(s, size, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsUTF8String(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsUTF8String(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeUTF32(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeUTF32(s, size, errors, byteorder);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsUTF32String(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsUTF32String(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeUTF16(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeUTF16(s, size, errors, byteorder);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsUTF16String(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsUTF16String(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeUTF7(const Py_UNICODE* s, Py_ssize_t  size, int  base64SetO, int  base64WhiteSpace, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeUTF7(s, size, base64SetO, base64WhiteSpace, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeUnicodeEscape(s, size);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsUnicodeEscapeString(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsUnicodeEscapeString(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeRawUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeRawUnicodeEscape(s, size, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsRawUnicodeEscapeString(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsRawUnicodeEscapeString(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeLatin1(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeLatin1(s, size, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsLatin1String(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsLatin1String(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeASCII(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeASCII(s, size, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsASCIIString(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsASCIIString(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeCharmap(const Py_UNICODE* s, Py_ssize_t  size, R_object mapping, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeCharmap(s, size, mapping, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsCharmapString(R_object unicode, R_object mapping) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsCharmapString(unicode, mapping);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_TranslateCharmap(const Py_UNICODE* s, Py_ssize_t  size, R_object table, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_TranslateCharmap(s, size, table, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_EncodeMBCS(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_EncodeMBCS(s, size, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_AsMBCSString(R_object unicode) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_AsMBCSString(unicode);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Concat(R_object left, R_object right) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Concat(left, right);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Split(R_object s, R_object sep, Py_ssize_t  maxsplit) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Split(s, sep, maxsplit);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Splitlines(R_object s, int  keepend) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Splitlines(s, keepend);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Translate(R_object str, R_object table, const char* errors) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Translate(str, table, errors);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Join(R_object separator, R_object seq) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Join(separator, seq);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Replace(R_object str, R_object substr, R_object replstr, Py_ssize_t  maxcount) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Replace(str, substr, replstr, maxcount);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_Format(R_object format, R_object args) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_Format(format, args);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyUnicode_InternFromString(const char* v) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyUnicode_InternFromString(v);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/veryhigh.rst */
R_object _hotpy_proxy_PyRun_String(const char* str, int  start, R_object globals, R_object locals) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_String(str, start, globals, locals);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyRun_StringFlags(const char* str, int  start, R_object globals, R_object locals, PyCompilerFlags* flags) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_StringFlags(str, start, globals, locals, flags);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyRun_File(FILE* fp, const char* filename, int  start, R_object globals, R_object locals) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_File(fp, filename, start, globals, locals);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyRun_FileEx(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, int  closeit) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_FileEx(fp, filename, start, globals, locals, closeit);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyRun_FileFlags(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, PyCompilerFlags* flags) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_FileFlags(fp, filename, start, globals, locals, flags);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyRun_FileExFlags(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, int  closeit, PyCompilerFlags* flags) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyRun_FileExFlags(fp, filename, start, globals, locals, closeit, flags);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_Py_CompileString(const char* str, const char* filename, int  start) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPy_CompileString(str, filename, start);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_Py_CompileStringFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPy_CompileStringFlags(str, filename, start, flags);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_Py_CompileStringExFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags, int  optimize) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPy_CompileStringExFlags(str, filename, start, flags, optimize);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyEval_EvalCode(R_object co, R_object globals, R_object locals) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyEval_EvalCode(co, globals, locals);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyEval_EvalCodeEx(R_object co, R_object globals, R_object locals, PyObject** args, int  argcount, PyObject** kws, int  kwcount, PyObject** defs, int  defcount, R_object closure) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyEval_EvalCodeEx(co, globals, locals, args, argcount, kws, kwcount, defs, defcount, closure);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyEval_EvalFrame(R_frame f) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyEval_EvalFrame(f);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyEval_EvalFrameEx(R_frame f, int  throwflag) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyEval_EvalFrameEx(f, throwflag);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}



/* /home/mark/Python-3.2b2/Doc/c-api/weakref.rst */
R_object _hotpy_proxy_PyWeakref_NewRef(R_object ob, R_object callback) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyWeakref_NewRef(ob, callback);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyWeakref_NewProxy(R_object ob, R_object callback) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyWeakref_NewProxy(ob, callback);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyWeakref_GetObject(R_object ref) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyWeakref_GetObject(ref);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

R_object _hotpy_proxy_PyWeakref_GET_OBJECT(R_object ref) {
    R_object result;
    PyObject* pinned;
    result = (R_object)HotPyWeakref_GET_OBJECT(ref);
    if (!gvmt_is_tagged((GVMT_Object)result)) {
        PyObject* pinned = (PyObject*)gvmt_pin((GVMT_Object)result);
        Py_INCREF(pinned);
    }
    return result;
}

