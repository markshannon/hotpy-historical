#ifndef HOTPY_TUPLE_OBJECT_H
#define HOTPY_TUPLE_OBJECT_H

GVMT_OBJECT(tuple) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uintptr_t length;   
    GVMT_OBJECT(object)* array[]; 
};

GVMT_OBJECT(tupleiterator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t next;  
    GVMT_OBJECT(tuple)* tuple; 
};


#define gc_allocate_tuple(count) ((R_tuple)gvmt_malloc(offsetof(GVMT_OBJECT(tuple), array[0]) + (count)*sizeof(R_object)))

R_tuple tuple_concat(R_tuple t1, R_tuple t2);

R_tuple tuple_prepend(R_object o1, R_tuple t2);

R_tuple tuple_slice(R_tuple t, int start, int end);

void unpack_tuple(int count, R_tuple sequence, GVMT_Value* values);

static R_tuple make_tuple1(R_object x) {
    R_tuple t = gc_allocate_tuple(1);
    SET_TYPE(t, type_tuple);
    t->length = 1;
    ITEM(t, 0) = x;
    return t;
}

static R_tuple make_tuple2(R_object x, R_object y) {
    R_tuple t = gc_allocate_tuple(2);
    SET_TYPE(t, type_tuple);
    t->length = 2;
    ITEM(t, 0) = x;
    ITEM(t, 1) = y;
    return t;
}

R_object PY_FUNC2(tuple_from_list, R_type cls, R_list l);

int tuple_equals(R_tuple t1, R_tuple t2);

intptr_t hotpy_tuple_hash(R_tuple t);

R_tuple HotPySequence_Tuple(R_object s);

#endif
