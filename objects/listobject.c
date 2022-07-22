#include "hotpy.h"
#include "operators.h"

#ifdef NO_LOCKING

#define LOCK_LIST(l) ((void)0)
#define UNLOCK_LIST(l) ((void)0)

#else

#define LOCK_LIST(l) if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(list), lock))
#define UNLOCK_LIST(l) if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(list), lock))

#endif

#ifdef READ_LOCKING

#define READ_LIST_LOCK(l) LOCK_LIST(l)
#define READ_LIST_UNLOCK(l) UNLOCK_LIST(l)

#else

#define READ_LIST_LOCK(l) ((void)0)
#define READ_LIST_UNLOCK(l) ((void)0)

#endif

R_list make_list(int capacity) {
    R_list result = gc_allocate(list);
    int size = 4;
    SET_TYPE(result, type_list);
    result->lock = GVMT_LOCK_INITIALIZER;
    while (size < capacity)
        size <<= 1;
    result->array = create_array(size);
    return result;
}

static R_list new_list(R_type cls, int capacity) {
    R_list result;
    int size = 4;
    if (cls == type_list)
        return make_list(capacity);
    // Extra space for dict
    result = (R_list)gvmt_malloc(sizeof(GVMT_OBJECT(list))+sizeof(void*));
    assert(is_subclass_of(cls, type_list));
    SET_TYPE(result, cls);
    result->lock = GVMT_LOCK_INITIALIZER;
    while (size < capacity)
        size <<= 1;
    result->array = create_array(size);
    return result;
}

void list_append(R_list list, R_object item) {
    intptr_t index;
    R_array a;
    LOCK_LIST(list);
    index = LIST_SIZE(list);
    a = list->array;
    if (index == a->length) {
        a = expand_array(a);
        list->array = a;
    }
    ITEM(a, index) = item;
    a->a_size = index + 1;
    UNLOCK_LIST(list);
}

void list_append_nolock(R_list list, R_object item) {
    intptr_t index;
    R_array a;
    index = LIST_SIZE(list);
    a = list->array;
    if (index == a->length) {
        a = expand_array(a);
        list->array = a;
    }
    ITEM(a, index) = item;
    a->a_size = index + 1;
}

R_object list_pop(R_list list) {
    intptr_t index;
    R_object  result;
    LOCK_LIST(list);
    index = LIST_SIZE(list) - 1;
    result = ITEM(list->array, index);
    ITEM(list->array, index) = (R_object)None;
    SET_LIST_SIZE(list, index);
    UNLOCK_LIST(list);
    return result;
}

void unpack_list(int count, R_list sequence, GVMT_Value* values) {
    int i;
    R_array a = sequence->array;
    if (count != LIST_SIZE(sequence))
        wrong_unpack(count, LIST_SIZE(sequence));
    for (i = 0; i < count; i++) {
        GVMT_Object o = (GVMT_Object)ITEM(a, i);
        if (o)
            gvmt_stack_write_object(values+i, o);
        else
            // Maybe should raise some sort of concurrent mod exception?
            wrong_unpack(count, i);
    }
}

R_object PY_FUNC1(new_list, R_type cls) {
    return (R_object)new_list(cls, 8);
}

R_object PY_FUNC2(list_append, R_list list, R_object item) {
    list_append(list, item);
    return (R_object)None;
}

R_object PY_FUNC1(list_reverse, R_list list) {
    R_array a;
    uintptr_t lower, upper;
    LOCK_LIST(list);
    a = list->array;
    lower = 0;
    upper = LIST_SIZE(list)-1;
    while (lower < upper) {
        R_object l = ITEM(a, lower);
        R_object u = ITEM(a, upper);
        ITEM(a, lower) = u;
        ITEM(a, upper) = l;
        upper--;
        lower++;
    }
    UNLOCK_LIST(list);
    return (R_object)None;
}

R_object PY_FUNC1(list_reversed, R_list list) {
    R_list result = gc_allocate(list);
    SET_TYPE(result, type_list);
    result->lock = GVMT_LOCK_INITIALIZER;
    LOCK_LIST(list);
    result->array = reverse_array(list->array);
    UNLOCK_LIST(list);
    return (R_object)result;
}

R_object PY_FUNC1(list_len, R_list list) {
    return (R_object)R_int_from_ssize_t(LIST_SIZE(list));
}

R_list pack_list_reverse(int count, GVMT_Value* values) {    
    R_list result;
    R_array a;
    int i, size = 4;
    while (size < count)
        size <<= 1;
    a = create_array(size);
    a->a_size = count;
    values += count-1;
    for(i = 0; i < count; i++) {
        R_object o = (R_object)gvmt_stack_read_object(values-i);
        ITEM(a, i) = o;
    }
    result = gc_allocate(list);
    SET_TYPE(result, type_list);
    result->lock = GVMT_LOCK_INITIALIZER;   
    result->array = a;
    return result;
}


#ifdef BOOTSTRAP

GVMT_OBJECT(function) func_list_init = { 0 };

#endif
R_function list_init_function = &func_list_init;

R_list HotPySequence_List(R_object o) {
    if (IS_LIST(o)) {
       return (R_list)o; 
    } else {
        R_list l = make_list(8);
        R_tuple t = make_tuple2((R_object)l, o);
        CALLABLE(list_init_function)((R_object)list_init_function, t, 0);
        return (R_list)l;
    }
}

R_array get_sequence_as_array(R_object seq, int *seq_len) {
    if (IS_TUPLE(seq)) {    
        *seq_len = ((R_tuple)seq)->length;
        return (R_array)seq;
    } else if (IS_LIST(seq)) {
        R_list list = (R_list)seq;
        R_array result, a;
        int i;
        LOCK_LIST(list);
        a = list->array;
        result = create_array(LIST_SIZE(list));
        for(i = 0; i < LIST_SIZE(list); i++) {
            R_object t = ITEM(a, i);
            ITEM(result, i) = t;
        }
        *seq_len = LIST_SIZE(list);
        UNLOCK_LIST(list);
        return result;
    } else {
        R_list l = HotPySequence_List(seq);
        *seq_len = LIST_SIZE(l);
        return l->array;
    }
}

void list_insert_nolock(R_list list, intptr_t index, R_object item) {
    intptr_t i;
    R_array a;
    a = list->array;
    if (index < 0) {
        index = LIST_SIZE(list) + index;
        if (index < 0)
            index = 0;
    } else {
        if (index > LIST_SIZE(list))
            index = LIST_SIZE(list);
    }
    if (LIST_SIZE(list) == a->length) {
        a = expand_array(a);
        list->array = a;
    }
    for(i = LIST_SIZE(list); i > index; i--) {
        R_object temp = ITEM(a, i-1);    
        ITEM(a, i) = temp;
    }
    ITEM(a, index) = item;
    LIST_SIZE(list)++;
}

R_object PY_FUNC3(list_insert, R_list list, R_int idx, R_object item) {
    intptr_t index;
    R_array a;
    index = int_from_py_int(idx);
    LOCK_LIST(list);
    list_insert_nolock(list, index, item);
    UNLOCK_LIST(list);
    return (R_object)None;
}

R_object list_pop_unchecked(R_list list, intptr_t index) {
    intptr_t i, size;
    R_array a = list->array;
    R_object result = ITEM(a, index);
    size = LIST_SIZE(list)-1;
    for(i = index; i < size; i++) {
        R_object temp = ITEM(a, i+1);
        ITEM(a, i) = temp;
    }
    ITEM(a, size) = (R_object)None;
    SET_LIST_SIZE(list, size);
    return result;
}

R_object PY_FUNC2(list_pop, R_list list, R_int idx) {
    intptr_t i, size, index;
    R_object result;
    R_array a;
    LOCK_LIST(list);
    if (idx == PY_SMALL_INT(-1)) {
        result = list_pop(list);
        UNLOCK_LIST(list);
        return result;
    }
    a = list->array;
    index = int_from_py_int(idx);
    size = LIST_SIZE(list);
    if (size == 0) {
        UNLOCK_LIST(list);
        raise_char(type_IndexError, "pop from empty list");
    }
    if (index < -size || index >= size) {   
        char buf[40];
        UNLOCK_LIST(list);
        sprintf(buf, "list index out of bounds: %d\n", index);
        raise_char(type_IndexError, buf);
    }
    if (index < 0)
        index = size + index;
    result = list_pop_unchecked(list, index);
    UNLOCK_LIST(list);
    return result;
}

R_object PY_FUNC2(list_contains, R_list list, R_object item) {
    return (R_object)in_array(list->array, LIST_SIZE(list), item); 
}

R_object PY_FUNC1(list_iter, R_list l) {
    R_listiterator it = gc_allocate(listiterator);
    TYPE_ASSERT(l, list);
    SET_TYPE(it, type_listiterator);
    it->next = 0;
    it->list = l;
    return (R_object)it;
}

R_object PY_FUNC1(listiterator_next, R_listiterator it) {
    intptr_t next = it->next;
    R_object result;
    TYPE_ASSERT(it, listiterator);
    READ_LIST_LOCK(it->list);
    if (next >= LIST_SIZE(it->list)) {
        READ_LIST_UNLOCK(it->list);
        stop_iteration();
        return 0;
    } else {
        it->next = next+1;
        result = ITEM(it->list->array, next);
#ifndef READ_LOCKING
        if (result == 0) {
            raise_char(type_RuntimeError, "list modified during iteration");   
        }
#endif
        READ_LIST_UNLOCK(it->list);
        return result;
    }
}    

R_object PY_FUNC1(list_bool, R_list l) {
    TYPE_ASSERT(l, list);
    return (R_object) booleans[LIST_SIZE(l) != 0];
}

static R_list sub_list(R_list list, intptr_t start, intptr_t stop) {
    R_list result;
    R_array from, to;
    intptr_t i, len = stop - start;
    assert(LIST_SIZE(list) >= stop);   
    assert(start >= 0);
    result = make_list(len);
    from = list->array;
    to = result->array;
	for (i = 0; i < len; i++) {
        R_object t = ITEM(from, i+start);
        ITEM(to, i) = t;
	}
    to->a_size = len;
	return result;
}

static void delitem_list_slice(R_list list, R_slice s) {
    intptr_t i, start, stop, size, step, len;
    R_array a = list->array;
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(s, slice);
    size = LIST_SIZE(list);
    LOCK_LIST(list);
    len = decode_slice(s, size, &start, &stop, &step);
    if (step == 1) {
        array_copy(a, stop, a, start, size-stop);
        SET_LIST_SIZE(list, size-stop+start);
        UNLOCK_LIST(list);
    } else {
        UNLOCK_LIST(list);
        NOT_IMPLEMENTED_ERROR;
    }
}

R_object PY_FUNC2(delitem_list, R_list list, R_object index) {
    if (!IS_INT(index)) {
        if (IS_SLICE(index)) {
            delitem_list_slice(list, (R_slice)index);   
            return (R_object)None;
        }
        assert(0 && "To do");
    }
    PY_list_pop((R_int)index, list);
    return (R_object)None;
}

R_object PY_FUNC2(iadd_list_list, R_list list, R_list other) {
    R_array a;
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(other, list);
    LOCK_LIST(list);
    a = list->array;
    if (list != other)
        // Potential deadlock
        READ_LIST_LOCK(other);
    while (LIST_SIZE(other) + LIST_SIZE(list) > a->length) {
        a = expand_array(a);
    }
    array_copy(other->array, 0, a, LIST_SIZE(list), LIST_SIZE(other));
    LIST_SIZE(list) += LIST_SIZE(other);
    list->array = a;
    if (list != other)
        READ_LIST_UNLOCK(other);
    UNLOCK_LIST(list);
    return (R_object)list;
}

R_object PY_FUNC2(add_list_list, R_list self, R_list other) {
    // Lock other
    // Copy into end of array
    // Unlock other & lock self
    // If self has been resized, resize array.
    // Copy self into start of array.
    // Unlock self.
    intptr_t len_self, new_capacity, len_other;
    R_list result;
    R_array to;
    TYPE_ASSERT(self, list);
    TYPE_ASSERT(other, list);
    len_other = LIST_SIZE(other);
    READ_LIST_LOCK(self);
    len_self = LIST_SIZE(self);
    result = make_list(len_self + len_other);
    to = result->array;
    array_copy(self->array, 0, to, 0, len_self);
    if (self != other) {
        READ_LIST_LOCK(self);
        READ_LIST_UNLOCK(other);  
    }
    if (LIST_SIZE(other) != len_other) {
        result = make_list(len_self + LIST_SIZE(other));
        array_copy(to, 0, result->array, 0, len_self);
        len_other = LIST_SIZE(other);
        to = result->array;
    }
    array_copy(other->array, 0, to, len_self, len_other);
    to->a_size = len_self + len_other;
    READ_LIST_UNLOCK(other);
    return (R_object)result;
}

static void setitem_list_simpleslice(R_list list, intptr_t start, intptr_t stop, R_array other, intptr_t len) {
    int diff = len - stop + start;
    R_array a = list->array;
    while (LIST_SIZE(list) + diff > a->length) {
        list->array = a = expand_array(a);
    }
    array_copy(a, stop, a, stop+diff, LIST_SIZE(list)-stop);
    array_copy(other, 0, a, start, len);
    LIST_SIZE(list) += diff; 
}

static R_array multiply_array(R_array a, intptr_t count, intptr_t mult) {
    int i, size = 4;
    R_array result;
    while (size < count*mult)
        size <<= 1;
    result = create_array(size);
    for (i = 0; i < mult; i++) {
        array_copy(a, 0, result, i*count, count);
    }
    return result;
}

R_object PY_FUNC2(mul_list_int, R_list list, R_int i) {
    intptr_t count = int_from_py_int(i);
    R_list result = make_list(0);
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(i, int);
    READ_LIST_LOCK(list);
    result->array = multiply_array(list->array, LIST_SIZE(list), count);
    SET_LIST_SIZE(result, LIST_SIZE(list) * count);
    READ_LIST_UNLOCK(list);
    return (R_object)result;
}  

R_object PY_FUNC2(imul_list_int, R_list list, R_int i) {
    intptr_t j, count;
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(i, int);
    count = int_from_py_int(i);
    LOCK_LIST(list);
    list->array = multiply_array(list->array, LIST_SIZE(list), count);
    SET_LIST_SIZE(list, LIST_SIZE(list) * count);
    UNLOCK_LIST(list);
    return (R_object)list;
}

R_object PY_FUNC3(list_setitem_int, R_list list, R_int index, R_object value) {
    int i, limit;
    LOCK_LIST(list);
    i = int_from_py_int(index);
    limit = LIST_SIZE(list);
    if (i < 0)
        i = limit + i;
    if (((unsigned)i) >= ((unsigned)limit)) {
        UNLOCK_LIST(list);
        return index_out_of_bounds(i);
    } else {
        ITEM(list->array, i) = value;
        UNLOCK_LIST(list);
        return (R_object)None;
    }
}

#define SAFE_EQUALS(a,b) ((a) == (b) || (((a)!= 0) && ((b)!= 0) && \
    ((R_bool)binary_call(operator_eq, (R_object)(a), (R_object)(b))) == TRUE))

static int list_equals(R_list l1, R_list l2) {
        int i, len = LIST_SIZE(l1);
        R_array a1, a2;
        if (len != LIST_SIZE(l2))
            return 0;
        a1 = l1->array;
        a2 = l2->array;
        for (i = 0; i < len; i++) {
            R_object l, r;
            l = ITEM(a1, i);
            r = ITEM(a2, i);
            if (!SAFE_EQUALS(l, r))
                return 0;
        }
        return 1;
}

R_object PY_FUNC2(list_eq, R_list l1, R_object o) {
    if (IS_LIST(o)) {
        return (R_object)booleans[list_equals(l1, (R_list)o)];
    } else {
        return (R_object)FALSE; 
    }
}

R_object PY_FUNC2(list_ne, R_list l1, R_object o) {
    if (IS_LIST(o)) {
        return (R_object)booleans[1-list_equals(l1, (R_list)o)];
    } else {
        return (R_object)TRUE; 
    }
}

/**** Specialised forms for list.__getitem__ ******/

R_object PY_FUNC1(list_copy, R_list list) {
    R_list result;
    R_array to;
    intptr_t size;
    LOCK_LIST(list);
    size = LIST_SIZE(list);
    result = make_list(size);
    to = result->array;
    array_copy(list->array, 0, to, 0, size);
    SET_LIST_SIZE(result, size);
    UNLOCK_LIST(list);
    return (R_object)result;
}

static intptr_t get_slice_index(R_list l, R_object o) {
    intptr_t val, limit;
    if (gvmt_is_tagged(o)) {
        val = ((intptr_t)gvmt_untag(o)) >> 1;
    } else {
        val = int_from_py_int(HotPyNumber_Index(o));
    }
    limit = LIST_SIZE(l);
    if (val < 0)
        val += limit;
    if (val < 0)
        val = 0;
    if (val > limit)
        val = limit;
    return val;
}

R_object PY_FUNC2(list_left_slice, R_list l, R_object o) {
    intptr_t i;
    R_list result;
    i = get_slice_index(l, o);
    result = make_list(i);
    array_copy(l->array, 0, result->array, 0, i);
    SET_LIST_SIZE(result, i);
    return (R_object)result;
}

R_object PY_FUNC2(list_right_slice, R_list l, R_object o) {
    intptr_t i;
    R_list result;
    i = get_slice_index(l, o);
    result = make_list(LIST_SIZE(l) - i);
    array_copy(l->array, i, result->array, 0, LIST_SIZE(l) - i);
    SET_LIST_SIZE(result, LIST_SIZE(l)-i);
    return (R_object)result;
}

R_object PY_FUNC2(list_getitem_slice, R_list list, R_slice s) {
    R_list result;
    intptr_t i, cur, start, stop, step, length;
    R_array from, to;
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(s, slice);
    length = decode_slice(s, LIST_SIZE(list), &start, &stop, &step);
    assert(length >= 0);
    if (step == 1) {
        return (R_object)sub_list(list, start, stop);
    }
    result = make_list(length);
    from = list->array;
    to = result->array;
    for (cur = start, i = 0; i < length; cur += step, i++) {
        R_object t = ITEM(from, cur);
        ITEM(to, i) = t;
    }
    SET_LIST_SIZE(result, length);
    return (R_object)result;  
}

R_object PY_FUNC2(list_getitem_int, R_list list, R_int i) {
    R_object result;
    intptr_t limit, index = int_from_py_int(i);
    TYPE_ASSERT(list, list);
    READ_LIST_LOCK(list);
    limit = LIST_SIZE(list);
    assert (limit >= 0);
    if (index < 0)
        index += limit;
    if (((unsigned)index) >= ((unsigned)limit)) {
        READ_LIST_UNLOCK(list);
        return index_out_of_bounds(index);
    } else {
        result = ITEM(list->array, index);   
        READ_LIST_UNLOCK(list);
        return result;
    }
}

R_object PY_FUNC2(list_getitem_index, R_list list, R_object o) {
    R_int idx = HotPyNumber_Index(o);
    return PY_list_getitem_int(idx, list);
}

/** Unspecialised version */
R_object PY_FUNC2(list_getitem, R_list list, R_object o) {
    R_object result;
    TYPE_ASSERT(list, list);
    READ_LIST_LOCK(list);
    if (gvmt_is_tagged(o)) {
        result = PY_list_getitem_int((R_int)o, list);
    } else if (IS_SLICE(o)) {
        result = (R_object)PY_list_getitem_slice((R_slice)o, list); 
    } else {
        result = PY_list_getitem_index(o, list);
    }
    READ_LIST_UNLOCK(list);
    return result;
}

/**** Specialised forms for list.__setitem__ *******/

R_object PY_FUNC2(list_update, R_list list, R_list from) {
    R_list copy = (R_list)PY_list_copy(from);
    LOCK_LIST(list);
    list->array = copy->array;
    SET_LIST_SIZE(list, LIST_SIZE(copy));
    UNLOCK_LIST(list);
    return (R_object)None;
}

R_object PY_FUNC3(list_setitem_slice_list_, R_list list, R_slice s, R_list other) {
    intptr_t diff, i, cur, start, stop, size, step, seq_len, len;
    R_array a;
    // Ensure consistent order to avoid deadlock. 
    // If GC occurs, it could mess this up. May need to do some copying.
    if (list < other) {
        LOCK_LIST(list); 
        LOCK_LIST(other);  
    } else {
        LOCK_LIST(other); 
        LOCK_LIST(list);  
    }
    size = LIST_SIZE(list);
    len = decode_slice(s, size, &start, &stop, &step);
    a = list->array;
    if (step == 1) {
        int diff = LIST_SIZE(other) - stop + start;
        
        while (LIST_SIZE(list) + diff > a->length) {
            list->array = a = expand_array(a);
        }
        array_copy(a, stop, a, stop+diff, LIST_SIZE(list)-stop);
        array_copy(other->array, 0, a, start, LIST_SIZE(other));
        LIST_SIZE(list) += diff; 
    } else {
        if (len != LIST_SIZE(other)) {
            char buf[100];
            UNLOCK_LIST(list);
            UNLOCK_LIST(other);
            sprintf(buf, "attempt to assign sequence of size %d to extended slice of size %d", LIST_SIZE(other), len);
            raise_char(type_ValueError, buf);
        }
        for (cur = start, i = 0; i < len; cur += step, i++) {
            R_object t = ITEM(other->array, i);
            ITEM(a, cur) = t;
        }
    }
    UNLOCK_LIST(list);
    UNLOCK_LIST(other);
    return (R_object)None;
}

R_object PY_FUNC3(list_left_assign, R_list list, R_int index, R_list other) {
    R_array new_array;
    int new_size, cut, size;
    if (list < other) {
        LOCK_LIST(list); 
        LOCK_LIST(other);  
    } else {
        LOCK_LIST(other); 
        LOCK_LIST(list);  
    }
    cut = get_slice_index(list, (R_object)index);
    if (cut == LIST_SIZE(other)) {
        array_copy(other->array, 0, list->array, 0, cut);
    } else {
        new_size = LIST_SIZE(other) + LIST_SIZE(list) - cut;
        size = LIST_SIZE(other);
        while (size < new_size)
            size <<= 1;
        new_array = create_array(size);
        array_copy(other->array, 0, new_array, 0, LIST_SIZE(other));
        array_copy(list->array, cut, new_array, LIST_SIZE(other), LIST_SIZE(list) - cut);
        list->array = new_array;
        SET_LIST_SIZE(list, new_size);
    }
    UNLOCK_LIST(list);
    UNLOCK_LIST(other);
    return (R_object)None;
}

R_object PY_FUNC3(list_right_assign, R_list list, R_int index, R_list other) {
    R_array new_array;
    int new_size, cut, size;
    if (list < other) {
        LOCK_LIST(list); 
        LOCK_LIST(other);  
    } else {
        LOCK_LIST(other); 
        LOCK_LIST(list);  
    }
    cut = get_slice_index(list, (R_object)index);
    if (LIST_SIZE(list) - cut == LIST_SIZE(other)) {
        array_copy(other->array, cut, list->array, cut, LIST_SIZE(list) - cut);
    } else {
        assert(0 && "To do");
    }
    UNLOCK_LIST(list);
    UNLOCK_LIST(other);
    return (R_object)None;
}


R_object PY_FUNC3(list_setitem_slice, R_list list, R_slice s, R_object other) {
    intptr_t diff, i, cur, start, stop, size, step, seq_len, len;
    R_array seq, a;
    TYPE_ASSERT(list, list);
    TYPE_ASSERT(s, slice);
    if (IS_LIST(other)) {
        PY_list_setitem_slice_list_((R_list)other, s, list);
    } else {
        seq = get_sequence_as_array(other, &seq_len);
        LOCK_LIST(list);
        a = list->array;
        size = LIST_SIZE(list);
        len = decode_slice(s, size, &start, &stop, &step);
        if (step == 1) {
            setitem_list_simpleslice(list, start, stop, seq, seq_len);
            UNLOCK_LIST(list);
        } else {
            if (len != seq_len) {
                char buf[100];
                UNLOCK_LIST(list);
                sprintf(buf, "attempt to assign sequence of size %d to extended slice of size %d", seq_len, len);
                raise_char(type_ValueError, buf);
            }
            for (cur = start, i = 0; i < len; cur += step, i++) {
                R_object t = ITEM(seq, i);
                ITEM(a, cur) = t;
            }
            UNLOCK_LIST(list);
        }
    }
    return (R_object)None;
}

R_object PY_FUNC3(list_setitem, R_list list, R_object index, R_object value) {
    TYPE_ASSERT(list, list);
    if (IS_INT(index)) {
        int i, limit;
        LOCK_LIST(list);
        i = int_from_py_int((R_int)index);
        limit = LIST_SIZE(list);
        if (i < 0)
            i = limit + i;
        if (((unsigned)i) >= ((unsigned)limit)) {
            UNLOCK_LIST(list);
            return index_out_of_bounds(i);
        } else {
            ITEM(list->array, i) = value;
            UNLOCK_LIST(list);
            return (R_object)None;
        }
    } else if (IS_SLICE(index)) {
        return PY_list_setitem_slice(value, (R_slice)index, list);
    } else {
        return (R_object)NotImplemented;
    }
}

void __hotpy_list_type_assert(R_list l, R_type cls) {
    intptr_t i, size;
    TYPE_ASSERT(l, list);
    size = LIST_SIZE(l);
    for (i = 0; i < size; i++) {
        R_object obj = ITEM(l->array, i);
        assert(is_subclass_of(TYPE(obj), cls));
    }
}

