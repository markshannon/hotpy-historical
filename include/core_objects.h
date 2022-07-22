#ifndef HOTPY_CORE_OBJECTS_H
#define HOTPY_CORE_OBJECTS_H

extern R_type type_StopIteration;
extern R_type type_TypeError;
extern R_type type_NameError;
extern R_type type_KeyError;
extern R_type type_ValueError;
extern R_type type_GeneratorExit;
extern R_type type_SystemExit;
extern R_type type_MemoryError;
extern R_type type_ImportError;
extern R_type type_IOError;
extern R_type type_AttributeError;
extern R_type type_MethodAmbiguityError;
extern R_type type_UnboundLocalError;
extern R_type type_InternalError;
extern R_type type_IndexError;
extern R_type type_KeyboardInterrupt;
extern R_type type_OverflowError;
extern R_type type_ZeroDivisionError;
extern R_type type_RuntimeError;
extern R_type type_SyntaxError;
extern R_type type_ellipsis;

extern GVMT_OBJECT(function) import_file;

extern GVMT_OBJECT(function) _print_object;

extern GVMT_OBJECT(tuple) operator_tuple;

extern R_operator operators[];

extern R_dict canonical_strings;

extern GVMT_OBJECT(tuple) standard_names_table;
extern GVMT_OBJECT(tuple) operator_names_table;
extern R_tuple standard_names;
extern R_tuple operator_names;

extern R_function import_function;
extern R_function print_function;

extern GVMT_OBJECT(function) func___import__;

extern GVMT_OBJECT(function) func_print;

extern GVMT_OBJECT(function) func_c3_mro;
extern GVMT_OBJECT(function) func_list_init;

extern R_function function_new_object_no_init;
extern R_function function_alloc_and_init;
extern R_function function_type_call;
extern R_builtin_function function_object_allocate;
extern R_builtin_function function_hasattr;
extern R_builtin_function function_tagged_floordiv;
extern R_builtin_function function_tagged_mod;
extern R_function function_getitem_surrogate;
extern R_function function_setitem_surrogate;
extern R_function function_iter_surrogate;
extern R_function function_next_surrogate;
extern R_function function_contains_surrogate;
extern R_function function_load_attr_surrogate;
extern R_function function_store_attr_surrogate;
extern R_function function_truth_surrogate;
extern R_function function_load_special_surrogate;

extern GVMT_OBJECT(function) code_run_script;

extern GVMT_OBJECT(module) module_builtins;

extern GVMT_OBJECT(module) module_sys;

#define FALSE (booleans[0])
#define TRUE (booleans[1])

extern R_bool booleans[];

extern GVMT_OBJECT(NoneType) NONE;
extern GVMT_OBJECT(NotImplementedType) NOT_IMPLEMENTED;
extern GVMT_OBJECT(ellipsis) ELLIPSIS;

extern GVMT_OBJECT(dict) empty_dict_object;

extern R_function object_new;
extern R_function object_init;

extern GVMT_OBJECT(str) str_self;
extern GVMT_OBJECT(str) str_args;
extern GVMT_OBJECT(str) str_kws;
extern GVMT_OBJECT(str) str_p0;

extern GVMT_OBJECT(str) str___lt__;
extern GVMT_OBJECT(str) str___le__;
extern GVMT_OBJECT(str) str___eq__;
extern GVMT_OBJECT(str) str___ne__;
extern GVMT_OBJECT(str) str___gt__;
extern GVMT_OBJECT(str) str___ge__;

extern GVMT_OBJECT(str) str___neg__;
extern GVMT_OBJECT(str) str___pos__;
extern GVMT_OBJECT(str) str___abs__;
extern GVMT_OBJECT(str) str___invert__;

extern GVMT_OBJECT(str) str___add__;
extern GVMT_OBJECT(str) str___sub__;
extern GVMT_OBJECT(str) str___mul__;
extern GVMT_OBJECT(str) str___div__;
extern GVMT_OBJECT(str) str___floordiv__;
extern GVMT_OBJECT(str) str___mod__;
extern GVMT_OBJECT(str) str___lshift__;
extern GVMT_OBJECT(str) str___rshift__;
extern GVMT_OBJECT(str) str___and__;
extern GVMT_OBJECT(str) str___or__;
extern GVMT_OBJECT(str) str___xor__;

extern GVMT_OBJECT(str) str___radd__;
extern GVMT_OBJECT(str) str___rsub__;
extern GVMT_OBJECT(str) str___rmul__;
extern GVMT_OBJECT(str) str___rdiv__;
extern GVMT_OBJECT(str) str___rfloordiv__;
extern GVMT_OBJECT(str) str___rmod__;
extern GVMT_OBJECT(str) str___rlshift__;
extern GVMT_OBJECT(str) str___rrshift__;
extern GVMT_OBJECT(str) str___rand__;
extern GVMT_OBJECT(str) str___ror__;
extern GVMT_OBJECT(str) str___rxor__;

extern GVMT_OBJECT(str) str___iadd__;
extern GVMT_OBJECT(str) str___isub__;
extern GVMT_OBJECT(str) str___imul__;
extern GVMT_OBJECT(str) str___idiv__;
extern GVMT_OBJECT(str) str___ifloordiv__;
extern GVMT_OBJECT(str) str___imod__;
extern GVMT_OBJECT(str) str___ilshift__;
extern GVMT_OBJECT(str) str___irshift__;
extern GVMT_OBJECT(str) str___iand__;
extern GVMT_OBJECT(str) str___ior__;
extern GVMT_OBJECT(str) str___ixor__;

extern GVMT_OBJECT(str) str___call__;
extern GVMT_OBJECT(str) str___del__;
extern GVMT_OBJECT(str) str___class__;
extern GVMT_OBJECT(str) str___getattribute__;
extern GVMT_OBJECT(str) str___getattr__;
extern GVMT_OBJECT(str) str___delattr__;
extern GVMT_OBJECT(str) str___setattr__;
extern GVMT_OBJECT(str) str___name__;
extern GVMT_OBJECT(str) str___builtins__;
extern GVMT_OBJECT(str) str___debug__;
extern GVMT_OBJECT(str) str___metaclass__;
extern GVMT_OBJECT(str) str___repr__;
extern GVMT_OBJECT(str) str___index__;
extern GVMT_OBJECT(str) str___len__;
extern GVMT_OBJECT(str) str___main__;
extern GVMT_OBJECT(str) str___iter__;
extern GVMT_OBJECT(str) str___next__;
extern GVMT_OBJECT(str) str___new__;
extern GVMT_OBJECT(str) str___bool__;
extern GVMT_OBJECT(str) str___int__;
extern GVMT_OBJECT(str) str___init__;
extern GVMT_OBJECT(str) str___float__;
extern GVMT_OBJECT(str) str___str__;
extern GVMT_OBJECT(str) str___dict__;
extern GVMT_OBJECT(str) str___slots__;
extern GVMT_OBJECT(str) str___hash__;
extern GVMT_OBJECT(str) str___getitem__;
extern GVMT_OBJECT(str) str___setitem__;
extern GVMT_OBJECT(str) str___delitem__;
extern GVMT_OBJECT(str) str___contains__;
extern GVMT_OBJECT(str) str___debug__;
extern GVMT_OBJECT(str) str_argv;
extern GVMT_OBJECT(str) str_return;
extern GVMT_OBJECT(str) str_raise;
extern GVMT_OBJECT(str) str_call;
extern GVMT_OBJECT(str) str_line;
extern GVMT_OBJECT(str) str_sys;
extern GVMT_OBJECT(str) str_run;
extern GVMT_OBJECT(str) str_modules;
extern GVMT_OBJECT(str) str_trace;
extern GVMT_OBJECT(str) str_profile;
extern GVMT_OBJECT(str) str___doc__;
extern GVMT_OBJECT(str) str___package__;
extern GVMT_OBJECT(str) str___missing__;
extern GVMT_OBJECT(str) str_None;
extern GVMT_OBJECT(str) str_True;
extern GVMT_OBJECT(str) str_False;
extern GVMT_OBJECT(str) str_super;

extern int trace_on;
extern int sys_recursion_limit;
extern R_bool profile_on;
extern R_bool print_stats;
extern R_bool global_debug;
extern R_bool trace_show_stack;

extern R_str name_self;
extern R_str name_args;
extern R_str name_kws;
extern R_str name_p0;

extern R_str name___lt__;
extern R_str name___le__;
extern R_str name___eq__;
extern R_str name___ne__;
extern R_str name___gt__;
extern R_str name___ge__;

extern R_str name___neg__;
extern R_str name___pos__;
extern R_str name___abs__;
extern R_str name___invert__;

extern R_str name___add__;
extern R_str name___sub__;
extern R_str name___mul__;
extern R_str name___div__;
extern R_str name___floordiv__;
extern R_str name___mod__;
extern R_str name___lshift__;
extern R_str name___rshift__;
extern R_str name___and__;
extern R_str name___or__;
extern R_str name___xor__;

extern R_str name___radd__;
extern R_str name___rsub__;
extern R_str name___rmul__;
extern R_str name___rdiv__;
extern R_str name___rfloordiv__;
extern R_str name___rmod__;
extern R_str name___rlshift__;
extern R_str name___rrshift__;
extern R_str name___rand__;
extern R_str name___ror__;
extern R_str name___rxor__;

extern R_str name___iadd__;
extern R_str name___isub__;
extern R_str name___imul__;
extern R_str name___idiv__;
extern R_str name___ifloordiv__;
extern R_str name___imod__;
extern R_str name___ilshift__;
extern R_str name___irshift__;
extern R_str name___iand__;
extern R_str name___ior__;
extern R_str name___ixor__;

extern R_str name___del__;
extern R_str name___getattribute__;
extern R_str name___getattr__;
extern R_str name___delattr__;
extern R_str name___call__;
extern R_str name___iter__;
extern R_str name___main__;
extern R_str name___name__;
extern R_str name___init__;
extern R_str name___int__;
extern R_str name___bool__;
extern R_str name___len__;
extern R_str name___float__;
extern R_str name___hash__;
extern R_str name___next__;
extern R_str name___setattr__;
extern R_str name___class__;
extern R_str name___new__;
extern R_str name___repr__;
extern R_str name___builtins__;
extern R_str name___metaclass__;
extern R_str name___str__;
extern R_str name___dict__;
extern R_str name___slots__;
extern R_str name___index__;
extern R_str name___getitem__;
extern R_str name___setitem__;
extern R_str name___delitem__;
extern R_str name___contains__;
extern R_str name___debug__;
extern R_str name_argv;
extern R_str name_call;
extern R_str name_line;
extern R_str name_raise;
extern R_str name_return;
extern R_str name_run;
extern R_str name_trace;
extern R_str name_profile;
extern R_str name___doc__;
extern R_str name___package__;
extern R_str name___missing__;
extern R_str name_None;
extern R_str name_True;
extern R_str name_False;
extern R_str name_super;

extern R_NoneType None;
extern R_NotImplementedType NotImplemented;
extern R_ellipsis Ellipsis;
extern GVMT_OBJECT(dict_table) empty_dict_table_object;
extern R_dict_table empty_dict_table;

extern R_str empty_string;

extern R_tuple empty_tuple;

extern R_module builtins;
extern R_module sys;

extern GVMT_OBJECT(tuple) empty_tuple_object;
extern GVMT_OBJECT(str) empty_string_object;

extern GVMT_OBJECT(dict_keys) empty_dict_keys_object;
extern GVMT_OBJECT(dict_values) empty_dict_values_object;
extern GVMT_OBJECT(dict_values) empty_module_dict_values_object;

extern R_dict_keys empty_dict_keys;
extern R_dict_values empty_dict_values;
extern R_dict_values empty_module_dict_values;

extern GVMT_OBJECT(BaseException) stop_iteration_object;
extern R_BaseException STOP_ITERATION_OBJECT;

#define C_NAME (1 << 16)

#endif
