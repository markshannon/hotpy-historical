#ifndef SCAN_FUNCS_H
#define SCAN_FUNCS_H

#include "hotpy.h"

void user_marshal_type_hash(FILE* m, R_type t);

void user_marshal_type_call(FILE* m, R_type t);

void user_marshal_type_get(FILE* m, R_type t);

void user_marshal_type_set(FILE* m, R_type t);

void user_marshal_builtin_function_executable(FILE* m, R_builtin_function f);

void user_marshal_function_execute(FILE* m, R_function f);

void user_marshal_type___getattribute__(FILE* m, R_type t);

void user_marshal_type___setattr__(FILE* m, R_type t);

void user_marshal_function_bytecodes(FILE* m, R_function f);

void user_marshal_type_marshaller(FILE* m, R_type obj);

void user_marshal_type_tp_name(FILE* m, R_type obj);

void user_marshal_type_api_getattro(FILE* out, R_type t);

void user_marshal_type_api_setattro(FILE* out, R_type t);

void user_marshal_type_api_call(FILE* out, R_type t);

void user_marshal_method_api_func(FILE* m, R_method_api obj);

void user_marshal_getset_wrapper_getter(FILE* m, R_getset_wrapper x);

void user_marshal_getset_wrapper_setter(FILE* m, R_getset_wrapper x);

void user_marshal_getset_wrapper_closure(FILE* m, R_getset_wrapper x);

void user_marshal_len_api_func(FILE* m, R_len_api x);

void user_marshal_binary_api_func(FILE* m, R_binary_api x);

void user_marshal_unary_api_func(FILE* m, R_unary_api x);

void user_marshal_binary_pred_api_func(FILE* m, R_binary_pred_api x);

void user_marshal_unary_pred_api_func(FILE* m, R_unary_pred_api x);

void user_marshal_binary_r_api_func(FILE* m, R_binary_r_api x);

void user_marshal_objint_api_func(FILE* m, R_objint_api x);

void user_marshal_objintobj_api_func(FILE* m, R_objintobj_api x);

void user_marshal_setitem_api_func(FILE* m, R_setitem_api x);

void user_marshal_hash_api_func(FILE* m, R_hash_api x);

void user_marshal_new_api_func(FILE* m, R_new_api x);

void user_marshal_compare_api_func(FILE* m, R_compare_api x);


#endif // SCAN_FUNCS_H
