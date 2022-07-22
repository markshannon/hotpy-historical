#ifndef HOTPY_OBJECT_H
#define HOTPY_OBJECT_H

GVMT_OBJECT(object) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

GVMT_OBJECT(NoneType) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

GVMT_OBJECT(NotImplementedType) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

GVMT_OBJECT(ellipsis) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

/** Macros for simple standard object accesses */

/** Object's type */
#define TYPE(x) (gvmt_is_tagged(x) ? type_int : (x)->ob_type)
//#define TYPE(x) ((x)->ob_type)
#define SET_TYPE(x, t) ((x)->ob_type = t)

/** Object's type's name */
#define TYPE_NAME(x) (TYPE(x)->__name__)

/** Variable sized objects length */
#define LEN(x) ((x)->length)

/** Item in array like objects, bytes, arrays, tuples, etc. 0 <= i < length */
#define ITEM(x,i) ((x)->array[i])

/** Callable for object */
#define CALLABLE(o) (TYPE(o)->call ? TYPE(o)->call : _callable((R_object)o))

/** Is this a callable */
#define IS_CALLABLE(o) (CALLABLE(o) != not_callable)

/** Various type-tests. */  

#define IS_INT(o) (gvmt_is_tagged(o) || ((o)->ob_type->flags & INT_SUBCLASS_FLAG))
#define IS_LIST(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & LIST_SUBCLASS_FLAG))
#define IS_TUPLE(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & TUPLE_SUBCLASS_FLAG))
#define IS_BYTES(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & BYTES_SUBCLASS_FLAG))
#define IS_TYPE(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & TYPE_SUBCLASS_FLAG))
#define IS_DICT(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & DICT_SUBCLASS_FLAG))
#define IS_RAISEABLE(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & BASE_EXC_SUBCLASS_FLAG))
#define IS_STR(o) (!gvmt_is_tagged(o) && ((o)->ob_type->flags & STR_SUBCLASS_FLAG))
#define IS_DICT_EXACT(o) (!gvmt_is_tagged(o) && ((o)->ob_type == type_dict))

#define IS_BOOL(o) (!gvmt_is_tagged(o) && ((o)->ob_type == type_bool))
#define IS_PY_FUNCTION(o) (!gvmt_is_tagged(o) && ((o)->ob_type == type_function))
#define IS_C_FUNCTION(o) (!gvmt_is_tagged(o) && ((o)->ob_type == type_builtin_function))
#define IS_FUNCTION(o) (!gvmt_is_tagged(o) && \
(((o)->ob_type == type_builtin_function) || ((o)->ob_type == type_function)))
#define IS_SLICE(o) (!gvmt_is_tagged(o) && ((o)->ob_type == type_slice))

R_object PY_FUNC2(object_getattribute, R_object object, R_object key);

R_object PY_FUNC3(object_setattr, R_object object, R_object key, R_object value);

GETATTRIBUTE_FUNCTION(_HotPyObject_GenericGetAttr);

SETATTR_FUNCTION(_HotPyObject_GenericSetAttr);

#define ZERO_REF_COUNT(o) (o)->ref_count = 0;

intptr_t hotpy_object_hash(R_object object);

R_object _HotPyObject_dict_get(R_object object, R_str key);

//R_object _HotPyObject_dict_get_str(R_object object, R_str key);

void _HotPyObject_dict_set(R_object object, R_str key, R_object value);

#endif
