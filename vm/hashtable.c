#include "hotpy.h"

#ifdef DEBUG
static void sanity(R_hashtable hash) {
    int count = 0;
    assert(hash->size <= hash->load);
    assert(hash->TYPE(codes) == type_int_vector);
    assert(hash->TYPE(values) == type_array);
    assert(hash->codes->length == hash->values->length);
    for(int i = 0; i < hash->codes->length; i++) {
        if (hash->codes->vector[i] == 0)
            assert(ITEM(hash->values, i) ==(R_object)NULL_R);
        if (ITEM(hash->values, i)) {
            assert(hash->codes->vector[i]);
            count++;
        }
    }   
    assert(count == hash->size);
}
#else
#define sanity(x)
#endif

/****** Function hash table implementation *****/

extern GVMT_OBJECT(type) _int_vector;
extern GVMT_OBJECT(type) _array;

GVMT_OBJECT(int_vector) empty_codes_object = {
    &_int_vector,
    IMMORTAL, /* ref_count */
    2, { 0, 0 }
};

GVMT_OBJECT(array) empty_values_object = {
    &_array,
    IMMORTAL, /* ref_count */
    2, { 0, 0 }
};

static R_int_vector empty_codes = &empty_codes_object; 
static R_array empty_values_array = &empty_values_object; 

R_hashtable new_empty_hashtable(void) {
    R_hashtable table = gc_allocate(hashtable);
    SET_TYPE(table, type_hashtable);
    table->size = 0;
    table->load = 0;
    table->mask = 1;
    table->codes = empty_codes;
    table->values = empty_values_array;
    sanity(table);
    return table;    
}

R_object hashtable_get(R_hashtable hash, int code) {
	R_int_vector codes = hash->codes;
    uint32_t i = code;
    assert(code);
    do {
        uint32_t index = i & hash->mask;
        uint32_t key = codes->vector[index];
        if (key == code || key == 0) {
            return ITEM(hash->values, index);
        }
        i = ((i << 2) + i + 1);
    } while (1);
    // Keep Compiler quiet
    return 0;
}

/* Only use this insert if hashtable is known to be of sufficient size */
static void fast_insert(R_hashtable hash, int code, R_object value) {
    uint32_t i = code;
	R_int_vector codes = hash->codes;
    assert(code);
    do {
        uint32_t index = i & hash->mask;
        if (codes->vector[index] == code) {
            ITEM(hash->values, index) = value;
            sanity(hash);
            return;
        }
        if (codes->vector[index] == ZERO) {
            codes->vector[index] = code;
            ITEM(hash->values, index) = value;
            hash->size++;
            sanity(hash);
            return;
        }
        i = ((i << 2) + i + 1);
    } while (1);
}

static void hashtable_resize(R_hashtable hash) {
    uintptr_t i,n;
    R_int_vector old_codes = hash->codes;
    R_array old_functions = hash->values;
    uintptr_t capacity = old_codes->length << 1;
    sanity(hash);
    assert(is_power2(capacity));
    hash->codes = gc_allocate_var(int_vector, vector, capacity);
    SET_TYPE(hash->codes, type_int_vector);
    hash->codes->length = capacity;
    hash->values = gc_allocate_array(capacity);
    SET_TYPE(hash->values, type_array);
    hash->values->length = capacity;
    hash->size = 0;
    hash->load = (capacity >> 2) * 3;
    hash->mask = capacity-1;
    for (i = 0, n = old_codes->length; i < n; i++) {
        if (old_codes->vector[i] != ZERO && 
            ITEM(old_functions, i) != (R_object)NULL_R) {
            fast_insert(hash, old_codes->vector[i], ITEM(old_functions, i));
        }
    }
    sanity(hash);
}

void hashtable_set(R_hashtable hash, int code, R_object value) {
    assert(code);
    if (hash->size >= hash->load) {
        hashtable_resize(hash);
    }
    fast_insert(hash, code, value);
    sanity(hash);
}
