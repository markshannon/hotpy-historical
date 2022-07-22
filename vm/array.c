#include "hotpy.h"
#include "operators.h"
#include "optimise.h"

/** Array and other internal sequence stuff 
 * Mainly used by list and tuple
 */

R_array create_array(int size) {
    R_array result = gc_allocate_array(size);
    SET_TYPE(result, type_array);
    result->a_size = 0;
    result->length = size;
    return result;
}

R_array reverse_array(R_array a) {
    int i, size = LEN(a);
    R_array result = gc_allocate_array(size);
    SET_TYPE(result, type_array);
    LEN(result) = size;
    for (i = 0; i < size; i++) {
        R_object tmp = ITEM(a, i);
        ITEM(result, size-1-i) = tmp;
    }
    result->a_size = a->a_size;
    return result;
}

/* Not thread safe, requires external locking */
R_array expand_array(R_array old) {
    int i, new_size;
    R_array new_array;
    if (old->length) 
        new_size = old->length * 2;
    else
        new_size = 8;
    new_array = gc_allocate_array(new_size);
    SET_TYPE(new_array, type_array);
    new_array->length = new_size;
    for (i = 0; i < old->length; i++) {
        R_object o = ITEM(old, i);
        ITEM(new_array, i) = o;
    }
    new_array->a_size = old->a_size;
    return new_array;
}

/* Not thread safe, requires external locking */
void array_copy(R_array src, int src_offset, R_array dst, 
                int dst_offset, int size) {
    int i;
    // Destination and source may be the same, so beware of overlaps.
    if (src_offset > dst_offset) {
        for (i = 0; i < size; i++) {
            R_object val = ITEM(src, src_offset + i);
            ITEM(dst, dst_offset + i) =  val;
        }
    } else {
        for (i = size-1; i >= 0; i--) {
            R_object val = ITEM(src, src_offset + i);
            ITEM(dst, dst_offset + i) =  val;          
        }
    }
}

R_object PY_FUNC2(tuple_add, R_tuple t1, R_tuple t2) {   
    R_tuple t;
    TYPE_ASSERT(t1, tuple);
    if (TYPE(t2) != type_tuple) {
        R_str msg = py_sprintf(
            "can only concatenate tuple (not \"%s\") to tuple", TYPE_NAME(t2));
        raise_str(type_TypeError, msg);
    }
    t = make_tuple(t1->length + t2->length);
    array_copy((R_array)t1, 0, (R_array)t, 0, t1->length);
    array_copy((R_array)t2, 0, (R_array)t, t1->length, t2->length);
    return (R_object)t;
}

/** Optimised for common case where array is homogenous.
 **/
R_bool in_array(R_array a, int limit, R_object o) {
    int i;
    R_base_function eqs; 
    binary_behaviour exe;
    R_type t;
    if (limit == 0)
        return FALSE;
    t = TYPE(ITEM(a, 0));
    eqs = binary_lookup_exact(operator_eq, TYPE(o), t);
    if (eqs && IS_C_FUNCTION(eqs)) {
        R_builtin_function b_eqs = (R_builtin_function)eqs;
        exe = (binary_behaviour)b_eqs->executable;
    } else {
        t = 0;
        exe = 0;
    }
    for (i = 0; i < limit; i++) {
        R_object key = ITEM(a, i);
        R_object is_equal;
        if (key == o)
            return TRUE;
        if (TYPE(key) == t) {
            is_equal = (R_object)exe(o, key);
        } else {
            is_equal = (R_object)binary_call(operator_eq, o, key);
        }
        if (is_equal == (R_object)TRUE || 
            ((is_equal != (R_object)FALSE) && bool_value(is_equal)))
            return TRUE;
    }
    return FALSE;
}

GVMT_Object wrong_unpack(int count, int available) {
    if (count > available) {
        raise_char(type_ValueError, "too few values to unpack");
    } else {                                                     
        raise_char(type_ValueError, "too many values to unpack");
    } 
    return 0;
}
 
R_object index_out_of_bounds(intptr_t val) {
    char buf[40];
    sprintf(buf, "Index out of bounds: %d", val);
    raise_char(type_IndexError, buf); 
    return 0;
}

intptr_t sequence_index(R_int value, int limit) {
    intptr_t index, val = int_from_py_int(value);
    assert (limit >= 0);
    if (val < 0)
        index = limit + val;
    else
        index = val;
    if (((unsigned)index) >= ((unsigned)limit)) {
        index_out_of_bounds(val);
    }
    return index;
}

