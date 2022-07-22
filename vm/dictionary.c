#include "hotpy.h"
#include "dictionary.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "attributes.h"
#include "intobject.h"
#include "operators.h"
#include "guard.h"

/** Dictionary implementation. Uses the same algorithm as CPython.
 * Most of the code is dict_algorithms as it is shared between dict and str_dict.
 */

#define PERTURB_SHIFT 5

#define KEY(t, i) (t)->array[(i)*2] 
#define VALUE(t, i) (t)->array[(i)*2+1]

extern GVMT_OBJECT(type) _dict_table;

GVMT_OBJECT(dict_table) empty_dict_table_object = {
    &_dict_table,
    0, /* size */
    8, /* length */
    0, /* load */
    0, /* used */
    { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }
};

/** size must be a power of 2 (or else!) */
R_dict_keys new_dict_keys(int32_t size) {
    R_dict_keys k = gc_allocate_var(dict_keys, array, size);
    assert(size > 0);
    assert(is_power2(size));
    SET_TYPE(k, type_dict_keys);
    k->length = size;
    k->used = 0;
    k->load = (size+1) * 2 / 3;
    return k;
}

R_dict_values new_dict_values(R_dict_keys keys) {
    R_dict_values v;
    int32_t len = keys->length;
    v = gc_allocate_var(dict_values, array, len);
    SET_TYPE(v, type_dict_values);
    v->length = len;
    v->keys = keys;
    return v;
}

/** size must be a power of 2 (or else!) */
R_module_dict_values new_module_dict_values(R_dict_keys keys) {
    R_module_dict_values v;
    int32_t len = keys->length;
    v = gc_allocate_var(module_dict_values, array, len);
    SET_TYPE(v, type_module_dict_values);
    v->length = len;
    v->size = 0;
    v->keys = keys;
    return v;
}

/** size must be a power of 2 (or else!) */
R_dict_table new_dict_table(int32_t size) {
    int len = size * 2;
    R_dict_table new_table = gc_allocate_var(dict_table, array, len);
#ifndef NDEBUG
    int tmp, bits = 0;
    tmp = size;
    while (tmp > 1) {
        tmp >>= 1;
        bits += 1;
    }
    if(size != (1 << bits))
        abort();
#endif
    assert(size > 0);
    SET_TYPE(new_table, type_dict_table);
    new_table->length = len;
    new_table->size = 0;
    new_table->used = 0;
    new_table->load = size * 2 / 3;
    return new_table;
}

static int general_lookup(R_dict_table table, R_object key) {
	uint32_t i, perturb, mask;
    perturb = HotPyObject_Hash(key);
    mask = (table->length >> 1)-1;
    i = perturb & mask;
    do {                   
        R_object key_i = KEY(table, i);
        R_bool b;
        if (key_i == key || key_i == 0) {
            return i;
        }
        b = (R_bool)binary_call(operator_eq, key, key_i);
        assert(IS_BOOL(b));
        if (b->value) {
            return i;
        }
        i = (i * 5 + perturb + 1) & mask;
        perturb >>= PERTURB_SHIFT;
    } while (1);
    // Keep dumb compiler quiet.
    return 0;
}

/** Do not change algorithm without changing algorithm in make_base.py */
static int string_lookup(R_dict_table table, R_str key) {
	uint32_t i;
	uint32_t hash = key->hashcode;
    uint32_t perturb = hash;
    uint32_t mask = (table->length >> 1)-1;
    i = perturb & mask;
    do {
        R_str key_i = (R_str)KEY(table, i);
        if (key_i == key || key_i == 0) {
            return i;
        }
        if (hash == key_i->hashcode && string_equals(key, key_i)) {
            return i;   
        }
        i = ((i << 2) + i + perturb + 1) & mask;
        perturb >>= PERTURB_SHIFT;
    } while (1);
    // Keep dumb compiler quiet.
    return 0;
}

#define KEY_TYPE R_object
#define PREFIX(x) dict##x
#define POSTFIX(x) x##dict
#define KEY_ERROR(key) raise_str(type_KeyError, HotPyObject_Repr((R_object)key))
#define LOOKUP general_lookup

#include "dict_algorithms.c"

#undef KEY_TYPE
#undef PREFIX
#undef POSTFIX
#undef KEY_ERROR
#undef LOOKUP
#undef ALLOCATE

#define KEY_TYPE R_str
#define PREFIX(x) str_dict##x
#define POSTFIX(x) x##str_dict
#define KEY_ERROR(key) raise_str(type_KeyError, key)
#define LOOKUP string_lookup

#include "dict_algorithms.c"

#undef KEY_TYPE
#undef PREFIX
#undef POSTFIX
#undef KEY_ERROR
#undef LOOKUP
#undef ALLOCATE

int general_lookup_keys(R_dict_keys keys, R_object key) {
	uint32_t i, perturb, mask;
#ifdef BOOTSTRAP
    // Do this to allow handling of dicts with str keys during bootstrap.
    if (TYPE(key) == type_str)
        return probable_str_lookup(keys, (R_str)key);
#endif
    perturb = HotPyObject_Hash(key);
    mask = keys->length-1;
    i = perturb & mask;
    do {                   
        R_object key_i = ITEM(keys, i);
        R_bool b;
        if (key_i == key || key_i == 0) {
            return i;
        }
        b = (R_bool)binary_call(operator_eq, key, key_i);
        assert(IS_BOOL(b));
        if (b->value) {
            return i;
        }
        i = (i * 5 + perturb + 1) & mask;
        perturb >>= PERTURB_SHIFT;
    } while (1);
    // Keep compiler quiet.
    return 0;
}

int probable_str_lookup(R_dict_keys keys, R_str key) {
	uint32_t i, perturb, hash, mask;
    hash = key->hashcode;
    perturb = hash;
    mask = keys->length-1;
    i = perturb & mask;
    do {                   
        R_str key_i = (R_str)ITEM(keys, i);
        R_bool b;
        if (key_i == key || key_i == 0) {
            return i;
        }
        if (IS_STR(key_i)) {
            if (hash == key_i->hashcode && string_equals(key, key_i)) {
                return i;   
            }
        } else {
            b = (R_bool)binary_call(operator_eq, (R_object)key, (R_object)key_i);
            assert(IS_BOOL(b));
            if (b->value) {
                return i;
            }
        }
        i = (i * 5 + perturb + 1) & mask;
        perturb >>= PERTURB_SHIFT;
    } while (1);
    // Keep compiler quiet.
    return 0;
}

static void trigger_all_guards(R_dict guards) {
    R_dict_values v = guards->values;   
    int i, len = LEN(v);
    for (i = 0; i < len; i++) {
        R_guard g = (R_guard)ITEM(v, i);
        if (g) {
            TYPE_ASSERT(g, guard);
            trigger_guard(g);
        }
    }
}

void dict_resize(R_dict d) {
    R_dict_values new_v, old_v = d->values;
    size_t i, len = old_v->length;
    size_t size = old_v->size;
    R_dict_keys new_k, old_k = old_v->keys;
    size_t computed_load = (len+1)*2/3;
    size_t new_len;
    if (old_k->load < computed_load) {
        // Must be an object dict with cache keys, don't resize
        new_len = len;
    } else {
        new_len = 4;
        while (new_len < (size << 1))
            new_len <<= 1;
    }
    new_k = new_dict_keys(new_len);
    if (TYPE(old_v) == type_module_dict_values) {
        R_object value_guards, position_guards;
        value_guards = ITEM(old_v, LEN(old_v));
        assert(value_guards == NULL || TYPE(value_guards) == type_dict);
        position_guards = ITEM(old_v, LEN(old_v)+1);
        assert(position_guards == NULL || TYPE(position_guards) == type_dict);
        if (position_guards)
            trigger_all_guards((R_dict)position_guards);
        new_v = (R_dict_values)new_module_dict_values(new_k);
        ITEM(new_v, LEN(new_v)-1) = value_guards;

    } else {
        new_v = new_dict_values(new_k);
    }
    for (i = 0; i < len; i++) {
        R_object val = ITEM(old_v, i);
        if (val != (R_object)NULL_R) {
            R_object key = ITEM(old_k, i);
            uint32_t index = general_lookup_keys(new_k, key);
            assert(ITEM(new_k, index) == (R_object)NULL_R);
            ITEM(new_k, index) = key;
            ITEM(new_v, index) = val;
        }
    } 
    new_k->used = new_v->size = old_v->size;
    d->values = new_v;
}

R_dict_values values_resize_str(R_dict_values old_v) {
    R_dict_values new_v;
    int i, len = old_v->length;
    int size = old_v->size;
    R_dict_keys new_k, old_k = old_v->keys;
    size_t computed_load = (len+1)*2/3;
    size_t new_len;
    if (old_k->load < computed_load) {
        // Must be an object dict with cache keys, don't resize
        new_len = len;
    } else {
        new_len = 4;
        while (new_len < (size << 1))
            new_len <<= 1;
    }
    new_k = new_dict_keys(new_len);
    if (TYPE(old_v) == type_module_dict_values) {
        R_object value_guards, position_guards;
        value_guards = ITEM(old_v, LEN(old_v));
        assert(value_guards == NULL || TYPE(value_guards) == type_dict);
        position_guards = ITEM(old_v, LEN(old_v)+1);
        assert(position_guards == NULL || TYPE(position_guards) == type_dict);
        if (position_guards)
            trigger_all_guards((R_dict)position_guards);
        new_v = (R_dict_values)new_module_dict_values(new_k);
        ITEM(new_v, LEN(new_v)-1) = value_guards;
    } else {
        new_v = new_dict_values(new_k);
    }
    for (i = 0; i < len; i++) {
        R_object val = ITEM(old_v, i);
        if (val != (R_object)NULL_R) {
            R_object key = ITEM(old_k, i);
            uint32_t index;
            if (IS_STR(key))
                index = probable_str_lookup(new_k, (R_str)key);
            else
                index = general_lookup_keys(new_k, key);
            assert(ITEM(new_k, index) == (R_object)NULL_R);
            ITEM(new_k, index) = key;
            ITEM(new_v, index) = val;
        }
    } 
    new_k->used = new_v->size = old_v->size;
    return new_v;
}

void dict_resize_str(R_dict d) {
    d->values = values_resize_str(d->values);
}

R_dict_table clone_table(R_dict_table dict) {
    R_dict_table new_table = gc_allocate_var(dict_table, array, dict->length);
    uint32_t i, len = dict->length >> 1;
    SET_TYPE(new_table, type_dict_table);
    new_table->length = dict->length;
    assert(dict->load >= 0);
    new_table->size = dict->size;
    new_table->used = dict->used;
    new_table->load = dict->load;
    for (i = 0; i < len; i++) {
        R_object key, value;
        key = KEY(dict, i);
        KEY(new_table, i) = key;
        value = VALUE(dict, i);
        VALUE(new_table, i) = value;
    } 
    return new_table;
}

R_str canonical_string(R_str s) {
    R_dict_keys keys = canonical_strings->values->keys;
    int i = probable_str_lookup(keys, s);
    R_object key = ITEM(keys, i);
    if (key == NULL) {
        dict_set_str(canonical_strings, s, (R_object)None);
        return s;
    } else {
        assert(IS_STR(key));
        return (R_str)key;
    }  
}

/** Returns an arbitrary key from the dict or 0 if empty. */
R_object dict_any_key(R_dict dict) {
    R_dict_values v = dict->values;
    R_dict_keys k = v->keys;
    uint32_t i, len = k->length;
    if (dict->values->size == 0)
        return 0;
    for (i = 0; ITEM(v, i) == (R_object)NULL_R; i++);
    assert(ITEM(k, i) != (R_object)NULL_R);
    return ITEM(k, i);
}

void insert_keyword_arg(R_dict d, R_str key, R_object value) {
    R_dict_values v = d->values;
    uint32_t index = probable_str_lookup(v->keys, key);
    TYPE_ASSERT(v, dict_values);
    if (ITEM(v, index) != (R_object)NULL_R) {
        R_str msg = py_sprintf("got multiple values for keyword argument '%s'", key);
        raise_str(type_TypeError, msg);
    }
    if (v->keys->used >= v->keys->load) {
        dict_resize_str(d);
    }
    v->keys->used += (ITEM(v->keys, index) == (R_object)NULL_R);
    assert(ITEM(d->values, index) == (R_object)NULL_R);
    ITEM(v->keys, index) = (R_object)key;
    v->size++;
    ITEM(v, index) = value;    
}


R_object dict_remove_named_item(R_dict d, R_str key) {
    R_dict_values v = d->values;
    int index = probable_str_lookup(v->keys, key);  
    R_object val = ITEM(v, index);
    assert(TYPE(v) != type_module_dict_values);
    if (val != (R_object)NULL_R) {
        ITEM(v, index) = (R_object)NULL_R; 
        v->size--;
        assert(v->size >= 0);        
    } 
    return val;
}

    
