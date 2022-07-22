/** This file is auto-generated */
#include <stdarg.h>
#include <wchar.h>


/* /home/mark/Python-3.2b2/Doc/c-api/allocation.rst */
PyObject* PyObject_Init(PyObject* op, PyTypeObject* type) ;

PyVarObject* PyObject_InitVar(PyVarObject* op, PyTypeObject* type, Py_ssize_t  size) ;

void PyObject_Del(PyObject* op) ;



/* /home/mark/Python-3.2b2/Doc/c-api/arg.rst */
int PyArg_ParseTuple(PyObject* args, const char* format, ... ) ;

int PyArg_VaParse(PyObject* args, const char* format, va_list  vargs) ;

int PyArg_ParseTupleAndKeywords(PyObject* args, PyObject* kw, const char* format, char* keywords, ... ) ;

int PyArg_VaParseTupleAndKeywords(PyObject* args, PyObject* kw, const char* format, char* keywords, va_list  vargs) ;

int PyArg_ValidateKeywordArguments(PyObject* p0) ;

int PyArg_Parse(PyObject* args, const char* format, ... ) ;

int PyArg_UnpackTuple(PyObject* args, const char* name, Py_ssize_t  min, Py_ssize_t  max, ... ) ;

PyObject* Py_BuildValue(const char* format, ... ) ;

PyObject* Py_VaBuildValue(const char* format, va_list  vargs) ;



/* /home/mark/Python-3.2b2/Doc/c-api/bytearray.rst */
int PyByteArray_Check(PyObject* o) ;

int PyByteArray_CheckExact(PyObject* o) ;

PyObject* PyByteArray_FromObject(PyObject* o) ;

PyObject* PyByteArray_FromStringAndSize(const char* string, Py_ssize_t  len) ;

PyObject* PyByteArray_Concat(PyObject* a, PyObject* b) ;

Py_ssize_t PyByteArray_Size(PyObject* bytearray) ;

char* PyByteArray_AsString(PyObject* bytearray) ;

int PyByteArray_Resize(PyObject* bytearray, Py_ssize_t  len) ;

char* PyByteArray_AS_STRING(PyObject* bytearray) ;



/* /home/mark/Python-3.2b2/Doc/c-api/bytes.rst */
int PyBytes_Check(PyObject* o) ;

int PyBytes_CheckExact(PyObject* o) ;

PyObject* PyBytes_FromString(const char* v) ;

PyObject* PyBytes_FromStringAndSize(const char* v, Py_ssize_t  len) ;

PyObject* PyBytes_FromFormat(const char* format, ... ) ;

PyObject* PyBytes_FromFormatV(const char* format, va_list  vargs) ;

PyObject* PyBytes_FromObject(PyObject* o) ;

Py_ssize_t PyBytes_Size(PyObject* o) ;

char* PyBytes_AsString(PyObject* o) ;

char* PyBytes_AS_STRING(PyObject* string) ;

int PyBytes_AsStringAndSize(PyObject* obj, char** buffer, Py_ssize_t* length) ;

void PyBytes_Concat(PyObject** bytes, PyObject* newpart) ;

void PyBytes_ConcatAndDel(PyObject** bytes, PyObject* newpart) ;



/* /home/mark/Python-3.2b2/Doc/c-api/capsule.rst */
int PyCapsule_CheckExact(PyObject* p) ;

PyObject* PyCapsule_New(void* pointer, const char* name, PyCapsule_Destructor  destructor) ;

void* PyCapsule_GetPointer(PyObject* capsule, const char* name) ;

void* PyCapsule_GetContext(PyObject* capsule) ;

const char* PyCapsule_GetName(PyObject* capsule) ;

int PyCapsule_IsValid(PyObject* capsule, const char* name) ;

int PyCapsule_SetContext(PyObject* capsule, void* context) ;

int PyCapsule_SetDestructor(PyObject* capsule, PyCapsule_Destructor  destructor) ;

int PyCapsule_SetName(PyObject* capsule, const char* name) ;

int PyCapsule_SetPointer(PyObject* capsule, void* pointer) ;



/* /home/mark/Python-3.2b2/Doc/c-api/complex.rst */
int PyComplex_Check(PyObject* p) ;

int PyComplex_CheckExact(PyObject* p) ;

PyObject* PyComplex_FromDoubles(double  real, double  imag) ;



/* /home/mark/Python-3.2b2/Doc/c-api/dict.rst */
int PyDict_Check(PyObject* p) ;

int PyDict_CheckExact(PyObject* p) ;

PyObject* PyDictProxy_New(PyObject* dict) ;

void PyDict_Clear(PyObject* p) ;

int PyDict_Contains(PyObject* p, PyObject* key) ;

PyObject* PyDict_Copy(PyObject* p) ;

int PyDict_SetItem(PyObject* p, PyObject* key, PyObject* val) ;

int PyDict_SetItemString(PyObject* p, const char* key, PyObject* val) ;

int PyDict_DelItem(PyObject* p, PyObject* key) ;

int PyDict_DelItemString(PyObject* p, char* key) ;

PyObject* PyDict_GetItem(PyObject* p, PyObject* key) ;

PyObject* PyDict_GetItemWithError(PyObject* p, PyObject* key) ;

PyObject* PyDict_GetItemString(PyObject* p, const char* key) ;

PyObject* PyDict_Items(PyObject* p) ;

PyObject* PyDict_Keys(PyObject* p) ;

PyObject* PyDict_Values(PyObject* p) ;

Py_ssize_t PyDict_Size(PyObject* p) ;

int PyDict_Next(PyObject* p, Py_ssize_t* ppos, PyObject** pkey, PyObject** pvalue) ;

int PyDict_Merge(PyObject* a, PyObject* b, int  override) ;

int PyDict_Update(PyObject* a, PyObject* b) ;

int PyDict_MergeFromSeq2(PyObject* a, PyObject* seq2, int  override) ;



/* /home/mark/Python-3.2b2/Doc/c-api/exceptions.rst */
int PyErr_ExceptionMatches(PyObject* exc) ;

int PyErr_GivenExceptionMatches(PyObject* given, PyObject* exc) ;

void PyErr_Restore(PyObject* type, PyObject* value, PyObject* traceback) ;

void PyErr_SetString(PyObject* type, const char* message) ;

void PyErr_SetObject(PyObject* type, PyObject* value) ;

void PyErr_SetNone(PyObject* type) ;

PyObject* PyErr_SetFromErrno(PyObject* type) ;

PyObject* PyErr_SetFromErrnoWithFilename(PyObject* type, const char* filename) ;

PyObject* PyErr_SetFromWindowsErr(int  ierr) ;

PyObject* PyErr_SetExcFromWindowsErr(PyObject* type, int  ierr) ;

PyObject* PyErr_SetFromWindowsErrWithFilename(int  ierr, const char* filename) ;

PyObject* PyErr_SetExcFromWindowsErrWithFilename(PyObject* type, int  ierr, char* filename) ;

int PyErr_WarnEx(PyObject* category, char* message, int  stack_level) ;

int PyErr_WarnExplicit(PyObject* category, const char* message, const char* filename, int  lineno, const char* module, PyObject* registry) ;

int PyErr_WarnFormat(PyObject* category, Py_ssize_t  stack_level, const char* format, ... ) ;

PyObject* PyErr_NewException(char* name, PyObject* base, PyObject* dict) ;

PyObject* PyErr_NewExceptionWithDoc(char* name, char* doc, PyObject* base, PyObject* dict) ;

void PyErr_WriteUnraisable(PyObject* obj) ;

PyObject* PyException_GetTraceback(PyObject* ex) ;

int PyException_SetTraceback(PyObject* ex, PyObject* tb) ;

PyObject* PyException_GetContext(PyObject* ex) ;

void PyException_SetContext(PyObject* ex, PyObject* ctx) ;

PyObject* PyException_GetCause(PyObject* ex) ;

void PyException_SetCause(PyObject* ex, PyObject* ctx) ;

PyObject* PyUnicodeDecodeError_Create(const char* encoding, const char* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) ;

PyObject* PyUnicodeEncodeError_Create(const char* encoding, const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) ;

PyObject* PyUnicodeTranslateError_Create(const Py_UNICODE* object, Py_ssize_t  length, Py_ssize_t  start, Py_ssize_t  end, const char* reason) ;

PyObject* PyUnicodeDecodeError_GetEncoding(PyObject* exc) ;

PyObject* PyUnicodeDecodeError_GetObject(PyObject* exc) ;

int PyUnicodeDecodeError_GetStart(PyObject* exc, Py_ssize_t* start) ;

int PyUnicodeDecodeError_SetStart(PyObject* exc, Py_ssize_t  start) ;

int PyUnicodeDecodeError_GetEnd(PyObject* exc, Py_ssize_t* end) ;

int PyUnicodeDecodeError_SetEnd(PyObject* exc, Py_ssize_t  end) ;

PyObject* PyUnicodeDecodeError_GetReason(PyObject* exc) ;

int PyUnicodeDecodeError_SetReason(PyObject* exc, const char* reason) ;

int Py_ReprEnter(PyObject* object) ;

void Py_ReprLeave(PyObject* object) ;



/* /home/mark/Python-3.2b2/Doc/c-api/file.rst */
int PyObject_AsFileDescriptor(PyObject* p) ;

PyObject* PyFile_GetLine(PyObject* p, int  n) ;

int PyFile_WriteObject(PyObject* obj, PyObject* p, int  flags) ;

int PyFile_WriteString(const char* s, PyObject* p) ;



/* /home/mark/Python-3.2b2/Doc/c-api/float.rst */
int PyFloat_Check(PyObject* p) ;

int PyFloat_CheckExact(PyObject* p) ;

PyObject* PyFloat_FromString(PyObject* str) ;

PyObject* PyFloat_FromDouble(double  v) ;



/* /home/mark/Python-3.2b2/Doc/c-api/gen.rst */
PyObject* PyGen_New(PyFrameObject* frame) ;



/* /home/mark/Python-3.2b2/Doc/c-api/import.rst */
PyObject* PyImport_ImportModule(const char* name) ;

PyObject* PyImport_ImportModuleNoBlock(const char* name) ;

PyObject* PyImport_ImportModuleEx(char* name, PyObject* globals, PyObject* locals, PyObject* fromlist) ;

PyObject* PyImport_ImportModuleLevel(char* name, PyObject* globals, PyObject* locals, PyObject* fromlist, int  level) ;

PyObject* PyImport_Import(PyObject* name) ;

PyObject* PyImport_ReloadModule(PyObject* m) ;

PyObject* PyImport_AddModule(const char* name) ;

PyObject* PyImport_ExecCodeModule(char* name, PyObject* co) ;

PyObject* PyImport_ExecCodeModuleEx(char* name, PyObject* co, char* pathname) ;

PyObject* PyImport_ExecCodeModuleWithPathnames(char* name, PyObject* co, char* pathname, char* cpathname) ;

PyObject* PyImport_GetImporter(PyObject* path) ;



/* /home/mark/Python-3.2b2/Doc/c-api/init.rst */
int PyThreadState_SetAsyncExc(long  id, PyObject* exc) ;

void PyEval_SetProfile(Py_tracefunc  func, PyObject* obj) ;

void PyEval_SetTrace(Py_tracefunc  func, PyObject* obj) ;

PyObject* PyEval_GetCallStats(PyObject* self) ;



/* /home/mark/Python-3.2b2/Doc/c-api/iterator.rst */
PyObject* PySeqIter_New(PyObject* seq) ;

PyObject* PyCallIter_New(PyObject* callable, PyObject* sentinel) ;



/* /home/mark/Python-3.2b2/Doc/c-api/iter.rst */
int PyIter_Check(PyObject* o) ;

PyObject* PyIter_Next(PyObject* o) ;



/* /home/mark/Python-3.2b2/Doc/c-api/list.rst */
int PyList_Check(PyObject* p) ;

int PyList_CheckExact(PyObject* p) ;

PyObject* PyList_New(Py_ssize_t  len) ;

Py_ssize_t PyList_Size(PyObject* list) ;

Py_ssize_t PyList_GET_SIZE(PyObject* list) ;

PyObject* PyList_GetItem(PyObject* list, Py_ssize_t  index) ;

PyObject* PyList_GET_ITEM(PyObject* list, Py_ssize_t  i) ;

int PyList_SetItem(PyObject* list, Py_ssize_t  index, PyObject* item) ;

void PyList_SET_ITEM(PyObject* list, Py_ssize_t  i, PyObject* o) ;

int PyList_Insert(PyObject* list, Py_ssize_t  index, PyObject* item) ;

int PyList_Append(PyObject* list, PyObject* item) ;

PyObject* PyList_GetSlice(PyObject* list, Py_ssize_t  low, Py_ssize_t  high) ;

int PyList_SetSlice(PyObject* list, Py_ssize_t  low, Py_ssize_t  high, PyObject* itemlist) ;

int PyList_Sort(PyObject* list) ;

int PyList_Reverse(PyObject* list) ;

PyObject* PyList_AsTuple(PyObject* list) ;



/* /home/mark/Python-3.2b2/Doc/c-api/long.rst */
int PyLong_Check(PyObject* p) ;

int PyLong_CheckExact(PyObject* p) ;

PyObject* PyLong_FromLong(long  v) ;

PyObject* PyLong_FromUnsignedLong(unsigned long p0) ;

PyObject* PyLong_FromSsize_t(Py_ssize_t  v) ;

PyObject* PyLong_FromSize_t(size_t  v) ;

PyObject* PyLong_FromLongLong(PY_LONG_LONG  v) ;

PyObject* PyLong_FromUnsignedLongLong(unsigned  PY_LONG_LONG) ;

PyObject* PyLong_FromDouble(double  v) ;

PyObject* PyLong_FromString(char* str, char** pend, int  base) ;

PyObject* PyLong_FromUnicode(Py_UNICODE* u, Py_ssize_t  length, int  base) ;

PyObject* PyLong_FromVoidPtr(void* p) ;

long PyLong_AsLong(PyObject* pylong) ;

long PyLong_AsLongAndOverflow(PyObject* pylong, int* overflow) ;

Py_ssize_t PyLong_AsSsize_t(PyObject* pylong) ;

void* PyLong_AsVoidPtr(PyObject* pylong) ;



/* /home/mark/Python-3.2b2/Doc/c-api/mapping.rst */
int PyMapping_Check(PyObject* o) ;

Py_ssize_t PyMapping_Size(PyObject* o) ;

int PyMapping_DelItemString(PyObject* o, char* key) ;

int PyMapping_DelItem(PyObject* o, PyObject* key) ;

int PyMapping_HasKeyString(PyObject* o, char* key) ;

int PyMapping_HasKey(PyObject* o, PyObject* key) ;

PyObject* PyMapping_Keys(PyObject* o) ;

PyObject* PyMapping_Values(PyObject* o) ;

PyObject* PyMapping_Items(PyObject* o) ;

PyObject* PyMapping_GetItemString(PyObject* o, char* key) ;

int PyMapping_SetItemString(PyObject* o, char* key, PyObject* v) ;



/* /home/mark/Python-3.2b2/Doc/c-api/marshal.rst */
void PyMarshal_WriteObjectToFile(PyObject* value, FILE* file, int  version) ;

PyObject* PyMarshal_WriteObjectToString(PyObject* value, int  version) ;

PyObject* PyMarshal_ReadObjectFromFile(FILE* file) ;

PyObject* PyMarshal_ReadLastObjectFromFile(FILE* file) ;

PyObject* PyMarshal_ReadObjectFromString(char* string, Py_ssize_t  len) ;



/* /home/mark/Python-3.2b2/Doc/c-api/method.rst */
int PyInstanceMethod_Check(PyObject* o) ;

PyObject* PyInstanceMethod_New(PyObject* func) ;

PyObject* PyInstanceMethod_Function(PyObject* im) ;

PyObject* PyInstanceMethod_GET_FUNCTION(PyObject* im) ;

int PyMethod_Check(PyObject* o) ;

PyObject* PyMethod_New(PyObject* func, PyObject* self) ;

PyObject* PyMethod_Function(PyObject* meth) ;

PyObject* PyMethod_GET_FUNCTION(PyObject* meth) ;

PyObject* PyMethod_Self(PyObject* meth) ;

PyObject* PyMethod_GET_SELF(PyObject* meth) ;



/* /home/mark/Python-3.2b2/Doc/c-api/module.rst */
int PyModule_Check(PyObject* p) ;

int PyModule_CheckExact(PyObject* p) ;

PyObject* PyModule_New(const char* name) ;

PyObject* PyModule_GetDict(PyObject* module) ;

char* PyModule_GetName(PyObject* module) ;

char* PyModule_GetFilename(PyObject* module) ;

PyObject* PyModule_GetFilenameObject(PyObject* module) ;

void* PyModule_GetState(PyObject* module) ;

PyObject* PyModule_Create(PyModuleDef* module) ;

PyObject* PyModule_Create2(PyModuleDef* module, int  module_api_version) ;

int PyModule_AddObject(PyObject* module, const char* name, PyObject* value) ;

int PyModule_AddIntConstant(PyObject* module, const char* name, long  value) ;

int PyModule_AddStringConstant(PyObject* module, const char* name, const char* value) ;



/* /home/mark/Python-3.2b2/Doc/c-api/number.rst */
int PyNumber_Check(PyObject* o) ;

PyObject* PyNumber_Add(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Subtract(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Multiply(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_FloorDivide(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_TrueDivide(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Remainder(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Divmod(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Power(PyObject* o1, PyObject* o2, PyObject* o3) ;

PyObject* PyNumber_Negative(PyObject* o) ;

PyObject* PyNumber_Positive(PyObject* o) ;

PyObject* PyNumber_Absolute(PyObject* o) ;

PyObject* PyNumber_Invert(PyObject* o) ;

PyObject* PyNumber_Lshift(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Rshift(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_And(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Xor(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Or(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceAdd(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceSubtract(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceMultiply(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceFloorDivide(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceTrueDivide(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceRemainder(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlacePower(PyObject* o1, PyObject* o2, PyObject* o3) ;

PyObject* PyNumber_InPlaceLshift(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceRshift(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceAnd(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceXor(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_InPlaceOr(PyObject* o1, PyObject* o2) ;

PyObject* PyNumber_Long(PyObject* o) ;

PyObject* PyNumber_Float(PyObject* o) ;

PyObject* PyNumber_Index(PyObject* o) ;

PyObject* PyNumber_ToBase(PyObject* n, int  base) ;

Py_ssize_t PyNumber_AsSsize_t(PyObject* o, PyObject* exc) ;

int PyIndex_Check(PyObject* o) ;



/* /home/mark/Python-3.2b2/Doc/c-api/object.rst */
int PyObject_Print(PyObject* o, FILE* fp, int  flags) ;

int PyObject_HasAttr(PyObject* o, PyObject* attr_name) ;

int PyObject_HasAttrString(PyObject* o, const char* attr_name) ;

PyObject* PyObject_GetAttr(PyObject* o, PyObject* attr_name) ;

PyObject* PyObject_GetAttrString(PyObject* o, const char* attr_name) ;

PyObject* PyObject_GenericGetAttr(PyObject* o, PyObject* name) ;

int PyObject_SetAttr(PyObject* o, PyObject* attr_name, PyObject* v) ;

int PyObject_SetAttrString(PyObject* o, const char* attr_name, PyObject* v) ;

int PyObject_GenericSetAttr(PyObject* o, PyObject* name, PyObject* value) ;

int PyObject_DelAttr(PyObject* o, PyObject* attr_name) ;

int PyObject_DelAttrString(PyObject* o, const char* attr_name) ;

PyObject* PyObject_RichCompare(PyObject* o1, PyObject* o2, int  opid) ;

int PyObject_RichCompareBool(PyObject* o1, PyObject* o2, int  opid) ;

PyObject* PyObject_Repr(PyObject* o) ;

PyObject* PyObject_Str(PyObject* o) ;

int PyObject_IsInstance(PyObject* inst, PyObject* cls) ;

int PyObject_IsSubclass(PyObject* derived, PyObject* cls) ;

int PyCallable_Check(PyObject* o) ;

PyObject* PyObject_Call(PyObject* callable_object, PyObject* args, PyObject* kw) ;

PyObject* PyObject_CallObject(PyObject* callable_object, PyObject* args) ;

PyObject* PyObject_CallFunction(PyObject* callable, char* format, ... ) ;

PyObject* PyObject_CallMethod(PyObject* o, char* method, char* format, ... ) ;

PyObject* PyObject_CallFunctionObjArgs(PyObject* callable, ... ) ;

PyObject* PyObject_CallMethodObjArgs(PyObject* o, PyObject* name, ... ) ;

int PyObject_IsTrue(PyObject* o) ;

int PyObject_Not(PyObject* o) ;

Py_ssize_t PyObject_Length(PyObject* o) ;

PyObject* PyObject_GetItem(PyObject* o, PyObject* key) ;

int PyObject_SetItem(PyObject* o, PyObject* key, PyObject* v) ;

int PyObject_DelItem(PyObject* o, PyObject* key) ;

PyObject* PyObject_GetIter(PyObject* o) ;



/* /home/mark/Python-3.2b2/Doc/c-api/sequence.rst */
int PySequence_Check(PyObject* o) ;

Py_ssize_t PySequence_Size(PyObject* o) ;

PyObject* PySequence_Concat(PyObject* o1, PyObject* o2) ;

PyObject* PySequence_Repeat(PyObject* o, Py_ssize_t  count) ;

PyObject* PySequence_InPlaceConcat(PyObject* o1, PyObject* o2) ;

PyObject* PySequence_InPlaceRepeat(PyObject* o, Py_ssize_t  count) ;

PyObject* PySequence_GetItem(PyObject* o, Py_ssize_t  i) ;

PyObject* PySequence_GetSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2) ;

int PySequence_SetItem(PyObject* o, Py_ssize_t  i, PyObject* v) ;

int PySequence_DelItem(PyObject* o, Py_ssize_t  i) ;

int PySequence_SetSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2, PyObject* v) ;

int PySequence_DelSlice(PyObject* o, Py_ssize_t  i1, Py_ssize_t  i2) ;

Py_ssize_t PySequence_Count(PyObject* o, PyObject* value) ;

int PySequence_Contains(PyObject* o, PyObject* value) ;

Py_ssize_t PySequence_Index(PyObject* o, PyObject* value) ;

PyObject* PySequence_List(PyObject* o) ;

PyObject* PySequence_Tuple(PyObject* o) ;

PyObject* PySequence_Fast(PyObject* o, const char* m) ;

PyObject* PySequence_ITEM(PyObject* o, Py_ssize_t  i) ;



/* /home/mark/Python-3.2b2/Doc/c-api/set.rst */
int PySet_Check(PyObject* p) ;

int PyFrozenSet_Check(PyObject* p) ;

int PyAnySet_Check(PyObject* p) ;

int PyAnySet_CheckExact(PyObject* p) ;

int PyFrozenSet_CheckExact(PyObject* p) ;

PyObject* PySet_New(PyObject* iterable) ;

PyObject* PyFrozenSet_New(PyObject* iterable) ;

Py_ssize_t PySet_Size(PyObject* anyset) ;

Py_ssize_t PySet_GET_SIZE(PyObject* anyset) ;

int PySet_Contains(PyObject* anyset, PyObject* key) ;

int PySet_Add(PyObject* set, PyObject* key) ;

int PySet_Discard(PyObject* set, PyObject* key) ;

PyObject* PySet_Pop(PyObject* set) ;

int PySet_Clear(PyObject* set) ;



/* /home/mark/Python-3.2b2/Doc/c-api/slice.rst */
int PySlice_Check(PyObject* ob) ;

PyObject* PySlice_New(PyObject* start, PyObject* stop, PyObject* step) ;

int PySlice_GetIndices(PyObject* slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step) ;

int PySlice_GetIndicesEx(PyObject* slice, Py_ssize_t  length, Py_ssize_t* start, Py_ssize_t* stop, Py_ssize_t* step, Py_ssize_t* slicelength) ;



/* /home/mark/Python-3.2b2/Doc/c-api/sys.rst */
int PySys_SetObject(char* name, PyObject* v) ;

void PySys_AddWarnOptionUnicode(PyObject* unicode) ;



/* /home/mark/Python-3.2b2/Doc/c-api/tuple.rst */
int PyTuple_Check(PyObject* p) ;

int PyTuple_CheckExact(PyObject* p) ;

PyObject* PyTuple_New(Py_ssize_t  len) ;

PyObject* PyTuple_Pack(Py_ssize_t  n, ... ) ;

Py_ssize_t PyTuple_Size(PyObject* p) ;

PyObject* PyTuple_GetItem(PyObject* p, Py_ssize_t  pos) ;

PyObject* PyTuple_GET_ITEM(PyObject* p, Py_ssize_t  pos) ;

PyObject* PyTuple_GetSlice(PyObject* p, Py_ssize_t  low, Py_ssize_t  high) ;

int PyTuple_SetItem(PyObject* p, Py_ssize_t  pos, PyObject* o) ;

void PyTuple_SET_ITEM(PyObject* p, Py_ssize_t  pos, PyObject* o) ;



/* /home/mark/Python-3.2b2/Doc/c-api/type.rst */
int PyType_Check(PyObject* o) ;

int PyType_CheckExact(PyObject* o) ;

void PyType_Modified(PyTypeObject* type) ;

int PyType_HasFeature(PyObject* o, int  feature) ;

int PyType_IS_GC(PyObject* o) ;

int PyType_IsSubtype(PyTypeObject* a, PyTypeObject* b) ;

PyObject* PyType_GenericAlloc(PyTypeObject* type, Py_ssize_t  nitems) ;

PyObject* PyType_GenericNew(PyTypeObject* type, PyObject* args, PyObject* kwds) ;

int PyType_Ready(PyTypeObject* type) ;



/* /home/mark/Python-3.2b2/Doc/c-api/unicode.rst */
int PyUnicode_Check(PyObject* o) ;

int PyUnicode_CheckExact(PyObject* o) ;

Py_ssize_t PyUnicode_GET_DATA_SIZE(PyObject* o) ;

const char* PyUnicode_AS_DATA(PyObject* o) ;

PyObject* PyUnicode_FromUnicode(const Py_UNICODE* u, Py_ssize_t  size) ;

PyObject* PyUnicode_FromStringAndSize(const char* u, Py_ssize_t  size) ;

PyObject* PyUnicode_FromFormat(const char* format, ... ) ;

PyObject* PyUnicode_FromFormatV(const char* format, va_list  vargs) ;

PyObject* PyUnicode_TransformDecimalToASCII(Py_UNICODE* s, Py_ssize_t  size) ;

Py_ssize_t PyUnicode_GetSize(PyObject* unicode) ;

PyObject* PyUnicode_FromEncodedObject(PyObject* obj, const char* encoding, const char* errors) ;

PyObject* PyUnicode_FromObject(PyObject* obj) ;

PyObject* PyUnicode_DecodeFSDefaultAndSize(const char* s, Py_ssize_t  size) ;

PyObject* PyUnicode_DecodeFSDefault(const char* s) ;

PyObject* PyUnicode_EncodeFSDefault(PyObject* unicode) ;

PyObject* PyUnicode_FromWideChar(const wchar_t* w, Py_ssize_t  size) ;

PyObject* PyUnicode_Decode(const char* s, Py_ssize_t  size, const char* encoding, const char* errors) ;

PyObject* PyUnicode_Encode(const Py_UNICODE* s, Py_ssize_t  size, const char* encoding, const char* errors) ;

PyObject* PyUnicode_AsEncodedString(PyObject* unicode, const char* encoding, const char* errors) ;

PyObject* PyUnicode_DecodeUTF8(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_DecodeUTF8Stateful(const char* s, Py_ssize_t  size, const char* errors, Py_ssize_t* consumed) ;

PyObject* PyUnicode_EncodeUTF8(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_AsUTF8String(PyObject* unicode) ;

PyObject* PyUnicode_DecodeUTF32(const char* s, Py_ssize_t  size, const char* errors, int* byteorder) ;

PyObject* PyUnicode_DecodeUTF32Stateful(const char* s, Py_ssize_t  size, const char* errors, int* byteorder, Py_ssize_t* consumed) ;

PyObject* PyUnicode_EncodeUTF32(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) ;

PyObject* PyUnicode_AsUTF32String(PyObject* unicode) ;

PyObject* PyUnicode_DecodeUTF16(const char* s, Py_ssize_t  size, const char* errors, int* byteorder) ;

PyObject* PyUnicode_DecodeUTF16Stateful(const char* s, Py_ssize_t  size, const char* errors, int* byteorder, Py_ssize_t* consumed) ;

PyObject* PyUnicode_EncodeUTF16(const Py_UNICODE* s, Py_ssize_t  size, const char* errors, int  byteorder) ;

PyObject* PyUnicode_AsUTF16String(PyObject* unicode) ;

PyObject* PyUnicode_DecodeUTF7(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_DecodeUTF7Stateful(const char* s, Py_ssize_t  size, const char* errors, Py_ssize_t* consumed) ;

PyObject* PyUnicode_EncodeUTF7(const Py_UNICODE* s, Py_ssize_t  size, int  base64SetO, int  base64WhiteSpace, const char* errors) ;

PyObject* PyUnicode_DecodeUnicodeEscape(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_EncodeUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size) ;

PyObject* PyUnicode_AsUnicodeEscapeString(PyObject* unicode) ;

PyObject* PyUnicode_DecodeRawUnicodeEscape(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_EncodeRawUnicodeEscape(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_AsRawUnicodeEscapeString(PyObject* unicode) ;

PyObject* PyUnicode_DecodeLatin1(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_EncodeLatin1(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_AsLatin1String(PyObject* unicode) ;

PyObject* PyUnicode_DecodeASCII(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_EncodeASCII(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_AsASCIIString(PyObject* unicode) ;

PyObject* PyUnicode_DecodeCharmap(const char* s, Py_ssize_t  size, PyObject* mapping, const char* errors) ;

PyObject* PyUnicode_EncodeCharmap(const Py_UNICODE* s, Py_ssize_t  size, PyObject* mapping, const char* errors) ;

PyObject* PyUnicode_AsCharmapString(PyObject* unicode, PyObject* mapping) ;

PyObject* PyUnicode_TranslateCharmap(const Py_UNICODE* s, Py_ssize_t  size, PyObject* table, const char* errors) ;

PyObject* PyUnicode_DecodeMBCS(const char* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_DecodeMBCSStateful(const char* s, int  size, const char* errors, int* consumed) ;

PyObject* PyUnicode_EncodeMBCS(const Py_UNICODE* s, Py_ssize_t  size, const char* errors) ;

PyObject* PyUnicode_AsMBCSString(PyObject* unicode) ;

PyObject* PyUnicode_Concat(PyObject* left, PyObject* right) ;

PyObject* PyUnicode_Split(PyObject* s, PyObject* sep, Py_ssize_t  maxsplit) ;

PyObject* PyUnicode_Splitlines(PyObject* s, int  keepend) ;

PyObject* PyUnicode_Translate(PyObject* str, PyObject* table, const char* errors) ;

PyObject* PyUnicode_Join(PyObject* separator, PyObject* seq) ;

int PyUnicode_Tailmatch(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) ;

Py_ssize_t PyUnicode_Find(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end, int  direction) ;

Py_ssize_t PyUnicode_Count(PyObject* str, PyObject* substr, Py_ssize_t  start, Py_ssize_t  end) ;

PyObject* PyUnicode_Replace(PyObject* str, PyObject* substr, PyObject* replstr, Py_ssize_t  maxcount) ;

int PyUnicode_Compare(PyObject* left, PyObject* right) ;

int PyUnicode_CompareWithASCIIString(PyObject* uni, char* string) ;

int PyUnicode_RichCompare(PyObject* left, PyObject* right, int  op) ;

PyObject* PyUnicode_Format(PyObject* format, PyObject* args) ;

int PyUnicode_Contains(PyObject* container, PyObject* element) ;

PyObject* PyUnicode_InternFromString(const char* v) ;



/* /home/mark/Python-3.2b2/Doc/c-api/veryhigh.rst */
PyObject* PyRun_String(const char* str, int  start, PyObject* globals, PyObject* locals) ;

PyObject* PyRun_StringFlags(const char* str, int  start, PyObject* globals, PyObject* locals, PyCompilerFlags* flags) ;

PyObject* PyRun_File(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals) ;

PyObject* PyRun_FileEx(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, int  closeit) ;

PyObject* PyRun_FileFlags(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, PyCompilerFlags* flags) ;

PyObject* PyRun_FileExFlags(FILE* fp, const char* filename, int  start, PyObject* globals, PyObject* locals, int  closeit, PyCompilerFlags* flags) ;

PyObject* Py_CompileString(const char* str, const char* filename, int  start) ;

PyObject* Py_CompileStringFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags) ;

PyObject* Py_CompileStringExFlags(const char* str, const char* filename, int  start, PyCompilerFlags* flags, int  optimize) ;

PyObject* PyEval_EvalCode(PyObject* co, PyObject* globals, PyObject* locals) ;

PyObject* PyEval_EvalCodeEx(PyObject* co, PyObject* globals, PyObject* locals, PyObject** args, int  argcount, PyObject** kws, int  kwcount, PyObject** defs, int  defcount, PyObject* closure) ;

PyObject* PyEval_EvalFrame(PyFrameObject* f) ;

PyObject* PyEval_EvalFrameEx(PyFrameObject* f, int  throwflag) ;



/* /home/mark/Python-3.2b2/Doc/c-api/weakref.rst */
PyObject* PyWeakref_NewRef(PyObject* ob, PyObject* callback) ;

PyObject* PyWeakref_NewProxy(PyObject* ob, PyObject* callback) ;

PyObject* PyWeakref_GetObject(PyObject* ref) ;

PyObject* PyWeakref_GET_OBJECT(PyObject* ref) ;

