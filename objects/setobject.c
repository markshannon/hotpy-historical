#include "hotpy.h"
#include <assert.h>

/** Use dict_table and dict_table algorithms 
 *  In dictionary a key/value pair has three possible states:
 *  NULL/NULL = Vacant slot
 *  key/value = Used entry
 *  key/NULL = Deleted entry.
 *
 *  Sets work similarly.
 *  NULL/NULL = Vacant slot
 *  key/XX = Used entry. 
 *  XX is currently key, may change if useful for implementation, XX != NULL.
 *  key/NULL = Removed entry.
 *
 *  set.add(i) == dict.set(i,XX) . Currently XX = i.
 *  set.discard(i) == dict_table.discard(i)
 *  set.remove(i) == dict_table.delete(i)
 */
  
R_set new_set(void) {
    R_set s = gc_allocate(set);    
    SET_TYPE(s, type_set);
    s->values = empty_dict_values;
    s->lock = GVMT_LOCK_INITIALIZER; 
    return s;
}

R_object PY_FUNC1(make_set, R_type cls) {
    R_set s;
    if (cls != type_set) {
        // Extra space for dict
        s = (R_set)gvmt_malloc(sizeof(GVMT_OBJECT(set)) + sizeof(void*));
    } else {
        s = gc_allocate(set);
    }
    SET_TYPE(s, cls);
    ZERO_REF_COUNT(s);
    s->values = empty_dict_values;
    s->lock = GVMT_LOCK_INITIALIZER; 
    if (cls != type_set) {
        assert(cls->dict_offset > 0);
        assert(cls->keys_cache);
        FIELD(R_dict_values, s, cls->dict_offset) 
            = new_dict_values(cls->keys_cache);
    }
    return (R_object)s;
}
 
/** Non-mutating methods **/

extern R_dict_table clone_table(R_dict_table dict);

R_object PY_FUNC1(set_copy, R_set s) {
    R_dict_values v;
    R_dict_keys k, s_k;
    R_set copy;
    LOCK_SET(s);
    v = s->values;
    if (s->values->size == 0)
        return (R_object)new_set();
    s_k = v->keys;
    if (is_immutable_keys(s_k))
        k = s_k;
    else
        k = dict_keys_copy(s_k);
    v = dict_values_copy(v, k);
    UNLOCK_SET(s) ;  
    copy = gc_allocate(set);
    SET_TYPE(copy, type_set);
    copy->values = v;
    copy->lock = GVMT_LOCK_INITIALIZER;
    return (R_object)copy;
}

R_object PY_FUNC2(set_difference, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_intersection, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_issubset, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_issuperset, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_symmetric_difference, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_union, R_set s1, R_set s2) {
    assert(0 && "To do");
    return 0;   
}

R_object PY_FUNC2(set_contains, R_set s, R_object o) {
    R_object in;
    in = dict_get((R_dict)s, o);
    return (R_object)(in == NULL ? FALSE : TRUE);
}

/***** Mutating methods *****/
 
R_object PY_FUNC2(set_add, R_set s, R_object o) {
    dict_set((R_dict)s, o, o);
    return (R_object)None;
}
 
R_object PY_FUNC1(set_clear, R_set s) {
    LOCK_SET(s);
    s->values = empty_dict_values;
    UNLOCK_SET(s);
    return (R_object)None;
}

R_object PY_FUNC2(set_discard, R_set s, R_object o) {
    int index;
    LOCK_SET(s);
    index = general_lookup_keys(s->values->keys, o);
    if (ITEM(s->values, index) != (R_object)NULL_R) {
        ITEM(s->values, index) = (R_object)NULL_R;
        s->values->size--;
    }
    UNLOCK_SET(s);
    return (R_object)None;
}

R_object PY_FUNC2(set_remove, R_set s, R_object o) {
    dict_del((R_dict)s, o);
    return (R_object)None;
}
 
R_object PY_FUNC1(set_pop, R_set s) {
    R_dict_values v;
    R_dict_keys k;  
    R_object result;
    uint32_t i, len;
    LOCK_SET(s);
    v = s->values;
    k = v->keys;
    len = k->length;
    if (s->values->size == 0) {
        UNLOCK_SET(s);
        raise_char(type_KeyError, "pop from an empty set");
        return 0;
    }
    for (i = 0; ITEM(v, i) == (R_object)NULL_R; i++);
    assert(ITEM(k, i) != (R_object)NULL_R);
    result = ITEM(v, i);
    ITEM(v, i) = (R_object)NULL_R;
    v->size--;
    UNLOCK_SET(s);
    return result;
}

R_object PY_FUNC1(set_empty, R_set s) {
    if (s->values->size == 0)
        return (R_object)TRUE;
    else
        return (R_object)FALSE;
}

