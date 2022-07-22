#ifndef HOTPY_PyAPI_OBJECT_H
#define HOTPY_PyAPI_OBJECT_H
// This is the internal, but native api code.
#include <stdio.h>
#include "hotpy_types.h"

typedef struct PyObject {
	struct _typeobject* dont_access_this_directly_type;
	uintptr_t dont_access_this_directly_ref_count;
} PyObject;

typedef struct {
	PyObject dont_access_this_directly_type;
	Py_ssize_t dont_access_this_directly_size; /* Number of items in variable part */
} PyVarObject;

typedef struct _typeobject {
	struct _typeobject* dont_access_this_directly_type;
	uintptr_t dont_access_this_directly_ref_count;
	uint32_t flags;
    const char* tp_name;
} PyTypeObject;

typedef struct {
    PyVarObject header;
} PyFrameObject;

typedef struct {
    PyObject header;
} PyCodeObject;
    

#define PyObject_VAR_HEAD      PyVarObject ob_base;

typedef PyObject *(*getter)(PyObject *, void *);
typedef int (*setter)(PyObject *, PyObject *, void *);

typedef struct PyGetSetDef {
	char *name;
	getter get;
	setter set;
	char *doc;
	void *closure;
} PyGetSetDef;

typedef struct PyMemberDef {
	char *name;
	int type;
	Py_ssize_t offset;
	int flags;
	char *doc;
} PyMemberDef;

/* Magic code */
#define PyModuleDef_HEAD_INIT {'H','o','t','M','o','d','0','0'}

/* Types */
#define T_SHORT		0
#define T_INT		1
#define T_LONG		2
#define T_FLOAT		3
#define T_DOUBLE	4
#define T_STRING	5
#define T_OBJECT	6

#define T_CHAR		7	/* 1-character string */
#define T_BYTE		8	/* 8-bit signed int */
/* unsigned variants: */
#define T_UBYTE		9
#define T_USHORT	10
#define T_UINT		11
#define T_ULONG		12

#define T_BOOL		14

#define T_OBJECT_EX	16	/* Like T_OBJECT, but raises AttributeError
				   when the value is NULL, instead of
				   converting to None. */
#ifdef HAVE_LONG_LONG
#define T_LONGLONG      17  
#define T_ULONGLONG     18
#endif /* HAVE_LONG_LONG */

#define T_PYSSIZET      19      /* Py_ssize_t */

/* Flags */
#define READONLY	1

#define Py_REFCNT(ob)	(((intptr_t)ob) & 1 ? \
    1 : ((PyObject*)(ob))->dont_access_this_directly_ref_count)
#define Py_TYPE(ob)		(((intptr_t)ob) & 1 ? \
    PyLong_Type : ((PyObject*)(ob))->dont_access_this_directly_type)
#define Py_SIZE(ob)		(((PyVarObject*)(ob))->dont_access_this_directly_size)

typedef PyObject * (*unaryfunc)(PyObject *);
typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef int (*inquiry)(PyObject *);
typedef Py_ssize_t (*lenfunc)(PyObject *);
typedef PyObject *(*ssizeargfunc)(PyObject *, Py_ssize_t);
typedef PyObject *(*ssizessizeargfunc)(PyObject *, Py_ssize_t, Py_ssize_t);
typedef int(*ssizeobjargproc)(PyObject *, Py_ssize_t, PyObject *);
typedef int(*ssizessizeobjargproc)(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);
typedef int(*objobjproc)(PyObject *, PyObject *);
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);

/* buffer interface */
typedef struct bufferinfo {
	void *buf;   
	PyObject *obj;        /* owned reference */
	Py_ssize_t len;
	Py_ssize_t itemsize;  /* This is Py_ssize_t so it can be 
			         pointed to by strides in simple case.*/
	int readonly;
	int ndim;
	char *format;
	Py_ssize_t *shape;
	Py_ssize_t *strides;
	Py_ssize_t *suboffsets;
	Py_ssize_t smalltable[2];  /* static store for shape and strides of
				      mono-dimensional buffers. */
	void *internal;
} Py_buffer;

typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
typedef void (*releasebufferproc)(PyObject *, Py_buffer *);

void hold_ref_counted_object(PyObject* op);
void release_ref_counted_object(PyObject* op);
void _make_immortal(PyObject* op);

/** The top 12 bits of the ref_count field are the actual reference count,
 * which saturates and sticks at (1<<11). ie. objects with (1<<11) ref counts
 * become immortal. 
 * The remaining 20 bits are the index into a global array holding references
 * to ref-counted objects, to prevent GC from collecting them.
 * If more than (1<<20-1)objects are required then raise an Exception.
 */
#define IMMORTAL (1<<31)
#define REF_COUNT_FLAG 2

extern int is_ref_counted(PyObject* op);

#define Py_INCREF(op) if((((intptr_t)op)&1)==0) { \
    int count; \
    assert(is_ref_counted((PyObject*)(op))); \
    count = ((PyObject*)(op))->dont_access_this_directly_ref_count; \
    if (count == 0) { \
        hold_ref_counted_object((PyObject*)(op)); \
    } \
    ((PyObject*)(op))->dont_access_this_directly_ref_count = count + ((count >= 0) << 20); \
}

#define Py_DECREF(op) if((((intptr_t)op)&1)==0) { \
    int count, new_count; \
    assert(is_ref_counted((PyObject*)(op))); \
    count = ((PyObject*)(op))->dont_access_this_directly_ref_count; \
    new_count = count - ((count >= 0) << 20); \
    ((PyObject*)(op))->dont_access_this_directly_ref_count = new_count; \
    if ((new_count & 0xfff00000) == 0) { \
        release_ref_counted_object((PyObject*)(op)); \
    } \
}
    
extern void* HotPyAPI_GIL;
    
#define ACQUIRE_GIL os_mutex_lock(HotPyAPI_GIL);
#define RELEASE_GIL os_mutex_unlock(HotPyAPI_GIL);

/* Macros to use in case the object pointer may be NULL: */
#define Py_XINCREF(op) do { if ((op) == NULL) ; else Py_INCREF(op); } while (0)
#define Py_XDECREF(op) do { if ((op) == NULL) ; else Py_DECREF(op); } while (0)

extern PyObject *Py_None;
extern PyObject *Py_NotImplemented;

/* Rich comparison opcodes */
#define Py_LT 0
#define Py_LE 1
#define Py_EQ 2
#define Py_NE 3
#define Py_GT 4
#define Py_GE 5

/* Lots of stuff for C extensions to create types. */
typedef struct {
	binaryfunc nb_add;
	binaryfunc nb_subtract;
	binaryfunc nb_multiply;
	binaryfunc nb_remainder;
	binaryfunc nb_divmod;
	ternaryfunc nb_power;
	unaryfunc nb_negative;
	unaryfunc nb_positive;
	unaryfunc nb_absolute;
	inquiry nb_bool;
	unaryfunc nb_invert;
	binaryfunc nb_lshift;
	binaryfunc nb_rshift;
	binaryfunc nb_and;
	binaryfunc nb_xor;
	binaryfunc nb_or;
	unaryfunc nb_int;
	void *nb_reserved;  
	unaryfunc nb_float;

	binaryfunc nb_inplace_add;
	binaryfunc nb_inplace_subtract;
	binaryfunc nb_inplace_multiply;
	binaryfunc nb_inplace_remainder;
	ternaryfunc nb_inplace_power;
	binaryfunc nb_inplace_lshift;
	binaryfunc nb_inplace_rshift;
	binaryfunc nb_inplace_and;
	binaryfunc nb_inplace_xor;
	binaryfunc nb_inplace_or;

	binaryfunc nb_floor_divide;
	binaryfunc nb_true_divide;
	binaryfunc nb_inplace_floor_divide;
	binaryfunc nb_inplace_true_divide;

	unaryfunc nb_index;
} PyNumberMethods;

typedef struct {
	lenfunc sq_length;
	binaryfunc sq_concat;
	ssizeargfunc sq_repeat;
	ssizeargfunc sq_item;
	void *was_sq_slice;
	ssizeobjargproc sq_ass_item;
	void *was_sq_ass_slice;
	objobjproc sq_contains;

	binaryfunc sq_inplace_concat;
	ssizeargfunc sq_inplace_repeat;
} PySequenceMethods;

typedef struct {
	lenfunc mp_length;
	binaryfunc mp_subscript;
	objobjargproc mp_ass_subscript;
} PyMappingMethods;

typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);
typedef PyObject *(*PyCFunctionWithKeywords)(PyObject *, PyObject *,
					     PyObject *);
typedef PyObject *(*PyNoArgsFunction)(PyObject *);


#define METH_VARARGS  0x0001
#define METH_KEYWORDS 0x0002
/* METH_NOARGS and METH_O must not be combined with the flags above. */
#define METH_NOARGS   0x0004
#define METH_O        0x0008

/* METH_CLASS and METH_STATIC are a little different; these control
   the construction of methods for a class.  These cannot be used for
   functions in modules. */
#define METH_CLASS    0x0010
#define METH_STATIC   0x0020

/* METH_COEXIST allows a method to be entered even though a slot has
   already filled the entry.  When defined, the flag allows a separate
   method, "__contains__" for example, to coexist with a defined 
   slot like sq_contains. */
#define METH_COEXIST   0x0040

struct PyMethodDef {
    const char	*ml_name;	/* The name of the built-in function/method */
    PyCFunction  ml_meth;	/* The C function that implements it */
    int		 ml_flags;	/* Combination of METH_xxx flags, which mostly
				   describe the args expected by the C func */
    const char	*ml_doc;	/* The __doc__ attribute, or NULL */
};

typedef struct PyMethodDef PyMethodDef;

typedef struct {
     getbufferproc bf_getbuffer;
     releasebufferproc bf_releasebuffer;
} PyBufferProcs;

typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef long (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(struct _typeobject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(struct _typeobject *, Py_ssize_t);

typedef struct PyModuleDef {
    char header[8];
    const char* m_name;
    const char* m_doc;
    Py_ssize_t m_size;
    PyMethodDef *m_methods;
    inquiry m_reload;
    traverseproc m_traverse;
    inquiry m_clear;
    freefunc m_free;
} PyModuleDef;

typedef struct PyTypeDef {
    PyTypeObject* meta_class;
	char header[8];
	const char *tp_name; /* For printing, in format "<module>.<name>" */
	Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */

	/* Methods to implement standard operations */

	destructor tp_dealloc;
	printfunc tp_print;
	getattrfunc tp_getattr;
	setattrfunc tp_setattr;
	void *tp_reserved; /* formerly known as tp_compare */
	reprfunc tp_repr;

	/* Method suites for standard classes */

	PyNumberMethods *tp_as_number;
	PySequenceMethods *tp_as_sequence;
	PyMappingMethods *tp_as_mapping;

	/* More standard operations (here for binary compatibility) */

	hashfunc tp_hash;
	ternaryfunc tp_call;
	reprfunc tp_str;
	getattrofunc tp_getattro;
	setattrofunc tp_setattro;

	/* Functions to access object as input/output buffer */
	PyBufferProcs *tp_as_buffer;

	/* Flags to define presence of optional/expanded features */
	long tp_flags;

	const char *tp_doc; /* Documentation string */

	/* Assigned meaning in release 2.0 */
	/* call function for all accessible objects */
	void* tp_traverse;

	/* delete references to contained objects */
	inquiry tp_clear;

	/* Assigned meaning in release 2.1 */
	/* rich comparisons */
	richcmpfunc tp_richcompare;

	/* weak reference enabler */
	Py_ssize_t tp_weaklistoffset;

	/* Iterators */
	getiterfunc tp_iter;
	iternextfunc tp_iternext;

	/* Attribute descriptor and subclassing stuff */
	struct PyMethodDef *tp_methods;
	struct PyMemberDef *tp_members;
	struct PyGetSetDef *tp_getset;
	struct _typeobject *tp_base;
	PyObject *tp_dict;
	descrgetfunc tp_descr_get;
	descrsetfunc tp_descr_set;
	Py_ssize_t tp_dictoffset;
	initproc tp_init;
	allocfunc tp_alloc;
	newfunc tp_new;
	freefunc tp_free; /* Low-level free-memory routine */
	inquiry tp_is_gc; /* For PyObject_IS_GC */
	PyObject *tp_bases;
	PyObject *tp_mro; /* method resolution order */
	PyObject *tp_cache;
	PyObject *tp_subclasses;
	PyObject *tp_weaklist;
	destructor tp_del;

	/* Type attribute cache version tag. Added in version 2.6 */
	unsigned int tp_version_tag;

} PyTypeDef;

/* Magic code */
#define PyVarObject_HEAD_INIT(type, size) \
    type, {'H','o','t','T','y','p','e','0'},

#define PyAPI_FUNC(x) extern x

/* Generic operations on objects */
PyAPI_FUNC(int) PyObject_Print(PyObject *, FILE *, int);
PyAPI_FUNC(PyObject *) PyObject_Repr(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_Str(PyObject *);
//PyAPI_FUNC(PyObject *) PyObject_ASCII(PyObject *);
//PyAPI_FUNC(PyObject *) PyObject_Bytes(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_RichCompare(PyObject *, PyObject *, int);
PyAPI_FUNC(int) PyObject_RichCompareBool(PyObject *, PyObject *, int);
PyAPI_FUNC(PyObject *) PyObject_GetAttrString(PyObject *, const char *);
PyAPI_FUNC(int) PyObject_SetAttrString(PyObject *, const char *, PyObject *);
PyAPI_FUNC(int) PyObject_HasAttrString(PyObject *, const char *);
PyAPI_FUNC(PyObject *) PyObject_GetAttr(PyObject *, PyObject *);
PyAPI_FUNC(int) PyObject_SetAttr(PyObject *, PyObject *, PyObject *);
PyAPI_FUNC(int) PyObject_HasAttr(PyObject *, PyObject *);
PyAPI_FUNC(PyObject **) _PyObject_GetDictPtr(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_SelfIter(PyObject *);
PyAPI_FUNC(PyObject *) _PyObject_NextNotImplemented(PyObject *);
PyAPI_FUNC(PyObject *) PyObject_GenericGetAttr(PyObject *, PyObject *);
PyAPI_FUNC(int) PyObject_GenericSetAttr(PyObject *,
					      PyObject *, PyObject *);
PyAPI_FUNC(long) PyObject_Hash(PyObject *);
PyAPI_FUNC(long) PyObject_HashNotImplemented(PyObject *);
PyAPI_FUNC(int) PyObject_IsTrue(PyObject *);
PyAPI_FUNC(int) PyObject_Not(PyObject *);
PyAPI_FUNC(int) PyCallable_Check(PyObject *);

PyAPI_FUNC(void) PyObject_ClearWeakRefs(PyObject *);

PyAPI_FUNC(PyObject *) PyErr_Format(PyObject *, const char *, ...);

extern PyObject* PyExc_TypeError;

/* Generic type check */
PyAPI_FUNC(int) PyType_IsSubtype(PyTypeObject *, PyTypeObject *);
#define PyObject_TypeCheck(ob, tp) \
	(Py_TYPE(ob) == (tp) || PyType_IsSubtype(Py_TYPE(ob), (tp)))

typedef void (*PyCapsule_Destructor)(PyObject *);

typedef struct {
    double real;
    double imag;
} Py_complex;

typedef int (*Py_tracefunc)(PyObject*, PyFrameObject*, int, PyObject*);

#endif // HOTPY_PyAPI_OBJECT_H

