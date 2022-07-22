/** This file is auto-generated */
#include "hotpy.h"


/* /home/mark/Python-3.2b2/Doc/c-api/allocation.rst */
R_object HotPyObject_Init(R_object op, R_type type) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_InitVar(R_object op, R_type type, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_Del(R_object op) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/arg.rst */
void HotPyArg_ParseTuple(R_object args, const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_VaParse(R_object args, const char* format, va_list  vargs) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_ParseTupleAndKeywords(R_object args, R_object kw, const char* format, char* keywords, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_VaParseTupleAndKeywords(R_object args, R_object kw, const char* format, char* keywords, va_list  vargs) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_ValidateKeywordArguments(R_object p0) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_Parse(R_object args, const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyArg_UnpackTuple(R_object args, const char* name, Py_ssize_t  min, Py_ssize_t  max, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPy_BuildValue(const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPy_VaBuildValue(const char* format, va_list  vargs) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytearray.rst */
int HotPyByteArray_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyByteArray_CheckExact(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyByteArray_FromObject(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyByteArray_FromStringAndSize(const char* string, Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyByteArray_Concat(R_object a, R_object b) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyByteArray_Size(R_object bytearray) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyByteArray_AsString(R_object bytearray) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyByteArray_Resize(R_object bytearray, Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyByteArray_AS_STRING(R_object bytearray) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/bytes.rst */
int HotPyBytes_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyBytes_CheckExact(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyBytes_FromString(const char* v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyBytes_FromStringAndSize(const char* v, Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyBytes_FromFormat(const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyBytes_FromFormatV(const char* format, va_list  vargs) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyBytes_FromObject(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyBytes_Size(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyBytes_AsString(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyBytes_AS_STRING(R_object string) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyBytes_AsStringAndSize(R_object obj, char** buffer, Py_ssize_t* length) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyBytes_Concat(PyObject** bytes, R_object newpart) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyBytes_ConcatAndDel(PyObject** bytes, R_object newpart) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/capsule.rst */
int HotPyCapsule_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyCapsule_New(void* pointer, const char* name, PyCapsule_Destructor  destructor) {
    raise_char(type_InternalError, "Not implemented");
}

void* HotPyCapsule_GetPointer(R_object capsule, const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

void* HotPyCapsule_GetContext(R_object capsule) {
    raise_char(type_InternalError, "Not implemented");
}

const char* HotPyCapsule_GetName(R_object capsule) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyCapsule_IsValid(R_object capsule, const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyCapsule_SetContext(R_object capsule, void* context) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyCapsule_SetDestructor(R_object capsule, PyCapsule_Destructor  destructor) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyCapsule_SetName(R_object capsule, const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyCapsule_SetPointer(R_object capsule, void* pointer) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/complex.rst */
int HotPyComplex_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyComplex_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyComplex_FromDoubles(double  real, double  imag) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/dict.rst */
int HotPyDict_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyDict_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDictProxy_New(R_object dict) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_Clear(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_Contains(R_object p, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_Copy(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_SetItem(R_object p, R_object key, R_object val) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_SetItemString(R_object p, const char* key, R_object val) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_DelItem(R_object p, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_DelItemString(R_object p, char* key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_GetItem(R_object p, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_GetItemWithError(R_object p, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_GetItemString(R_object p, const char* key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_Items(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_Keys(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyDict_Values(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyDict_Size(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_Next(R_object p, Py_ssize_t* ppos, PyObject** pkey, PyObject** pvalue) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_Merge(R_object a, R_object b, int  override) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_Update(R_object a, R_object b) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyDict_MergeFromSeq2(R_object a, R_object seq2, int  override) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/exceptions.rst */
void HotPyErr_ExceptionMatches(R_object exc) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_GivenExceptionMatches(R_object given, R_object exc) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_Restore(R_object type, R_object value, R_object traceback) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_SetString(R_object type, const char* message) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_SetObject(R_object type, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_SetNone(R_object type) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetFromErrno(R_object type) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetFromErrnoWithFilename(R_object type, const char* filename) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetFromWindowsErr(int  ierr) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetExcFromWindowsErr(R_object type, int  ierr) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetFromWindowsErrWithFilename(int  ierr, const char* filename) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_SetExcFromWindowsErrWithFilename(R_object type, int  ierr, char* filename) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_WarnEx(R_object category, char* message, int  stack_level) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_WarnExplicit(R_object category, const char* message, const char* filename, int  lineno, const char* module, R_object registry) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_WarnFormat(R_object category, Py_ssize_t  stack_level, const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_NewException(char* name, R_object base, R_object dict) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyErr_NewExceptionWithDoc(char* name, char* doc, R_object base, R_object dict) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyErr_WriteUnraisable(R_object obj) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyException_GetTraceback(R_object ex) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyException_SetTraceback(R_object ex, R_object tb) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyException_GetContext(R_object ex) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyException_SetContext(R_object ex, R_object ctx) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyException_GetCause(R_object ex) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyException_SetCause(R_object ex, R_object ctx) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicodeEncodeError_Create(const char* encoding, const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicodeTranslateError_Create(const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPy_ReprEnter(R_object object) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPy_ReprLeave(R_object object) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/file.rst */
void HotPyObject_AsFileDescriptor(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyFile_GetLine(R_object p, int  n) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyFile_WriteObject(R_object obj, R_object p, int  flags) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyFile_WriteString(const char* s, R_object p) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/float.rst */
int HotPyFloat_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyFloat_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyFloat_FromString(R_object str) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyFloat_FromDouble(double  v) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/gen.rst */
R_object HotPyGen_New(R_frame frame) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/import.rst */
R_object HotPyImport_ImportModule(const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ImportModuleNoBlock(const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ImportModuleEx(char* name, R_object globals, R_object locals, R_object fromlist) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ImportModuleLevel(char* name, R_object globals, R_object locals, R_object fromlist, int  level) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_Import(R_object name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ReloadModule(R_object m) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_AddModule(const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ExecCodeModule(char* name, R_object co) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ExecCodeModuleEx(char* name, R_object co, char* pathname) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_ExecCodeModuleWithPathnames(char* name, R_object co, char* pathname, char* cpathname) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyImport_GetImporter(R_object path) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/init.rst */
void HotPyThreadState_SetAsyncExc(long  id, R_object exc) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyEval_SetProfile(Py_tracefunc  func, R_object obj) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyEval_SetTrace(Py_tracefunc  func, R_object obj) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyEval_GetCallStats(R_object self) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/iterator.rst */
R_object HotPySeqIter_New(R_object seq) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyCallIter_New(R_object callable, R_object sentinel) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/iter.rst */
int HotPyIter_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyIter_Next(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/list.rst */
int HotPyList_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyList_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyList_New(Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyList_Size(R_object list) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyList_GET_SIZE(R_object list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyList_GetItem(R_object list, Py_ssize_t  index) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyList_GET_ITEM(R_object list, Py_ssize_t  i) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_SetItem(R_object list, Py_ssize_t  index, R_object item) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_SET_ITEM(R_object list, Py_ssize_t  i, R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_Insert(R_object list, Py_ssize_t  index, R_object item) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_Append(R_object list, R_object item) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyList_GetSlice(R_object list, Py_ssize_t  low, Py_ssize_t  high) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_SetSlice(R_object list, Py_ssize_t  low, Py_ssize_t  high, R_object itemlist) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_Sort(R_object list) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyList_Reverse(R_object list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyList_AsTuple(R_object list) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/long.rst */
int HotPyLong_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyLong_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromLong(long  v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromUnsignedLong(unsigned long p0) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromSsize_t(Py_ssize_t  v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromSize_t(size_t  v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromLongLong(PY_LONG_LONG  v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromUnsignedLongLong(unsigned  PY_LONG_LONG) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromDouble(double  v) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromString(char* str, char** pend, int  base) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromUnicode(Py_UNICODE* u, Py_ssize_t  length, int  base) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyLong_FromVoidPtr(void* p) {
    raise_char(type_InternalError, "Not implemented");
}

long HotPyLong_AsLong(R_object pylong) {
    raise_char(type_InternalError, "Not implemented");
}

long HotPyLong_AsLongAndOverflow(R_object pylong, int* overflow) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyLong_AsSsize_t(R_object pylong) {
    raise_char(type_InternalError, "Not implemented");
}

void* HotPyLong_AsVoidPtr(R_object pylong) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/mapping.rst */
int HotPyMapping_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyMapping_Size(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyMapping_DelItemString(R_object o, char* key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyMapping_DelItem(R_object o, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyMapping_HasKeyString(R_object o, char* key) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyMapping_HasKey(R_object o, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMapping_Keys(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMapping_Values(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMapping_Items(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMapping_GetItemString(R_object o, char* key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyMapping_SetItemString(R_object o, char* key, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/marshal.rst */
void HotPyMarshal_WriteObjectToFile(R_object value, FILE* file, int  version) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMarshal_WriteObjectToString(R_object value, int  version) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMarshal_ReadObjectFromFile(FILE* file) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMarshal_ReadLastObjectFromFile(FILE* file) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMarshal_ReadObjectFromString(char* string, Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/method.rst */
int HotPyInstanceMethod_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyInstanceMethod_New(R_object func) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyInstanceMethod_Function(R_object im) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyInstanceMethod_GET_FUNCTION(R_object im) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyMethod_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMethod_New(R_object func, R_object self) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMethod_Function(R_object meth) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMethod_GET_FUNCTION(R_object meth) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMethod_Self(R_object meth) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyMethod_GET_SELF(R_object meth) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/module.rst */
int HotPyModule_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyModule_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyModule_New(const char* name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyModule_GetDict(R_object module) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyModule_GetName(R_object module) {
    raise_char(type_InternalError, "Not implemented");
}

char* HotPyModule_GetFilename(R_object module) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyModule_GetFilenameObject(R_object module) {
    raise_char(type_InternalError, "Not implemented");
}

void* HotPyModule_GetState(R_object module) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyModule_Create(PyModuleDef* module) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyModule_Create2(PyModuleDef* module, int  module_api_version) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyModule_AddObject(R_object module, const char* name, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyModule_AddIntConstant(R_object module, const char* name, long  value) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyModule_AddStringConstant(R_object module, const char* name, const char* value) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/number.rst */
int HotPyNumber_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Add(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Subtract(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Multiply(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_FloorDivide(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_TrueDivide(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Remainder(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Divmod(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Power(R_object o1, R_object o2, R_object o3) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Negative(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Positive(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Absolute(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Invert(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Lshift(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Rshift(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_And(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Xor(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Or(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceAdd(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceSubtract(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceMultiply(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceFloorDivide(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceTrueDivide(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceRemainder(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlacePower(R_object o1, R_object o2, R_object o3) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceLshift(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceRshift(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceAnd(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceXor(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_InPlaceOr(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Long(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Float(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_Index(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyNumber_ToBase(R_object n, int  base) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyNumber_AsSsize_t(R_object o, R_object exc) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyIndex_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/object.rst */
void HotPyObject_Print(R_object o, FILE* fp, int  flags) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_HasAttr(R_object o, R_object attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_HasAttrString(R_object o, const char* attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_GetAttr(R_object o, R_object attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_GetAttrString(R_object o, const char* attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_GenericGetAttr(R_object o, R_object name) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_SetAttr(R_object o, R_object attr_name, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_SetAttrString(R_object o, const char* attr_name, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_GenericSetAttr(R_object o, R_object name, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_DelAttr(R_object o, R_object attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_DelAttrString(R_object o, const char* attr_name) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_RichCompare(R_object o1, R_object o2, int  opid) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_RichCompareBool(R_object o1, R_object o2, int  opid) {
    raise_char(type_InternalError, "Not implemented");
}

R_str HotPyObject_Repr(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_Str(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_IsInstance(R_object inst, R_object cls) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_IsSubclass(R_object derived, R_object cls) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyCallable_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_Call(R_object callable_object, R_object args, R_object kw) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_CallObject(R_object callable_object, R_object args) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_CallFunction(R_object callable, char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_CallMethod(R_object o, char* method, char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_CallFunctionObjArgs(R_object callable, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_CallMethodObjArgs(R_object o, R_object name, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_IsTrue(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyObject_Not(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyObject_Length(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_GetItem(R_object o, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_SetItem(R_object o, R_object key, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyObject_DelItem(R_object o, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyObject_GetIter(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/sequence.rst */
int HotPySequence_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPySequence_Size(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_Concat(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_Repeat(R_object o, Py_ssize_t  count) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_InPlaceConcat(R_object o1, R_object o2) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_InPlaceRepeat(R_object o, Py_ssize_t  count) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_GetItem(R_object o, Py_ssize_t  i) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_GetSlice(R_object o, Py_ssize_t  i1, Py_ssize_t  i2) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySequence_SetItem(R_object o, Py_ssize_t  i, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySequence_DelItem(R_object o, Py_ssize_t  i) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySequence_SetSlice(R_object o, Py_ssize_t  i1, Py_ssize_t  i2, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySequence_DelSlice(R_object o, Py_ssize_t  i1, Py_ssize_t  i2) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPySequence_Count(R_object o, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySequence_Contains(R_object o, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPySequence_Index(R_object o, R_object value) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_List(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_Tuple(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_Fast(R_object o, const char* m) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySequence_ITEM(R_object o, Py_ssize_t  i) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/set.rst */
int HotPySet_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyFrozenSet_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyAnySet_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyAnySet_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyFrozenSet_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySet_New(R_object iterable) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyFrozenSet_New(R_object iterable) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPySet_Size(R_object anyset) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPySet_GET_SIZE(R_object anyset) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySet_Contains(R_object anyset, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySet_Add(R_object set, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySet_Discard(R_object set, R_object key) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySet_Pop(R_object set) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySet_Clear(R_object set) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/slice.rst */
int HotPySlice_Check(R_object ob) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPySlice_New(R_object start, R_object stop, R_object step) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySlice_GetIndices(R_object slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySlice_GetIndicesEx(R_object slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step, Py_ssize_t* slicelength) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/sys.rst */
void HotPySys_SetObject(char* name, R_object v) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPySys_AddWarnOptionUnicode(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/tuple.rst */
int HotPyTuple_Check(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyTuple_CheckExact(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyTuple_New(Py_ssize_t  len) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyTuple_Pack(Py_ssize_t  n, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyTuple_Size(R_object p) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyTuple_GetItem(R_object p, Py_ssize_t  pos) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyTuple_GET_ITEM(R_object p, Py_ssize_t  pos) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyTuple_GetSlice(R_object p, Py_ssize_t  low, Py_ssize_t  high) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyTuple_SetItem(R_object p, Py_ssize_t  pos, R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyTuple_SET_ITEM(R_object p, Py_ssize_t  pos, R_object o) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/type.rst */
int HotPyType_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyType_CheckExact(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyType_Modified(R_type type) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyType_HasFeature(R_object o, int  feature) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyType_IS_GC(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyType_IsSubtype(R_type a, R_type b) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyType_GenericAlloc(R_type type, Py_ssize_t  nitems) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyType_GenericNew(R_type type, R_object args, R_object kwds) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyType_Ready(R_type type) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/unicode.rst */
int HotPyUnicode_Check(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

int HotPyUnicode_CheckExact(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyUnicode_GET_DATA_SIZE(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

const char* HotPyUnicode_AS_DATA(R_object o) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromUnicode(const Py_UNICODE* u, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromStringAndSize(const char* u, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromFormat(const char* format, va_list  _ap_list) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromFormatV(const char* format, va_list  vargs) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_TransformDecimalToASCII(Py_UNICODE* s, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyUnicode_GetSize(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromEncodedObject(R_object obj, const char* encoding, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromObject(R_object obj) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeFSDefault(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_FromWideChar(const wchar_t* w, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Encode(const Py_UNICODE* s, Py_ssize_t  size, const char* encoding, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsEncodedString(R_object unicode, const char* encoding, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeUTF8(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsUTF8String(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeUTF32(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsUTF32String(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeUTF16(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsUTF16String(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeUTF7(const Py_UNICODE* s, Py_ssize_t  size, int  base64SetO, int  base64WhiteSpace, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsUnicodeEscapeString(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeRawUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsRawUnicodeEscapeString(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeLatin1(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsLatin1String(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeASCII(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsASCIIString(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeCharmap(const Py_UNICODE* s, Py_ssize_t  size, R_object mapping, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsCharmapString(R_object unicode, R_object mapping) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_TranslateCharmap(const Py_UNICODE* s, Py_ssize_t  size, R_object table, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_EncodeMBCS(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_AsMBCSString(R_object unicode) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Concat(R_object left, R_object right) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Split(R_object s, R_object sep, Py_ssize_t  maxsplit) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Splitlines(R_object s, int  keepend) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Translate(R_object str, R_object table, const char* errors) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Join(R_object separator, R_object seq) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyUnicode_Tailmatch(R_object str, R_object substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyUnicode_Find(R_object str, R_object substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) {
    raise_char(type_InternalError, "Not implemented");
}

Py_ssize_t HotPyUnicode_Count(R_object str, R_object substr, Py_ssize_t  start, Py_ssize_t  end) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Replace(R_object str, R_object substr, R_object replstr, Py_ssize_t  maxcount) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyUnicode_Compare(R_object left, R_object right) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyUnicode_CompareWithASCIIString(R_object uni, char* string) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyUnicode_RichCompare(R_object left, R_object right, int  op) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_Format(R_object format, R_object args) {
    raise_char(type_InternalError, "Not implemented");
}

void HotPyUnicode_Contains(R_object container, R_object element) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyUnicode_InternFromString(const char* v) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/veryhigh.rst */
R_object HotPyRun_String(const char* str, int  start, R_object globals, R_object locals) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyRun_StringFlags(const char* str, int  start, R_object globals, R_object locals, PyCompilerFlags* flags) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyRun_File(FILE* fp, const char* filename, int  start, R_object globals, R_object locals) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyRun_FileEx(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, int  closeit) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyRun_FileFlags(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, PyCompilerFlags* flags) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyRun_FileExFlags(FILE* fp, const char* filename, int  start, R_object globals, R_object locals, int  closeit, PyCompilerFlags* flags) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPy_CompileString(const char* str, const char* filename, int  start) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPy_CompileStringFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPy_CompileStringExFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags, int  optimize) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyEval_EvalCode(R_object co, R_object globals, R_object locals) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyEval_EvalCodeEx(R_object co, R_object globals, R_object locals, PyObject** args, int  argcount, PyObject** kws, int  kwcount, PyObject** defs, int  defcount, R_object closure) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyEval_EvalFrame(R_frame f) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyEval_EvalFrameEx(R_frame f, int  throwflag) {
    raise_char(type_InternalError, "Not implemented");
}



/* /home/mark/Python-3.2b2/Doc/c-api/weakref.rst */
R_object HotPyWeakref_NewRef(R_object ob, R_object callback) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyWeakref_NewProxy(R_object ob, R_object callback) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyWeakref_GetObject(R_object ref) {
    raise_char(type_InternalError, "Not implemented");
}

R_object HotPyWeakref_GET_OBJECT(R_object ref) {
    raise_char(type_InternalError, "Not implemented");
}

