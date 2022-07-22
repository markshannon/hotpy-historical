#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "gvmt/marshaller.h"
#include "hotpy.h"

/** Support for marshalling heap to GSC file */

void fatal(char* fmt, ...);

typedef void (*fptr)(void);


void user_marshal_type_hash(FILE* m, R_type t){
    hash_func h = t->hash;
    if (h == HotPyObject_HashNotImplemented) {
        gsc_address(m, "  HotPyObject_HashNotImplemented"); 
    } else if (h == NULL) {
        gsc_address(m, "0");
    } else {
        char buffer[160];
        char* p = buffer + sprintf(buffer, "hotpy_");  
        p += py_print(p, t->__name__);
        sprintf(p, "_hash");
        gsc_address(m, buffer);
    }
}


static void marshall_type_desc(FILE* m, R_type t, char* name, fptr f) {
    if (f) {
        if (f == (fptr)not_callable) {
            gsc_address(m, "not_callable");
        } else {
            char buffer[160];
            char*p = buffer + sprintf(buffer, "desc_%s_", name);
            py_print(p, t->__name__);
            gsc_address(m, buffer);
        }
    } else {
        gsc_address(m, "0");
    }
}   

void user_marshal_type_call(FILE* m, R_type t){
    marshall_type_desc(m, t, "call", (fptr)t->call);
}

void user_marshal_type_get(FILE* m, R_type t){
    marshall_type_desc(m, t, "get", (fptr)t->get);
}

void user_marshal_type_set(FILE* m, R_type t){
    marshall_type_desc(m, t, "set", (fptr)t->set);
}

void user_marshal_builtin_function_executable(FILE* m, R_builtin_function f) {
    char buffer[160];
    sprintf(buffer, "PY_");
    py_print(buffer + 3, f->source_name);
    gsc_address(m, buffer);
}

void user_marshal_function_execute(FILE* m, R_function f) {
    gsc_address(m, "0");
}

void user_marshal_type___getattribute__(FILE* m, R_type t) {
    if (t == type_type)
        gsc_address(m, "type_getattribute");
    else
        gsc_address(m, "_HotPyObject_GenericGetAttr");
}

void user_marshal_type___setattr__(FILE* m, R_type t) {
    if (t == type_type)
        gsc_address(m, "type_setattr");
    else
        gsc_address(m, "_HotPyObject_GenericSetAttr");
}

extern void write_bytes_to_gsc(FILE* m, int length, uint8_t* bytes, char* name);

void user_marshal_function_bytecodes(FILE* m, R_function f) {
    gsc_address(m, "0");
}

void user_marshal_type_marshaller(FILE* m, R_type obj) {
    gsc_address(m, "0");
}

void user_marshal_type_tp_name(FILE* m, R_type obj) {
    static int string_id = 1;
    char buf[20];
    char name[160];
    py_print(name, obj->__name__);
    fprintf(m, ".opaque\n");
    fprintf(m, ".public string_%d\n", string_id);
    fprintf(m, ".label string_%d\n", string_id);
    gsc_string(m, name);
    fprintf(m, ".heap\n");
    sprintf(buf, "string_%d\n", string_id++);
    gsc_address(m, buf);
}

void user_marshal_type_api_getattro(FILE* out, R_type t) {
    gsc_address(out, "0");  
}

void user_marshal_type_api_setattro(FILE* out, R_type t) {
    gsc_address(out, "0");  
}

void user_marshal_type_api_call(FILE* out, R_type t) {
    gsc_address(out, "0");  
}

void user_marshal_builtin_function_root(FILE* m, R_builtin_function obj) {
    gsc_address(m, "0");
}

void user_marshal_function_root(FILE* m, R_function obj) {
    gsc_address(m, "0");
}

void user_marshal_method_api_root(FILE* m, R_method_api obj) {
    gsc_address(m, "0");
}

void user_marshal_method_api_func(FILE* m, R_method_api obj) {
    gsc_address(m, "0");
}

void user_marshal_getset_wrapper_getter(FILE* m, R_getset_wrapper x) {
    gsc_address(m, "0");
}

void user_marshal_getset_wrapper_setter(FILE* m, R_getset_wrapper x) {
    gsc_address(m, "0");
}

void user_marshal_getset_wrapper_closure(FILE* m, R_getset_wrapper x) {
    gsc_address(m, "0");
}

void user_marshal_len_api_func(FILE* m, R_len_api x) {
    gsc_address(m, "0");
}

void user_marshal_binary_api_func(FILE* m, R_binary_api x) {
    gsc_address(m, "0");
}

void user_marshal_unary_api_func(FILE* m, R_unary_api x) {
    gsc_address(m, "0");
}

void user_marshal_binary_pred_api_func(FILE* m, R_binary_pred_api x) {
    gsc_address(m, "0");
}

void user_marshal_unary_pred_api_func(FILE* m, R_unary_pred_api x) {
    gsc_address(m, "0");
}

void user_marshal_binary_r_api_func(FILE* m, R_binary_r_api x) {
    gsc_address(m, "0");
}

void user_marshal_objint_api_func(FILE* m, R_objint_api x) {
    gsc_address(m, "0");
}

void user_marshal_objintobj_api_func(FILE* m, R_objintobj_api x) {
    gsc_address(m, "0");
}

void user_marshal_setitem_api_func(FILE* m, R_setitem_api x) {
    gsc_address(m, "0");
}

void user_marshal_hash_api_func(FILE* m, R_hash_api x) {
    gsc_address(m, "0");
}

void user_marshal_new_api_func(FILE* m, R_new_api x) {
    gsc_address(m, "0");
}

void user_marshal_compare_api_func(FILE* m, R_compare_api x) {
    gsc_address(m, "0");
}

