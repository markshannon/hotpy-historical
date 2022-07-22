#ifndef _DESCRIPTORS_H
#define _DESCRIPTORS_H

#include "gvmt/gvmt.h"
#include "typedefs.h"

GVMT_OBJECT(dict_descriptor) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
};

GVMT_OBJECT(class_descriptor) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

GVMT_OBJECT(int_slot_descriptor) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t offset;   
    int32_t flags;
    GVMT_OBJECT(str)* name; 
    GVMT_OBJECT(str)* __doc__;    /* read-write */
};

GVMT_OBJECT(slot_descriptor) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t offset;    
    int32_t flags; 
    GVMT_OBJECT(str)* name;    
    GVMT_OBJECT(str)* __doc__; /* read-write */
    GVMT_OBJECT(type)* type;
};

GVMT_OBJECT(call_descriptor) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
};

#define BEHAVIOUR_SIGNATURE(name) R_object name(void)
typedef BEHAVIOUR_SIGNATURE((*behaviour));

typedef void (*del_func)(R_object descriptor, R_object );

typedef R_object (*get_func)(R_object descriptor, R_object obj, R_type cls);

typedef void (*set_func)(R_object descriptor, R_object obj, R_object value);

#define DESC_CALL_SIGNATURE(name)  R_object name(R_object callable, R_tuple tuple, R_dict dict)
typedef DESC_CALL_SIGNATURE((*call_func));

typedef intptr_t (*hash_func)(R_object obj);

#define GETATTRIBUTE_FUNCTION(fname) R_object fname(R_object object, R_str name)
typedef GETATTRIBUTE_FUNCTION((*getattribute_function));

#define SETATTR_FUNCTION(fname) void fname(R_object object, R_str name, R_object value)
typedef SETATTR_FUNCTION((*setattr_function));

//DESC_CALL_SIGNATURE(desc_call_object);

DESC_CALL_SIGNATURE(desc_call_type);

R_object desc_get_slot_descriptor(R_object descriptor, R_object obj, R_type cls);
void desc_set_slot_descriptor(R_object descriptor, R_object obj, R_object value);

R_object desc_get_int_slot_descriptor(R_object descriptor, R_object obj, R_type cls);
void desc_set_int_slot_descriptor(R_object descriptor, R_object obj, R_object value);

R_object desc_get_function(R_object descriptor, R_object obj, R_type cls);
DESC_CALL_SIGNATURE(desc_call_function);

R_object desc_get_unary_operator(R_object descriptor, R_object obj, R_type cls);
DESC_CALL_SIGNATURE(desc_call_unary_operator);

R_object desc_get_binary_operator(R_object descriptor, R_object obj, R_type cls);
DESC_CALL_SIGNATURE(desc_call_binary_operator);

R_object desc_get_operator(R_object descriptor, R_object obj, R_type cls);
DESC_CALL_SIGNATURE(desc_call_operator);

R_object desc_get_bound_method(R_object descriptor, R_object obj, R_type cls);
DESC_CALL_SIGNATURE(desc_call_bound_method);

DESC_CALL_SIGNATURE(desc_call_dict_descriptor);
R_object desc_get_dict_descriptor(R_object descriptor, R_object obj, R_type cls);

GETATTRIBUTE_FUNCTION(override_getattribute);

#define SLOT_WRITEABLE 1
#define SLOT_NOT_NULL 2

#endif
