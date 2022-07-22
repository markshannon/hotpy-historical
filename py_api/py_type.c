#include <string.h>
#include "hotpy.h"
#include "py_api/Python.h"
#define PIN(x) (gvmt_is_tagged(x) ? ((PyObject*)gvmt_untag(x)) : ((PyObject*)gvmt_pin((GVMT_Object)(x))))
#define UNPIN(x) ((R_object)gvmt_pinned_object(x))
#define UNPIN_TYPE(x) ((R_type)gvmt_pinned_object(x))

static R_object make_member(PyMemberDef* member, R_str name) {
    int type = member->type;
    int writeable = (member->flags != READONLY);
    R_str doc =  string_from_c_string(member->doc);
    R_object result;
    if (type == T_INT && sizeof(int) == sizeof(long) || type == T_LONG) {
        R_int_slot_descriptor d = gc_allocate(int_slot_descriptor);
        SET_TYPE(d, type_int_slot_descriptor);
        d->name = name;
        if (member->doc)
            d->__doc__ = string_from_c_string(member->doc);
        else 
            d->__doc__ = empty_string;
        d->offset = member->offset;
        if (writeable)
            d->flags = SLOT_WRITEABLE;
        result = (R_object)d;
    } else if (type == T_OBJECT_EX) {
        R_slot_descriptor d = gc_allocate(slot_descriptor);
        SET_TYPE(d, type_slot_descriptor);
        d->name = name;
        if (member->doc)
            d->__doc__ = string_from_c_string(member->doc);
        else 
            d->__doc__ = empty_string;
        d->offset = member->offset;
        result = (R_object)d;
    } else {
        assert ("To do..." && 0);   
        result = 0;
    }
    return result;
}

static void add_method(R_type cls, PyMethodDef* def) {
    R_str name = string_from_c_string(def->ml_name);
    int flags = def->ml_flags;
    R_method_api f;
    if ((flags & METH_COEXIST) == 0) {
        if (str_dict_table_get(cls->attributes, name) != NULL)
            return;
    }
    f = gc_allocate(method_api);
    SET_TYPE(f, type_method_api);
    f->__name__ = name;
    f->flags = flags;
    f->func = def->ml_meth;
    if (def->ml_doc)
        f->__doc__ = string_from_c_string(def->ml_doc);
    else 
        f->__doc__ = empty_string;
    f->defaults = empty_tuple;
    if (flags & METH_KEYWORDS) {
        f->paramNames = tuple_prepend((R_object)name_self, 
                        make_tuple2((R_object)name_args, (R_object)name_kws)); 
        f->parameter_format = TUPLE_FUNCTION|DICT_FUNCTION;
    } else if (flags & METH_NOARGS) {
        f->parameter_format = 0;
        f->paramNames = make_tuple1((R_object)name_self);
    } else if (flags & METH_O) {
        f->parameter_format = 0;
        f->paramNames = make_tuple2((R_object)name_self, (R_object)name_p0);
    } else {
        f->parameter_format = TUPLE_FUNCTION;
        f->paramNames = make_tuple2((R_object)name_self, (R_object)name_args);
    }
}

R_object desc_get_method_api(R_object func, R_object obj, R_type cls) {
    assert(TYPE(func) == type_method_api);
    if (obj != NULL) {
        R_bound_method bm = gc_allocate(bound_method);
        SET_TYPE(bm, type_bound_method);
        bm->object = obj;
        bm->callable = func;
        return (R_object)bm;
    } else {
        return func;       
    }
}

DESC_CALL_SIGNATURE(desc_call_method_api) {
    R_method_api m = (R_method_api)callable;
    int flags = m->flags;
    R_tuple args;
    R_object self;
    PyObject *s, *t, *d, *x;
    R_object result;
    assert(tuple->length > 0);
    if (flags & METH_STATIC) {
        s = NULL;
    } else {
        self = ITEM(tuple, 0);
        if (flags & METH_CLASS) {
            if (!isinstance(self, type_type)) {
                self = (R_object)TYPE(self);
            }
        }
        s = PIN(self);
    }
    if (flags & METH_KEYWORDS) {
        assert((flags & METH_NOARGS) == 0);
        assert((flags & METH_O) == 0);
        args = tuple_slice(tuple, 1, tuple->length);
        t = PIN(args);
        d = PIN(dict);
        ACQUIRE_GIL;
        x = ((PyCFunctionWithKeywords)m->func)(s, t, d);
        gvmt_refer((GVMT_Object)args);/* Ensure that arguments aren't GCed */
    } else {
        if (!is_empty(dict)) {
            no_keywords(m->__name__);
        }
        if (flags & METH_NOARGS) {
            args = 0;
            assert((flags & METH_O) == 0);
            assert((flags & METH_VARARGS) == 0);
            if (tuple->length > 1) {
                wrong_number_parameters(m->__name__, tuple->length, 1, 1);
            }
            ACQUIRE_GIL;
            x = ((PyCFunction)m->func)(s, NULL);
        }  else if (flags & METH_O) {
            assert((flags & METH_VARARGS) == 0);
            if (tuple->length != 2) {
                wrong_number_parameters(m->__name__, tuple->length, 2, 2);
            }
            args = tuple;
            t = PIN(ITEM(tuple, 1));
            ACQUIRE_GIL;
            x = ((PyCFunction)m->func)(s, t);
        } else { // VARARGS
            args = tuple_slice(tuple, 1, tuple->length);
            t = PIN(args);
            ACQUIRE_GIL;
            x = ((PyCFunction)m->func)(s, t);
        }
    }
    gvmt_refer((GVMT_Object)dict);
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    return result;
}

R_object desc_get_getset_wrapper(R_object descriptor, R_object obj, R_type cls) {
    R_getset_wrapper gs = (R_getset_wrapper)descriptor;
    R_object result;
    PyObject *x, *o;
    o = PIN(obj);
    ACQUIRE_GIL;
    x = gs->getter(o, gs->closure);
    gvmt_refer((GVMT_Object)obj);
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    return result;
}

void desc_set_getset_wrapper(R_object descriptor, R_object obj, R_object value) {
    R_getset_wrapper gs = (R_getset_wrapper)descriptor;
    PyObject *v, *o;
    int x;
    o = PIN(obj);
    v = PIN(value);
    ACQUIRE_GIL;
    x = gs->setter(o, v, gs->closure);
    RELEASE_GIL;
    gvmt_refer((GVMT_Object)obj);
    gvmt_refer((GVMT_Object)value);
    if (x)
        RAISE(THREAD_STATE->c_exception);
}

static R_object make_prop(PyGetSetDef* def, R_str name) {
    R_getset_wrapper gs = gc_allocate(getset_wrapper);
    SET_TYPE(gs, type_getset_wrapper);
    gs->name = name;
    gs->getter = def->get;
    gs->setter = def->set;
    if (def->doc)
        gs->__doc__ = string_from_c_string(def->doc);
    else 
        gs->__doc__ = empty_string;
    gs->closure = def->closure;
    return (R_object)gs;
}

static void make_slots(R_type cls, PyMemberDef* members) {
    PyMemberDef* member = members;
    while(member) {
        R_str name = string_from_c_string(member->name);
        type_setattr((R_object)cls, name, make_member(member, name));
        member++;   
    }
}

static R_object wrapped_getattro(R_object object, R_str name) {
    PyObject *attr, *obj, *n;
    R_object result;
    getattrofunc func = TYPE(object)->api_getattro;
    assert(func);
    obj = PIN(object);
    n = PIN(name);
    attr = func(obj, n);
    gvmt_refer((GVMT_Object)object);
    gvmt_refer((GVMT_Object)name);
    ACQUIRE_GIL;
    if (attr) {
        result = UNPIN(attr);
        Py_DECREF(attr);
        RELEASE_GIL;
        return result;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        return 0;
    }
}

static void wrapped_setattro(R_object object, R_str name, R_object value) {
    int res;
    PyObject *attr, *obj, *n;
    setattrofunc func = TYPE(object)->api_setattro;
    assert(func);
    obj = PIN(object);
    n = PIN(name);
    attr = PIN(value);
    res = func(obj, n, attr);
    if (res) {
        RAISE(THREAD_STATE->c_exception);
    }
}

static R_object wrapped_call(R_object callable, R_tuple tuple, R_dict dict) {
    PyObject *c, *t, *d, *x;
    R_object result;
    c = PIN(callable);
    t = PIN(tuple);
    d = PIN(dict);
    ACQUIRE_GIL;
    x = TYPE(callable)->api_call(c, t, d);
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    return result;
    
}

DESC_CALL_SIGNATURE(desc_call_len_api) {
    R_len_api lf = (R_len_api)callable;
    Py_ssize_t l;
    if (!is_empty(dict)) {
        no_keywords(name___len__);
    }
    if (tuple->length != 1) 
        wrong_number_parameters(name___len__, tuple->length, 1, 1);
    ACQUIRE_GIL;
    l = lf->func(PIN(ITEM(tuple, 0)));
    RELEASE_GIL;
    gvmt_refer((GVMT_Object)tuple);
    if (l < 0)
        RAISE(THREAD_STATE->c_exception);
    return (R_object)R_int_from_ssize_t(l);
}

static R_object wrap_len_func(lenfunc func) {
    R_len_api len = gc_allocate(len_api);   
    SET_TYPE(len, type_len_api);
    len->func = func;
    return (R_object)len;
}

DESC_CALL_SIGNATURE(desc_call_binary_api) {
    R_binary_api bf = (R_binary_api)callable;
    PyObject *x;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(bf->name);
    }
    if (tuple->length != 2) 
        wrong_number_parameters(bf->name, tuple->length, 2, 2);
    ACQUIRE_GIL;
    x = bf->func(PIN(ITEM(tuple, 0)), PIN(ITEM(tuple, 1)));
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    return result;
}

static R_object wrap_binary_func(binaryfunc func, R_str name) {
    R_binary_api binary = gc_allocate(binary_api);   
    SET_TYPE(binary, type_binary_api);
    binary->name = name;
    binary->func = func;
    return (R_object)binary;
}

DESC_CALL_SIGNATURE(desc_call_binary_r_api) {
    R_binary_r_api bf = (R_binary_r_api)callable;
    PyObject *x;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(bf->name);
    }
    if (tuple->length != 2) 
        wrong_number_parameters(bf->name, tuple->length, 2, 2);
    ACQUIRE_GIL;
    x = bf->func(PIN(ITEM(tuple, 1)), PIN(ITEM(tuple, 2)));
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    return result;
}

static R_object wrap_binary_r_func(binaryfunc func, R_str name) {
    R_binary_r_api r = gc_allocate(binary_r_api);   
    SET_TYPE(r, type_binary_r_api);
    r->name = name;
    r->func = func;
    return (R_object)r;
}

DESC_CALL_SIGNATURE(desc_call_binary_pred_api) {
    R_binary_pred_api pred = (R_binary_pred_api)callable;
    int x;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(pred->name);
    }
    if (tuple->length != 2) 
        wrong_number_parameters(pred->name, tuple->length, 2, 2);
    ACQUIRE_GIL;
    x = pred->func(PIN(ITEM(tuple, 0)), PIN(ITEM(tuple, 1)));
    RELEASE_GIL;
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    if (x > 0) {
        return (R_object)TRUE;
    } else if (x == 0) {
        return (R_object)FALSE;
    } else {
        RAISE(THREAD_STATE->c_exception);
        return 0;
    }
}

static R_object wrap_binary_pred(objobjproc func, R_str name) {
    R_binary_pred_api pred = gc_allocate(binary_pred_api);   
    SET_TYPE(pred, type_binary_pred_api);
    pred->name = name;
    pred->func = func;
    return (R_object)pred;
}

DESC_CALL_SIGNATURE(desc_call_unary_pred_api) {
    R_unary_pred_api pred = (R_unary_pred_api)callable;
    int x;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(pred->name);
    }
    if (tuple->length != 1) 
        wrong_number_parameters(pred->name, tuple->length, 1, 1);
    ACQUIRE_GIL;
    x = pred->func(PIN(ITEM(tuple, 0)));
    RELEASE_GIL;
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    if (x > 0) {
        return (R_object)TRUE;
    } else if (x == 0) {
        return (R_object)FALSE;
    } else {
        RAISE(THREAD_STATE->c_exception);
        return 0;
    }
}

static R_object wrap_unary_pred(inquiry func, R_str name) {
    R_unary_pred_api pred = gc_allocate(unary_pred_api);   
    SET_TYPE(pred, type_unary_pred_api);
    pred->name = name;
    pred->func = func;
    return (R_object)pred;
}

DESC_CALL_SIGNATURE(desc_call_hash_api) {
    R_hash_api h = (R_hash_api)callable;
    int x;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(h->name);
    }
    if (tuple->length != 1) 
        wrong_number_parameters(h->name, tuple->length, 1, 1);
    ACQUIRE_GIL;
    x = h->func(PIN(ITEM(tuple, 0)));
    RELEASE_GIL;
    
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    if (x == -1) {
        RAISE(THREAD_STATE->c_exception);
    }
    return (R_object)R_int_from_ssize_t(x);
}

static R_object wrap_hash(hashfunc func) {
    R_hash_api h = gc_allocate(hash_api);   
    SET_TYPE(h, type_hash_api);
    h->name = name___hash__;
    h->func = func;
    return (R_object)h;
}

DESC_CALL_SIGNATURE(desc_call_unary_api) {
    R_unary_api bf = (R_unary_api)callable;
    PyObject *x;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(bf->name);
    }
    if (tuple->length != 1) 
        wrong_number_parameters(bf->name, tuple->length, 1, 1);
    ACQUIRE_GIL;
    x = bf->func(PIN(ITEM(tuple, 0)));
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    return result;
}

static R_object wrap_unary_func(unaryfunc func, R_str name) {
    R_unary_api unary = gc_allocate(unary_api);   
    SET_TYPE(unary, type_unary_api);
    unary->name = name;
    unary->func = func;
    return (R_object)unary;
}

DESC_CALL_SIGNATURE(desc_call_setitem_api) {
    R_setitem_api tp = (R_setitem_api)callable;
    int x;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(tp->name);
    }
    if (tuple->length != 3) 
        wrong_number_parameters(tp->name, tuple->length, 3, 3);
    ACQUIRE_GIL;
    x = tp->func(PIN(ITEM(tuple, 0)), PIN(ITEM(tuple, 1)), PIN(ITEM(tuple, 2)));
    RELEASE_GIL;
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed during call */
    if (x)
        RAISE(THREAD_STATE->c_exception);
    return (R_object)None;
}

static R_object wrap_setitem(objobjargproc proc, R_str name) {
    R_setitem_api s = gc_allocate(setitem_api);   
    SET_TYPE(s, type_setitem_api);
    s->name = name;
    s->func = proc;
    return (R_object)s;
}

DESC_CALL_SIGNATURE(desc_call_objint_api) {
    R_objint_api r = (R_objint_api)callable;
    PyObject *x;
    R_int i;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(r->name);
    }
    if (tuple->length != 2) 
        wrong_number_parameters(r->name, tuple->length, 2, 2);
    i = (R_int)ITEM(tuple, 1);
    if (!IS_INT(i))
        i = HotPyNumber_Index((R_object)i);
    ACQUIRE_GIL;
    x = r->func(PIN(ITEM(tuple, 0)), int_from_py_int(i));
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        result = 0;   
    }
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    return result;
}

static R_object wrap_objint_func(ssizeargfunc func, R_str name) {
    R_objint_api s = gc_allocate(objint_api);   
    SET_TYPE(s, type_objint_api);
    s->name = name;
    s->func = func;
    return (R_object)s;
}

DESC_CALL_SIGNATURE(desc_call_objintobj_api) {
    R_objintobj_api p = (R_objintobj_api)callable;
    int x;
    R_int i;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(p->name);
    }
    if (tuple->length != 3) 
        wrong_number_parameters(p->name, tuple->length, 3, 3);
    i = (R_int)ITEM(tuple, 1);
    if (!IS_INT(i))
        i = HotPyNumber_Index((R_object)i);
    ACQUIRE_GIL;
    x = p->func(PIN(ITEM(tuple, 0)), int_from_py_int(i), PIN(ITEM(tuple, 2)));
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    if (x)
        RAISE(THREAD_STATE->c_exception);
    return (R_object)None;
}

static R_object wrap_objintobj_proc(ssizeobjargproc func, R_str name) {
    R_objintobj_api s = gc_allocate(objintobj_api);   
    SET_TYPE(s, type_objintobj_api);
    s->name = name;
    s->func = func;
    return (R_object)s;
}

static void mapping_protocol(R_type cls, PyMappingMethods *mapping) {
    if (mapping->mp_length)
        type_setattr((R_object)cls, name___len__, 
                     wrap_len_func(mapping->mp_length));  
    if (mapping->mp_subscript)
        type_setattr((R_object)cls, name___getitem__, 
                     wrap_binary_func(mapping->mp_subscript, name___getitem__));
    if (mapping->mp_ass_subscript)
        type_setattr((R_object)cls, name___setitem__,
                     wrap_setitem(mapping->mp_ass_subscript, name___setitem__));
}

static void sequence_protocol(R_type cls, PySequenceMethods *sequence) {
    if (sequence->sq_length)
        type_setattr((R_object)cls, name___len__, 
                         wrap_len_func(sequence->sq_length));   
    if (sequence->sq_concat)
        type_setattr((R_object)cls, name___add__,
                     wrap_binary_func(sequence->sq_concat, name___add__));
    if (sequence->sq_repeat)
        type_setattr((R_object)cls, name___mul__,
                     wrap_objint_func(sequence->sq_repeat, name___mul__));
    if (sequence->sq_item)
        type_setattr((R_object)cls, name___getitem__,
                     wrap_objint_func(sequence->sq_item, name___getitem__));
    if (sequence->sq_contains)
        type_setattr((R_object)cls, name___contains__,
                     wrap_binary_pred(sequence->sq_contains, name___contains__));
    if (sequence->sq_ass_item)
        type_setattr((R_object)cls, name___setitem__,
                     wrap_objintobj_proc(sequence->sq_ass_item, name___setitem__));
        
    if (sequence->sq_inplace_concat)
        type_setattr((R_object)cls, name___iadd__, 
                     wrap_binary_func(sequence->sq_inplace_concat, name___iadd__));
    if (sequence->sq_inplace_repeat)
        type_setattr((R_object)cls, name___imul__,
                     wrap_objint_func(sequence->sq_inplace_repeat, name___imul__));
        
}

static void number_protocol(R_type cls, PyNumberMethods *number) {
    if (number->nb_add) {
        type_setattr((R_object)cls, name___add__, 
                     wrap_binary_func(number->nb_add, name___add__));
        type_setattr((R_object)cls, name___radd__, 
                     wrap_binary_r_func(number->nb_add, name___add__));
    }
    if (number->nb_subtract) {
        type_setattr((R_object)cls, name___sub__, 
                     wrap_binary_func(number->nb_subtract, name___sub__));
        type_setattr((R_object)cls, name___rsub__, 
                     wrap_binary_r_func(number->nb_subtract, name___sub__));
    }
    if (number->nb_multiply) {    
        type_setattr((R_object)cls, name___mul__, 
                     wrap_binary_func(number->nb_multiply, name___mul__));
        type_setattr((R_object)cls, name___rmul__, 
                     wrap_binary_r_func(number->nb_multiply, name___mul__));
    }
    if (number->nb_remainder) {    
        type_setattr((R_object)cls, name___mod__, 
                     wrap_binary_func(number->nb_remainder, name___mod__));
        type_setattr((R_object)cls, name___rmod__, 
                     wrap_binary_r_func(number->nb_remainder, name___mod__));
    }
    if (number->nb_true_divide) {    
        type_setattr((R_object)cls, name___div__, 
                     wrap_binary_func(number->nb_true_divide, name___div__));
        type_setattr((R_object)cls, name___rdiv__, 
                     wrap_binary_r_func(number->nb_true_divide, name___div__));
    }
    if (number->nb_floor_divide) {    
        type_setattr((R_object)cls, name___floordiv__, 
                     wrap_binary_func(number->nb_floor_divide, name___floordiv__));
        type_setattr((R_object)cls, name___rfloordiv__, 
                     wrap_binary_r_func(number->nb_floor_divide, name___floordiv__));
    }
    
    if (number->nb_negative)   
        type_setattr((R_object)cls, name___neg__, 
                     wrap_unary_func(number->nb_negative, name___neg__));
    if (number->nb_positive)   
        type_setattr((R_object)cls, name___pos__, 
                     wrap_unary_func(number->nb_positive, name___pos__));
    if (number->nb_absolute)   
        type_setattr((R_object)cls, name___abs__, 
                     wrap_unary_func(number->nb_absolute, name___abs__));
    if (number->nb_invert)
        type_setattr((R_object)cls, name___invert__, 
                     wrap_unary_func(number->nb_invert, name___invert__));
    if (number->nb_bool)          
        type_setattr((R_object)cls, name___bool__, 
                     wrap_unary_pred(number->nb_bool, name___bool__));
        
    if (number->nb_and) {        
        type_setattr((R_object)cls, name___and__, 
                     wrap_binary_func(number->nb_and, name___and__));
        type_setattr((R_object)cls, name___rand__, 
                     wrap_binary_r_func(number->nb_and, name___and__));
    }
    if (number->nb_or) {  
        type_setattr((R_object)cls, name___or__, 
                     wrap_binary_func(number->nb_or, name___or__));
        type_setattr((R_object)cls, name___ror__, 
                     wrap_binary_r_func(number->nb_or, name___or__));
    }
    if (number->nb_xor) {
        type_setattr((R_object)cls, name___xor__, 
                     wrap_binary_func(number->nb_xor, name___xor__));
        type_setattr((R_object)cls, name___rxor__, 
                     wrap_binary_r_func(number->nb_xor, name___xor__));
    }

    if (number->nb_int)
        type_setattr((R_object)cls, name___int__, 
                     wrap_unary_func(number->nb_int, name___int__));

    if (number->nb_float)
        type_setattr((R_object)cls, name___float__, 
                     wrap_unary_func(number->nb_float, name___float__));
    
    if (number->nb_inplace_add) {
        type_setattr((R_object)cls, name___iadd__, 
                     wrap_binary_func(number->nb_inplace_add, name___iadd__));
    }
    if (number->nb_inplace_subtract) {
        type_setattr((R_object)cls, name___isub__, 
                     wrap_binary_func(number->nb_inplace_subtract, name___isub__));
    }
    if (number->nb_inplace_multiply) {
        type_setattr((R_object)cls, name___imul__, 
                     wrap_binary_func(number->nb_inplace_multiply, name___imul__));
    }
    if (number->nb_inplace_remainder) {
        type_setattr((R_object)cls, name___imod__, 
                     wrap_binary_func(number->nb_inplace_remainder, name___imod__));
    }
    if (number->nb_inplace_true_divide) {
        type_setattr((R_object)cls, name___idiv__, 
                     wrap_binary_func(number->nb_inplace_true_divide, name___idiv__));
    }
    if (number->nb_inplace_floor_divide) {
        type_setattr((R_object)cls, name___ifloordiv__, 
                     wrap_binary_func(number->nb_inplace_floor_divide, name___ifloordiv__));
    }
    if (number->nb_inplace_and) {
        type_setattr((R_object)cls, name___iand__, 
                     wrap_binary_func(number->nb_inplace_and, name___iand__));
    }
    if (number->nb_inplace_or) {
        type_setattr((R_object)cls, name___ior__, 
                     wrap_binary_func(number->nb_inplace_or, name___ior__));
    }
    if (number->nb_inplace_xor) {
        type_setattr((R_object)cls, name___ixor__, 
                     wrap_binary_func(number->nb_inplace_xor, name___ixor__));
    }
    if (number->nb_inplace_lshift) {
        type_setattr((R_object)cls, name___ilshift__, 
                     wrap_binary_func(number->nb_inplace_lshift, name___ilshift__));
    }
    if (number->nb_inplace_rshift) {
        type_setattr((R_object)cls, name___irshift__, 
                     wrap_binary_func(number->nb_inplace_rshift, name___irshift__));
    }

    if (number->nb_index)
        type_setattr((R_object)cls, name___index__, 
                     wrap_unary_func(number->nb_index, name___index__));

}

static void  not_hashable(R_type cls) {
    // Should set some flag...   
}

static void hash_protocol(R_type cls, hashfunc func) {
    if (func) {
        if (func == PyObject_HashNotImplemented)
            not_hashable(cls);
        else
            type_setattr((R_object)cls, name___hash__,
                         wrap_hash(func));
    }    
}

static void buffer_protocol(R_type cls, PyBufferProcs *procs) {
    // To do...
}

DESC_CALL_SIGNATURE(desc_call_compare_api) {
    R_compare_api comp = (R_compare_api)callable;
    PyObject *u, *v, *x;
    R_object result;
    assert(dict);
    if (!is_empty(dict)) {
        no_keywords(comp->name);
    }
    if (tuple->length != 2) 
        wrong_number_parameters(comp->name, tuple->length, 2, 2);
    u = PIN(ITEM(tuple, 0));
    v = PIN(ITEM(tuple, 1));
    ACQUIRE_GIL;
    x = comp->func(u, v, comp->op);
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed */
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
        return result;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        return 0;
    }
}

static R_object wrap_compare(richcmpfunc func, R_str name, int op) {
    R_compare_api comp = gc_allocate(compare_api);   
    SET_TYPE(comp, type_compare_api);
    comp->func = func;
    comp->name = name;
    comp->op = op;
    return (R_object)comp;
}

static void compare_protocol(R_type cls, richcmpfunc richcompare) {
    type_setattr((R_object)cls, name___lt__,
                 wrap_compare(richcompare, name___lt__, Py_LT));
    type_setattr((R_object)cls, name___le__,
                 wrap_compare(richcompare, name___le__, Py_LE));
    type_setattr((R_object)cls, name___eq__,
                 wrap_compare(richcompare, name___eq__, Py_EQ));
    type_setattr((R_object)cls, name___ne__,
                 wrap_compare(richcompare, name___ne__, Py_NE));
    type_setattr((R_object)cls, name___gt__,
                 wrap_compare(richcompare, name___gt__, Py_GT));
    type_setattr((R_object)cls, name___ge__,
                 wrap_compare(richcompare, name___ge__, Py_GE));
}

DESC_CALL_SIGNATURE(desc_call_new_api) { 
    PyObject *c, *t, *d, *x;    
    R_new_api n = (R_new_api)callable;
    R_type cls;
    R_object result;
    assert(tuple->length > 0);
    cls = (R_type)ITEM(tuple, 0);
    TYPE_CHECK(cls, type_type);
    tuple = tuple_slice(tuple, 1, tuple->length);
    c = PIN(cls);
    t = PIN(tuple);
    d = PIN(dict);
    ACQUIRE_GIL;
    x = n->func((PyTypeObject*)c, t, d);
    gvmt_refer((GVMT_Object)tuple); /* Ensure that arguments aren't GCed during call */
    gvmt_refer((GVMT_Object)dict); 
    gvmt_refer((GVMT_Object)cls);
    if (x) {
        result = UNPIN(x);
        Py_DECREF(x);
        RELEASE_GIL;
        return result;
    } else {
        RELEASE_GIL;
        RAISE(THREAD_STATE->c_exception);
        return  0;   
    }
}

static R_object wrap_new(newfunc func) {
    R_new_api n = gc_allocate(new_api);   
    SET_TYPE(n, type_new_api);
    n->func = func;
    n->name = name___new__;
    return (R_object)n;
}

R_type HotPyAPI_Type_Create(PyTypeDef* def) {
    int i, len;
    R_dict dict;
    R_str doc;
    R_type new_class, super_type, meta_class;
    PyMethodDef* method;
    PyGetSetDef* property;
    if (def->tp_base) {
        super_type = UNPIN_TYPE(def->tp_base);
    } else {
        super_type = type_object; 
    }
    new_class = gc_allocate(type);
    meta_class = UNPIN_TYPE(def->meta_class);
    if (meta_class == NULL)
        meta_class = type_type;
    SET_TYPE(new_class, meta_class);
    TYPE_ASSERT(TYPE(new_class), type);
    new_class->lock = GVMT_LOCK_INITIALIZER; 
    new_class->__name__ = string_from_c_string(def->tp_name);
    new_class->tp_name = (char*)def->tp_name;
    new_class->__bases__ = make_tuple1((R_object)super_type);
    new_class->__mro__ = tuple_prepend((R_object)new_class, super_type->__mro__);
    //new_class->__setattr__ =  super_type->__setattr__;
    //new_class->__getattribute__ = super_type->__getattribute__;
    new_class->call = NULL;
    new_class->get = super_type->get;
    new_class->set = super_type->set;
    lay_out(new_class);
    new_class->attributes = empty_dict_table;
    new_class->unique_index = get_next_class_id();
    make_slots(new_class, def->tp_members);
    if (def->tp_getattro == NULL || def->tp_getattro == PyObject_GenericGetAttr) {
        new_class->__getattribute__ = NULL;
    } else {
        new_class->__getattribute__ = wrapped_getattro;
        new_class->api_getattro = def->tp_getattro;
    }
    if (def->tp_setattro == NULL || def->tp_setattro == PyObject_GenericSetAttr) {
        new_class->__setattr__ = NULL;
    } else {
        new_class->__setattr__ = wrapped_setattro;
        new_class->api_setattro = def->tp_setattro;
    }
    if (def->tp_call != NULL) {
        new_class->call = wrapped_call;
        new_class->api_call = def->tp_call;
    }
    if (def->tp_doc) {
        type_setattr((R_object)new_class, name___doc__, 
                     (R_object)string_from_c_string(def->tp_doc));
    }
    new_class->guards = new_dict(0);
    new_class->keys_cache = empty_dict_keys;
    // Do in reverse order so operators in number overwrite others.
    mapping_protocol(new_class, def->tp_as_mapping);
    sequence_protocol(new_class, def->tp_as_sequence);
    number_protocol(new_class, def->tp_as_number);
    hash_protocol(new_class, def->tp_hash);
    buffer_protocol(new_class, def->tp_as_buffer);
    compare_protocol(new_class, def->tp_richcompare);
    if (def->tp_iter)
        type_setattr((R_object)new_class, name___iter__, 
                     wrap_unary_func(def->tp_iter, name___iter__));
    if (def->tp_iternext)
        type_setattr((R_object)new_class, name___next__, 
                     wrap_unary_func(def->tp_iternext, name___next__));
    method = def->tp_methods;
    while(method) {
        add_method(new_class, method);
        method++;   
    }
    property = def->tp_getset;
    while(property) {
        R_str name = string_from_c_string(property->name);
        type_setattr((R_object)new_class, name, make_prop(property, name));
        property++;   
    }
    if (def->tp_init)
        type_setattr((R_object)new_class, name___init__,
                     wrap_setitem(def->tp_init, name___init__));
    if (def->tp_new)
        type_setattr((R_object)new_class, name___new__,
                     wrap_new(def->tp_new));    
    // Extension classes do not allow monkey-patching
    new_class->flags |= IMMUTABLE_FLAG;
    return new_class;
}

R_array pinned_object_array;

void resize_pinned_object_array(R_array a) {
    R_array new_array;
    assert(pinned_object_array == a);
    new_array = expand_array(a);
    gvmt_pin((GVMT_Object)new_array);
    pinned_object_array = new_array;
}

PyObject* PyExc_TypeError;
PyObject* Py_None;
PyObject* Py_NotImplemented;

R_object api_init(void) {
    // turn off for now...
    return NULL;
    //pinned_object_array = create_array(1000);
    //PyExc_TypeError = PIN(type_TypeError); 
    //PyExc_TypeError->ref_count = IMMORTAL;
    //Py_None = PIN(None);
    //Py_None->ref_count = IMMORTAL;
    //Py_NotImplemented = PIN(NotImplemented);
    //Py_NotImplemented->ref_count = IMMORTAL;
    //return NULL;
}

void HotPyAPI_Object_GenericGetAttr(PyObject** result, R_object obj, R_object name) {
    R_object a;
    GVMT_Object ex; 
    GVMT_TRY(ex)
        if (isinstance(name, type_str)) {
            RELEASE_GIL;
            a = _HotPyObject_GenericGetAttr(obj, (R_str)name);
            ACQUIRE_GIL;
            *result = PIN(a);
            Py_INCREF(*result);
        } else {
            R_str msg = py_sprintf("'%s' object has no attribute %s", 
                        TYPE_NAME(obj), name);
            THREAD_STATE->c_exception = make_exception(type_AttributeError, msg);
        }
    GVMT_CATCH
        THREAD_STATE->c_exception = (R_BaseException)ex;
        *result = NULL;
    GVMT_END_TRY
}

void HotPyAPI_Object_GenericSetAttr(int* err, R_object obj, R_object name, R_object value) {
    GVMT_Object ex; 
    GVMT_TRY(ex)
        if (isinstance(name, type_str)) {
            RELEASE_GIL;
            _HotPyObject_GenericSetAttr(obj, (R_str)name, value);
            *err = 0;
            ACQUIRE_GIL;
        } else {
            R_str msg = py_sprintf("'%s' object has no attribute %s", 
                        TYPE_NAME(obj), name);
            THREAD_STATE->c_exception = make_exception(type_AttributeError, msg);
            *err = -1;
        }
    GVMT_CATCH
        THREAD_STATE->c_exception = (R_BaseException)ex;
        *err = -1;
    GVMT_END_TRY
}

