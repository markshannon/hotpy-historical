#include <assert.h>
#include "dictionary.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "hotpy.h"
#include "core_objects.h"
#include "attributes.h"
#include "dictionary.h"
#include "function.h"
#include "gvmt/compiler.h"
#include "intobject.h"
#include "operators.h"
#include "optimise.h"
#include "opcodes.h"
 
/** Function calling (not function-object) related stuff.
 * Maintain parameter marshalling */

/** Calling convention --

At F_CALL the stack is laid out as follows:
callable tuple dict
F_CALL calls TYPE(callable)->call(callable, tuple, dict);
dict may be NULL -- Equivalent to empty dict, but saves allocation.
**/
 
#define roundup(x, n) (((x)+((n)-1))&(-(n)))

/** Only to be used in macro CALLABLE, as it skips the first class in the mro */
call_func _callable(R_object o) {
    R_tuple mro = TYPE(o)->__mro__;
    call_func f;
    int i;
    assert(TYPE(o) == (R_type)ITEM(mro, 0));
    assert(TYPE(o)->call == NULL);
    // Since object has its call slot filled, no need to check bounds.
    for (i = 1; ; i++) {
        assert(i < mro->length);
        f = ((R_type)ITEM(mro, i))->call;
        if (f)
            return f;
    }
}
    

void handle_kws(R_function func, int new_count, R_dict d, R_frame f) {
    int i;   
    for (i = 0; i < new_count; i++) {
        R_str pname = (R_str)ITEM(func->paramNames, i);
        R_object val = dict_remove_named_item(d, pname);
        if (ITEM(f, i) == (R_object)NULL_R) {
            ITEM(f, i) = val;
        } else if (val) {
            R_str name = func->__name__;
            R_str msg = py_sprintf("%s() got multiple values for keyword argument '%s'", name, pname);
            raise_str(type_TypeError, msg);
        }
    }
}

void wrong_number_parameters(R_str name, int actual, int min, int max) {
    char* fmt;
    assert(actual < min || actual > max);
    assert(min <= max);
    if (min == max) {
        fmt = (min == 1) ? "%s() takes exactly %i argument (%i given)" : "%s() takes exactly %i arguments (%i given)";
        raise_str(type_TypeError, py_sprintf(fmt, name, R_int_from_ssize_t(min), R_int_from_ssize_t(actual)));
    } else {
        fmt = "%s() takes between %d and %d arguments (%i given)";
        raise_str(type_TypeError, py_sprintf(fmt, name, R_int_from_ssize_t(min), R_int_from_ssize_t(max), R_int_from_ssize_t(actual)));
    }
}

void unexpected_keyword_error(R_function func, R_dict d) {
    R_str fname = func->__name__;
    R_str kname = (R_str)dict_any_key(d);
    R_str msg;
    assert(kname);
    TYPE_ASSERT(kname, str);
    msg = py_sprintf("%s() got an unexpected keyword argument '%s'", 
                     fname, kname);
    raise_str(type_TypeError, msg);    
}

R_object get_default_parameter(R_base_function func, int index) {
    int a_index = index - func->paramNames->length + func->defaults->length;
    if (a_index < 0)
        return 0;
    else
        return ITEM(func->defaults, a_index);
}

/** Insert default parameters at and of parameter list. Do not overwrite pre-existing parameters */
void insert_default_parameters(R_function func, int new_count, R_frame frame, int done) {
    int i, dlen = func->defaults->length;
    for (i = 1; new_count-i >= done; i++) {
        if (ITEM(frame, new_count-i) == (R_object)NULL_R) {
            R_object o = ITEM(func->defaults, dlen-i);
            ITEM(frame, new_count-i) = o;
        }
    }        
}

static void undefined_parameter_error(R_function func, int index) {
    R_str name = func->__name__;
    R_object pname = ITEM(func->paramNames, index);
    R_str msg = py_sprintf("%s(): parameter '%s' undefined", name, pname);
    raise_str(type_TypeError, msg);
}

#define tuple_into_frame(t, f, c) { \
    int i; \
    for (i = 0; i < (c); i++) { \
        R_object o = (t)->array[i]; \
        (f)->array[i] = o; \
    } \
}

#define check_no_names_unassigned(func, start, end, frame) { \
    int i; \
    for(i = start; i < end; i++) { \
        if (ITEM(frame, i) == (R_object)NULL_R) { \
            undefined_parameter_error(func, i); \
        } \
    } \
}

static void params_fixup(R_function func, R_tuple t, R_dict d, R_frame frame) {
    int new_count = func->paramNames->length;
    int tcount = t->length;
    if (tcount > new_count) 
        wrong_number_parameters(func->__name__, tcount, new_count, new_count);
    tuple_into_frame(t, frame, tcount); 
    if (d && !is_empty(d)) {
        handle_kws(func, new_count, d, frame);
        if (!is_empty(d))
            unexpected_keyword_error(func, d);         
    }
    insert_default_parameters(func, new_count, frame, tcount);
    check_no_names_unassigned(func, tcount, new_count, frame);
}

static void params_fixup_tuple(R_function func, R_tuple t, R_dict d, R_frame frame) {
    int new_count = func->paramNames->length-1;
    int tcount = t->length;
    if (tcount > new_count) { 
        tuple_into_frame(t, frame, new_count);
        ITEM(frame, new_count) = (R_object)tuple_slice(t, new_count, tcount);
    } else {
        tuple_into_frame(t, frame, tcount);
        ITEM(frame, new_count) = (R_object)empty_tuple;
    }
    if (d && !is_empty(d)) {
        handle_kws(func, new_count, d, frame);
        if (!is_empty(d))
            unexpected_keyword_error(func, d);         
    }
    insert_default_parameters(func, new_count, frame, tcount);
    check_no_names_unassigned(func, tcount, new_count, frame);
}     

static void params_fixup_dict(R_function func, R_tuple t, R_dict d, R_frame frame) {
    int new_count = func->paramNames->length-1;
    int tcount = t->length;
    if (tcount > new_count) 
        wrong_number_parameters(func->__name__, tcount, new_count, new_count);
    tuple_into_frame(t, frame, tcount); 
    if (d && !is_empty(d)) {
        handle_kws(func, new_count, d, frame);
    }
    if (d) {
        ITEM(frame, new_count) = (R_object)d;
    } else {
        ITEM(frame, new_count) = (R_object)new_dict(0);
    }   
    insert_default_parameters(func, new_count, frame, tcount);
    check_no_names_unassigned(func, tcount, new_count, frame);
}     
                     
static void params_fixup_tuple_dict(R_function func, R_tuple t, R_dict d, R_frame frame) {
    int new_count = func->paramNames->length-2;
    int tcount = t->length;
    if (tcount > new_count) { 
        tuple_into_frame(t, frame, new_count);
        ITEM(frame, new_count) = (R_object)tuple_slice(t, new_count, tcount);
    } else {
        tuple_into_frame(t, frame, tcount);
        ITEM(frame, new_count) = (R_object)empty_tuple;
    }
    if (d && !is_empty(d)) {
        handle_kws(func, new_count, d, frame);
    }
    if (d) {
        ITEM(frame, new_count+1) = (R_object)d;
    } else {
        ITEM(frame, new_count+1) = (R_object)new_dict(0);
    }   
    insert_default_parameters(func, new_count, frame, tcount);
    check_no_names_unassigned(func, tcount, new_count, frame);
}

R_frame new_frame(R_code_object code_obj, R_scopes scopes, R_frame caller) {
    R_frame fp;
    int i, lcount = code_obj->locals;
    fp = gc_allocate_var(frame, array, lcount);
    SET_TYPE(fp, type_frame);
    ZERO_REF_COUNT(fp);
    fp->length = lcount;
    fp->f_lineno = 0;
    fp->next_ip = 0;
    fp->exception_stack = 0;
    fp->f_code = code_obj;
    fp->scopes = scopes;
    for (i = 0; i < lcount; i++)
        ITEM(fp, i) = 0;    
    fp->f_back = caller;
    if (caller)
        fp->depth = caller->depth+1;
    else
        fp->depth = 0;
    gvmt_fully_initialized((GVMT_Object)fp);
    fp->next_ip = get_bytecodes_from_code_object(code_obj);
    return fp;
}

R_frame stop_frame(void) {
    R_frame fp = gc_allocate_var(frame, array, 0);
    SET_TYPE(fp, type_frame);
    return fp;
}

R_frame push_frame(R_frame f) {
    R_frame fp;
    fp = gc_allocate_var(frame, array, 1);
    SET_TYPE(fp, type_frame);
    ZERO_REF_COUNT(fp);
    fp->length = 1;
    fp->f_lineno = f->f_lineno;
    fp->next_ip = 0;
    fp->exception_stack = 0;
    fp->f_code = f->f_code;
    fp->f_back = f;
    fp->depth = f->depth+1;
    fp->array[0] = (R_object)new_dict(0);
    fp->scopes = NULL;
    gvmt_fully_initialized((GVMT_Object)fp);
    fp->scopes = new_scopes(f->scopes);
    return fp;
}

R_frame function_frame(R_function func, R_frame caller) {
    int i, lcount;
    R_frame fp;
    TYPE_ASSERT(func, function);
    lcount = func->__code__->locals;
    fp = gc_allocate_var(frame, array, lcount);
    SET_TYPE(fp, type_frame);
    ZERO_REF_COUNT(fp);
    fp->length = lcount;
    fp->f_lineno = 0;
    fp->next_ip = 0;
    fp->exception_stack = 0;
    fp->f_code = func->__code__;
    fp->scopes = func->scopes;
    assert(func->scopes->locals == NULL);
    fp->f_back = caller;
    fp->depth = caller->depth+1;
    for (i = 0; i < lcount; i++)
        ITEM(fp, i) = 0;    
    gvmt_fully_initialized((GVMT_Object)fp);
    if (fp->depth > sys_recursion_limit) {
        raise_char(type_RuntimeError, "maximum recursion depth exceeded");
    }
    return fp;
}

void init_frame_td(R_frame frame, R_function func, R_tuple t, R_dict d) {
    int format = func->parameter_format & TUPLE_DICT_FUNCTION;
    if (func->parameter_format & CLOSURE) {
        assert(func->func_closure);
        t = tuple_prepend((R_object)func->func_closure, t);
    }
    if (format == 0) {  
        params_fixup(func, t, d, frame);
    } else {
        if (format == TUPLE_DICT_FUNCTION) {
            params_fixup_tuple_dict(func, t, d, frame);            
        } else {
            if (format & DICT_FUNCTION) {
                params_fixup_dict(func, t, d, frame);
            } else {
                params_fixup_tuple(func, t, d, frame);
            }
        }
    }
}

static R_scopes make_scopes(R_module module) {
    R_scopes scopes = (R_scopes)gc_allocate(scopes);
    SET_TYPE(scopes, type_scopes);
    scopes-> module = (R_object)module->dictionary;
    scopes->builtins = (R_object)builtins->dictionary;
    return scopes;
}

#ifdef BOOTSTRAP
void parameter_type_check(R_builtin_function func) { }

R_object py_call(R_function func, R_tuple t, R_dict d) {
    R_frame frame = function_frame(func, THREAD_STATE->current_frame);
    init_frame_td(frame, func, t, d);
    THREAD_STATE->current_frame = frame;
    return interpreter(func->bytecodes, THREAD_STATE, frame);
}

void run_script(R_code_object script, R_module module, R_frame caller) {
    R_frame frame = new_frame(script, make_scopes(module), caller);
    THREAD_STATE->current_frame = frame;
    interpreter(get_bytecodes_from_code_object(script), THREAD_STATE, frame);
}

#else

void run_script(R_code_object script, R_module module, R_frame caller) {
    R_frame frame = new_frame(script, make_scopes(module), caller);
    super_interpreter(frame);
}

R_object py_call(R_function func, R_tuple t, R_dict d) {
    R_frame frame = function_frame(func, THREAD_STATE->current_frame);
    init_frame_td(frame, func, t, d);
    if (func->bytecodes == NULL)
        func->bytecodes = get_bytecodes_from_code_object(func->__code__);
    frame->next_ip = func->bytecodes;
    return super_interpreter(frame);
}

static void parameter_type_error(R_builtin_function func, int index, R_type ptype) {
    R_object pname = ITEM(func->paramNames, index);
    R_str fname = func->__name__;
    R_str ptname = ptype->__name__;
    R_type fptype = (R_type)ITEM(func->param_types, index);
    R_str ftname = fptype->__name__;
    char* fmt = "Parameter %s in %s() has incorrect type %s, "
                "should be (subclass of) %s";
    R_str msg = py_sprintf(fmt, pname, fname, ptname, ftname);
    raise_str(type_TypeError, msg);
}

void parameter_type_check(R_builtin_function func) {
    GVMT_Value* parameters = gvmt_stack_top();
    R_tuple param_types = func->param_types;
    int i, len = param_types->length;
    for (i = 0; i < len; i++) {
        R_object p = (R_object)gvmt_stack_read_object(parameters+len-1-i);
        R_type fptype = (R_type)ITEM(param_types, i);
        R_type ptype = TYPE(p);
        if (ptype != fptype && fptype != type_object) {   
            if (!is_proper_subclass_of(ptype, fptype)) {
                parameter_type_error(func, i, TYPE(p));
            }
        }
    }
}

#endif

uint8_t* get_bytecodes_from_code_object(R_code_object co) {
    R_bytes b = co->code;  
    uint8_t* ptr = (uint8_t*)gvmt_pin((GVMT_Object)b);
    return ptr + offsetof(GVMT_OBJECT(bytes), bytes);
}


