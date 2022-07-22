#include "hotpy.h"
#include "gvmt/compiler.h"
#include "alloca.h"
#include <stdio.h>
#include <assert.h>

/** Contents of this file are necessary to bootstrap the VM,
 *  but not for the full version.
 */

extern GVMT_OBJECT(type) _bool;
extern GVMT_OBJECT(type) _module;
extern GVMT_OBJECT(type) _dict;

extern GVMT_OBJECT(bool) _false;
extern GVMT_OBJECT(bool) _true;

R_bool booleans[2] = {
    &_false,
    &_true
};

static GVMT_OBJECT(dict) _sys_dict =  {
    &_dict,
    0,  /* ref_count */
    GVMT_LOCK_INITIALIZER,
    &empty_dict_values_object
};

GVMT_OBJECT(module) module_sys = {
    &_module,
    0,  /* ref_count */
    &_sys_dict
};

/**  These objects exist to keep the linker happy and should never be used.
 */

GVMT_OBJECT(function) func___import__ = { 0 };

GVMT_OBJECT(function) func_object_new = { 0 };

GVMT_OBJECT(function) func_print = { 0 };

GVMT_OBJECT(operator) operator_eq = { 0 };

GVMT_OBJECT(operator) operator_le = { 0 };

void disassemble(R_code_object c, FILE* out) {
}

R_frame enter_surrogate(R_function func, R_thread_state ts, uint8_t* next_ip) {
    assert(0 && "Cannot use low-level during bootstrapping");
}

R_function function_getitem_surrogate = 0;
R_function function_setitem_surrogate = 0;
R_function function_iter_surrogate = 0;
R_function function_next_surrogate = 0;
R_function function_contains_surrogate = 0;
R_function function_load_attr_surrogate = 0;
R_function function_store_attr_surrogate = 0;
R_function function_truth_surrogate = 0;
R_function function_load_special_surrogate = 0;


