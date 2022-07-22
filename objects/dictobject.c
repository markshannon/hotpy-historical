
#include "hotpy.h"
#include "guard.h"

/************************************************************
 * Dictionary implementation 
 * Each dictionary consist of a body which contains a reference to a values array.
 * Keys are either shared or mutable. 
 * Ie a shared keys array is immutable, an unshared one can be mutable.
 * Value arrays are always mutable and never shared.
 ***************************************************************/

R_dict new_empty_dict(void) {
    R_dict dict = gc_allocate(dict);
    SET_TYPE(dict, type_dict);
    dict->lock = GVMT_LOCK_INITIALIZER;
    dict->values = empty_dict_values;
    return dict;
}

R_dict new_dict(int capacity) {
    R_dict dict = gc_allocate(dict);
    SET_TYPE(dict, type_dict);
    dict->lock = GVMT_LOCK_INITIALIZER;
    if (capacity <= 0) {
        dict->values = empty_dict_values;
    } else {
        int power2 = 8;
        while (power2 < capacity)
            power2 <<= 1;
        dict->values = new_dict_values(new_dict_keys(power2));
    }
    return dict;
}

R_dict new_module_dict(int capacity) {
    R_dict dict = gc_allocate(dict);
    SET_TYPE(dict, type_dict);
    dict->lock = GVMT_LOCK_INITIALIZER;
    if (capacity <= 0) {
        dict->values = empty_module_dict_values;
    } else {
        int power2 = 8;
        while (power2 < capacity)
            power2 <<= 1;
        dict->values = (R_dict_values)new_module_dict_values(new_dict_keys(power2));
    }
    return dict;
}

R_dict new_dict_with_keys(R_dict_keys keys) {
    R_dict_values values = new_dict_values(keys);
    R_dict dict = gc_allocate(dict);
    SET_TYPE(dict, type_dict);
    dict->lock = GVMT_LOCK_INITIALIZER; 
    assert(is_immutable_keys(keys));
    dict->values = values;
    return dict;
}

R_object PY_FUNC1(dict_len, R_dict d) {
    int len = d->values->size;
    return (R_object)R_int_from_ssize_t(len);
}

R_object PY_FUNC1(make_dict, R_type cls) {
    R_dict dict = (R_dict)_HotPyObject_New(cls);
    SET_TYPE(dict, cls);
    dict->values = empty_dict_values;
    dict->lock = GVMT_LOCK_INITIALIZER; 
    return (R_object)dict;
}

R_object PY_FUNC1(dict_clear, R_dict d) {
    TYPE_ASSERT(d, dict);
    LOCK_DICT(d);
    d->values = empty_dict_values;
    UNLOCK_DICT(d);
    return (R_object)None;
}

R_object PY_FUNC2(dict_iter_init, R_dict_iterator it, R_dict d) {
    TYPE_ASSERT(it, dict_iterator);
    TYPE_ASSERT(d, dict);
    it->keys = d->values->keys;
    return (R_object)None;    
}

R_object PY_FUNC1(dict_iter_next, R_dict_iterator it) {
    TYPE_ASSERT(it, dict_iterator);
    while (it->next < it->keys->length) {
        int this = it->next;
        it->next ++;
        if (ITEM(it->keys, this) != (R_object)NULL_R)  { 
            R_object result = ITEM(it->keys, this);
            return result;            
        }
    }
    stop_iteration();
    return 0;
}

R_object values_get(R_dict_values values, R_object key) {
    int index = general_lookup_keys(values->keys, key);
    return ITEM(values, index);
}

R_object values_get_str(R_dict_values values, R_str key) {
    int index = probable_str_lookup(values->keys, key);
    return ITEM(values, index);
}

R_object dict_get_str(R_dict dict, R_str key) {
    int index = probable_str_lookup(dict->values->keys, key);
    return ITEM(dict->values, index);
}

int dict_str_in_keys(R_dict_keys keys, R_str key) {
    int index = probable_str_lookup(keys, key);
    return ITEM(keys, index) != NULL;
}

R_object dict_get(R_dict dict, R_object key) {
    int index = general_lookup_keys(dict->values->keys, key);
    return ITEM(dict->values, index);
}

R_object PY_FUNC1(dict_values, R_dict dict) {
    R_dict_values_iterator it = gc_allocate(dict_values_iterator);
    SET_TYPE(it, type_dict_values_iterator);
    it->dict = dict;
    LOCK_DICT(dict);
    it->values = dict->values;
    it->size = dict->values->size;
    UNLOCK_DICT(dict);
    it->next = 0;
    return (R_object)it;
}

R_object PY_FUNC1(dict_values_iterator_iter, R_dict_values_iterator it) {
    TYPE_ASSERT(it, dict_values_iterator);
    return (R_object)it;
}

R_object PY_FUNC1(dict_values_iterator_next, R_dict_values_iterator it) {
    TYPE_ASSERT(it, dict_values_iterator);
    READ_DICT_LOCK(it->dict);
    if (it->dict->values != it->values || it->size != it->values->size) {
        READ_DICT_UNLOCK(it->dict);
        raise_char(type_RuntimeError, "concurrent modification of dictionary");
    }
    while (it->next < it->values->length) {
        int this = it->next;
        R_object result = ITEM(it->values, this);
        it->next = this+1;
        if (result) {
            READ_DICT_UNLOCK(it->dict);
            return result;
        }
    }
    READ_DICT_UNLOCK(it->dict);
    stop_iteration();
    return 0;
}

R_object PY_FUNC2(getitem_dict, R_dict d, R_object key) {
    R_object val;
    TYPE_ASSERT(d, dict);
    val = dict_get(d, key);
    if (val) {
        return val;
    } else {
        R_object missing = class_attribute(d->ob_type, name___missing__);
        if (missing) {
            call_func f = CALLABLE(missing);
            R_tuple t = make_tuple2((R_object)d, key); 
            return f(missing, t, 0);
        }
        raise_str(type_KeyError, HotPyObject_Str(key));
        return NULL;
    }
}

R_object PY_FUNC2(getitem_dict_proxy, R_dict_proxy d, R_str s) {
    TYPE_ASSERT(d, dict_proxy);
    if (TYPE(s) != type_str)
        raise_char(type_TypeError, "dict_proxy indices must strings");
    return str_dict_table_get_raise(d->type->attributes, s);
}

static void check_guards(R_module_dict_values values, R_str key, int offset) {
    R_dict guards = (R_dict)ITEM(values, LEN(values)-offset);
    if (guards) {
        R_guard g;
        TYPE_ASSERT(guards, dict);
        g = (R_guard)dict_get_str(guards, key);
        if (g) {
            trigger_guard(g);
            dict_set_str(guards, key, NULL);
        }
    }
}

void dict_del(R_dict d, R_object key) {
    int index;
    TYPE_ASSERT(d, dict);
    LOCK_DICT(d);
    if (TYPE(d->values) == type_module_dict_values && TYPE(key) == type_str) {
        check_guards((R_module_dict_values)d->values, (R_str)key, 1);
    }
    index = general_lookup_keys(d->values->keys, key);
    if (ITEM(d->values, index) == (R_object)NULL_R) {
        UNLOCK_DICT(d);
        raise_str(type_KeyError, HotPyObject_Str(key));
    } else {
        ITEM(d->values, index) = (R_object)NULL_R;
        d->values->size--;
    }
    UNLOCK_DICT(d);
}

void values_del_str(R_dict_values v, R_str key) {
    int index;
    if (TYPE(v) == type_module_dict_values) {
        check_guards((R_module_dict_values)v, key, 1);
    }
    index = probable_str_lookup(v->keys, key);
    assert(ITEM(v, index) != (R_object)NULL_R);
    ITEM(v, index) = (R_object)NULL_R;
    v->size--;
}

void dict_del_str(R_dict d, R_str key) {
    int index;
    TYPE_ASSERT(d, dict);
    LOCK_DICT(d);
    if (TYPE(d->values) == type_module_dict_values) {
        check_guards((R_module_dict_values)d->values, key, 1);
    }
    index = probable_str_lookup(d->values->keys, key);
    if (ITEM(d->values, index) == (R_object)NULL_R) {
        UNLOCK_DICT(d);
        raise_str(type_KeyError, key);
    } else {
        ITEM(d->values, index) = (R_object)NULL_R;
        d->values->size--;
    }
    UNLOCK_DICT(d);
}

R_object PY_FUNC2(delitem_dict_object, R_dict d, R_object key) {
    dict_del(d, key);
    return (R_object)None;
}

void dict_set(R_dict d, R_object key, R_object val) {
    R_dict_keys k;
    R_dict_values v = d->values;
    int index;
    LOCK_DICT(d);
    if (TYPE(v) == type_module_dict_values && TYPE(key) == type_str) {
        check_guards((R_module_dict_values)d->values, (R_str)key, 2);
    }
    k = v->keys;
    index = general_lookup_keys(k, key);
    if (ITEM(k, index) == (R_object)NULL_R) {
        if (k->load <= k->used) {
            // Need to resize.   
            dict_resize(d);
            v = d->values;
            k = v->keys;
            index = general_lookup_keys(k, key);
        }
        ITEM(k, index) = key;
        k->used++;
        assert(k->used <= k->load);
    }
    v->size += (ITEM(v, index) == (R_object)NULL_R);
    ITEM(v, index) = val;
    UNLOCK_DICT(d);
}

R_object PY_FUNC3(setitem_dict, R_dict d, R_object key, R_object value) {
    TYPE_ASSERT(d, dict);
    dict_set(d, key, value);
    return (R_object)None;
}

R_dict_values values_set_str(R_dict_values v, R_str key, R_object val) {
    R_dict_keys k;
    int index;
    k = v->keys;
    if (TYPE(v) == type_module_dict_values) {
        check_guards((R_module_dict_values)v, (R_str)key, 2);
    }
    index = probable_str_lookup(k, key);
    if (ITEM(k, index) == (R_object)NULL_R) {
        if (k->load <= k->used) {
            // Need to resize.   
            v = values_resize_str(v);
            k = v->keys;
            index = probable_str_lookup(k, key);
        }
        ITEM(k, index) = (R_object)key;
        k->used++;
    }
    v->size += (ITEM(v, index) == (R_object)NULL_R);
    ITEM(v, index) = val;
    return v;
}

void dict_set_str(R_dict d, R_str key, R_object val) {
    R_dict_keys k;
    R_dict_values v = d->values;
    int index;
    TYPE_ASSERT(d, dict);
    LOCK_DICT(d);
    d->values = values_set_str(d->values, key, val);
    //k = v->keys;
    //if (TYPE(v) == type_module_dict_values) {
    //    check_guards((R_module_dict_values)v, (R_str)key, 2);
    //}
    //index = probable_str_lookup(k, key);
    //if (ITEM(k, index) == (R_object)NULL_R) {
    //    if (k->load <= k->used) {
    //        // Need to resize.   
    //        dict_resize_str(d);
    //        v = d->values;
    //        k = v->keys;
    //        index = probable_str_lookup(k, key);
    //    }
    //    ITEM(k, index) = (R_object)key;
    //    k->used++;
    //}
    //v->size += (ITEM(v, index) == (R_object)NULL_R);
    //ITEM(v, index) = val;
    UNLOCK_DICT(d);
}


        
R_object PY_FUNC3(setitem_dict_proxy, R_dict_proxy d, R_str key, R_object value) {
    TYPE_ASSERT(d, dict);
    raise_char(type_TypeError, "'dict_proxy' object does not support item assignment");
    return (R_object)NULL_R;
}

R_object PY_FUNC2(dict_contains, R_dict d, R_object key) {
    R_object value;
    TYPE_ASSERT(d, dict);
    LOCK_DICT(d);
    value = dict_get(d, key);
    UNLOCK_DICT(d);
    if (value)
        return (R_object)TRUE;
    else
        return (R_object)FALSE;  
}

R_object PY_FUNC2(dict_proxy_contains, R_dict_proxy d, R_object s) {
    R_object value;
    TYPE_ASSERT(d, dict_proxy);
    LOCK_DICT(d);
    if (IS_STR(s)) { 
        value  = str_dict_table_get(d->type->attributes, (R_str)s);
        if (value) {
            UNLOCK_DICT(d);
            return (R_object)TRUE;
        }   
    } 
    UNLOCK_DICT(d);
    return (R_object)FALSE;    
}

R_dict_keys dict_keys_copy(R_dict_keys k) {
    int i, len = k->length;   
    R_dict_keys copy = gc_allocate_var(dict_keys, array, len);
    SET_TYPE(copy, type_dict_keys);
    copy->length = len;
    copy->used = k->used;
    copy->load = k->load;
    for (i = 0; i < len; i++) { 
        R_object o = ITEM(k, i);
        ITEM(copy, i) = o;
    }
    gvmt_fully_initialized((GVMT_Object)copy);
    return copy;
}

R_dict_values dict_values_copy(R_dict_values old, R_dict_keys k) {
    int i, len = old->length;   
    R_dict_values copy = gc_allocate_var(dict_values, array, len);
    SET_TYPE(copy, type_dict_values);
    copy->length = len;
    copy->size = old->size;
    copy->keys = k;
    for (i = 0; i < len; i++) { 
        R_object o = ITEM(old, i);
        ITEM(copy, i) = o;
    }
    gvmt_fully_initialized((GVMT_Object)copy);
    return copy;
}

R_dict dict_copy(R_dict d) {
    R_dict_values v = d->values;
    R_dict_keys k, d_k;
    R_dict copy;
    if (d->values == empty_dict_values)
        return new_empty_dict();
    d_k = v->keys;
    LOCK_DICT(d);
    if (is_immutable_keys(d_k))
        k = d_k;
    else
        k = dict_keys_copy(d_k);
    v = dict_values_copy(v, k);
    UNLOCK_DICT(d) ;  
    copy = gc_allocate(dict);
    SET_TYPE(copy, type_dict);
    copy->values = v;
    copy->lock = GVMT_LOCK_INITIALIZER;
    return copy;
}

R_object PY_FUNC1(dict_copy, R_dict dict) {
    return (R_object)dict_copy(dict);   
}

