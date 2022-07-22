#ifndef HOTPY_SEQUENCE_H
#define HOTPY_SEQUENCE_H

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(array) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    intptr_t a_size;  /* Ref-count slot */
    uintptr_t length;              
    GVMT_OBJECT(object)* array[];
};

R_object vm_contains(R_object seq, R_object item);
R_object HotPyObject_GetItem(R_object seq, R_object index);
void HotPyObject_SetItem(R_object seq, R_object index, R_object value);
void HotPyObject_DelItem(R_object seq, R_object index);

R_array expand_array(R_array old);

void array_copy(R_array src, int src_offset, R_array dst, int dst_offset, int size);

R_array reverse_array(R_array a);

intptr_t sequence_index(R_int value, int limit);

R_bool in_array(R_array a, int limit, R_object o);

R_object index_out_of_bounds(intptr_t val);

R_array create_array(int size);

GVMT_Object wrong_unpack(int count, int available);

R_type_info optimise_is(R_type_info l, R_type_info r, R_bytelist out);

#define gc_allocate_array(count) ((R_array)gvmt_malloc(offsetof(GVMT_OBJECT(array), array[0]) + (count)*sizeof(R_object)))
#define gc_allocate_tuple(count) ((R_tuple)gvmt_malloc(offsetof(GVMT_OBJECT(tuple), array[0]) + (count)*sizeof(R_object)))

#endif
