#include <assert.h>
#include "hotpy.h"
#include "core_objects.h"
#include "function.h"
#include "operators.h"
#include "intobject.h"

#define CLASS_ID(t) ((t)->unique_index)
#define CODE1(t) (~(CLASS_ID(t)))
#define CODE2(t1, t2) (~(CLASS_ID(t1) << 16 | CLASS_ID(t2)))


static void insert_operator(R_operator op) {
#ifdef BOOTSTRAP
    int i;
    TYPE_ASSERT(op, operator);
    for (i = 0; i < OPERATOR_COUNT; i++) {
        R_str op_name = (R_str)ITEM(operator_names, i);
        if (string_equals(op->name, op_name)) {
            operators[i] = op;
            return;
        }
    }
    py_fprint(stdout, op->name);
    fprintf(stdout, "\n");
    fatal("Operator does not have a name in range\n");
#else
    fatal("Cannot add operators except at VM build\n");
#endif
}

R_object PY_FUNC4(make_operator, R_str name, R_str func_name,
         R_object fallback, R_int arity) {
    R_unary_operator op = gc_allocate(unary_operator);
    SET_TYPE(op, type_unary_operator);
    op->arity = int_from_py_int(arity);
    op->pre_defined = new_empty_hashtable();
    op->name = name;
    op->func_name = func_name;
    op->fallback = fallback;
    assert(TYPE(fallback)->call);
    insert_operator((R_operator)op);
    return (R_object)op;
}

R_object PY_FUNC3(make_unary_operator, R_str name,
         R_str func_name, R_function fallback) {
    R_unary_operator op = gc_allocate(unary_operator);
    SET_TYPE(op, type_unary_operator);
    op->arity = 1;
    op->pre_defined = new_empty_hashtable();
    op->name = name;
    op->func_name = func_name;
    op->fallback = (R_object)fallback;
    insert_operator((R_operator)op);
    return (R_object)op;
}

R_binary_operator new_binary_operator(R_type cls, R_str name, R_str func_name,
         R_str rfunc_name, R_object fallback) {
    R_binary_operator op = gc_allocate(binary_operator);
    SET_TYPE(op, cls);
    op->arity = 2;
    op->pre_defined = new_empty_hashtable();
    op->name = name;
    op->func_name = func_name;
    op->rfunc_name = rfunc_name;
    op->fallback = fallback;
    insert_operator((R_operator)op);
    return op;
}

R_object PY_FUNC4(make_binary_operator, R_str name, R_str func_name,
         R_str rfunc_name, R_object fallback) {
    R_binary_operator op = new_binary_operator(type_binary_operator,
        name, func_name, rfunc_name, fallback);
    return (R_object)op;
}

R_object PY_FUNC4(make_comparison_operator, R_str name, R_str func_name,
         R_str rfunc_name, R_object fallback) {
    R_binary_operator op;
    TYPE_CHECK(name, type_str);
    TYPE_CHECK(func_name, type_str);
    TYPE_CHECK(rfunc_name, type_str);
    op = new_binary_operator(type_comparison_operator, name, func_name,
                             rfunc_name, fallback);
    return (R_object)op;
}

#define ANNOTATION_ERROR_MSG "Annotation of parameter '%s' must be a class"

R_object PY_FUNC3(add_binary_behaviour, R_binary_operator op,
         R_base_function func, R_tuple params) {
    uint32_t i, code, plen;
    assert(IS_PY_FUNCTION(func) ||
           IS_C_FUNCTION(func));
    if (params == (R_tuple)None) {
        if (IS_PY_FUNCTION(func)) {
            params = get_types_from_annotation((R_function)func);
        } else {
            params = ((R_builtin_function)func)->param_types;
        }
    }
    TYPE_ASSERT(params, tuple);
    assert(func->paramNames->length == 2);
    plen = params->length;
    assert(plen <= 2);
    code = 0;
    for (i = 0; i < plen; i++) {
        R_type pcls = TYPE(ITEM(params, i));
        code <<= 16;
        if (!IS_TYPE(pcls)) {
            R_object pname = ITEM(func->paramNames, i);
            R_str msg = py_sprintf(ANNOTATION_ERROR_MSG, pname);
            raise_str(type_TypeError, msg);
        }
        code |= ((R_type)ITEM(params, i))->unique_index;
    }
    for (; i < 2; i++) {
        code <<= 16;
        code |= type_object->unique_index;
    }
    code = ~code;
    // TO DO -- ACQUIRE LOCK ON op
    hashtable_set(op->pre_defined, code, (R_object)func);
    // TO DO -- RELEASE LOCK ON op
    return (R_object)op;
}

/* Unary operator functions */

R_object unary_call(R_operator op, R_object x) {
    R_object o;
    R_tuple t;
    TYPE_ASSERT(op, operator);
    assert(op->arity == 1);
    o = class_attribute(TYPE(x), op->func_name);
    if (o) {
        if (IS_C_FUNCTION(o)) {
            R_builtin_function f = (R_builtin_function)o;
            GVMT_PUSH(x);
            assert(f->paramNames->length == 1);
            return f->executable();
        } else {
            call_func f = CALLABLE(o);
            t = make_tuple1(x);
            return f(o, t, 0);
        }
    }
    t = make_tuple(1);
    ITEM(t, 0) = x;
    return CALLABLE(op->fallback)(op->fallback, t, 0);
}

R_object HotPyIter_Next(R_object x) {
    R_object o = class_attribute(TYPE(x), name___next__);
    R_tuple t;
    R_str msg;
    R_object value;
    R_BaseException ex = 0;
    R_frame f = THREAD_STATE->current_frame;
    if (o) {
        GVMT_TRY(ex)
            if (IS_C_FUNCTION(o)) {
                R_builtin_function f = (R_builtin_function)o;
                GVMT_PUSH(x);
                assert(f->paramNames->length == 1);
                value = f->executable();
            } else {
                call_func f = CALLABLE(o);
                t = make_tuple1(x);
                value = f(o, t, 0);
            }
        GVMT_CATCH
            if (!isinstance(ex, type_StopIteration))
                gvmt_raise((GVMT_Object)ex);
            THREAD_STATE->current_frame = f;
            value = NULL;
        GVMT_END_TRY
        return value;
    }
    msg = py_sprintf("%s object is not an iterator", TYPE_NAME(x));
    raise_str(type_TypeError, msg);
    return NULL;
}

void no_keywords(R_str name) {
    R_str msg = py_sprintf("%s() takes no keyword arguments", name);
    raise_str(type_TypeError, msg);
}

DESC_CALL_SIGNATURE(desc_call_unary_operator) {
    R_operator op = (R_operator)callable;
    R_object x;
    int pcount = tuple->length;
    if (pcount != 1)
        wrong_number_parameters(op->name, pcount, 1, 1);
    x = ITEM(tuple, 0);
    return unary_call(op, x);
}

/* Binary operator functions */

static R_object bin_call(R_object o, R_object x, R_object y) {
    if (IS_C_FUNCTION(o)) {
        R_builtin_function f = (R_builtin_function)o;
        GVMT_PUSH(x);
        GVMT_PUSH(y);
        assert(f->paramNames->length == 2);
        return f->executable();
    } else {
        R_tuple t;
        call_func call = CALLABLE(o);
        t = make_tuple2(x, y);
        return call(o, t, 0);
    }
}

R_object binary_call(R_operator a, R_object x, R_object y) {
    R_binary_operator op = (R_binary_operator)a;
    R_object o;
    R_tuple t;
    R_object result;
    R_type t1 = TYPE(x);
    R_type t2 = TYPE(y);
    // This code is not a hash code as such,
    // since it must be unique for each type pair t1, t2.
    uint32_t code = CODE2(t1, t2);
    TYPE_ASSERT(op, binary_operator);
    assert ((CLASS_ID(t1) & 0xffff) == CLASS_ID(t1));
    assert ((CLASS_ID(t2) & 0xffff) == CLASS_ID(t2));
    o = hashtable_get(op->pre_defined, code);
    if (o) {
        return bin_call(o, x, y);
    }
    if (t1 != t2 && is_proper_subclass_of(t2, t1)) {
        o = class_attribute(t2, op->rfunc_name);
        if (o) {
            result = bin_call(o, x, y);
            if (result != (R_object)NotImplemented)
                return result;
        }
        o = class_attribute(t1, op->func_name);
        if (o) {
            result = bin_call(o, x, y);
            if (result != (R_object)NotImplemented)
                return result;
        }
    } else {
        o = class_attribute(t1, op->func_name);
        if (o) {
            result = bin_call(o, x, y);
            if (result != (R_object)NotImplemented)
                return result;
        }
        o = class_attribute(t2, op->rfunc_name);
        if (o) {
            // Reverse operands
            result = bin_call(o, y, x);
            if (result != (R_object)NotImplemented)
                return result;
        }
    }
    return bin_call(op->fallback, x, y);
}

int binary_compare(uint8_t index, R_object o1, R_object o2) {
    // Need to reverse parameters to match R_object PY_FUNC calling convention.
    R_object b = (R_object)binary_call(operators[index], o1, o2);
    if (IS_BOOL(b))
        return ((R_bool)b)->value;
    else
        return bool_value(b);
}

R_base_function binary_lookup_exact(R_operator o, R_type t0, R_type t1) {
    R_binary_operator op = (R_binary_operator)o;
    uint32_t code = CODE2(t0, t1);
    TYPE_ASSERT(op, binary_operator);
    return (R_base_function) hashtable_get(op->pre_defined, code);
}

DESC_CALL_SIGNATURE(desc_call_binary_operator) {
    R_operator op = (R_operator)callable;
    int pcount = tuple->length;
    R_object x, y;
    if (pcount != 2)
        wrong_number_parameters(op->name, pcount, 2, 2);
    x = ITEM(tuple, 0);
    y = ITEM(tuple, 1);
    return binary_call(op, x, y);
}

/* General operator functions */

R_object inplace_call(R_operator op, R_object x, R_object y) {
    R_type t1 = TYPE(x);
    R_type t2 = TYPE(y);
    R_object f = class_attribute(t1, op->func_name);
    R_object result;
    assert(op->arity == 2);
    TYPE_ASSERT(op->fallback, binary_operator);
    if (f) {
        result = bin_call(f, x, y);
        if (result != (R_object)NotImplemented)
            return result;
    }
    return binary_call((R_operator)op->fallback, x, y);
}

DESC_CALL_SIGNATURE(desc_call_operator) {
    R_operator op = (R_operator)callable;
    R_object o;
    R_object f;
    if (dict && !is_empty(dict))
        no_keywords(op->name);
    if (op->arity != tuple->length) {
        wrong_number_parameters(op->name, tuple->length, op->arity, op->arity);
    }
    assert("To do.." && 0);
//    f = class_attribute(TYPE(o), op->func_name);
//    parameters[pcount-1] = (R_object)f;
//    return CALL(f, pcount);
    return 0;
}
/*
R_object desc_get_operator(R_object descriptor, R_object obj, R_type cls) {
    return obj;
}
*/

R_object vm_contains(R_object cont, R_object item) {
    R_object o;
    R_type tcont = TYPE(cont);
    o = class_attribute(tcont, name___contains__);
    if (o) {
        if (IS_C_FUNCTION(o)) {
            GVMT_PUSH(cont);
            GVMT_PUSH(item);
            return ((R_builtin_function)o)->executable();
        } else {
            R_tuple t;
            call_func call = CALLABLE(o);
            t = make_tuple2(cont, item);
            return call(o, t, 0);
        }
    } else {
        R_str msg = py_sprintf(
            "%s instance has no attribute '__contains__'", tcont->__name__);
        raise_str(type_AttributeError, msg);
        return 0;
    }
}

R_partial_operator make_partial_operator(R_binary_operator op,
                                         R_type type, int right) {
    R_partial_operator pop = gc_allocate(partial_operator);
    SET_TYPE(pop, type_partial_operator);
    pop->type = type;
    pop->op = op;
    pop->right = right;
    return pop;
}

/*************** Partial operators ***********
 * Partial operators are operators whose first parameter
 * type has already been selected. They exist so that int.__add__
 * and similar can exist.
 *
 * When called, partial operators check the type of their first parameter,
 * They then dispatch on the pre-defined operator table only,
 * returning NotImplemented if no suitable method */

R_object partial_operator_call(R_partial_operator op, R_object l, R_object r) {
    int code;
    R_tuple t;
    R_base_function f;
    R_type ltype, rtype;
    call_func call;
    ltype = op->type;
    rtype = TYPE(r);
    if (!isinstance(l, op->type)) {
        R_str msg = py_sprintf(
            "partial operator '%s' requires a '%s', but recieved a '%s'",
            TYPE_NAME(op), op->op->name, op->type, TYPE(l));
        raise_str(type_TypeError, msg);
    }
    if (op->right)
        code = CODE2(TYPE(r), op->type);
    else
        code = CODE2(op->type, TYPE(r));
    f = (R_base_function) hashtable_get(op->op->pre_defined, code);
    if (f == 0)
        return (R_object)NotImplemented;
    if (op->right) {
        R_object tmp = l;
        l = r;
        r = tmp;
    }
    call = CALLABLE(f);
    t = make_tuple(2);
    ITEM(t, 0) = l;
    ITEM(t, 1) = r;
    return call((R_object)f, t, 0);
}


