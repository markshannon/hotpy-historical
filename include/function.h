#ifndef FUNCTION_H
#define FUNCTION_H

#include <assert.h>
#include "dictionary.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <alloca.h>
#include "hotpy.h"
#include "attributes.h"
#include "dictionary.h"

GVMT_OBJECT(code_object) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t kind;
    uint32_t params;
    uint32_t locals;
    GVMT_OBJECT(tuple)* names;
    GVMT_OBJECT(bytes)* code;    /*  read-only */
    GVMT_OBJECT(str)* name;    /*  read-only */
    GVMT_OBJECT(Constants)* constants;    /*  read-only */
    GVMT_OBJECT(tuple)* local_names;    /*  read-only */
};

GVMT_OBJECT(base_function) {  // special notype
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t parameter_format; 
    GVMT_OBJECT(str)* __name__;    /*  read-write */
    GVMT_OBJECT(str)* __doc__;    /* read-write */
    GVMT_OBJECT(tuple)* paramNames;    /* read-only */
    GVMT_OBJECT(tuple)* defaults;    /*  read-only */
};

GVMT_OBJECT(builtin_function) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t parameter_format;    
    GVMT_OBJECT(str)* __name__;    /*  read-write */
    GVMT_OBJECT(str)* __doc__;    /* read-write */
    GVMT_OBJECT(tuple)* paramNames;    /* read-only */
    GVMT_OBJECT(tuple)* defaults;      /* read-only */
    GVMT_OBJECT(str)* source_name; 
    GVMT_OBJECT(tuple)* param_types;    /* read-only */
    GVMT_OBJECT(type)* return_type;
    behaviour executable;    /* pointer */
};             

GVMT_OBJECT(function) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t parameter_format; 
    GVMT_OBJECT(str)* __name__;    /* read-write */
    GVMT_OBJECT(str)* __doc__;    /*  read-write */
    GVMT_OBJECT(tuple)* paramNames;    /* read-only */
    GVMT_OBJECT(tuple)* defaults;    /*  read-only */
    GVMT_OBJECT(dict)* dictionary;  
    GVMT_OBJECT(code_object)* __code__;    /* read-only */
    GVMT_OBJECT(scopes)* scopes;
    GVMT_OBJECT(dict)* __annotations__;    /*  read-only */
    GVMT_OBJECT(guard)* code_guard; 
    GVMT_OBJECT(frame)* func_closure; /* NULL or a frame object */
    GVMT_OBJECT(exec_link)* link;
    int32_t execution_count;
    uint8_t* bytecodes;    /* pointer  */
    int32_t byte_code_length;  
};

GVMT_OBJECT(static_method) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(object)* callable; 
};

GVMT_OBJECT(bound_method) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(object)* object;   
    GVMT_OBJECT(object)* callable; 
};

GVMT_OBJECT(class_method) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(object)* callable; 
};

uint8_t* get_bytecodes_from_code_object(R_code_object co);

R_function make_function(R_code_object f, R_tuple defaults, R_dict annotations, R_scopes scopes);

R_tuple get_types_from_annotation(R_function func);
R_type get_return_type_from_annotation(R_function func);

void pack_parameters_tuple(int pcount, GVMT_Object* parameters, int new_count);

void too_few_parameters(R_base_function func, int pcount, GVMT_Object* parameters, int new_count);

R_object py_call(R_function func, R_tuple t, R_dict d);

void parameter_type_check(R_builtin_function func);

R_object get_default_parameter(R_base_function func, int index);

R_object not_callable(R_object object, R_tuple tuple, R_dict dict);

int is_simple_function(R_base_function f, int param_count);

call_func _callable(R_object o);

extern char* PY_name(R_function f, char* buf);

void null_local(R_frame frame, uintptr_t index);
   
R_dict make_annotations(R_tuple names, R_list types);

void run_script(R_code_object code, R_module module, R_frame caller);

R_object PY_FUNC1(c_function, R_function f);

void unpack_native_parameters(R_builtin_function func, R_tuple tuple, R_dict dict);

void wrong_number_parameters(R_str func_name, int actual, int min, int max);

R_scopes new_scopes(R_scopes old);

R_object _HotPy_bind_method(R_object obj, R_object callable);

/************ Flags *********/

#define TUPLE_FUNCTION 4
#define DICT_FUNCTION 8
#define TUPLE_DICT_FUNCTION 12

#define FUNCTION_NO_TRACE (1<<5)
#define FUNCTION_PURE (1<<6)
#define FUNCTION_NO_RAISE (1<<7)
#define FUNCTION_NO_OVERFLOW (1<<8)
#define GENERATOR_FUNCTION (1<<9)
#define NESTED_FUNCTION (1<<10)
#define CLOSURE (1<<11)

#define NO_DELETES (1<<13)



#endif
   
