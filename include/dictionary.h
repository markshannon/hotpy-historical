#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "gvmt/gvmt.h"
#include "typedefs.h"

#define KEY_TYPE R_object
#define PREFIX(x) dict##x
#define POSTFIX(x) x##dict
#define KEY_ERROR(key) raise_str(type_KeyError, HotPyObject_Str(key))
#define LOOKUP general_lookup

#include "dict_algorithms.h"

#undef KEY_TYPE
#undef PREFIX
#undef POSTFIX
#undef KEY_ERROR
#undef LOOKUP

#define KEY_TYPE R_str
#define PREFIX(x) str_dict##x
#define POSTFIX(x) x##str_dict
#define KEY_ERROR(key) raise_str(type_KeyError, key)
#define LOOKUP string_lookup

#include "dict_algorithms.h"

#undef KEY_TYPE
#undef PREFIX
#undef POSTFIX
#undef KEY_ERROR
#undef LOOKUP

R_object dict_get(R_dict dict, R_object key);

R_object dict_get_str(R_dict dict, R_str key);

R_object values_get_str(R_dict_values values, R_str key);

R_object values_get(R_dict_values values, R_object key);

void dict_set(R_dict dict, R_object key, R_object val);

void dict_set_str(R_dict d, R_str key, R_object item);

R_dict_values values_set_str(R_dict_values values, R_str key, R_object item);

void dict_del(R_dict dict, R_object key);

void dict_del_str(R_dict d, R_str key);

void values_del_str(R_dict_values v, R_str key);


R_object dict_any_key(R_dict dict);

#define dict_table_valid(t) ((t)->size >= 0)

int probable_str_lookup(R_dict_keys keys, R_str key);

void insert_keyword_arg(R_dict d, R_str key, R_object value);

#define is_empty(d) ((d)->values->size == 0)

R_dict_keys new_dict_keys(int32_t size);

R_dict_values new_dict_values(R_dict_keys keys);

R_module_dict_values new_module_dict_values(R_dict_keys keys);

int general_lookup_keys(R_dict_keys keys, R_object key);

void dict_resize(R_dict d);

void dict_resize_str(R_dict d);

R_dict_values values_resize_str(R_dict_values v);

#endif

