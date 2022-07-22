#include "hotpy.h"
#include "guard.h"

R_object PY_FUNC1(object_allocate, R_type cls) {
    return _HotPyObject_New(cls);
}

R_object PY_FUNC1(object_init, R_object self) {
    return (R_object)None;
}

intptr_t hotpy_object_hash(R_object object) {
    uintptr_t bits;
    assert(!gvmt_is_tagged(object));
    bits = ((uintptr_t)gvmt_pin((GVMT_Object)object));
    // bits have format --xxx--yy00
    // hash should have form 00yy--xxx--
    // This will ensure that the hash value will fit into a tagged int,
    // that the low order bits are likely to vary
    // and that the hash != -1.
    return (intptr_t)((bits >> 4) | ((bits & 12) << ( 8 * sizeof(void*) - 4)));
}

// Support for marshalling.
int gvmt_user_unique_id_for_object(GVMT_Object object) {
    if (gvmt_is_tagged(object)) {
        return (intptr_t)gvmt_untag(object);
    } else {
        return (uintptr_t)gvmt_pin(object);
    }
}

R_object PY_FUNC1(object_hash, R_object o) {
    return (R_object)PY_SMALL_INT(hotpy_object_hash(o));
}

R_object PY_FUNC1(object_str, R_object o) {
    return (R_object)HotPyObject_Repr(o);
}

// id() function
R_object PY_FUNC1(id, R_object object) {
    if (gvmt_is_tagged(object)) {
        return object;
    } else {
        intptr_t id;
        // Ensure id can be stored as a tagged int by restricting to 31 bits.
        // Since objects are at least 4 byte aligned, last bit remains zero.
        id = ((intptr_t)gvmt_pin((GVMT_Object)object))>>1;
        return (R_object)PY_SMALL_INT(id);
    }
}

R_object PY_FUNC1(object_repr, R_object o) {
    R_str tname = TYPE_NAME(o);
    R_int id = (R_int)PY_FUNC1(id, o);
    intptr_t x;
    char buf[24];
    if (gvmt_is_tagged(o))
        x = ((intptr_t)gvmt_untag(id)) << 1;
    else
        x = ((intptr_t)gvmt_pin((GVMT_Object)o));
    GVMT_NO_GC_BLOCK_BEGIN
        sprintf(buf, "<%%s id at 0x%x>", x);
    GVMT_NO_GC_BLOCK_END
    return (R_object)py_sprintf(buf, tname);
}

void load_attr_error(R_object object, int key_index) {
    R_dict_keys keys;
    R_dict_values values;
    R_type klass = TYPE(object);
    intptr_t dict_offset = klass->dict_offset;
    R_object name = NULL;
    assert(klass->dict_offset != 0);
    if (dict_offset < 0) {
        assert(klass->variable);
        dict_offset = ((R_int)object)->length*sizeof(void*)-dict_offset;
    }
    values = FIELD(R_dict_values, object, dict_offset);
    if (TYPE(values) == type_dict) {
        values = ((R_dict)values)->values;
    }
    TYPE_ASSERT(values, dict_values);
    keys = values->keys;
    if (key_index < LEN(keys)) {
        name = ITEM(keys, key_index);
    }
    if (name == NULL) {
        name = (R_object)string_from_c_string("???");
    }
    attribute_error(TYPE(object), name);
}

static intptr_t dict_offset(R_object object) {
    R_type klass = TYPE(object);
    intptr_t dict_offset = klass->dict_offset;
    assert(klass->dict_offset != 0);
    if (dict_offset < 0) {
        assert(klass->variable);
        dict_offset = ((R_int)object)->length*sizeof(void*)-dict_offset;
    }
    return dict_offset;
}


R_object _HotPyObject_dict_get(R_object object, R_str key) {
    R_object o = FIELD(R_object, object, dict_offset(object));
    if (TYPE(o) == type_dict_values) {
        return values_get_str((R_dict_values)o, key);
    } else {
        TYPE_ASSERT(o, dict);
        return dict_get_str((R_dict)o, key);
    }
}
void _HotPyObject_dict_set(R_object object, R_str key, R_object value) {
    intptr_t offset = dict_offset(object);
    R_object o = FIELD(R_object, object, offset);
    if (TYPE(o) == type_dict_values) {
        R_dict_values values = (R_dict_values)o;
        R_dict_keys k = values->keys;
        values = values_set_str(values, key, value);
        if (k != values->keys) {
            //Need to lock the dict and/or the klass here (probably both?)
            R_type klass = TYPE(object);
            R_dict_keys new_k = values->keys;
            if (k == klass->keys_cache &&
                k->used + 1 == new_k->used &&
                is_immutable_keys(k) && new_k->used < 12) {
                new_k->load = new_k->used; // Make immutable
                klass->keys_cache = new_k;
            }
        }
        FIELD(R_dict_values, object, offset) = values;
    } else {
        TYPE_ASSERT(o, dict);
        dict_set_str((R_dict)o, key, value);
    }
}

static R_object object_dict_del(R_object object, R_str key) {
    R_object val = _HotPyObject_dict_get(object, key);
    if (val != NULL) {
        intptr_t offset = dict_offset(object);
        R_object o = FIELD(R_object, object, offset);
        if (TYPE(o) == type_dict_values) {
            values_del_str((R_dict_values)o, key);
        } else {
            TYPE_ASSERT(o, dict);
            dict_del_str((R_dict)o, key);
        }
    }
    return val;
}

static R_object getattr_or_error(R_object object, R_object name) {
    R_object d = class_attribute(TYPE(object), name___getattr__);
    if (d) {
        call_func call = CALLABLE(d);
        if (call) {
            R_tuple t = make_tuple2(object, name);
            return call(d, t, 0);
        }
    }
    attribute_error(TYPE(object), name);
    return 0; // Keep compiler happy.
}

get_func _HotPyObject_getter(R_object attr) {
    R_tuple mro = TYPE(attr)->__mro__;
    int i, limit = mro->length-1;
    for (i = 0; i < limit; i++) {
        R_type t = (R_type)ITEM(mro, i);
        if (t->get) {
            return t->get;
        }
    }
    return NULL;
}

R_object _HotPyObject_GenericGetAttr(R_object object, R_str name) {
    R_type klass = TYPE(object);
    R_object cls_attr = class_attribute(klass, name);
    if (cls_attr) {
        R_tuple mro = TYPE(cls_attr)->__mro__;
        int i, overriding = 0;
        get_func get = NULL;
        // Set start to len-2 to avoid checking object class.
        for (i = mro->length-2; i >= 0; i--) {
            R_type t = (R_type)ITEM(mro, i);
            if (t->get) {
                get = t->get;
            }
            overriding |= (t->set != NULL);
        }
        // Check for overriding descriptor.
        if (get && overriding) {
            return get(cls_attr, object, klass);
        }
        // Look for attribute in object dictionary
        if (klass->dict_offset) {
            R_object val = _HotPyObject_dict_get(object, name);
            if (val)
                return val;
        }
        // Check for non-overriding descriptor.
        if (get) {
            return get(cls_attr, object, klass);
        }
        // Not a descriptor, just return it.
        return cls_attr;
    } else {
        if (klass->dict_offset) {
            R_object result = _HotPyObject_dict_get(object, name);
            if (result)
                return result;
        }
    }
    return getattr_or_error(object, (R_object)name);
}

R_object PY_FUNC2(object_getattribute, R_object object, R_object key) {
    if (IS_STR(key)) {
        return _HotPyObject_GenericGetAttr(object, (R_str)key);
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                                TYPE_NAME(object));
        raise_str(type_TypeError, msg);
        return 0;
    }
}

void _HotPyObject_GenericSetAttr(R_object object, R_str name, R_object value) {
    R_type klass = TYPE(object);
    R_object cls_attr = class_attribute(klass, name);
    if (cls_attr) {
        R_tuple mro = TYPE(cls_attr)->__mro__;
        // Set limit to len-1, to avoid checking object class.
        int i, limit = mro->length-1;
        for (i = 0; i < limit; i++) {
            R_type t = (R_type)ITEM(mro, i);
            if (t->set) {
                t->set(cls_attr, object, value);
                return;
            }
        }
    }
    if (klass->dict_offset) {
        _HotPyObject_dict_set(object, name, value);
    } else {
        R_str cname = klass->__name__;
        R_str msg = py_sprintf("'%s' object has no attribute '%s'", cname, name);
        raise_str(type_AttributeError, msg);
    }
}


R_object PY_FUNC3(object_setattr, R_object object, R_object key, R_object value) {
    if (IS_STR(key)) {
        _HotPyObject_GenericSetAttr(object, (R_str)key, value);
        return (R_object)None;
    } else {
        raise_char(type_TypeError, "setattr(): attribute name must be string");
        return 0;
    }
}

R_object PY_FUNC2(object_delattr, R_object object, R_object key) {
    R_str name = (R_str)key;
    R_type klass = TYPE(object);
    R_object cls_attr;
    if (!IS_STR(key)) {
        raise_char(type_TypeError, "delattr(): attribute name must be string");
        return 0;
    }
    cls_attr = class_attribute(klass, name);
    if (cls_attr) {
        R_tuple mro = TYPE(cls_attr)->__mro__;
        // Set limit to len-1, to avoid checking object class.
        int i, limit = mro->length-1;
        for (i = 0; i < limit; i++) {
            R_type t = (R_type)ITEM(mro, i);
            if (t->set) {
                t->set(cls_attr, object, 0);
                return (R_object)None;
            }
        }
    }
    if (klass->dict_offset) {
        if (object_dict_del(object, name))
            return (R_object)None;
    }
    {
        R_str cname = klass->__name__;
        R_str msg = py_sprintf("'%s' object has no attribute '%s'", cname, name);
        raise_str(type_AttributeError, msg);
        return 0; // Keep compiler happy.
    }
}

void user_finalise_object(GVMT_Object o) {
    R_type klass = TYPE((R_object)o);
    R_object d = class_attribute(klass, name___del__);
    if (d) {
        R_BaseException ex;
        if (TYPE(d)->call) {
            R_tuple t = make_tuple1((R_object)o);
            CALLABLE(d)(d, t, 0);
        }
    } else {
        // fprintf(stderr, "__del__ not found");
    }
}

/****** Descriptors for __dict__ ***********/

R_object desc_get_dict_descriptor(R_object descriptor, R_object obj, R_type cls) {
    assert (obj != NULL);
    if (cls->dict_offset) {
        int dict_offset = cls->dict_offset;
        if (dict_offset < 0) {
            assert(cls->variable);
            dict_offset = ((R_int)obj)->length*sizeof(void*)-dict_offset;
        }
        return FIELD(R_object, obj, dict_offset);
    } else {
        R_str name = TYPE_NAME(obj);
        raise_str(type_AttributeError, py_sprintf(
            "'%s' object has no attribute '__dict__'", name));
        return NULL;
    }
}

void desc_set_dict_descriptor(R_object descriptor, R_object obj, R_object value) {
    R_object object = (R_object)obj;
    R_type cls = TYPE(object);
    int dict_offset = cls->dict_offset;
    if (dict_offset) {
        if (dict_offset < 0) {
            assert(cls->variable);
            dict_offset = ((R_int)obj)->length*sizeof(void*)-dict_offset;
        }
        if (isinstance(value, type_dict)) {
            FIELD(R_object, object, dict_offset) = value;
        } else {
            R_str msg = py_sprintf("__dict__ must be set to a dictionary, not a '%s'",
                                   TYPE_NAME(value));
            raise_str(type_TypeError, msg);
        }
    } else {
        R_str name = cls->__name__;
        raise_str(type_AttributeError, py_sprintf(
            "'%s' object has no attribute '__dict__'", name));
    }
}

intptr_t hotpy_NoneType_hash(R_object none) {
    return 0x3b598e6a;
}

/********** Repr enter/exit functions *******/

/* These functions prevent infinite recursion in repr methods
 * All container objects that may contain themselves need to
 * call these.
 */
int HotPy_ReprEnter(R_object obj) {
    R_list list = THREAD_STATE->repr_list;
    int i;
    if (list == NULL) {
        THREAD_STATE->repr_list = list = make_list(4);
    }
    i = LIST_SIZE(list);
    while (--i >= 0) {
        if (ITEM(list->array, i) == obj) return 1;
    }
    list_append(list, obj);
    return 0;
}

void HotPy_ReprLeave(R_object obj) {
    R_list list = THREAD_STATE->repr_list;
    int i;
    if (list == NULL) return;
    i = LIST_SIZE(list);
    while (--i >= 0) {
        if (ITEM(list->array, i) == obj) {
            list_pop_unchecked(list, i);
            break;
        }
    }
}

R_object PY_FUNC2(object_type, R_type cls, R_object o) {
    return (R_object)TYPE(o);
}

/**** Python visible versions ****/

R_object PY_FUNC1(_repr_enter, R_object o) {
    return HotPy_ReprEnter(o) ? (R_object)TRUE : (R_object)FALSE;
}


R_object PY_FUNC1(_repr_leave, R_object o) {
    HotPy_ReprLeave(o);
    return (R_object)None;
}


