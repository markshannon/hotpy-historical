#include "hotpy.h"
#include "operators.h"
#include <assert.h>

extern GVMT_OBJECT(type) _tuple;

GVMT_OBJECT(tuple) empty_tuple_object = {
    &_tuple,
    IMMORTAL,
    0
};

static R_tuple new_tuple(R_type cls, int size) {
    R_tuple tuple;
    assert(is_subclass_of(cls, type_tuple));
    if (cls != type_tuple) {
        // Extra space for dict
        tuple = gc_allocate_tuple(size + 1);
    } else {
        tuple = gc_allocate_tuple(size);
    }
    SET_TYPE(tuple, cls);
    ZERO_REF_COUNT(tuple);
    tuple->length = size;
    if (cls != type_tuple) {
        assert(cls->dict_offset < 0);
        assert(cls->keys_cache);
        FIELD(R_dict_values, tuple, size*sizeof(void*)-cls->dict_offset) 
            = new_dict_values(cls->keys_cache);
    }
    return tuple;
}

R_tuple make_tuple(int size) {
    R_tuple tuple = gc_allocate_tuple(size);
    SET_TYPE(tuple, type_tuple);
    tuple->length = size;
    return tuple;
}

R_object PY_FUNC2(tuple_from_list, R_type cls, R_list l) {
    R_tuple tuple;
    TYPE_ASSERT(l, list);
    tuple = new_tuple(cls, LIST_SIZE(l));
    array_copy(l->array, 0, (R_array)tuple, 0, LIST_SIZE(l));
    return (R_object)tuple;
}

#define EQUALS(a,b) ((a) == (b) || ((R_bool)binary_call(operator_eq, (R_object)(a), (R_object)(b))) == TRUE)

int tuple_equals(R_tuple t1, R_tuple t2) {
        int i, len = t1->length;
        TYPE_ASSERT(t1, tuple);
        TYPE_ASSERT(t2, tuple);
        if (len != t2->length)
            return 0;
        for (i = 0; i < len; i++) {
            R_object l, r;
            l = ITEM(t1, i);
            r = ITEM(t2, i);
            if (!EQUALS(l, r)) {
                return 0;
            }
        }
        return 1;
}

// This is important for performance, so partially unroll.
void unpack_tuple(int count, R_tuple sequence, GVMT_Value* values) {
    int i;
    if (count != sequence->length)
        wrong_unpack(count, sequence->length);
    if (count) {
        R_object o = ITEM(sequence, 0);
        gvmt_stack_write_object(values, (GVMT_Object)o);
        if (count > 1) {
            o = ITEM(sequence, 1);
            gvmt_stack_write_object(values+1, (GVMT_Object)o);
            for (i = 2; i < count; i++) {
                R_object o = ITEM(sequence, i);
                gvmt_stack_write_object(values+i, (GVMT_Object)o);
            }
        }
    }
}

R_object PY_FUNC2(tuple_eq, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) {  
        return (R_object)booleans[tuple_equals(t1, (R_tuple)o)];
    } else {
        return (R_object)FALSE; 
    }
}

R_object PY_FUNC2(tuple_ne, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) {  
        return (R_object)booleans[1-tuple_equals(t1, (R_tuple)o)];
    } else {
        return (R_object)TRUE; 
    }
}

R_tuple tuple_concat(R_tuple t1, R_tuple t2) {
    int len1, len2, i;
    R_tuple t3;
    len1 = t1->length;
    len2 = t2->length;
    t3 = make_tuple(len1+len2);
    for (i = 0; i < len1; i++) {
        R_object o = ITEM(t1, i);
        ITEM(t3, i) = o;
    }
    for (i = 0; i < len2; i++) {
        R_object o = ITEM(t2, i);
        ITEM(t3, i+len1) = o;
    }
    return t3;
}

R_tuple tuple_prepend(R_object o1, R_tuple t2) {
    int len, i;
    R_tuple t3;
    len = t2->length;
    t3 = make_tuple(1+len);
    ITEM(t3, 0) = o1;
    for (i = 0; i < len; i++) {
        R_object o = ITEM(t2, i);
        ITEM(t3, i+1) = o;
    }
    return t3;
}

R_tuple tuple_slice(R_tuple t, int start, int end) {
    int i, size = end-start;
    R_tuple part = make_tuple(size);
    assert(start >= 0 && start <= end && end <= LEN(t));
    for (i = 0; i < size; i++) {
        R_object o = ITEM(t, i+start);
        ITEM(part, i) = o;
    }
    return part;
}

/*
static int tuple_compare(R_tuple t1, R_tuple t2) {
    int i, len;
    if (t1->length > t2->length) {
        len = t2->length;
    } else {
        len = t1->length;
    }
    for (i = 0; i < len; i++) {
        R_object l, r;
        R_int c;
        l = ITEM(t1, i);
        r = ITEM(t2, i);
        c = binary_call(operator_compare, l, r);
        TYPE_ASSERT(c, int);
        if (c) {
            return c;
        }
    }
    if (t1->length == t2->length)
        return 0;
    else if (t1->length > t2->length)
        return 1;
    else
        return -1;
}

R_object PY_FUNC2(tuple_compare, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) { 
        return (R_object)R_int_from_ssize_t(tuple_compare(t1, (R_tuple)o));
    } else {
        R_str msg = py_sprintf("unorderable types: 'tuple' and '%s'",
                               "TYPE_NAME(" +_1 + ")");
        raise_str(type_TypeError, msg);
        return 0;
    }
}

R_object PY_FUNC2(tuple_lt, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) { 
        return (R_object)booleans[tuple_compare(t1, (R_tuple)o) < 0];
    } else {
        R_str msg = py_sprintf("unorderable types: 'tuple' and '%s'",
                               "TYPE_NAME(" +_1 + ")");
        raise_str(type_TypeError, msg);
        return 0;
    }
}

R_object PY_FUNC2(tuple_gt, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) { 
        return (R_object)booleans[tuple_compare(t1, (R_tuple)o) > 0];
    } else {
        R_str msg = py_sprintf("unorderable types: 'tuple' and '%s'",
                               "TYPE_NAME(" +_1 + ")");
        raise_str(type_TypeError, msg);
        return 0;
    }
}

R_object PY_FUNC2(tuple_le, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) { 
        return (R_object)booleans[tuple_compare(t1, (R_tuple)o) <= 0];
    } else {
        R_str msg = py_sprintf("unorderable types: 'tuple' and '%s'",
                               "TYPE_NAME(" +_1 + ")");
        raise_str(type_TypeError, msg);
        return 0;
    }
}

R_object PY_FUNC2(tuple_ge, R_tuple t1, R_object o) {
    if (is_subclass_of(TYPE(o), type_tuple)) { 
        return (R_object)booleans[tuple_compare(t1, (R_tuple)o) >= 0];
    } else {
        R_str msg = py_sprintf("unorderable types: 'tuple' and '%s'",
                               "TYPE_NAME(" +_1 + ")");
        raise_str(type_TypeError, msg);
        return 0;
    }
}

*/

R_object PY_FUNC2(tuple_getitem, R_tuple tuple, R_object i) {
    TYPE_ASSERT(tuple, tuple);
    if (IS_INT(i))
        return ITEM(tuple, sequence_index((R_int)i, tuple->length));
    else
        assert(0 && "To do");
    return (R_object)NotImplemented;
}

R_object PY_FUNC2(tuple_getitem_int, R_tuple tuple, R_int i) {
    TYPE_ASSERT(tuple, tuple);
    TYPE_ASSERT(i, int);
    return ITEM(tuple, sequence_index(i, tuple->length));
}

R_object PY_FUNC2(tuple_contains, R_tuple t, R_object item) {
    TYPE_ASSERT(t, tuple);
    return (R_object)in_array((R_array)t, t->length, item);  
}

intptr_t hotpy_tuple_hash(R_tuple t) {
    int i, n, y, x = 0x345678; 
    int mult = 1000003;
    int diff = t->length * 2 + 82520;
    for (i = 0, n = t->length; i < n; i++) {
        R_object o = ITEM(t, i);
        y = HotPyObject_Hash(o);
        x = (x ^ y) * mult;
        mult += diff; 
    }
    return x;
}

R_object PY_FUNC1(tuple_hash, R_tuple t) {
    return (R_object)R_int_from_ssize_t(hotpy_tuple_hash(t));
}

R_object PY_FUNC1(__len__tuple, R_tuple t) { 
    return (R_object)R_int_from_ssize_t(t->length);
}

R_object PY_FUNC1(seq_bool, R_tuple seq) {
    // This should be safe for any object, 
    // but it meaningless for non-builtin-sequences.
    // seq may not be a tuple, but if its a builtin sequence,
    // then its length is at the same offset (otherwise GC wouldn't work).
    return (R_object) booleans[seq->length != 0];
}

R_object PY_FUNC1(tuple_iter, R_tuple t) {
    R_tupleiterator it = gc_allocate(tupleiterator);
    TYPE_ASSERT(t, tuple);
    SET_TYPE(it, type_tupleiterator);
    it->next = 0;
    it->tuple = t;
    return (R_object)it;
}

R_object PY_FUNC1(tupleiterator_next, R_tupleiterator it) {
    intptr_t next = it->next;
    TYPE_ASSERT(it, tupleiterator);
    if (next >= it->tuple->length) {
        stop_iteration();
        return 0;
    } else {
        it->next = next+1;
        return ITEM(it->tuple, next);
    }
}    

R_tuple pack_tuple_reverse(int count, GVMT_Value* values) { 
    int i;
    R_tuple result = gc_allocate_tuple(count);
    SET_TYPE(result, type_tuple);
    ZERO_REF_COUNT(result);
    result->length = count;
    for (i = 0; i < count; i++) {
        R_object o = (R_object)gvmt_stack_read_object(values+count-1-i);
        ITEM(result, i) = o;
    }
    gvmt_fully_initialized((GVMT_Object)result);
    return result;
}

R_tuple HotPySequence_Tuple(R_object sequence) {
    R_tuple t;
    R_type scls = TYPE(sequence);
    if (scls == type_tuple) {
        t = (R_tuple)sequence;
    } else {
        if (scls != type_list) {
            sequence = (R_object)HotPySequence_List(sequence);  
        }
        t = (R_tuple)PY_tuple_from_list((R_list)sequence, type_tuple);
    }
    return t; 
}
