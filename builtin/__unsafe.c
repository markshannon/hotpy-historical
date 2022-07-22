#include "hotpy.h" 

R_object PY_FUNC0(array_type) {
    return (R_object)type_array;
}
 
R_object PY_FUNC1(list_array, R_list l) {
    TYPE_ASSERT(l, list);
    return (R_object)l->array;   
}

R_object PY_FUNC2(array_getitem_int, R_array a, R_int i) {
    intptr_t index = ((intptr_t)gvmt_untag(i)) >> 1;
    assert(TYPE(a) == type_array || TYPE(a) == type_tuple);
    assert(index >= 0 && index < a->length);
    assert(ITEM(a, index));
    return ITEM(a, index);
}

R_object PY_FUNC3(array_setitem_int, R_array a, R_int i, R_object o) {
    intptr_t index = ((intptr_t)gvmt_untag(i)) >> 1;
    TYPE_ASSERT(a, array);
    assert(index >= 0 && index < a->length);
    ITEM(a, index) = o;
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC3(array_sub_array, R_array a, R_int start, R_int stop) {
    intptr_t from = ((intptr_t)gvmt_untag(start)) >> 1;
    intptr_t to = ((intptr_t)gvmt_untag(stop)) >> 1;
    intptr_t index, len = to-from;
    R_array result;
    assert(TYPE(a) == type_array || TYPE(a) == type_tuple);
    TYPE_ASSERT(a, array);
    TYPE_ASSERT(start, int);
    TYPE_ASSERT(stop, int);
    assert(len > 0 && len < a->length);
    assert(from >= 0 && to < a->length);
    result = gc_allocate_array(len);
    SET_TYPE(result, type_array);
    result->a_size = len;
    result->length = len;
    for (index = 0; index < len; index++) {
        R_object tmp = ITEM(a, from+index);
        ITEM(result, index) = tmp;
    }
    gvmt_fully_initialized((GVMT_Object)result);
    return (R_object)result;
}

R_object PY_FUNC1(make_array, R_int size) {
    intptr_t len = ((intptr_t)gvmt_untag(size)) >> 1;
    R_array result;
    TYPE_ASSERT(size, int);
    assert(len > 0);
    result = gc_allocate_array(len);
    SET_TYPE(result, type_array);
    result->a_size = len;
    result->length = len;
    return (R_object)result;
}

R_object PY_FUNC5(array_copy, R_array src, R_int s_offset, 
                  R_array dst, R_int d_offset, R_int size) {
    intptr_t s_off = ((intptr_t)gvmt_untag(s_offset)) >> 1;
    intptr_t d_off = ((intptr_t)gvmt_untag(d_offset)) >> 1;
    intptr_t sz = ((intptr_t)gvmt_untag(size)) >> 1;
    intptr_t limit = s_off + sz;
    assert(TYPE(src) == type_array || TYPE(src) == type_tuple);
    assert(TYPE(dst) == type_array || TYPE(dst) == type_tuple);
    TYPE_ASSERT(s_offset, int);
    TYPE_ASSERT(d_offset, int);
    TYPE_ASSERT(size, int);
    assert(s_off >= 0 && s_off + sz <= src->length);
    assert(d_off >= 0 && d_off + sz <= dst->length);
    for (;s_off < limit; s_off++) {
        R_object tmp = ITEM(src, s_off);
        ITEM(dst, d_off) = tmp;
        ++d_off;
    }
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC3(array_reverse, R_array a, R_int start, R_int stop) {
    intptr_t lo = ((intptr_t)gvmt_untag(start)) >> 1;
    intptr_t hi = ((intptr_t)gvmt_untag(stop)) >> 1;
    assert(lo >= 0 && lo <= a->length);
    assert(hi >= 0 && hi <= a->length);
    assert(lo <= hi);
    --hi;
    while (lo < hi) {
        R_object t_lo = ITEM(a, lo);
        R_object t_hi = ITEM(a, hi);
        ITEM(a, lo) = t_hi;
        ITEM(a, hi) = t_lo;
        ++lo;
        --hi;
    }
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC1(lock_list, R_list l) {
    if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(list), lock));
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC1(unlock_list, R_list l) {
    if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(list), lock));
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC1(lock_dict, R_dict d) {
    if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(d), offsetof(GVMT_OBJECT(dict), lock));
    return (R_object)PY_SMALL_INT(0);
}

R_object PY_FUNC1(unlock_dict, R_dict d) {
    if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(d), offsetof(GVMT_OBJECT(dict), lock));
    return (R_object)PY_SMALL_INT(0);
}



