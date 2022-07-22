#include "hotpy.h"

R_bytearray make_bytearray(size_t size) {
    R_bytearray bytes;
    bytes = gc_allocate_var(bytearray, bytes, size);
    SET_TYPE(bytes, type_bytearray);
    ZERO_REF_COUNT(bytes);
    bytes->length = size;
    gvmt_fully_initialized((GVMT_Object)bytes);
    return bytes;
}

R_bytes make_bytes(size_t size) {
    R_bytes bytes;
    bytes = gc_allocate_var(bytes, bytes, size);
    SET_TYPE(bytes, type_bytes);
    ZERO_REF_COUNT(bytes);
    bytes->length = size;
    gvmt_fully_initialized((GVMT_Object)bytes);
    return bytes;
}

/* Put bytelist stuff here */

#define LOCK_LIST(l)  if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(bytelist), lock))
#define UNLOCK_LIST(l)  if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(l), offsetof(GVMT_OBJECT(bytelist), lock))

R_bytelist make_bytelist(size_t capacity) {
    R_bytelist result = gc_allocate(bytelist);
    int size = 4;
    SET_TYPE(result, type_bytelist);
    ZERO_REF_COUNT(result);
    result->size = 0;
    result->lock = GVMT_LOCK_INITIALIZER;
    while (size < capacity)
        size <<= 1;
    result->array = make_bytearray(size);
    return result;
}

static R_bytearray expand_bytearray(R_bytearray old) {
    R_bytearray new = make_bytearray(old->length << 1);
    int i;
    assert((old->length % 4) == 0);
    for (i = 0; i < old->length; i+= 4) {
        int32_t tmp = *(int32_t*)(&old->bytes[i]);
        *(int32_t*)(&new->bytes[i]) = tmp;
    }
    return new;
}

void bytelist_append_nolock(R_bytelist list, uint8_t byte) {
    intptr_t index;
    R_bytearray a;
    index = list->size;
    a = list->array;
    if (index == a->length) {
        a = expand_bytearray(a);
        list->array = a;
    }
    a->bytes[index] = byte;
    list->size++;
}


void bytelist_append_block_nolock(R_bytelist list, uint8_t *bytes, size_t count) {
    intptr_t i, index;
    R_bytearray a;
    index = list->size;
    a = list->array;
    while (index + count > a->length) {
        a = expand_bytearray(a);
        list->array = a;
    }
    for (i = 0; i < count; i++) {
        a->bytes[i+index] = bytes[i];
    }
    list->size += count;
}

void bytelist_append(R_bytelist list, uint8_t byte) {
    intptr_t index;
    R_bytearray a;
    LOCK_LIST(list);
    index = list->size;
    a = list->array;
    if (index == a->length) {
        a = expand_bytearray(a);
        list->array = a;
    }
    a->bytes[index] = byte;
    list->size++;
    UNLOCK_LIST(list);
}


void bytelist_clear(R_bytelist list) {
    list->size = 0;
}


void bytelist_append_long(R_bytelist list, int32_t value) {
    intptr_t index;
    R_bytearray a;
    LOCK_LIST(list);
    index = list->size;
    a = list->array;
    if (index+4 > a->length) {
        a = expand_bytearray(a);
        list->array = a;
    }
    a->bytes[index] = value >> 24;
    a->bytes[index+1] = value >> 16;
    a->bytes[index+2] = value >> 8;
    a->bytes[index+3] = value;
    list->size+=4;
    UNLOCK_LIST(list);
}

void bytelist_append_short(R_bytelist list, int16_t value) {
    intptr_t index;
    R_bytearray a;
    LOCK_LIST(list);
    index = list->size;
    a = list->array;
    if (index+2 > a->length) {
        a = expand_bytearray(a);
        list->array = a;
    }
    a->bytes[index] = value >> 8;
    a->bytes[index+1] = value;
    list->size+=2;
    UNLOCK_LIST(list);
}

int bytelist_pop(R_bytelist list) {
    intptr_t index;
    uint8_t  result;
    LOCK_LIST(list);
    index = list->size - 1;
    result = list->array->bytes[index];
    list->size = index;
    UNLOCK_LIST(list);
    return result;
}

intptr_t hotpy_bytes_hash(R_bytes b) {
    // Caching this is a possibility...
    if (LEN(b) == 0)
        return 0;
    else {
        intptr_t hash = b->bytes[0] << 7;
        size_t len = LEN(b);
        size_t i; 
        for (i = 1; i < len; i++) {
            hash = (1000003*hash) ^ b->bytes[i];
        }
        hash ^= LEN(b);
        return hash;
    }
}

R_object PY_FUNC1(bytes_length, R_bytes b) {
    return (R_object)R_int_from_ssize_t(b->length);
}

R_object PY_FUNC2(bytes_getitem, R_bytes bytes, R_object i) {
    TYPE_ASSERT(bytes, bytes);
    if (IS_INT(i)) {
        int index = sequence_index((R_int)i, bytes->length);
        return (R_object)PY_SMALL_INT(bytes->bytes[index]);
    } else {
        assert(0 && "To do");
    }
    return (R_object)NotImplemented;
}


