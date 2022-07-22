#include "hotpy.h"

/** A varied assortment of objects required by the VM.
 * Mainly types and strs, with a few functions
 * and odds and ends such as None and NotImplemented */

extern GVMT_OBJECT(type) _StopIteration;
extern GVMT_OBJECT(type) _SyntaxError;
extern GVMT_OBJECT(type) _TypeError;
extern GVMT_OBJECT(type) _NameError;
extern GVMT_OBJECT(type) _KeyError;
extern GVMT_OBJECT(type) _ValueError;
extern GVMT_OBJECT(type) _MemoryError;
extern GVMT_OBJECT(type) _GeneratorExit;
extern GVMT_OBJECT(type) _SystemExit;
extern GVMT_OBJECT(type) _ImportError;
extern GVMT_OBJECT(type) _IOError;
extern GVMT_OBJECT(type) _AttributeError;
extern GVMT_OBJECT(type) _UnboundLocalError;
extern GVMT_OBJECT(type) _InternalError;
extern GVMT_OBJECT(type) _IndexError;
extern GVMT_OBJECT(type) _KeyboardInterrupt;
extern GVMT_OBJECT(type) _OverflowError;
extern GVMT_OBJECT(type) _ZeroDivisionError;
extern GVMT_OBJECT(type) _RuntimeError;
extern GVMT_OBJECT(type) _ellipsis;

R_type type_StopIteration = &_StopIteration;
R_type type_TypeError = &_TypeError;
R_type type_NameError = &_NameError;
R_type type_KeyError = &_KeyError;
R_type type_ValueError = &_ValueError;
R_type type_MemoryError = &_MemoryError;
R_type type_ImportError = &_ImportError;
R_type type_IOError = &_IOError;
R_type type_AttributeError = &_AttributeError;
R_type type_UnboundLocalError = &_UnboundLocalError;
R_type type_InternalError = &_InternalError;
R_type type_IndexError = &_IndexError;
R_type type_OverflowError = &_OverflowError;
R_type type_ZeroDivisionError = &_ZeroDivisionError;
R_type type_RuntimeError = &_RuntimeError;
R_type type_KeyboardInterrupt = &_KeyboardInterrupt;
R_type type_SyntaxError = &_SyntaxError;
R_type type_GeneratorExit = &_GeneratorExit;
R_type type_SystemExit = &_SystemExit;
R_type type_ellipsis = &_ellipsis;

extern GVMT_OBJECT(function) func_object_new;
R_function object_new = &func_object_new;

#ifndef BOOTSTRAP

extern GVMT_OBJECT(function) func_object_init;
R_function object_init = &func_object_init;

extern GVMT_OBJECT(function) func_new_object_no_init;
extern GVMT_OBJECT(function) func_alloc_and_init;
extern GVMT_OBJECT(function) func_delegate_new;
extern GVMT_OBJECT(function) func_type_call;
extern GVMT_OBJECT(builtin_function) func_object_allocate;
extern GVMT_OBJECT(builtin_function) func_hasattr;
extern GVMT_OBJECT(builtin_function) func_tagged_floordiv;
extern GVMT_OBJECT(builtin_function) func_tagged_mod;
extern GVMT_OBJECT(function) func_getitem_surrogate;
extern GVMT_OBJECT(function) func_setitem_surrogate;
extern GVMT_OBJECT(function) func_iter_surrogate;
extern GVMT_OBJECT(function) func_contains_surrogate;
extern GVMT_OBJECT(function) func_next_surrogate;
extern GVMT_OBJECT(function) func_load_attr_surrogate;
extern GVMT_OBJECT(function) func_store_attr_surrogate;
extern GVMT_OBJECT(function) func_truth_surrogate;
extern GVMT_OBJECT(function) func_load_special_surrogate;

R_function function_new_object_no_init = &func_new_object_no_init;
R_function function_alloc_and_init = &func_alloc_and_init;
R_function function_type_call = &func_type_call;
R_builtin_function function_object_allocate = &func_object_allocate;
R_builtin_function function_hasattr = &func_hasattr;
R_builtin_function function_tagged_floordiv = &func_tagged_floordiv;
R_builtin_function function_tagged_mod = &func_tagged_mod;
R_function function_getitem_surrogate = &func_getitem_surrogate;
R_function function_setitem_surrogate = &func_setitem_surrogate;
R_function function_iter_surrogate = &func_iter_surrogate;
R_function function_next_surrogate = &func_next_surrogate;
R_function function_contains_surrogate = &func_contains_surrogate;
R_function function_load_attr_surrogate = &func_load_attr_surrogate;
R_function function_store_attr_surrogate = &func_store_attr_surrogate;
R_function function_truth_surrogate = &func_truth_surrogate;
R_function function_load_special_surrogate = &func_load_special_surrogate;

#endif

R_str name_self = &str_self;
R_str name_args = &str_args;
R_str name_kws  = &str_kws;
R_str name_p0   = &str_p0;

R_str name___lt__ = &str___lt__;
R_str name___le__ = &str___le__;
R_str name___eq__ = &str___eq__;
R_str name___ne__ = &str___ne__;
R_str name___gt__ = &str___gt__;
R_str name___ge__ = &str___ge__;

R_str name___neg__ = &str___neg__;
R_str name___pos__ = &str___pos__;
R_str name___abs__ = &str___abs__;
R_str name___invert__ = &str___invert__;

R_str name___add__ = &str___add__;
R_str name___sub__ = &str___sub__;
R_str name___mul__ = &str___mul__;
R_str name___div__ = &str___div__;
R_str name___floordiv__ = &str___floordiv__;
R_str name___mod__ = &str___mod__;
R_str name___lshift__ = &str___lshift__;
R_str name___rshift__ = &str___rshift__;
R_str name___and__ = &str___and__;
R_str name___or__ = &str___or__;
R_str name___xor__ = &str___xor__;

R_str name___radd__ = &str___radd__;
R_str name___rsub__ = &str___rsub__;
R_str name___rmul__ = &str___rmul__;
R_str name___rdiv__ = &str___rdiv__;
R_str name___rfloordiv__ = &str___rfloordiv__;
R_str name___rmod__ = &str___rmod__;
R_str name___rlshift__ = &str___rlshift__;
R_str name___rrshift__ = &str___rrshift__;
R_str name___rand__ = &str___rand__;
R_str name___ror__ = &str___ror__;
R_str name___rxor__ = &str___rxor__;

R_str name___iadd__ = &str___iadd__;
R_str name___isub__ = &str___isub__;
R_str name___imul__ = &str___imul__;
R_str name___idiv__ = &str___idiv__;
R_str name___ifloordiv__ = &str___ifloordiv__;
R_str name___imod__ = &str___imod__;
R_str name___ilshift__ = &str___ilshift__;
R_str name___irshift__ = &str___irshift__;
R_str name___iand__ = &str___iand__;
R_str name___ior__ = &str___ior__;
R_str name___ixor__ = &str___ixor__;

R_str name___del__ = &str___del__;
R_str name___getattribute__ = &str___getattribute__;
R_str name___getattr__ = &str___getattr__;
R_str name___delattr__ = &str___delattr__;
R_str name___call__ = &str___call__;
R_str name___iter__ = &str___iter__;
R_str name___main__ = &str___main__;
R_str name___name__ = &str___name__;
R_str name___len__ = &str___len__;
R_str name___bool__ = &str___bool__;
R_str name___int__ = &str___int__;
R_str name___init__ = &str___init__;
R_str name___float__ = &str___float__;
R_str name___hash__ = &str___hash__;
R_str name___next__ = &str___next__;
R_str name___setattr__ = &str___setattr__;
R_str name___class__ = &str___class__;
R_str name___new__ = &str___new__;
R_str name___str__ = &str___str__;
R_str name___dict__ = &str___dict__;
R_str name___slots__ = &str___slots__;
R_str name___repr__ = &str___repr__;
R_str name___index__ = &str___index__;
R_str name___getitem__ = &str___getitem__;
R_str name___setitem__ = &str___setitem__;
R_str name___delitem__ = &str___delitem__;
R_str name___contains__ = &str___contains__;
R_str name___builtins__ = &str___builtins__;
R_str name___metaclass__ = &str___metaclass__;
R_str name___debug__ = &str___debug__;
R_str name_argv = &str_argv;
R_str name_return = &str_return;
R_str name_run = &str_run;
R_str name_call = &str_call;
R_str name_raise = &str_raise;
R_str name_line = &str_line;
R_str name_trace = &str_trace;
R_str name_profile = &str_profile;
R_str name___doc__ = &str___doc__;
R_str name___package__ = &str___package__;
R_str name___missing__ = &str___missing__;
R_str name_None = &str_None;
R_str name_True = &str_True;
R_str name_False = &str_False;
R_str name_super = &str_super;

R_NoneType None = &NONE;
R_NotImplementedType NotImplemented = &NOT_IMPLEMENTED;
R_ellipsis Ellipsis = &ELLIPSIS;
R_dict_table empty_dict_table = &empty_dict_table_object;

R_str empty_string = &empty_string_object;
R_tuple empty_tuple = &empty_tuple_object;

R_function import_function = &func___import__;
R_function print_function = &func_print;

R_tuple standard_names = &standard_names_table;


GVMT_OBJECT(ellipsis) ELLIPSIS = {
    &_ellipsis,
    IMMORTAL, /* ref_count */
};

extern GVMT_OBJECT(bool) _false;
extern GVMT_OBJECT(bool) _true;

int trace_on = 0;
int sys_recursion_limit = 1000;
R_bool profile_on = &_false;
R_bool print_stats = &_false;
R_bool global_debug = &_true;
R_bool trace_show_stack = &_false;

R_dict_keys empty_dict_keys = &empty_dict_keys_object;
R_dict_values empty_dict_values = &empty_dict_values_object;
R_dict_values empty_module_dict_values = &empty_module_dict_values_object;
R_BaseException STOP_ITERATION_OBJECT = &stop_iteration_object;

