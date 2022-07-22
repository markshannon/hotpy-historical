/** This file is auto-generated */

#include "py_api/Python.h"
#include "gvmt/native.h"


/* /home/mark/Python-3.2b2/Doc/c-api/allocation.rst */
extern PyObject* _hotpy_proxy_PyObject_Init(void);

PyObject* PyObject_Init(PyObject* op, PyTypeObject* type) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_Init, 2, op, type).p;
}

extern PyVarObject* _hotpy_proxy_PyObject_InitVar(void);

PyVarObject* PyObject_InitVar(PyVarObject* op, PyTypeObject* type, Py_ssize_t  size) {
    return (PyVarObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_InitVar, 3, op, type, size).p;
}

extern void HotPyObject_Del(void);
void PyObject_Del(PyObject* op) {
    gvmt_call((gvmt_func_ptr)HotPyObject_Del, 1, op);
}



/* /home/mark/Python-3.2b2/Doc/c-api/arg.rst */
extern int HotPyArg_ParseTuple(void);
int PyArg_ParseTuple(PyObject* args, const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    int _result = (int)gvmt_call((gvmt_func_ptr)HotPyArg_ParseTuple, 3, args, format, _ap_list).i;
    va_end(_ap_list);
    return _result;
}

extern int HotPyArg_VaParse(void);
int PyArg_VaParse(PyObject* args, const char* format, va_list  vargs) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyArg_VaParse, 3, args, format, vargs).i;
}

extern int HotPyArg_ParseTupleAndKeywords(void);
int PyArg_ParseTupleAndKeywords(PyObject* args, PyObject* kw, const char* format, char* keywords, ... ) {
    va_list _ap_list;
    va_start(_ap_list, keywords);
    int _result = (int)gvmt_call((gvmt_func_ptr)HotPyArg_ParseTupleAndKeywords, 5, args, kw, format, keywords, _ap_list).i;
    va_end(_ap_list);
    return _result;
}

extern int HotPyArg_VaParseTupleAndKeywords(void);
int PyArg_VaParseTupleAndKeywords(PyObject* args, PyObject* kw, const char* format, char* keywords, va_list  vargs) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyArg_VaParseTupleAndKeywords, 5, args, kw, format, keywords, vargs).i;
}

extern int HotPyArg_ValidateKeywordArguments(void);
int PyArg_ValidateKeywordArguments(PyObject* p0) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyArg_ValidateKeywordArguments, 1, p0).i;
}

extern int HotPyArg_Parse(void);
int PyArg_Parse(PyObject* args, const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    int _result = (int)gvmt_call((gvmt_func_ptr)HotPyArg_Parse, 3, args, format, _ap_list).i;
    va_end(_ap_list);
    return _result;
}

extern int HotPyArg_UnpackTuple(void);
int PyArg_UnpackTuple(PyObject* args, const char* name, Py_ssize_t  min, Py_ssize_t  max, ... ) {
    va_list _ap_list;
    va_start(_ap_list, max);
    int _result = (int)gvmt_call((gvmt_func_ptr)HotPyArg_UnpackTuple, 5, args, name, min, max, _ap_list).i;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_Py_BuildValue(void);

PyObject* Py_BuildValue(const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_Py_BuildValue, 2, format, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_Py_VaBuildValue(void);

PyObject* Py_VaBuildValue(const char* format, va_list  vargs) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_Py_VaBuildValue, 2, format, vargs).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytearray.rst */
extern int HotPyByteArray_Check(void);
int PyByteArray_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyByteArray_Check, 1, o).i;
}

extern int HotPyByteArray_CheckExact(void);
int PyByteArray_CheckExact(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyByteArray_CheckExact, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyByteArray_FromObject(void);

PyObject* PyByteArray_FromObject(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyByteArray_FromObject, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyByteArray_FromStringAndSize(void);

PyObject* PyByteArray_FromStringAndSize(const char* string, Py_ssize_t  len) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyByteArray_FromStringAndSize, 2, string, len).p;
}

extern PyObject* _hotpy_proxy_PyByteArray_Concat(void);

PyObject* PyByteArray_Concat(PyObject* a, PyObject* b) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyByteArray_Concat, 2, a, b).p;
}

extern Py_ssize_t HotPyByteArray_Size(void);
Py_ssize_t PyByteArray_Size(PyObject* bytearray) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyByteArray_Size, 1, bytearray).j;
}

extern char* HotPyByteArray_AsString(void);
char* PyByteArray_AsString(PyObject* bytearray) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyByteArray_AsString, 1, bytearray).p;
}

extern int HotPyByteArray_Resize(void);
int PyByteArray_Resize(PyObject* bytearray, Py_ssize_t  len) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyByteArray_Resize, 2, bytearray, len).i;
}

extern char* HotPyByteArray_AS_STRING(void);
char* PyByteArray_AS_STRING(PyObject* bytearray) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyByteArray_AS_STRING, 1, bytearray).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytes.rst */
extern int HotPyBytes_Check(void);
int PyBytes_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyBytes_Check, 1, o).i;
}

extern int HotPyBytes_CheckExact(void);
int PyBytes_CheckExact(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyBytes_CheckExact, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyBytes_FromString(void);

PyObject* PyBytes_FromString(const char* v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyBytes_FromString, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyBytes_FromStringAndSize(void);

PyObject* PyBytes_FromStringAndSize(const char* v, Py_ssize_t  len) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyBytes_FromStringAndSize, 2, v, len).p;
}

extern PyObject* _hotpy_proxy_PyBytes_FromFormat(void);

PyObject* PyBytes_FromFormat(const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyBytes_FromFormat, 2, format, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyBytes_FromFormatV(void);

PyObject* PyBytes_FromFormatV(const char* format, va_list  vargs) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyBytes_FromFormatV, 2, format, vargs).p;
}

extern PyObject* _hotpy_proxy_PyBytes_FromObject(void);

PyObject* PyBytes_FromObject(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyBytes_FromObject, 1, o).p;
}

extern Py_ssize_t HotPyBytes_Size(void);
Py_ssize_t PyBytes_Size(PyObject* o) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyBytes_Size, 1, o).j;
}

extern char* HotPyBytes_AsString(void);
char* PyBytes_AsString(PyObject* o) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyBytes_AsString, 1, o).p;
}

extern char* HotPyBytes_AS_STRING(void);
char* PyBytes_AS_STRING(PyObject* string) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyBytes_AS_STRING, 1, string).p;
}

extern int HotPyBytes_AsStringAndSize(void);
int PyBytes_AsStringAndSize(PyObject* obj, char** buffer, Py_ssize_t* length) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyBytes_AsStringAndSize, 3, obj, buffer, length).i;
}

extern void HotPyBytes_Concat(void);
void PyBytes_Concat(PyObject** bytes, PyObject* newpart) {
    gvmt_call((gvmt_func_ptr)HotPyBytes_Concat, 2, bytes, newpart);
}

extern void HotPyBytes_ConcatAndDel(void);
void PyBytes_ConcatAndDel(PyObject** bytes, PyObject* newpart) {
    gvmt_call((gvmt_func_ptr)HotPyBytes_ConcatAndDel, 2, bytes, newpart);
}



/* /home/mark/Python-3.2b2/Doc/c-api/capsule.rst */
extern int HotPyCapsule_CheckExact(void);
int PyCapsule_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyCapsule_New(void);

PyObject* PyCapsule_New(void* pointer, const char* name, PyCapsule_Destructor  destructor) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyCapsule_New, 3, pointer, name, destructor).p;
}

extern void* HotPyCapsule_GetPointer(void);
void* PyCapsule_GetPointer(PyObject* capsule, const char* name) {
    return (void*)gvmt_call((gvmt_func_ptr)HotPyCapsule_GetPointer, 2, capsule, name).p;
}

extern void* HotPyCapsule_GetContext(void);
void* PyCapsule_GetContext(PyObject* capsule) {
    return (void*)gvmt_call((gvmt_func_ptr)HotPyCapsule_GetContext, 1, capsule).p;
}

extern const char* HotPyCapsule_GetName(void);
const char* PyCapsule_GetName(PyObject* capsule) {
    return (const char*)gvmt_call((gvmt_func_ptr)HotPyCapsule_GetName, 1, capsule).p;
}

extern int HotPyCapsule_IsValid(void);
int PyCapsule_IsValid(PyObject* capsule, const char* name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_IsValid, 2, capsule, name).i;
}

extern int HotPyCapsule_SetContext(void);
int PyCapsule_SetContext(PyObject* capsule, void* context) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_SetContext, 2, capsule, context).i;
}

extern int HotPyCapsule_SetDestructor(void);
int PyCapsule_SetDestructor(PyObject* capsule, PyCapsule_Destructor  destructor) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_SetDestructor, 2, capsule, destructor).i;
}

extern int HotPyCapsule_SetName(void);
int PyCapsule_SetName(PyObject* capsule, const char* name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_SetName, 2, capsule, name).i;
}

extern int HotPyCapsule_SetPointer(void);
int PyCapsule_SetPointer(PyObject* capsule, void* pointer) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCapsule_SetPointer, 2, capsule, pointer).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/complex.rst */
extern int HotPyComplex_Check(void);
int PyComplex_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyComplex_Check, 1, p).i;
}

extern int HotPyComplex_CheckExact(void);
int PyComplex_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyComplex_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyComplex_FromDoubles(void);

PyObject* PyComplex_FromDoubles(double  real, double  imag) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyComplex_FromDoubles, 2, real, imag).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/dict.rst */
extern int HotPyDict_Check(void);
int PyDict_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_Check, 1, p).i;
}

extern int HotPyDict_CheckExact(void);
int PyDict_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyDictProxy_New(void);

PyObject* PyDictProxy_New(PyObject* dict) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDictProxy_New, 1, dict).p;
}

extern void HotPyDict_Clear(void);
void PyDict_Clear(PyObject* p) {
    gvmt_call((gvmt_func_ptr)HotPyDict_Clear, 1, p);
}

extern int HotPyDict_Contains(void);
int PyDict_Contains(PyObject* p, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_Contains, 2, p, key).i;
}

extern PyObject* _hotpy_proxy_PyDict_Copy(void);

PyObject* PyDict_Copy(PyObject* p) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_Copy, 1, p).p;
}

extern int HotPyDict_SetItem(void);
int PyDict_SetItem(PyObject* p, PyObject* key, PyObject* val) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_SetItem, 3, p, key, val).i;
}

extern int HotPyDict_SetItemString(void);
int PyDict_SetItemString(PyObject* p, const char* key, PyObject* val) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_SetItemString, 3, p, key, val).i;
}

extern int HotPyDict_DelItem(void);
int PyDict_DelItem(PyObject* p, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_DelItem, 2, p, key).i;
}

extern int HotPyDict_DelItemString(void);
int PyDict_DelItemString(PyObject* p, char* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_DelItemString, 2, p, key).i;
}

extern PyObject* _hotpy_proxy_PyDict_GetItem(void);

PyObject* PyDict_GetItem(PyObject* p, PyObject* key) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_GetItem, 2, p, key).p;
}

extern PyObject* _hotpy_proxy_PyDict_GetItemWithError(void);

PyObject* PyDict_GetItemWithError(PyObject* p, PyObject* key) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_GetItemWithError, 2, p, key).p;
}

extern PyObject* _hotpy_proxy_PyDict_GetItemString(void);

PyObject* PyDict_GetItemString(PyObject* p, const char* key) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_GetItemString, 2, p, key).p;
}

extern PyObject* _hotpy_proxy_PyDict_Items(void);

PyObject* PyDict_Items(PyObject* p) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_Items, 1, p).p;
}

extern PyObject* _hotpy_proxy_PyDict_Keys(void);

PyObject* PyDict_Keys(PyObject* p) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_Keys, 1, p).p;
}

extern PyObject* _hotpy_proxy_PyDict_Values(void);

PyObject* PyDict_Values(PyObject* p) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyDict_Values, 1, p).p;
}

extern Py_ssize_t HotPyDict_Size(void);
Py_ssize_t PyDict_Size(PyObject* p) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyDict_Size, 1, p).j;
}

extern int HotPyDict_Next(void);
int PyDict_Next(PyObject* p, Py_ssize_t* ppos, PyObject** pkey, PyObject** pvalue) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_Next, 4, p, ppos, pkey, pvalue).i;
}

extern int HotPyDict_Merge(void);
int PyDict_Merge(PyObject* a, PyObject* b, int  override) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_Merge, 3, a, b, override).i;
}

extern int HotPyDict_Update(void);
int PyDict_Update(PyObject* a, PyObject* b) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_Update, 2, a, b).i;
}

extern int HotPyDict_MergeFromSeq2(void);
int PyDict_MergeFromSeq2(PyObject* a, PyObject* seq2, int  override) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyDict_MergeFromSeq2, 3, a, seq2, override).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/exceptions.rst */
extern int HotPyErr_ExceptionMatches(void);
int PyErr_ExceptionMatches(PyObject* exc) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyErr_ExceptionMatches, 1, exc).i;
}

extern int HotPyErr_GivenExceptionMatches(void);
int PyErr_GivenExceptionMatches(PyObject* given, PyObject* exc) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyErr_GivenExceptionMatches, 2, given, exc).i;
}

extern void HotPyErr_Restore(void);
void PyErr_Restore(PyObject* type, PyObject* value, PyObject* traceback) {
    gvmt_call((gvmt_func_ptr)HotPyErr_Restore, 3, type, value, traceback);
}

extern void HotPyErr_SetString(void);
void PyErr_SetString(PyObject* type, const char* message) {
    gvmt_call((gvmt_func_ptr)HotPyErr_SetString, 2, type, message);
}

extern void HotPyErr_SetObject(void);
void PyErr_SetObject(PyObject* type, PyObject* value) {
    gvmt_call((gvmt_func_ptr)HotPyErr_SetObject, 2, type, value);
}

extern void HotPyErr_SetNone(void);
void PyErr_SetNone(PyObject* type) {
    gvmt_call((gvmt_func_ptr)HotPyErr_SetNone, 1, type);
}

extern PyObject* _hotpy_proxy_PyErr_SetFromErrno(void);

PyObject* PyErr_SetFromErrno(PyObject* type) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetFromErrno, 1, type).p;
}

extern PyObject* _hotpy_proxy_PyErr_SetFromErrnoWithFilename(void);

PyObject* PyErr_SetFromErrnoWithFilename(PyObject* type, const char* filename) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetFromErrnoWithFilename, 2, type, filename).p;
}

extern PyObject* _hotpy_proxy_PyErr_SetFromWindowsErr(void);

PyObject* PyErr_SetFromWindowsErr(int  ierr) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetFromWindowsErr, 1, ierr).p;
}

extern PyObject* _hotpy_proxy_PyErr_SetExcFromWindowsErr(void);

PyObject* PyErr_SetExcFromWindowsErr(PyObject* type, int  ierr) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetExcFromWindowsErr, 2, type, ierr).p;
}

extern PyObject* _hotpy_proxy_PyErr_SetFromWindowsErrWithFilename(void);

PyObject* PyErr_SetFromWindowsErrWithFilename(int  ierr, const char* filename) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetFromWindowsErrWithFilename, 2, ierr, filename).p;
}

extern PyObject* _hotpy_proxy_PyErr_SetExcFromWindowsErrWithFilename(void);

PyObject* PyErr_SetExcFromWindowsErrWithFilename(PyObject* type, int  ierr, char* filename) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_SetExcFromWindowsErrWithFilename, 3, type, ierr, filename).p;
}

extern int HotPyErr_WarnEx(void);
int PyErr_WarnEx(PyObject* category, char* message, int  stack_level) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyErr_WarnEx, 3, category, message, stack_level).i;
}

extern int HotPyErr_WarnExplicit(void);
int PyErr_WarnExplicit(PyObject* category, const char* message, const char* filename, int  lineno, const char* module, PyObject* registry) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyErr_WarnExplicit, 6, category, message, filename, lineno, module, registry).i;
}

extern int HotPyErr_WarnFormat(void);
int PyErr_WarnFormat(PyObject* category, Py_ssize_t  stack_level, const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    int _result = (int)gvmt_call((gvmt_func_ptr)HotPyErr_WarnFormat, 4, category, stack_level, format, _ap_list).i;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyErr_NewException(void);

PyObject* PyErr_NewException(char* name, PyObject* base, PyObject* dict) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_NewException, 3, name, base, dict).p;
}

extern PyObject* _hotpy_proxy_PyErr_NewExceptionWithDoc(void);

PyObject* PyErr_NewExceptionWithDoc(char* name, char* doc, PyObject* base, PyObject* dict) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyErr_NewExceptionWithDoc, 4, name, doc, base, dict).p;
}

extern void HotPyErr_WriteUnraisable(void);
void PyErr_WriteUnraisable(PyObject* obj) {
    gvmt_call((gvmt_func_ptr)HotPyErr_WriteUnraisable, 1, obj);
}

extern PyObject* _hotpy_proxy_PyException_GetTraceback(void);

PyObject* PyException_GetTraceback(PyObject* ex) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyException_GetTraceback, 1, ex).p;
}

extern int HotPyException_SetTraceback(void);
int PyException_SetTraceback(PyObject* ex, PyObject* tb) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyException_SetTraceback, 2, ex, tb).i;
}

extern PyObject* _hotpy_proxy_PyException_GetContext(void);

PyObject* PyException_GetContext(PyObject* ex) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyException_GetContext, 1, ex).p;
}

extern void HotPyException_SetContext(void);
void PyException_SetContext(PyObject* ex, PyObject* ctx) {
    gvmt_call((gvmt_func_ptr)HotPyException_SetContext, 2, ex, ctx);
}

extern PyObject* _hotpy_proxy_PyException_GetCause(void);

PyObject* PyException_GetCause(PyObject* ex) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyException_GetCause, 1, ex).p;
}

extern void HotPyException_SetCause(void);
void PyException_SetCause(PyObject* ex, PyObject* ctx) {
    gvmt_call((gvmt_func_ptr)HotPyException_SetCause, 2, ex, ctx);
}

extern PyObject* _hotpy_proxy_PyUnicodeEncodeError_Create(void);

PyObject* PyUnicodeEncodeError_Create(const char* encoding, const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicodeEncodeError_Create, 6, encoding, object, length, start, end, reason).p;
}

extern PyObject* _hotpy_proxy_PyUnicodeTranslateError_Create(void);

PyObject* PyUnicodeTranslateError_Create(const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicodeTranslateError_Create, 5, object, length, start, end, reason).p;
}

extern int HotPy_ReprEnter(void);
int Py_ReprEnter(PyObject* object) {
    return (int)gvmt_call((gvmt_func_ptr)HotPy_ReprEnter, 1, object).i;
}

extern void HotPy_ReprLeave(void);
void Py_ReprLeave(PyObject* object) {
    gvmt_call((gvmt_func_ptr)HotPy_ReprLeave, 1, object);
}



/* /home/mark/Python-3.2b2/Doc/c-api/file.rst */
extern int HotPyObject_AsFileDescriptor(void);
int PyObject_AsFileDescriptor(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_AsFileDescriptor, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyFile_GetLine(void);

PyObject* PyFile_GetLine(PyObject* p, int  n) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyFile_GetLine, 2, p, n).p;
}

extern int HotPyFile_WriteObject(void);
int PyFile_WriteObject(PyObject* obj, PyObject* p, int  flags) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFile_WriteObject, 3, obj, p, flags).i;
}

extern int HotPyFile_WriteString(void);
int PyFile_WriteString(const char* s, PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFile_WriteString, 2, s, p).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/float.rst */
extern int HotPyFloat_Check(void);
int PyFloat_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFloat_Check, 1, p).i;
}

extern int HotPyFloat_CheckExact(void);
int PyFloat_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFloat_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyFloat_FromString(void);

PyObject* PyFloat_FromString(PyObject* str) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyFloat_FromString, 1, str).p;
}

extern PyObject* _hotpy_proxy_PyFloat_FromDouble(void);

PyObject* PyFloat_FromDouble(double  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyFloat_FromDouble, 1, v).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/gen.rst */
extern PyObject* _hotpy_proxy_PyGen_New(void);

PyObject* PyGen_New(PyFrameObject* frame) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyGen_New, 1, frame).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/import.rst */
extern PyObject* _hotpy_proxy_PyImport_ImportModule(void);

PyObject* PyImport_ImportModule(const char* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ImportModule, 1, name).p;
}

extern PyObject* _hotpy_proxy_PyImport_ImportModuleNoBlock(void);

PyObject* PyImport_ImportModuleNoBlock(const char* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ImportModuleNoBlock, 1, name).p;
}

extern PyObject* _hotpy_proxy_PyImport_ImportModuleEx(void);

PyObject* PyImport_ImportModuleEx(char* name, PyObject* globals, PyObject* locals, PyObject* fromlist) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ImportModuleEx, 4, name, globals, locals, fromlist).p;
}

extern PyObject* _hotpy_proxy_PyImport_ImportModuleLevel(void);

PyObject* PyImport_ImportModuleLevel(char* name, PyObject* globals, PyObject* locals, PyObject* fromlist, int  level) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ImportModuleLevel, 5, name, globals, locals, fromlist, level).p;
}

extern PyObject* _hotpy_proxy_PyImport_Import(void);

PyObject* PyImport_Import(PyObject* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_Import, 1, name).p;
}

extern PyObject* _hotpy_proxy_PyImport_ReloadModule(void);

PyObject* PyImport_ReloadModule(PyObject* m) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ReloadModule, 1, m).p;
}

extern PyObject* _hotpy_proxy_PyImport_AddModule(void);

PyObject* PyImport_AddModule(const char* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_AddModule, 1, name).p;
}

extern PyObject* _hotpy_proxy_PyImport_ExecCodeModule(void);

PyObject* PyImport_ExecCodeModule(char* name, PyObject* co) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ExecCodeModule, 2, name, co).p;
}

extern PyObject* _hotpy_proxy_PyImport_ExecCodeModuleEx(void);

PyObject* PyImport_ExecCodeModuleEx(char* name, PyObject* co, char* pathname) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ExecCodeModuleEx, 3, name, co, pathname).p;
}

extern PyObject* _hotpy_proxy_PyImport_ExecCodeModuleWithPathnames(void);

PyObject* PyImport_ExecCodeModuleWithPathnames(char* name, PyObject* co, char* pathname, char* cpathname) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_ExecCodeModuleWithPathnames, 4, name, co, pathname, cpathname).p;
}

extern PyObject* _hotpy_proxy_PyImport_GetImporter(void);

PyObject* PyImport_GetImporter(PyObject* path) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyImport_GetImporter, 1, path).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/init.rst */
extern int HotPyThreadState_SetAsyncExc(void);
int PyThreadState_SetAsyncExc(long  id, PyObject* exc) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyThreadState_SetAsyncExc, 2, id, exc).i;
}

extern void HotPyEval_SetProfile(void);
void PyEval_SetProfile(Py_tracefunc  func, PyObject* obj) {
    gvmt_call((gvmt_func_ptr)HotPyEval_SetProfile, 2, func, obj);
}

extern void HotPyEval_SetTrace(void);
void PyEval_SetTrace(Py_tracefunc  func, PyObject* obj) {
    gvmt_call((gvmt_func_ptr)HotPyEval_SetTrace, 2, func, obj);
}

extern PyObject* _hotpy_proxy_PyEval_GetCallStats(void);

PyObject* PyEval_GetCallStats(PyObject* self) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyEval_GetCallStats, 1, self).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/iterator.rst */
extern PyObject* _hotpy_proxy_PySeqIter_New(void);

PyObject* PySeqIter_New(PyObject* seq) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySeqIter_New, 1, seq).p;
}

extern PyObject* _hotpy_proxy_PyCallIter_New(void);

PyObject* PyCallIter_New(PyObject* callable, PyObject* sentinel) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyCallIter_New, 2, callable, sentinel).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/iter.rst */
extern int HotPyIter_Check(void);
int PyIter_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyIter_Check, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyIter_Next(void);

PyObject* PyIter_Next(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyIter_Next, 1, o).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/list.rst */
extern int HotPyList_Check(void);
int PyList_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_Check, 1, p).i;
}

extern int HotPyList_CheckExact(void);
int PyList_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyList_New(void);

PyObject* PyList_New(Py_ssize_t  len) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyList_New, 1, len).p;
}

extern Py_ssize_t HotPyList_Size(void);
Py_ssize_t PyList_Size(PyObject* list) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyList_Size, 1, list).j;
}

extern Py_ssize_t HotPyList_GET_SIZE(void);
Py_ssize_t PyList_GET_SIZE(PyObject* list) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyList_GET_SIZE, 1, list).j;
}

extern PyObject* _hotpy_proxy_PyList_GetItem(void);

PyObject* PyList_GetItem(PyObject* list, Py_ssize_t  index) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyList_GetItem, 2, list, index).p;
}

extern PyObject* _hotpy_proxy_PyList_GET_ITEM(void);

PyObject* PyList_GET_ITEM(PyObject* list, Py_ssize_t  i) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyList_GET_ITEM, 2, list, i).p;
}

extern int HotPyList_SetItem(void);
int PyList_SetItem(PyObject* list, Py_ssize_t  index, PyObject* item) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_SetItem, 3, list, index, item).i;
}

extern void HotPyList_SET_ITEM(void);
void PyList_SET_ITEM(PyObject* list, Py_ssize_t  i, PyObject* o) {
    gvmt_call((gvmt_func_ptr)HotPyList_SET_ITEM, 3, list, i, o);
}

extern int HotPyList_Insert(void);
int PyList_Insert(PyObject* list, Py_ssize_t  index, PyObject* item) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_Insert, 3, list, index, item).i;
}

extern int HotPyList_Append(void);
int PyList_Append(PyObject* list, PyObject* item) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_Append, 2, list, item).i;
}

extern PyObject* _hotpy_proxy_PyList_GetSlice(void);

PyObject* PyList_GetSlice(PyObject* list, Py_ssize_t  low, Py_ssize_t  high) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyList_GetSlice, 3, list, low, high).p;
}

extern int HotPyList_SetSlice(void);
int PyList_SetSlice(PyObject* list, Py_ssize_t  low, Py_ssize_t  high, PyObject* itemlist) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_SetSlice, 4, list, low, high, itemlist).i;
}

extern int HotPyList_Sort(void);
int PyList_Sort(PyObject* list) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_Sort, 1, list).i;
}

extern int HotPyList_Reverse(void);
int PyList_Reverse(PyObject* list) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyList_Reverse, 1, list).i;
}

extern PyObject* _hotpy_proxy_PyList_AsTuple(void);

PyObject* PyList_AsTuple(PyObject* list) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyList_AsTuple, 1, list).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/long.rst */
extern int HotPyLong_Check(void);
int PyLong_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyLong_Check, 1, p).i;
}

extern int HotPyLong_CheckExact(void);
int PyLong_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyLong_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyLong_FromLong(void);

PyObject* PyLong_FromLong(long  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromLong, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromUnsignedLong(void);

PyObject* PyLong_FromUnsignedLong(unsigned long p0) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromUnsignedLong, 1, p0).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromSsize_t(void);

PyObject* PyLong_FromSsize_t(Py_ssize_t  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromSsize_t, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromSize_t(void);

PyObject* PyLong_FromSize_t(size_t  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromSize_t, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromLongLong(void);

PyObject* PyLong_FromLongLong(PY_LONG_LONG  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromLongLong, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromUnsignedLongLong(void);

PyObject* PyLong_FromUnsignedLongLong(unsigned  PY_LONG_LONG) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromUnsignedLongLong, 1, PY_LONG_LONG).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromDouble(void);

PyObject* PyLong_FromDouble(double  v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromDouble, 1, v).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromString(void);

PyObject* PyLong_FromString(char* str, char** pend, int  base) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromString, 3, str, pend, base).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromUnicode(void);

PyObject* PyLong_FromUnicode(Py_UNICODE* u, Py_ssize_t  length, int  base) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromUnicode, 3, u, length, base).p;
}

extern PyObject* _hotpy_proxy_PyLong_FromVoidPtr(void);

PyObject* PyLong_FromVoidPtr(void* p) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyLong_FromVoidPtr, 1, p).p;
}

extern long HotPyLong_AsLong(void);
long PyLong_AsLong(PyObject* pylong) {
    return (long)gvmt_call((gvmt_func_ptr)HotPyLong_AsLong, 1, pylong).j;
}

extern long HotPyLong_AsLongAndOverflow(void);
long PyLong_AsLongAndOverflow(PyObject* pylong, int* overflow) {
    return (long)gvmt_call((gvmt_func_ptr)HotPyLong_AsLongAndOverflow, 2, pylong, overflow).j;
}

extern Py_ssize_t HotPyLong_AsSsize_t(void);
Py_ssize_t PyLong_AsSsize_t(PyObject* pylong) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyLong_AsSsize_t, 1, pylong).j;
}

extern void* HotPyLong_AsVoidPtr(void);
void* PyLong_AsVoidPtr(PyObject* pylong) {
    return (void*)gvmt_call((gvmt_func_ptr)HotPyLong_AsVoidPtr, 1, pylong).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/mapping.rst */
extern int HotPyMapping_Check(void);
int PyMapping_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_Check, 1, o).i;
}

extern Py_ssize_t HotPyMapping_Size(void);
Py_ssize_t PyMapping_Size(PyObject* o) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyMapping_Size, 1, o).j;
}

extern int HotPyMapping_DelItemString(void);
int PyMapping_DelItemString(PyObject* o, char* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_DelItemString, 2, o, key).i;
}

extern int HotPyMapping_DelItem(void);
int PyMapping_DelItem(PyObject* o, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_DelItem, 2, o, key).i;
}

extern int HotPyMapping_HasKeyString(void);
int PyMapping_HasKeyString(PyObject* o, char* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_HasKeyString, 2, o, key).i;
}

extern int HotPyMapping_HasKey(void);
int PyMapping_HasKey(PyObject* o, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_HasKey, 2, o, key).i;
}

extern PyObject* _hotpy_proxy_PyMapping_Keys(void);

PyObject* PyMapping_Keys(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMapping_Keys, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyMapping_Values(void);

PyObject* PyMapping_Values(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMapping_Values, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyMapping_Items(void);

PyObject* PyMapping_Items(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMapping_Items, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyMapping_GetItemString(void);

PyObject* PyMapping_GetItemString(PyObject* o, char* key) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMapping_GetItemString, 2, o, key).p;
}

extern int HotPyMapping_SetItemString(void);
int PyMapping_SetItemString(PyObject* o, char* key, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMapping_SetItemString, 3, o, key, v).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/marshal.rst */
extern void HotPyMarshal_WriteObjectToFile(void);
void PyMarshal_WriteObjectToFile(PyObject* value, FILE* file, int  version) {
    gvmt_call((gvmt_func_ptr)HotPyMarshal_WriteObjectToFile, 3, value, file, version);
}

extern PyObject* _hotpy_proxy_PyMarshal_WriteObjectToString(void);

PyObject* PyMarshal_WriteObjectToString(PyObject* value, int  version) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMarshal_WriteObjectToString, 2, value, version).p;
}

extern PyObject* _hotpy_proxy_PyMarshal_ReadObjectFromFile(void);

PyObject* PyMarshal_ReadObjectFromFile(FILE* file) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMarshal_ReadObjectFromFile, 1, file).p;
}

extern PyObject* _hotpy_proxy_PyMarshal_ReadLastObjectFromFile(void);

PyObject* PyMarshal_ReadLastObjectFromFile(FILE* file) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMarshal_ReadLastObjectFromFile, 1, file).p;
}

extern PyObject* _hotpy_proxy_PyMarshal_ReadObjectFromString(void);

PyObject* PyMarshal_ReadObjectFromString(char* string, Py_ssize_t  len) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMarshal_ReadObjectFromString, 2, string, len).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/method.rst */
extern int HotPyInstanceMethod_Check(void);
int PyInstanceMethod_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyInstanceMethod_Check, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyInstanceMethod_New(void);

PyObject* PyInstanceMethod_New(PyObject* func) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyInstanceMethod_New, 1, func).p;
}

extern PyObject* _hotpy_proxy_PyInstanceMethod_Function(void);

PyObject* PyInstanceMethod_Function(PyObject* im) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyInstanceMethod_Function, 1, im).p;
}

extern PyObject* _hotpy_proxy_PyInstanceMethod_GET_FUNCTION(void);

PyObject* PyInstanceMethod_GET_FUNCTION(PyObject* im) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyInstanceMethod_GET_FUNCTION, 1, im).p;
}

extern int HotPyMethod_Check(void);
int PyMethod_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyMethod_Check, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyMethod_New(void);

PyObject* PyMethod_New(PyObject* func, PyObject* self) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMethod_New, 2, func, self).p;
}

extern PyObject* _hotpy_proxy_PyMethod_Function(void);

PyObject* PyMethod_Function(PyObject* meth) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMethod_Function, 1, meth).p;
}

extern PyObject* _hotpy_proxy_PyMethod_GET_FUNCTION(void);

PyObject* PyMethod_GET_FUNCTION(PyObject* meth) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMethod_GET_FUNCTION, 1, meth).p;
}

extern PyObject* _hotpy_proxy_PyMethod_Self(void);

PyObject* PyMethod_Self(PyObject* meth) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMethod_Self, 1, meth).p;
}

extern PyObject* _hotpy_proxy_PyMethod_GET_SELF(void);

PyObject* PyMethod_GET_SELF(PyObject* meth) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyMethod_GET_SELF, 1, meth).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/module.rst */
extern int HotPyModule_Check(void);
int PyModule_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyModule_Check, 1, p).i;
}

extern int HotPyModule_CheckExact(void);
int PyModule_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyModule_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyModule_New(void);

PyObject* PyModule_New(const char* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyModule_New, 1, name).p;
}

extern PyObject* _hotpy_proxy_PyModule_GetDict(void);

PyObject* PyModule_GetDict(PyObject* module) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyModule_GetDict, 1, module).p;
}

extern char* HotPyModule_GetName(void);
char* PyModule_GetName(PyObject* module) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyModule_GetName, 1, module).p;
}

extern char* HotPyModule_GetFilename(void);
char* PyModule_GetFilename(PyObject* module) {
    return (char*)gvmt_call((gvmt_func_ptr)HotPyModule_GetFilename, 1, module).p;
}

extern PyObject* _hotpy_proxy_PyModule_GetFilenameObject(void);

PyObject* PyModule_GetFilenameObject(PyObject* module) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyModule_GetFilenameObject, 1, module).p;
}

extern void* HotPyModule_GetState(void);
void* PyModule_GetState(PyObject* module) {
    return (void*)gvmt_call((gvmt_func_ptr)HotPyModule_GetState, 1, module).p;
}

extern PyObject* _hotpy_proxy_PyModule_Create(void);

PyObject* PyModule_Create(PyModuleDef* module) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyModule_Create, 1, module).p;
}

extern PyObject* _hotpy_proxy_PyModule_Create2(void);

PyObject* PyModule_Create2(PyModuleDef* module, int  module_api_version) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyModule_Create2, 2, module, module_api_version).p;
}

extern int HotPyModule_AddObject(void);
int PyModule_AddObject(PyObject* module, const char* name, PyObject* value) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyModule_AddObject, 3, module, name, value).i;
}

extern int HotPyModule_AddIntConstant(void);
int PyModule_AddIntConstant(PyObject* module, const char* name, long  value) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyModule_AddIntConstant, 3, module, name, value).i;
}

extern int HotPyModule_AddStringConstant(void);
int PyModule_AddStringConstant(PyObject* module, const char* name, const char* value) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyModule_AddStringConstant, 3, module, name, value).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/number.rst */
extern int HotPyNumber_Check(void);
int PyNumber_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyNumber_Check, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyNumber_Add(void);

PyObject* PyNumber_Add(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Add, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Subtract(void);

PyObject* PyNumber_Subtract(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Subtract, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Multiply(void);

PyObject* PyNumber_Multiply(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Multiply, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_FloorDivide(void);

PyObject* PyNumber_FloorDivide(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_FloorDivide, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_TrueDivide(void);

PyObject* PyNumber_TrueDivide(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_TrueDivide, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Remainder(void);

PyObject* PyNumber_Remainder(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Remainder, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Divmod(void);

PyObject* PyNumber_Divmod(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Divmod, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Power(void);

PyObject* PyNumber_Power(PyObject* o1, PyObject* o2, PyObject* o3) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Power, 3, o1, o2, o3).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Negative(void);

PyObject* PyNumber_Negative(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Negative, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Positive(void);

PyObject* PyNumber_Positive(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Positive, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Absolute(void);

PyObject* PyNumber_Absolute(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Absolute, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Invert(void);

PyObject* PyNumber_Invert(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Invert, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Lshift(void);

PyObject* PyNumber_Lshift(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Lshift, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Rshift(void);

PyObject* PyNumber_Rshift(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Rshift, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_And(void);

PyObject* PyNumber_And(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_And, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Xor(void);

PyObject* PyNumber_Xor(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Xor, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Or(void);

PyObject* PyNumber_Or(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Or, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceAdd(void);

PyObject* PyNumber_InPlaceAdd(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceAdd, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceSubtract(void);

PyObject* PyNumber_InPlaceSubtract(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceSubtract, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceMultiply(void);

PyObject* PyNumber_InPlaceMultiply(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceMultiply, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceFloorDivide(void);

PyObject* PyNumber_InPlaceFloorDivide(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceFloorDivide, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceTrueDivide(void);

PyObject* PyNumber_InPlaceTrueDivide(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceTrueDivide, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceRemainder(void);

PyObject* PyNumber_InPlaceRemainder(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceRemainder, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlacePower(void);

PyObject* PyNumber_InPlacePower(PyObject* o1, PyObject* o2, PyObject* o3) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlacePower, 3, o1, o2, o3).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceLshift(void);

PyObject* PyNumber_InPlaceLshift(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceLshift, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceRshift(void);

PyObject* PyNumber_InPlaceRshift(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceRshift, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceAnd(void);

PyObject* PyNumber_InPlaceAnd(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceAnd, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceXor(void);

PyObject* PyNumber_InPlaceXor(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceXor, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_InPlaceOr(void);

PyObject* PyNumber_InPlaceOr(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_InPlaceOr, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Long(void);

PyObject* PyNumber_Long(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Long, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Float(void);

PyObject* PyNumber_Float(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Float, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_Index(void);

PyObject* PyNumber_Index(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_Index, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyNumber_ToBase(void);

PyObject* PyNumber_ToBase(PyObject* n, int  base) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyNumber_ToBase, 2, n, base).p;
}

extern Py_ssize_t HotPyNumber_AsSsize_t(void);
Py_ssize_t PyNumber_AsSsize_t(PyObject* o, PyObject* exc) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyNumber_AsSsize_t, 2, o, exc).j;
}

extern int HotPyIndex_Check(void);
int PyIndex_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyIndex_Check, 1, o).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/object.rst */
extern int HotPyObject_Print(void);
int PyObject_Print(PyObject* o, FILE* fp, int  flags) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_Print, 3, o, fp, flags).i;
}

extern int HotPyObject_HasAttr(void);
int PyObject_HasAttr(PyObject* o, PyObject* attr_name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_HasAttr, 2, o, attr_name).i;
}

extern int HotPyObject_HasAttrString(void);
int PyObject_HasAttrString(PyObject* o, const char* attr_name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_HasAttrString, 2, o, attr_name).i;
}

extern PyObject* _hotpy_proxy_PyObject_GetAttr(void);

PyObject* PyObject_GetAttr(PyObject* o, PyObject* attr_name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_GetAttr, 2, o, attr_name).p;
}

extern PyObject* _hotpy_proxy_PyObject_GetAttrString(void);

PyObject* PyObject_GetAttrString(PyObject* o, const char* attr_name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_GetAttrString, 2, o, attr_name).p;
}

extern PyObject* _hotpy_proxy_PyObject_GenericGetAttr(void);

PyObject* PyObject_GenericGetAttr(PyObject* o, PyObject* name) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_GenericGetAttr, 2, o, name).p;
}

extern int HotPyObject_SetAttr(void);
int PyObject_SetAttr(PyObject* o, PyObject* attr_name, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_SetAttr, 3, o, attr_name, v).i;
}

extern int HotPyObject_SetAttrString(void);
int PyObject_SetAttrString(PyObject* o, const char* attr_name, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_SetAttrString, 3, o, attr_name, v).i;
}

extern int HotPyObject_GenericSetAttr(void);
int PyObject_GenericSetAttr(PyObject* o, PyObject* name, PyObject* value) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_GenericSetAttr, 3, o, name, value).i;
}

extern int HotPyObject_DelAttr(void);
int PyObject_DelAttr(PyObject* o, PyObject* attr_name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_DelAttr, 2, o, attr_name).i;
}

extern int HotPyObject_DelAttrString(void);
int PyObject_DelAttrString(PyObject* o, const char* attr_name) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_DelAttrString, 2, o, attr_name).i;
}

extern PyObject* _hotpy_proxy_PyObject_RichCompare(void);

PyObject* PyObject_RichCompare(PyObject* o1, PyObject* o2, int  opid) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_RichCompare, 3, o1, o2, opid).p;
}

extern int HotPyObject_RichCompareBool(void);
int PyObject_RichCompareBool(PyObject* o1, PyObject* o2, int  opid) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_RichCompareBool, 3, o1, o2, opid).i;
}

extern PyObject* _hotpy_proxy_PyObject_Repr(void);

PyObject* PyObject_Repr(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_Repr, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyObject_Str(void);

PyObject* PyObject_Str(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_Str, 1, o).p;
}

extern int HotPyObject_IsInstance(void);
int PyObject_IsInstance(PyObject* inst, PyObject* cls) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_IsInstance, 2, inst, cls).i;
}

extern int HotPyObject_IsSubclass(void);
int PyObject_IsSubclass(PyObject* derived, PyObject* cls) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_IsSubclass, 2, derived, cls).i;
}

extern int HotPyCallable_Check(void);
int PyCallable_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyCallable_Check, 1, o).i;
}

extern PyObject* _hotpy_proxy_PyObject_Call(void);

PyObject* PyObject_Call(PyObject* callable_object, PyObject* args, PyObject* kw) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_Call, 3, callable_object, args, kw).p;
}

extern PyObject* _hotpy_proxy_PyObject_CallObject(void);

PyObject* PyObject_CallObject(PyObject* callable_object, PyObject* args) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_CallObject, 2, callable_object, args).p;
}

extern PyObject* _hotpy_proxy_PyObject_CallFunction(void);

PyObject* PyObject_CallFunction(PyObject* callable, char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_CallFunction, 3, callable, format, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyObject_CallMethod(void);

PyObject* PyObject_CallMethod(PyObject* o, char* method, char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_CallMethod, 4, o, method, format, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyObject_CallFunctionObjArgs(void);

PyObject* PyObject_CallFunctionObjArgs(PyObject* callable, ... ) {
    va_list _ap_list;
    va_start(_ap_list, callable);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_CallFunctionObjArgs, 2, callable, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyObject_CallMethodObjArgs(void);

PyObject* PyObject_CallMethodObjArgs(PyObject* o, PyObject* name, ... ) {
    va_list _ap_list;
    va_start(_ap_list, name);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_CallMethodObjArgs, 3, o, name, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern int HotPyObject_IsTrue(void);
int PyObject_IsTrue(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_IsTrue, 1, o).i;
}

extern int HotPyObject_Not(void);
int PyObject_Not(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_Not, 1, o).i;
}

extern Py_ssize_t HotPyObject_Length(void);
Py_ssize_t PyObject_Length(PyObject* o) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyObject_Length, 1, o).j;
}

extern PyObject* _hotpy_proxy_PyObject_GetItem(void);

PyObject* PyObject_GetItem(PyObject* o, PyObject* key) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_GetItem, 2, o, key).p;
}

extern int HotPyObject_SetItem(void);
int PyObject_SetItem(PyObject* o, PyObject* key, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_SetItem, 3, o, key, v).i;
}

extern int HotPyObject_DelItem(void);
int PyObject_DelItem(PyObject* o, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyObject_DelItem, 2, o, key).i;
}

extern PyObject* _hotpy_proxy_PyObject_GetIter(void);

PyObject* PyObject_GetIter(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyObject_GetIter, 1, o).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/sequence.rst */
extern int HotPySequence_Check(void);
int PySequence_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_Check, 1, o).i;
}

extern Py_ssize_t HotPySequence_Size(void);
Py_ssize_t PySequence_Size(PyObject* o) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPySequence_Size, 1, o).j;
}

extern PyObject* _hotpy_proxy_PySequence_Concat(void);

PyObject* PySequence_Concat(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_Concat, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PySequence_Repeat(void);

PyObject* PySequence_Repeat(PyObject* o, Py_ssize_t  count) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_Repeat, 2, o, count).p;
}

extern PyObject* _hotpy_proxy_PySequence_InPlaceConcat(void);

PyObject* PySequence_InPlaceConcat(PyObject* o1, PyObject* o2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_InPlaceConcat, 2, o1, o2).p;
}

extern PyObject* _hotpy_proxy_PySequence_InPlaceRepeat(void);

PyObject* PySequence_InPlaceRepeat(PyObject* o, Py_ssize_t  count) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_InPlaceRepeat, 2, o, count).p;
}

extern PyObject* _hotpy_proxy_PySequence_GetItem(void);

PyObject* PySequence_GetItem(PyObject* o, Py_ssize_t  i) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_GetItem, 2, o, i).p;
}

extern PyObject* _hotpy_proxy_PySequence_GetSlice(void);

PyObject* PySequence_GetSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_GetSlice, 3, o, i1, i2).p;
}

extern int HotPySequence_SetItem(void);
int PySequence_SetItem(PyObject* o, Py_ssize_t  i, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_SetItem, 3, o, i, v).i;
}

extern int HotPySequence_DelItem(void);
int PySequence_DelItem(PyObject* o, Py_ssize_t  i) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_DelItem, 2, o, i).i;
}

extern int HotPySequence_SetSlice(void);
int PySequence_SetSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_SetSlice, 4, o, i1, i2, v).i;
}

extern int HotPySequence_DelSlice(void);
int PySequence_DelSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_DelSlice, 3, o, i1, i2).i;
}

extern Py_ssize_t HotPySequence_Count(void);
Py_ssize_t PySequence_Count(PyObject* o, PyObject* value) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPySequence_Count, 2, o, value).j;
}

extern int HotPySequence_Contains(void);
int PySequence_Contains(PyObject* o, PyObject* value) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySequence_Contains, 2, o, value).i;
}

extern Py_ssize_t HotPySequence_Index(void);
Py_ssize_t PySequence_Index(PyObject* o, PyObject* value) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPySequence_Index, 2, o, value).j;
}

extern PyObject* _hotpy_proxy_PySequence_List(void);

PyObject* PySequence_List(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_List, 1, o).p;
}

extern PyObject* _hotpy_proxy_PySequence_Tuple(void);

PyObject* PySequence_Tuple(PyObject* o) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_Tuple, 1, o).p;
}

extern PyObject* _hotpy_proxy_PySequence_Fast(void);

PyObject* PySequence_Fast(PyObject* o, const char* m) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_Fast, 2, o, m).p;
}

extern PyObject* _hotpy_proxy_PySequence_ITEM(void);

PyObject* PySequence_ITEM(PyObject* o, Py_ssize_t  i) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySequence_ITEM, 2, o, i).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/set.rst */
extern int HotPySet_Check(void);
int PySet_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySet_Check, 1, p).i;
}

extern int HotPyFrozenSet_Check(void);
int PyFrozenSet_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFrozenSet_Check, 1, p).i;
}

extern int HotPyAnySet_Check(void);
int PyAnySet_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyAnySet_Check, 1, p).i;
}

extern int HotPyAnySet_CheckExact(void);
int PyAnySet_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyAnySet_CheckExact, 1, p).i;
}

extern int HotPyFrozenSet_CheckExact(void);
int PyFrozenSet_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyFrozenSet_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PySet_New(void);

PyObject* PySet_New(PyObject* iterable) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySet_New, 1, iterable).p;
}

extern PyObject* _hotpy_proxy_PyFrozenSet_New(void);

PyObject* PyFrozenSet_New(PyObject* iterable) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyFrozenSet_New, 1, iterable).p;
}

extern Py_ssize_t HotPySet_Size(void);
Py_ssize_t PySet_Size(PyObject* anyset) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPySet_Size, 1, anyset).j;
}

extern Py_ssize_t HotPySet_GET_SIZE(void);
Py_ssize_t PySet_GET_SIZE(PyObject* anyset) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPySet_GET_SIZE, 1, anyset).j;
}

extern int HotPySet_Contains(void);
int PySet_Contains(PyObject* anyset, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySet_Contains, 2, anyset, key).i;
}

extern int HotPySet_Add(void);
int PySet_Add(PyObject* set, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySet_Add, 2, set, key).i;
}

extern int HotPySet_Discard(void);
int PySet_Discard(PyObject* set, PyObject* key) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySet_Discard, 2, set, key).i;
}

extern PyObject* _hotpy_proxy_PySet_Pop(void);

PyObject* PySet_Pop(PyObject* set) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySet_Pop, 1, set).p;
}

extern int HotPySet_Clear(void);
int PySet_Clear(PyObject* set) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySet_Clear, 1, set).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/slice.rst */
extern int HotPySlice_Check(void);
int PySlice_Check(PyObject* ob) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySlice_Check, 1, ob).i;
}

extern PyObject* _hotpy_proxy_PySlice_New(void);

PyObject* PySlice_New(PyObject* start, PyObject* stop, PyObject* step) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PySlice_New, 3, start, stop, step).p;
}

extern int HotPySlice_GetIndices(void);
int PySlice_GetIndices(PyObject* slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySlice_GetIndices, 5, slice, length, start, stop, step).i;
}

extern int HotPySlice_GetIndicesEx(void);
int PySlice_GetIndicesEx(PyObject* slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step, Py_ssize_t* slicelength) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySlice_GetIndicesEx, 6, slice, length, start, stop, step, slicelength).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/sys.rst */
extern int HotPySys_SetObject(void);
int PySys_SetObject(char* name, PyObject* v) {
    return (int)gvmt_call((gvmt_func_ptr)HotPySys_SetObject, 2, name, v).i;
}

extern void HotPySys_AddWarnOptionUnicode(void);
void PySys_AddWarnOptionUnicode(PyObject* unicode) {
    gvmt_call((gvmt_func_ptr)HotPySys_AddWarnOptionUnicode, 1, unicode);
}



/* /home/mark/Python-3.2b2/Doc/c-api/tuple.rst */
extern int HotPyTuple_Check(void);
int PyTuple_Check(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyTuple_Check, 1, p).i;
}

extern int HotPyTuple_CheckExact(void);
int PyTuple_CheckExact(PyObject* p) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyTuple_CheckExact, 1, p).i;
}

extern PyObject* _hotpy_proxy_PyTuple_New(void);

PyObject* PyTuple_New(Py_ssize_t  len) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyTuple_New, 1, len).p;
}

extern PyObject* _hotpy_proxy_PyTuple_Pack(void);

PyObject* PyTuple_Pack(Py_ssize_t  n, ... ) {
    va_list _ap_list;
    va_start(_ap_list, n);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyTuple_Pack, 2, n, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern Py_ssize_t HotPyTuple_Size(void);
Py_ssize_t PyTuple_Size(PyObject* p) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyTuple_Size, 1, p).j;
}

extern PyObject* _hotpy_proxy_PyTuple_GetItem(void);

PyObject* PyTuple_GetItem(PyObject* p, Py_ssize_t  pos) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyTuple_GetItem, 2, p, pos).p;
}

extern PyObject* _hotpy_proxy_PyTuple_GET_ITEM(void);

PyObject* PyTuple_GET_ITEM(PyObject* p, Py_ssize_t  pos) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyTuple_GET_ITEM, 2, p, pos).p;
}

extern PyObject* _hotpy_proxy_PyTuple_GetSlice(void);

PyObject* PyTuple_GetSlice(PyObject* p, Py_ssize_t  low, Py_ssize_t  high) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyTuple_GetSlice, 3, p, low, high).p;
}

extern int HotPyTuple_SetItem(void);
int PyTuple_SetItem(PyObject* p, Py_ssize_t  pos, PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyTuple_SetItem, 3, p, pos, o).i;
}

extern void HotPyTuple_SET_ITEM(void);
void PyTuple_SET_ITEM(PyObject* p, Py_ssize_t  pos, PyObject* o) {
    gvmt_call((gvmt_func_ptr)HotPyTuple_SET_ITEM, 3, p, pos, o);
}



/* /home/mark/Python-3.2b2/Doc/c-api/type.rst */
extern int HotPyType_Check(void);
int PyType_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_Check, 1, o).i;
}

extern int HotPyType_CheckExact(void);
int PyType_CheckExact(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_CheckExact, 1, o).i;
}

extern void HotPyType_Modified(void);
void PyType_Modified(PyTypeObject* type) {
    gvmt_call((gvmt_func_ptr)HotPyType_Modified, 1, type);
}

extern int HotPyType_HasFeature(void);
int PyType_HasFeature(PyObject* o, int  feature) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_HasFeature, 2, o, feature).i;
}

extern int HotPyType_IS_GC(void);
int PyType_IS_GC(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_IS_GC, 1, o).i;
}

extern int HotPyType_IsSubtype(void);
int PyType_IsSubtype(PyTypeObject* a, PyTypeObject* b) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_IsSubtype, 2, a, b).i;
}

extern PyObject* _hotpy_proxy_PyType_GenericAlloc(void);

PyObject* PyType_GenericAlloc(PyTypeObject* type, Py_ssize_t  nitems) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyType_GenericAlloc, 2, type, nitems).p;
}

extern PyObject* _hotpy_proxy_PyType_GenericNew(void);

PyObject* PyType_GenericNew(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyType_GenericNew, 3, type, args, kwds).p;
}

extern int HotPyType_Ready(void);
int PyType_Ready(PyTypeObject* type) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyType_Ready, 1, type).i;
}



/* /home/mark/Python-3.2b2/Doc/c-api/unicode.rst */
extern int HotPyUnicode_Check(void);
int PyUnicode_Check(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_Check, 1, o).i;
}

extern int HotPyUnicode_CheckExact(void);
int PyUnicode_CheckExact(PyObject* o) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_CheckExact, 1, o).i;
}

extern Py_ssize_t HotPyUnicode_GET_DATA_SIZE(void);
Py_ssize_t PyUnicode_GET_DATA_SIZE(PyObject* o) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyUnicode_GET_DATA_SIZE, 1, o).j;
}

extern const char* HotPyUnicode_AS_DATA(void);
const char* PyUnicode_AS_DATA(PyObject* o) {
    return (const char*)gvmt_call((gvmt_func_ptr)HotPyUnicode_AS_DATA, 1, o).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromUnicode(void);

PyObject* PyUnicode_FromUnicode(const Py_UNICODE* u, Py_ssize_t  size) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromUnicode, 2, u, size).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromStringAndSize(void);

PyObject* PyUnicode_FromStringAndSize(const char* u, Py_ssize_t  size) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromStringAndSize, 2, u, size).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromFormat(void);

PyObject* PyUnicode_FromFormat(const char* format, ... ) {
    va_list _ap_list;
    va_start(_ap_list, format);
    PyObject* _result = (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromFormat, 2, format, _ap_list).p;
    va_end(_ap_list);
    return _result;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromFormatV(void);

PyObject* PyUnicode_FromFormatV(const char* format, va_list  vargs) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromFormatV, 2, format, vargs).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_TransformDecimalToASCII(void);

PyObject* PyUnicode_TransformDecimalToASCII(Py_UNICODE* s, Py_ssize_t  size) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_TransformDecimalToASCII, 2, s, size).p;
}

extern Py_ssize_t HotPyUnicode_GetSize(void);
Py_ssize_t PyUnicode_GetSize(PyObject* unicode) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyUnicode_GetSize, 1, unicode).j;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromEncodedObject(void);

PyObject* PyUnicode_FromEncodedObject(PyObject* obj, const char* encoding, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromEncodedObject, 3, obj, encoding, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromObject(void);

PyObject* PyUnicode_FromObject(PyObject* obj) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromObject, 1, obj).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeFSDefault(void);

PyObject* PyUnicode_EncodeFSDefault(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeFSDefault, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_FromWideChar(void);

PyObject* PyUnicode_FromWideChar(const wchar_t* w, Py_ssize_t  size) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_FromWideChar, 2, w, size).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Encode(void);

PyObject* PyUnicode_Encode(const Py_UNICODE* s, Py_ssize_t  size, const char* encoding, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Encode, 4, s, size, encoding, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsEncodedString(void);

PyObject* PyUnicode_AsEncodedString(PyObject* unicode, const char* encoding, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsEncodedString, 3, unicode, encoding, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeUTF8(void);

PyObject* PyUnicode_EncodeUTF8(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeUTF8, 3, s, size, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsUTF8String(void);

PyObject* PyUnicode_AsUTF8String(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsUTF8String, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeUTF32(void);

PyObject* PyUnicode_EncodeUTF32(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeUTF32, 4, s, size, errors, byteorder).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsUTF32String(void);

PyObject* PyUnicode_AsUTF32String(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsUTF32String, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeUTF16(void);

PyObject* PyUnicode_EncodeUTF16(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeUTF16, 4, s, size, errors, byteorder).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsUTF16String(void);

PyObject* PyUnicode_AsUTF16String(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsUTF16String, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeUTF7(void);

PyObject* PyUnicode_EncodeUTF7(const Py_UNICODE* s, Py_ssize_t  size, int  base64SetO, int  base64WhiteSpace, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeUTF7, 5, s, size, base64SetO, base64WhiteSpace, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeUnicodeEscape(void);

PyObject* PyUnicode_EncodeUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeUnicodeEscape, 2, s, size).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsUnicodeEscapeString(void);

PyObject* PyUnicode_AsUnicodeEscapeString(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsUnicodeEscapeString, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeRawUnicodeEscape(void);

PyObject* PyUnicode_EncodeRawUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeRawUnicodeEscape, 3, s, size, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsRawUnicodeEscapeString(void);

PyObject* PyUnicode_AsRawUnicodeEscapeString(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsRawUnicodeEscapeString, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeLatin1(void);

PyObject* PyUnicode_EncodeLatin1(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeLatin1, 3, s, size, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsLatin1String(void);

PyObject* PyUnicode_AsLatin1String(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsLatin1String, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeASCII(void);

PyObject* PyUnicode_EncodeASCII(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeASCII, 3, s, size, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsASCIIString(void);

PyObject* PyUnicode_AsASCIIString(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsASCIIString, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeCharmap(void);

PyObject* PyUnicode_EncodeCharmap(const Py_UNICODE* s, Py_ssize_t  size, PyObject* mapping, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeCharmap, 4, s, size, mapping, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsCharmapString(void);

PyObject* PyUnicode_AsCharmapString(PyObject* unicode, PyObject* mapping) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsCharmapString, 2, unicode, mapping).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_TranslateCharmap(void);

PyObject* PyUnicode_TranslateCharmap(const Py_UNICODE* s, Py_ssize_t  size, PyObject* table, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_TranslateCharmap, 4, s, size, table, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_EncodeMBCS(void);

PyObject* PyUnicode_EncodeMBCS(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_EncodeMBCS, 3, s, size, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_AsMBCSString(void);

PyObject* PyUnicode_AsMBCSString(PyObject* unicode) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_AsMBCSString, 1, unicode).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Concat(void);

PyObject* PyUnicode_Concat(PyObject* left, PyObject* right) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Concat, 2, left, right).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Split(void);

PyObject* PyUnicode_Split(PyObject* s, PyObject* sep, Py_ssize_t  maxsplit) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Split, 3, s, sep, maxsplit).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Splitlines(void);

PyObject* PyUnicode_Splitlines(PyObject* s, int  keepend) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Splitlines, 2, s, keepend).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Translate(void);

PyObject* PyUnicode_Translate(PyObject* str, PyObject* table, const char* errors) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Translate, 3, str, table, errors).p;
}

extern PyObject* _hotpy_proxy_PyUnicode_Join(void);

PyObject* PyUnicode_Join(PyObject* separator, PyObject* seq) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Join, 2, separator, seq).p;
}

extern int HotPyUnicode_Tailmatch(void);
int PyUnicode_Tailmatch(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_Tailmatch, 5, str, substr, start, end, direction).i;
}

extern Py_ssize_t HotPyUnicode_Find(void);
Py_ssize_t PyUnicode_Find(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyUnicode_Find, 5, str, substr, start, end, direction).j;
}

extern Py_ssize_t HotPyUnicode_Count(void);
Py_ssize_t PyUnicode_Count(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end) {
    return (Py_ssize_t)gvmt_call((gvmt_func_ptr)HotPyUnicode_Count, 4, str, substr, start, end).j;
}

extern PyObject* _hotpy_proxy_PyUnicode_Replace(void);

PyObject* PyUnicode_Replace(PyObject* str, PyObject* substr, PyObject* replstr, Py_ssize_t  maxcount) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Replace, 4, str, substr, replstr, maxcount).p;
}

extern int HotPyUnicode_Compare(void);
int PyUnicode_Compare(PyObject* left, PyObject* right) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_Compare, 2, left, right).i;
}

extern int HotPyUnicode_CompareWithASCIIString(void);
int PyUnicode_CompareWithASCIIString(PyObject* uni, char* string) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_CompareWithASCIIString, 2, uni, string).i;
}

extern int HotPyUnicode_RichCompare(void);
int PyUnicode_RichCompare(PyObject* left, PyObject* right, int  op) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_RichCompare, 3, left, right, op).i;
}

extern PyObject* _hotpy_proxy_PyUnicode_Format(void);

PyObject* PyUnicode_Format(PyObject* format, PyObject* args) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_Format, 2, format, args).p;
}

extern int HotPyUnicode_Contains(void);
int PyUnicode_Contains(PyObject* container, PyObject* element) {
    return (int)gvmt_call((gvmt_func_ptr)HotPyUnicode_Contains, 2, container, element).i;
}

extern PyObject* _hotpy_proxy_PyUnicode_InternFromString(void);

PyObject* PyUnicode_InternFromString(const char* v) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyUnicode_InternFromString, 1, v).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/veryhigh.rst */
extern PyObject* _hotpy_proxy_PyRun_String(void);

PyObject* PyRun_String(const char* str, int  start, PyObject* globals, PyObject* locals) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_String, 4, str, start, globals, locals).p;
}

extern PyObject* _hotpy_proxy_PyRun_StringFlags(void);

PyObject* PyRun_StringFlags(const char* str, int  start, PyObject* globals, PyObject* locals, PyCompilerFlags* flags) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_StringFlags, 5, str, start, globals, locals, flags).p;
}

extern PyObject* _hotpy_proxy_PyRun_File(void);

PyObject* PyRun_File(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_File, 5, fp, filename, start, globals, locals).p;
}

extern PyObject* _hotpy_proxy_PyRun_FileEx(void);

PyObject* PyRun_FileEx(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, int  closeit) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_FileEx, 6, fp, filename, start, globals, locals, closeit).p;
}

extern PyObject* _hotpy_proxy_PyRun_FileFlags(void);

PyObject* PyRun_FileFlags(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, PyCompilerFlags* flags) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_FileFlags, 6, fp, filename, start, globals, locals, flags).p;
}

extern PyObject* _hotpy_proxy_PyRun_FileExFlags(void);

PyObject* PyRun_FileExFlags(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, int  closeit, PyCompilerFlags* flags) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyRun_FileExFlags, 7, fp, filename, start, globals, locals, closeit, flags).p;
}

extern PyObject* _hotpy_proxy_Py_CompileString(void);

PyObject* Py_CompileString(const char* str, const char* filename, int  start) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_Py_CompileString, 3, str, filename, start).p;
}

extern PyObject* _hotpy_proxy_Py_CompileStringFlags(void);

PyObject* Py_CompileStringFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_Py_CompileStringFlags, 4, str, filename, start, flags).p;
}

extern PyObject* _hotpy_proxy_Py_CompileStringExFlags(void);

PyObject* Py_CompileStringExFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags, int  optimize) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_Py_CompileStringExFlags, 5, str, filename, start, flags, optimize).p;
}

extern PyObject* _hotpy_proxy_PyEval_EvalCode(void);

PyObject* PyEval_EvalCode(PyObject* co, PyObject* globals, PyObject* locals) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyEval_EvalCode, 3, co, globals, locals).p;
}

extern PyObject* _hotpy_proxy_PyEval_EvalCodeEx(void);

PyObject* PyEval_EvalCodeEx(PyObject* co, PyObject* globals, PyObject* locals, PyObject** args, int  argcount, PyObject** kws, int  kwcount, PyObject** defs, int  defcount, PyObject* closure) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyEval_EvalCodeEx, 10, co, globals, locals, args, argcount, kws, kwcount, defs, defcount, closure).p;
}

extern PyObject* _hotpy_proxy_PyEval_EvalFrame(void);

PyObject* PyEval_EvalFrame(PyFrameObject* f) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyEval_EvalFrame, 1, f).p;
}

extern PyObject* _hotpy_proxy_PyEval_EvalFrameEx(void);

PyObject* PyEval_EvalFrameEx(PyFrameObject* f, int  throwflag) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyEval_EvalFrameEx, 2, f, throwflag).p;
}



/* /home/mark/Python-3.2b2/Doc/c-api/weakref.rst */
extern PyObject* _hotpy_proxy_PyWeakref_NewRef(void);

PyObject* PyWeakref_NewRef(PyObject* ob, PyObject* callback) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyWeakref_NewRef, 2, ob, callback).p;
}

extern PyObject* _hotpy_proxy_PyWeakref_NewProxy(void);

PyObject* PyWeakref_NewProxy(PyObject* ob, PyObject* callback) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyWeakref_NewProxy, 2, ob, callback).p;
}

extern PyObject* _hotpy_proxy_PyWeakref_GetObject(void);

PyObject* PyWeakref_GetObject(PyObject* ref) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyWeakref_GetObject, 1, ref).p;
}

extern PyObject* _hotpy_proxy_PyWeakref_GET_OBJECT(void);

PyObject* PyWeakref_GET_OBJECT(PyObject* ref) {
    return (PyObject*)gvmt_call((gvmt_func_ptr)_hotpy_proxy_PyWeakref_GET_OBJECT, 1, ref).p;
}

