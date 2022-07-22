#include "hotpy.h"
#include "operators.h"
#include <malloc.h>
#include <stdlib.h>

/* /home/mark/Python-3.1.1/Doc/c-api/object.rst */
//void HotPyObject_Print(R_object o, FILE* fp, int flags) {
//    R_str s, encoding;
//    R_bytes b;
//    if (flags & Py_PRINT_RAW)
//        s = (R_str)HotPyObject_Str(o);
//    else
//        s = (R_str)HotPyObject_Repr(o);
//    b = HotPyUnicode_Encode(s, PyUnicode_GetDefaultEncoding(), NULL);
//    HotPyBytes_WriteToFile(b, fp);
//}

R_object _HotPyObject_GetAttr(R_object object, R_str name) {
    R_type klass = TYPE(object);
    int i = 1;
    getattribute_function getattr = klass->__getattribute__;
    for (i = 1; getattr == 0; i++) {
        getattr = ((R_type)ITEM(klass->__mro__, i))->__getattribute__;
    }
    return getattr(object, name);
}

int HotPyObject_HasAttr(R_object o, R_object attr_name) {
    int result = 0;
    R_BaseException ex = 0;
    R_frame f = THREAD_STATE->current_frame;
    GVMT_TRY(ex)
        if (!isinstance(attr_name, type_str))
            return 0;
        _HotPyObject_GetAttr(o, (R_str)attr_name);
        result = 1;
    GVMT_CATCH
        THREAD_STATE->current_frame = f;
    GVMT_END_TRY
    return result;
}

int HotPyObject_HasAttrString(R_object o, const char* attr_name) {
    int result = 0;
    R_BaseException ex = 0;
    R_frame f = THREAD_STATE->current_frame;
    GVMT_TRY(ex)
        _HotPyObject_GetAttr(o, string_from_c_string(attr_name));
        result = 1;
    GVMT_CATCH
        THREAD_STATE->current_frame = f;
    GVMT_END_TRY
    return result;
}

R_object HotPyObject_GetAttr(R_object o, R_object name) {
    if (isinstance(name, type_str)) {
        return _HotPyObject_GetAttr(o, (R_str)name);
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                    TYPE_NAME(name));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

R_object HotPyObject_GetAttrString(R_object o, const char* attr_name) {
    return _HotPyObject_GetAttr(o, string_from_c_string(attr_name));
}

R_object HotPyObject_GenericGetAttr(R_object o, R_object name) {
    if (isinstance(name, type_str)) {
        return _HotPyObject_GenericGetAttr(o, (R_str)name);
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                    TYPE_NAME(name));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

void HotPyObject_GenericSetAttr(R_object o, R_object name, R_object value) {
    if (isinstance(name, type_str)) {
        _HotPyObject_GenericSetAttr(o, (R_str)name, value);
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                    TYPE_NAME(name));
        raise_str(type_TypeError, msg);
    }
}

void _HotPyObject_SetAttr(R_object obj, R_str name, R_object v) {
    R_type klass = TYPE(obj);
    int i;
    setattr_function setattr = klass->__setattr__;
    for (i = 1; setattr == 0; i++) {
        setattr = ((R_type)ITEM(klass->__mro__, i))->__setattr__;
    }
    setattr(obj, name, v);
}

void HotPyObject_SetAttr(R_object o, R_object name, R_object value) {
    if (isinstance(name, type_str)) {
        _HotPyObject_SetAttr(o, (R_str)name, value);
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                    TYPE_NAME(name));
        raise_str(type_TypeError, msg);
    }
}

void HotPyObject_SetAttrString(R_object o, const char* attr_name, R_object v) {
    _HotPyObject_SetAttr(o, string_from_c_string(attr_name), v);
}

void HotPyObject_DelAttr(R_object o, R_object name) {
    R_type klass = TYPE(o);
    R_object d = class_attribute(klass, name___delattr__);
    call_func call;
    R_tuple t;
    assert(d != NULL && "object must have a __delattr__ method");
    call = CALLABLE(d);
    t = make_tuple2(o, name);
    call(d, t, 0);
}

void HotPyObject_DelAttrString(R_object o, const char* attr_name) {
    HotPyObject_DelAttr(o, (R_object)string_from_c_string(attr_name));
}

static R_operator _comparisons[6];

static R_object init_comparisons(void) {
    _comparisons[Py_LT] = operator_lt; assert(Py_LT > 0); assert(Py_LT < 6);
    _comparisons[Py_LE] = operator_le; assert(Py_LE > 0); assert(Py_LE < 6);
    _comparisons[Py_EQ] = operator_eq; assert(Py_EQ > 0); assert(Py_EQ < 6);
    _comparisons[Py_NE] = operator_ne; assert(Py_NE > 0); assert(Py_NE < 6);
    _comparisons[Py_GT] = operator_gt; assert(Py_GT > 0); assert(Py_GT < 6);
    _comparisons[Py_GE] = operator_ge; assert(Py_GE > 0); assert(Py_GE < 6);
    return NULL;
}

R_object HotPyObject_RichCompare(R_object o1, R_object o2, int opid) {
    R_object res;
    R_operator op;
    assert(Py_LT <= opid && opid <= Py_GE);
    if (_comparisons[0] == NULL)
        init_comparisons();
    op = _comparisons[opid];
    return binary_call(op, o1, o2);
}

int HotPyObject_RichCompareBool(R_object o1, R_object o2, int opid) {
    R_object res;
    if (o1 == o2) {
        if (opid == Py_EQ)
			return 1;
		else if (opid == Py_NE)
			return 0;
    }
    res = HotPyObject_RichCompare(o1, o2, opid);
    return HotPyObject_IsTrue(res);
}

/** repr(obj) */
R_str HotPyObject_Repr(R_object obj) {
    R_object repr;
    call_func f;
    R_type klass = TYPE(obj);
    R_str s;
    R_tuple t;
    if (klass == type_str) {
        return (R_str)obj;
    }
    repr = class_attribute(klass, name___repr__);
    assert(repr && "__repr__ must be defined for object");
    f = CALLABLE(repr);
    t = make_tuple(1);
    ITEM(t, 0) = obj;
    s = (R_str)f(repr, t, 0);
    if (IS_STR(s)) {
         return s;
    } else {
        R_str name = TYPE_NAME(obj);
        R_str msg = py_sprintf("__repr__ returned non-string (type %s)", name);
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

/** str(obj) */
R_str HotPyObject_Str(R_object obj) {
    R_object d;
    call_func f;
    R_type klass = TYPE(obj);
    R_str s;
    R_tuple t;
    if (klass == type_str) {
        return (R_str)obj;
    }
    d = class_attribute(klass, name___str__);
    if (d == NULL)
        return HotPyObject_Repr(obj);
    f = CALLABLE(d);
    t = make_tuple(1);
    ITEM(t, 0) = obj;
    s = (R_str)f(d, t, 0);
    if (IS_STR(s)) {
         return s;
    } else {
        R_str name = TYPE_NAME(obj);
        R_str msg = py_sprintf("__str__ returned non-string (type %s)", name);
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

int HotPyObject_IsInstance(R_object inst, R_object cls) {
    // Need to call Python function.
    assert("To do" && 0);
    return 0;
}

int HotPyObject_IsSubclass(R_object derived, R_object cls) {
    // Need to call Python function.
    assert("To do" && 0);
    return 0;
}

int HotPyCallable_Check(R_object o) {
    return CALLABLE(o) != not_callable;
}

R_object HotPyObject_Call(R_object callable, R_object args, R_object kw) {
    assert(TYPE(args) == type_tuple);
    assert(TYPE(kw) == type_dict);
    return CALLABLE(callable)(callable, (R_tuple)args, (R_dict)kw);
}

#define _tuple_args(a) (IS_TUPLE(a) ? ((R_tuple)a) : make_tuple1(a))

R_object HotPyObject_CallObject(R_object callable, R_object args) {
    R_tuple t = _tuple_args(args);
    return HotPyObject_Call(callable, (R_object)t, (R_object)new_empty_dict());
}

R_object HotPyObject_CallMethod(R_object o, char* method, char* format, va_list  va) {
    R_tuple args;
    R_object callable = HotPyObject_GetAttrString(0, method);
    if (CALLABLE(callable) != not_callable) {
        R_str msg = py_sprintf("attribute of type '%s' is not callable", TYPE_NAME(callable));
        raise_str(type_TypeError, msg);
    }
    if (format == NULL || format[0] == 0) {
        args = empty_tuple;
    } else {
        args = _tuple_args(HotPy_BuildValue(format, va));
    }
    return HotPyObject_Call(callable, (R_object)args, (R_object)new_empty_dict());
}

/** hash(obj) */
intptr_t HotPyObject_Hash(R_object obj) {
    R_type klass;
    hash_func func;
    R_tuple mro;
    int i;
    intptr_t hash;
    if (gvmt_is_tagged(obj)) {
        return ((intptr_t)gvmt_untag(obj)) >> 1;
    }
    klass = obj->ob_type;
    func = klass->hash;
    if (func == NULL) {
        // This relies on setattr in type to ensure that if '__hash__' is set,
        // then hash is set to call_py_hash.
        assert(str_dict_table_get(klass->attributes, name___hash__) == NULL);
        mro = klass->__mro__;
        hash = 0;
        for (i = 1; ; i++) {
            assert(i < LEN(mro));
            klass = (R_type)ITEM(mro, i);
            func = klass->hash;
            if (func) {
                break;
            }
            assert(str_dict_table_get(klass->attributes, name___hash__) == NULL);
        }
    }
    hash = func(obj);
    if (hash == -1)
        hash = -2;
    return hash;
}

intptr_t HotPyObject_HashNotImplemented(R_object o) {
    R_str msg = py_sprintf("unhashable type: '%s'", TYPE_NAME(o));
    raise_str(type_TypeError, msg);
    return -1;
}

int HotPyObject_IsTrue(R_object o) {
    if (gvmt_is_tagged(o)) {
        return ((intptr_t)gvmt_untag(o)) != 1;
    } else if (o->ob_type == type_bool) {
        return o == (R_object)TRUE;
    } else if (o->ob_type == type_NoneType) {
        return 0;
    } else {
        return truth(o) == TRUE;
    }
}

int HotPyObject_Not(R_object o) {
    return HotPyObject_IsTrue(o) != 0;
}

R_int HotPyObject_Size(R_object o) {
    // Call __len__ method.
    R_type klass = TYPE(o);
    R_object len = class_attribute(klass, name___len__);
    R_str msg;
    if (len) {
        call_func call = CALLABLE(len);
        R_tuple t = make_tuple1(o);
        R_object o = call(len, t, 0);
        if (TYPE(o) == type_int)
            return (R_int)o;
        else
            return HotPyNumber_Index(o);
    }
    msg = py_sprintf("object of type '%s' has no len()", klass->__name__);
    raise_str(type_TypeError, msg);
    return NULL;
}

R_object HotPyObject_GetItem(R_object seq, R_object index) {
    R_object o;
    R_tuple t;
    R_type tseq = TYPE(seq);
    o = class_attribute(tseq, name___getitem__);
    if (o) {
        if (IS_C_FUNCTION(o)) {
            R_builtin_function f = (R_builtin_function)o;
            GVMT_PUSH(seq);
            GVMT_PUSH(index);
            assert(f->paramNames->length == 2);
            return f->executable();
        } else {
            call_func f = CALLABLE(o);
            t = make_tuple2(seq, index);
            return f(o, t, 0);
        }
    } else {
        R_str msg = py_sprintf("'%s' object is not subscriptable",
                               TYPE_NAME(seq));
        raise_str(type_TypeError, msg);
        return 0;
    }
}

void HotPyObject_SetItem(R_object seq, R_object index, R_object value) {
    R_object o;
    R_tuple t;
    R_type tseq = TYPE(seq);
    o = class_attribute(tseq, name___setitem__);
    if (o) {
        if (IS_C_FUNCTION(o)) {
            GVMT_PUSH(seq);
            GVMT_PUSH(index);
            GVMT_PUSH(value);
            ((R_builtin_function)o)->executable();
        } else {
            t = make_tuple(3);
            ITEM(t, 0) = seq;
            ITEM(t, 1) = index;
            ITEM(t, 2) = value;
            CALLABLE(o)(o, t, 0);
        }
    } else {
        R_str msg = py_sprintf("'%s' object is not subscriptable",
                               TYPE_NAME(seq));
        raise_str(type_TypeError, msg);
    }
}

void HotPyObject_DelItem(R_object seq, R_object index) {
    R_object o;
    R_tuple t;
    R_type tseq = TYPE(seq);
    o = class_attribute(tseq, name___delitem__);
    if (o) {
        if (IS_C_FUNCTION(o)) {
            R_builtin_function f = (R_builtin_function)o;
            GVMT_PUSH(seq);
            GVMT_PUSH(index);
            f->executable();
        } else {
            call_func f = CALLABLE(o);
            t = make_tuple2(seq, index);
            f(o, t, 0);
        }
    } else {
        R_str msg = py_sprintf("'%s' object is not subscriptable",
                               TYPE_NAME(seq));
        raise_str(type_TypeError, msg);
    }
}

R_object HotPySeqIter_New(R_object o) {
    // This is awkward as we need to create a Python-level generator
    fprintf(stderr, "To do");
    abort();
    return NULL;
}

int HotPySequence_Check(R_object seq) {
    R_object o;
    R_tuple t;
    R_type tseq = TYPE(seq);
    if (is_subclass_of(tseq, type_dict))
        return 0;
    o = class_attribute(tseq, name___getitem__);
    return o != NULL;
}

/* Unlike the CPython version, this does not check if
 * returned value supports iterator protocol.
 * This check is not necessary as calls to next(it) must do
 * the check anyway. */
R_object HotPyObject_GetIter(R_object seq) {
    R_object o;
    R_tuple t;
    R_type tseq = TYPE(seq);
    o = class_attribute(tseq, name___iter__);
    if (o) {
        R_object iter;
        if (IS_C_FUNCTION(o)) {
            GVMT_PUSH(seq);
            iter = ((R_builtin_function)o)->executable();
        } else {
            t = make_tuple1(seq);
            iter = CALLABLE(o)(o, t, 0);
        }
        return iter;
    } else {
        if (HotPySequence_Check(o)) {
            return HotPySeqIter_New(o);
        } else {
            R_str msg = py_sprintf("%s' object is not iterable",
                                   TYPE_NAME(seq));
            raise_str(type_TypeError, msg);
            return NULL;
        }
    }
}

int HotPyNumber_Check(R_object o) {
    R_tuple mro;
    R_type k;
    int i, limit;
    if (gvmt_is_tagged(o))
        return 1;
    k = o->ob_type;
    mro = k->__mro__;
    limit = mro->length -1; // Don't bother checking object; its not a number.
    for (i = 0; i < limit; i++) {
        R_type cls = (R_type)ITEM(mro, i);
        if (str_dict_table_get(cls->attributes, name___int__) != NULL) {
            return 1;
        }
        if (str_dict_table_get(cls->attributes, name___float__) != NULL) {
            return 1;
        }
    }
    return 0;
}

R_object HotPyNumber_Power(R_object v, R_object w, R_object z) {
    R_object res = binary_call(operator_pow, v, w);
    if (z != (R_object)None) {
        res = binary_call(operator_mod, res, z);
    }
    return res;
}

R_object HotPySequence_Concat(R_object s, R_object o) {
    R_str msg;
    if (HotPySequence_Check(s) && HotPySequence_Check(o)) {
        return binary_call(operator_add, s, o);
    } else {
        msg = py_sprintf("'%s' object can't be concatenated", TYPE_NAME(s));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

R_object HotPySequence_InPlaceConcat(R_object s, R_object o) {
    R_str msg;
    if (HotPySequence_Check(s) && HotPySequence_Check(o)) {
        return inplace_call(operator_iadd, s, o);
    } else {
        msg = py_sprintf("'%s' object can't be concatenated", TYPE_NAME(s));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

R_object HotPySequence_Repeat(R_object s, Py_ssize_t count) {
    if (HotPySequence_Check(s)) {
        R_object o = (R_object)HotPyLong_FromSsize_t(count);
        return binary_call(operator_mul, s, o);
    } else {
        R_str msg = py_sprintf("'%s' object can't be repeated", TYPE_NAME(s));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

R_object HotPySequence_InPlaceRepeat(R_object s, Py_ssize_t count) {
    if (HotPySequence_Check(s)) {
        R_object o = (R_object)HotPyLong_FromSsize_t(count);
        return inplace_call(operator_imul, s, o);
    } else {
        R_str msg = py_sprintf("'%s' object can't be repeated", TYPE_NAME(s));
        raise_str(type_TypeError, msg);
        return NULL;
    }
}

R_object HotPySequence_Fast(R_object s, const char* msg) {
    R_type scls = TYPE(s);
    R_frame f = THREAD_STATE->current_frame;
    if (scls != type_tuple && scls != type_list) {
        R_BaseException ex = NULL;
        GVMT_TRY(ex)
            s = (R_object)HotPySequence_List(s);
        GVMT_END_TRY
        if (ex)
            raise_char(type_TypeError, msg);
    }
    return s;
}

