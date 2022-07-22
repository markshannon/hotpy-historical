
#include "hotpy.h"

extern R_object globals_to_consts(uint8_t* start, uint8_t* end, R_function f);
extern R_object substitute_specials_for_binary(uint8_t* start, uint8_t* end, R_function f);
extern R_object insert_tail_calls(uint8_t* start, uint8_t* end);

R_object PY_FUNC1(c_name, R_function f) {
    TYPE_CHECK(f, type_function);
    f->__code__->kind |= C_NAME;
    return (R_object)f;
}

R_object PY_FUNC1(_no_trace, R_function f) {
    TYPE_CHECK(f, type_function);
    f->__code__->kind |= FUNCTION_NO_TRACE;
    f->bytecodes = get_bytecodes_from_code_object(f->__code__);
    insert_tail_calls(f->bytecodes, f->bytecodes + LEN(f->__code__->code));
    return (R_object)f;
}

R_object PY_FUNC1(_globals_to_consts, R_function f) {
    TYPE_CHECK(f, type_function);
    f->bytecodes = get_bytecodes_from_code_object(f->__code__);
    globals_to_consts(f->bytecodes, f->bytecodes + LEN(f->__code__->code), f);
    return (R_object)f;
}

R_object PY_FUNC1(_no_overflow, R_function f) {
    TYPE_CHECK(f, type_function);
    f->__code__->kind |= FUNCTION_NO_OVERFLOW;
    return (R_object)f;
}

R_object PY_FUNC1(_pure, R_builtin_function f) {
    TYPE_CHECK(f, type_builtin_function);
    f->parameter_format |= FUNCTION_PURE;
    return (R_object)f;
}

R_object PY_FUNC1(_no_raise, R_builtin_function f) {
    TYPE_CHECK(f, type_builtin_function);
    f->parameter_format |= FUNCTION_NO_RAISE;
    return (R_object)f;
}

R_scopes new_scopes(R_scopes old) {
    R_scopes scopes = (R_scopes)gc_allocate(scopes);
    SET_TYPE(scopes, type_scopes);
    scopes->module = old->module;
    scopes->builtins = old->builtins;
    return scopes;
}

R_function make_function(R_code_object f, R_tuple defaults,
                         R_dict annotations, R_scopes scopes) {
    int i, kind, type_count;
    R_function function = gc_allocate(function);
    R_str fname;
    SET_TYPE(function, type_function);
    assert(f->code->length);
    function->__code__ = f;
    function->scopes = new_scopes(scopes);
    assert(function->scopes->locals == NULL);
    fname = f->name;
    function->__name__ = fname;
    type_count = f->params;
    assert(type_count <= f->local_names->length);
    function->paramNames = gc_allocate_tuple(type_count);
    SET_TYPE(function->paramNames, type_tuple);
    function->paramNames->length = type_count;
    for (i = 0; i < type_count; i++) {
        R_object o = ITEM(f->local_names, i);
        assert(o != NULL);
        ITEM(function->paramNames, i) = o;
    }
    function->parameter_format = f->kind;
    function->bytecodes = get_bytecodes_from_code_object(f);
    function->byte_code_length = f->code->length;
    function->dictionary = new_empty_dict();
    function->__annotations__ = annotations;
    function->defaults = defaults;
    return function;
}

#define IS_GENERATOR_FUNCTION(f) ((f)->parameter_format & GENERATOR_FUNCTION)

R_dict make_annotations(R_tuple names, R_list types) {
    int i, len = names->length;
    R_dict a = new_dict(len);
    if (len != LIST_SIZE(types))
        raise_char(type_InternalError, "Illegal annotations");
    for (i = 0; i < len; i++) {
        dict_set_str(a, (R_str)ITEM(names, i), ITEM(types->array, i));
    }
    return a;
}

R_tuple get_types_from_annotation(R_function func) {
    int i, type_count = func->paramNames->length;
    R_tuple types;
    if (func->parameter_format & TUPLE_FUNCTION)
        type_count--;
    if (func->parameter_format & DICT_FUNCTION)
        type_count--;
    types = make_tuple(type_count);
    for (i = 0; i < type_count; i++) {
        R_str name = (R_str)ITEM(func->paramNames, i);
        R_object type = (R_object)dict_get_str(func->__annotations__, name);
        if (type)
            ITEM(types, i) = type;
        else
            ITEM(types, i) = (R_object)type_object;
    }
    return types;
}

R_type get_return_type_from_annotation(R_function func) {
    R_object type = dict_get_str(func->__annotations__, name_return);
    if (type && TYPE(type) == type_type)
        return (R_type)type;
    else
        return NULL;
}

char* PY_name(R_function f, char* buf) {
    char *p = buf + 4;
    sprintf(buf, "PY_");
    p += py_print(p, f->__name__);
    if (f->__code__->kind & C_NAME)
        *p = 0;
    else
        sprintf(p, "_0");
    return buf;
}

void null_local(R_frame f, uintptr_t index) {
    raise_str(type_UnboundLocalError, (R_str)ITEM(f->f_code->local_names, index));
}

int is_simple_function(R_base_function f, int param_count) {
    int format;
    R_type klass;
    if (f == 0) {
        return 0;
    }
    klass = TYPE(f);
    if (klass == type_function || klass == type_builtin_function) {
        format = f->parameter_format & (TUPLE_FUNCTION | DICT_FUNCTION);
        return (format == 0 && f->paramNames->length == param_count);
    } else {
        return 0;
    }
}

/********* Function descriptor methods *********/

DESC_CALL_SIGNATURE(desc_call_function) {
    R_function func = (R_function)callable;
    GVMT_Object result;
    TYPE_ASSERT(func, function);
    // Could refactor this & new_frame/function_frame
    if (IS_GENERATOR_FUNCTION(func)) {
        R_frame frame;
        if (func->bytecodes == NULL)
            func->bytecodes = get_bytecodes_from_code_object(func->__code__);
        assert(func->scopes->locals == NULL);
        frame = new_frame(func->__code__, func->scopes, NULL);
        init_frame_td(frame, func, tuple, dict);
        return HotPyGen_New(frame);
    } else {
        return py_call(func, tuple, dict);
    }
}

R_object _HotPy_bind_method(R_object obj, R_object callable) {
    R_bound_method bm = gc_allocate(bound_method);
    SET_TYPE(bm, type_bound_method);
    bm->object = obj;
    assert(IS_CALLABLE(callable));
    bm->callable = callable;
    return (R_object)bm;
}

R_object desc_get_function(R_object func, R_object obj, R_type cls) {
    assert(IS_PY_FUNCTION(func));
    if (obj != NULL) {
        return _HotPy_bind_method(obj, func);
    } else {
        return func;
    }
}

/********* Builtin function descriptor methods *********/


/** This either unpacks the parameters to the stack OR
 *  raises an exception */
 /** This needs to be reorganised */
void unpack_native_parameters(R_builtin_function func, R_tuple tuple, R_dict dict) {
    int i, pcount, acount;
    GVMT_Value* stack;
    TYPE_ASSERT(func, builtin_function);
    if (dict) {
        // To do: No reason why we can't handle named parameters.
        if (!is_empty(dict)) {
            no_keywords(func->__name__);
        }
    }
    pcount = func->paramNames->length;
    acount = tuple->length;
    stack = gvmt_insert(pcount);
    assert((func->parameter_format & TUPLE_DICT_FUNCTION) == 0);
    if (acount > pcount) {
        int min = pcount - func->defaults->length;
        wrong_number_parameters(func->__name__, acount, min, pcount);
    }
    for (i = 0; i < acount; i++) {
        R_object o = ITEM(tuple, i);
        gvmt_stack_write_object(stack+pcount-1-i, (GVMT_Object)o);
    }
    if (acount < pcount) {
        int defaults = func->defaults->length;
        int min = pcount - defaults;
        if (acount < min) {
            wrong_number_parameters(func->__name__, acount, min, pcount);
        }
        for (; i < pcount; i++) {
            R_object o = ITEM(func->defaults, i-min);
            gvmt_stack_write_object(stack+pcount-1-i, (GVMT_Object)o);
        }
    }
    parameter_type_check(func);
}

DESC_CALL_SIGNATURE(desc_call_builtin_function) {
    R_builtin_function func = (R_builtin_function)callable;
    unpack_native_parameters(func, tuple, dict);
    return func->executable();
}

R_object desc_get_builtin_function(R_object func, R_object obj, R_type cls) {
    assert(IS_C_FUNCTION(func));
    if (obj != NULL) {
        return _HotPy_bind_method(obj, func);
    } else {
        return func;
    }
}

/******** Bound method descriptor methods ****************/


R_object desc_get_bound_method(R_object descriptor, R_object obj, R_type cls) {
    return (R_object)descriptor;
}

DESC_CALL_SIGNATURE(desc_call_bound_method) {
    R_bound_method bm = (R_bound_method)callable;
    R_tuple args;
    TYPE_ASSERT(bm, bound_method);
    args = tuple_prepend(bm->object, tuple);
    return CALLABLE(bm->callable)(bm->callable, args, dict);
}

/** Static method */

R_object PY_FUNC1(staticmethod, R_object func) {
    R_static_method sm = gc_allocate(static_method);
    SET_TYPE(sm, type_static_method);
    sm->callable = func;
    return (R_object)sm;
}

R_object desc_get_static_method(R_object desc, R_object obj, R_type cls) {
    R_static_method sm = (R_static_method)desc;
    return (R_object)sm->callable;
}

/** Class method */

R_object PY_FUNC1(classmethod, R_object func) {
    R_class_method cm = gc_allocate(class_method);
    SET_TYPE(cm, type_class_method);
    cm->callable = func;
    return (R_object)cm;
}

R_object desc_get_class_method(R_object desc, R_object obj, R_type cls) {
    R_class_method cm = (R_class_method)desc;
    if (cls == NULL)
        cls = TYPE(obj);
    return _HotPy_bind_method((R_object)cls, cm->callable);
}



