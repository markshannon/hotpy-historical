#include "gvmt/native.h"
#include "py_api/Python.h"
#include <assert.h>
#include <stdarg.h>

int is_ref_counted(PyObject* op) {
    PyTypeObject* t = op->dont_access_this_directly_type;
    return t->flags & REF_COUNT_FLAG;
}

typedef struct py_array {
    PyVarObject header;
    PyObject* array[1];
} py_array;

extern py_array *pinned_object_array;
static size_t used = 0;

#define REF_COUNT(x) (x)->dont_access_this_directly_ref_count

int resize_pinned_object_array(void);

void hold_ref_counted_object(PyObject* op) {
    assert(used <= Py_SIZE(&(pinned_object_array->header)));
    if (used == Py_SIZE(&(pinned_object_array->header))) {
        gvmt_call(resize_pinned_object_array, 1, pinned_object_array);
    }
    assert(used < Py_SIZE(&(pinned_object_array->header)));
    pinned_object_array->array[used] = op;
    assert(REF_COUNT(op) == 0);
    REF_COUNT(op) = used;
    used++;    
}

void release_ref_counted_object(PyObject* op) {
    PyObject* end;
    used--;
    end = pinned_object_array->array[used];
    assert((REF_COUNT(op)& 0xfff00000) == 0);
    pinned_object_array->array[REF_COUNT(op)] = end;
    REF_COUNT(end) = (REF_COUNT(end) & 0xfff00000) | REF_COUNT(op);
    REF_COUNT(op) = 0;
}

void _make_immortal(PyObject* op) {
    if ((REF_COUNT(op)  & 0xfff00000) == 0) { 
        hold_ref_counted_object((PyObject*)(op));
    }
    REF_COUNT(op) = IMMORTAL | (0xfffff & REF_COUNT(op));
}

/******* API function Template *********/
//
//   PyObject* PyObject_XXXX(PyObject* arg1, PyObject* arg2) {
//       If ref is stolen: DEC_REF(arg1)
//       R_object o1 = (R_object)gvmt_pinned_object(arg1);
//       R_object o2 = (R_object)gvmt_pinned_object(arg2);
//       R_object o3 gvmt_reenter(HotPy_XXXX, 2, arg1, arg2);
//       Must return a new reference. If it does not then
//       need a new api func that does.
//       return inc_ref_count(o3);
//   }
//

typedef PyObject* (*module_init_func)(void);

//PyObject import_native_module(char* name) {
//    char buf[400];
//    sprintf(buf, "PyInit_%s", name);
//    void* ptr = dlsym(symbols, buf);
//    return ((module_init_func)ptr)();
//}

/* Copied (verbatim) from CPython source */
PyObject *
PyErr_Format(PyObject *exception, const char *format, ...)
{
	va_list vargs;
	PyObject* string;
	va_start(vargs, format);
	string = PyUnicode_FromFormatV(format, vargs);
	PyErr_SetObject(exception, string);
	Py_XDECREF(string);
	va_end(vargs);
	return NULL;
}

PyAPI_FUNC(long) PyObject_HashNotImplemented(PyObject *v) {
    const char* type_name = (((intptr_t)v) & 1) ? "int" :
                      v->dont_access_this_directly_type->tp_name;
	PyErr_Format(PyExc_TypeError, "unhashable type: '%.200s'", type_name);
	return -1;
}

void* HotPyAPI_GIL;

