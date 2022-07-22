#include <assert.h>
#include <stddef.h>
#include <malloc.h>
#include "hotpy.h"
#include "core_objects.h"
#include "dictionary.h"
#include "descriptors.h"
#include "attributes.h"
#include "function.h"
#include "guard.h"
#include "thread.h"

#ifdef NO_LOCKING

#define LOCK_TYPE(t) ((void)0)
#define UNLOCK_TYPE(t) ((void)0)

#else

#define LOCK_TYPE(t) if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(t), offsetof(GVMT_OBJECT(type), lock))
#define UNLOCK_TYPE(t) if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(t), offsetof(GVMT_OBJECT(type), lock))

#endif

/** Returns 1(true) if k1 is a proper sub class of k2, 0 (false) otherwise */
int is_proper_subclass_of(R_type k1, R_type k2) {
    R_tuple mro = k1->__mro__;
    int i, len = mro->length;
    for (i = 1; i < len; i++) {
        if (k2 == (R_type)ITEM(mro, i))
            return 1;
    }
    return 0;
}

/* This is a simple (& fast) isinstance test for internal use
 * It does not have the full Python syntax */
int _isinstance(R_object o, R_type t) {
    R_type c = TYPE(o);
    return is_subclass_of(c, t);
}

#ifdef BOOTSTRAP

GVMT_OBJECT(function) func_c3_mro = { 0 };

#endif

R_function c3_mro_function = &func_c3_mro;

static R_tuple build_mro(R_tuple bases, R_type new_class) {
    R_type super_type;
    R_tuple mro;
    int i, len;
    if (bases->length == 0) {
        super_type = type_object;
    } else if (bases->length > 1) {
        R_tuple t = make_tuple2((R_object)new_class, (R_object)bases);
        return (R_tuple)CALLABLE(c3_mro_function)((R_object)c3_mro_function, t, 0);
    } else {
        super_type = (R_type)ITEM(bases, 0);
    }
    mro = gc_allocate_tuple(super_type->__mro__->length + 1);
    SET_TYPE(mro, type_tuple);
    mro->length = super_type->__mro__->length + 1;
    ITEM(mro, 0) = (R_object)new_class;
    len = super_type->__mro__->length;
    for (i = 0; i < len; i++) {
        R_object o = ITEM(super_type->__mro__, i);
        ITEM(mro, i + 1) = o;
    }
    return mro;
}

/* A type has a 'special' shape if its shape (ignoring dictionary slot) differs from object */
static int special_shape(R_type t) {
    if (t->shape[1] != ZERO || t->variable)
        return 1;
    if (t->dict_offset)
        return t->shape[0] != 8;
    else
        return t->shape[0] != 1;
}

/* Returns the offset of the first inserted slot */
static int add_slots_into_shape(R_type new_class, int slots) {
    int n, i = 0;
    int offset = 0;
    for (i = 0; n = new_class->shape[i]; i++) {
        if (n > 0)
            offset += n;
        else
            offset -= n;
    }
    if (new_class->shape[i-1]*slots >= 0) {
        new_class->shape[i-1] += slots;
    } else {
        assert(i < 4);
        new_class->shape[i] = slots;
        new_class->shape[i+1] = 0;
    }
    return offset;
}

void lay_out(R_type new_class) {
    R_tuple mro = new_class->__mro__;
    R_type master = type_object;
    int i, j, needs_dict = 0;
    int variable = 0;
    // Need to check that all superclasses have compatible shape & that dict offsets match up.
    for (i = 1; i < mro->length; i++) {
        R_type s1 = (R_type)ITEM(mro, i);
        if (s1->dict_offset)
            needs_dict = 1;
        if (s1->variable)
            variable = s1->variable;
        if (special_shape(s1)) {
            if (master == type_object)
                master = s1;
            for (j = i +1 ; j < mro->length; j++) {
                R_type s2 = (R_type)ITEM(mro, j);
                if (special_shape(s2) && !is_subclass_of(s1, s2)) {
                    raise_char(type_TypeError, "bases have instance lay-out conflict");
                }
            }
        }
    }
    for (i = 0; i < 3; i++) {
        new_class->shape[i] = master->shape[i];
    }
    if (variable) {
        new_class->variable = variable;
        // Only variable classes are array, str, tuple and subclasses thereof.
        new_class->dict_offset = -add_slots_into_shape(new_class, 0) * sizeof(void*);
    } else {
        new_class->dict_offset = master->dict_offset;
    }
}

static void do_inheritance(R_type new_class, R_tuple bases) {
    int i;
    R_type super_type;
    R_tuple mro;
    R_dict_table dict;
    for (i = 0; i < bases->length; i++) {
        R_type base_class = TYPE(ITEM(bases, i));
        if (!is_subclass_of(base_class, type_type)) {
            raise_char(type_TypeError, "Super class is not a class object");
        }
        new_class->flags |= ((R_type)ITEM(bases, i))->flags;
    }
    // Do not inherit immutable flag.
    new_class->flags &= ~IMMUTABLE_FLAG;
    new_class->__bases__ = bases;
    new_class->__mro__ = mro = build_mro(bases, new_class);
    super_type = (R_type)ITEM(mro, 1);
    //new_class->__setattr__ =  super_type->__setattr__;
    //new_class->__getattribute__ = super_type->__getattribute__;
    new_class->call = NULL;
    //new_class->get = super_type->get;
    //new_class->set = super_type->set;
    lay_out(new_class);
    new_class->attributes = empty_dict_table;
    new_class->unique_index = get_next_class_id();
}

static void do_slots(R_type new_class, R_object dict) {
    R_dict names;
    R_list slots;
    int i, dict_slot = -1;
    int n, super_slots;
    // Look up __slots__:
    if (IS_DICT_EXACT(dict)) {
        names = (R_dict)dict;
        slots = (R_list)dict_get_str(names, name___slots__);
    } else {
       slots = 0;
       raise_char(type_InternalError, "TO DO - Implement type.__new__ with non-dict");
    }
    if (slots == 0) {
        // May need to add a dictionary.
        if (new_class->dict_offset == 0 && new_class->variable == 0) {
            // Need to add one slot for dictionary
            new_class->dict_offset = add_slots_into_shape(new_class, 1) * sizeof(void*);
        }
        return;
    }
    slots = HotPySequence_List((R_object)slots);
    for (i = 0; i < LIST_SIZE(slots); i++) {
        R_str s = (R_str)ITEM(slots->array, i);
        if (TYPE(s) != type_str) {
            R_str msg = py_sprintf("__slots__ items must be strings not %s", TYPE_NAME(s));
            raise_str(type_TypeError, msg);
        }
        if (string_equals(s, name___dict__)) {
            dict_slot = i;
        }
    }
    if (new_class->variable) {
        for (i = 1; i < new_class->__mro__->length; i++) {
            R_type super_type = (R_type)ITEM(new_class->__mro__, i);
            if (super_type->variable) {
                R_str msg = py_sprintf("Cannot add __slots__to %s", super_type->__name__);
                raise_str(type_TypeError, msg);
            }
        }
        assert(0 && "Cannot have a variable class with no variable superclass");
    }
    // Modify shape to allow for slots.
    super_slots = add_slots_into_shape(new_class, LIST_SIZE(slots));
    // Correct dict_offset if __dict__ in __slots__
    if (dict_slot >= 0) {
        int n, count = 0;
        if (new_class->dict_offset)
            raise_char(type_TypeError, "__dict__ present in supertype");
        new_class->dict_offset = (dict_slot + super_slots) * sizeof(void*);
    }
    // Add slot descriptors
    for (i = 0; i < LIST_SIZE(slots); i++) {
        if (i != dict_slot) {
            R_str s = (R_str)ITEM(slots->array, i);
            R_slot_descriptor d = gc_allocate(slot_descriptor);
            SET_TYPE(d, type_slot_descriptor);
            d->name = s;
            d->__doc__ = empty_string;
            d->offset = (super_slots + i) * sizeof(void*);
            d->flags = SLOT_WRITEABLE;
            new_class->attributes = str_dict_table_set(new_class->attributes, s, (R_object)d);
        }
    }
}

// TO DO - Fix this to use operator.getitem to find metaclass.
static R_object get_metaclass(R_object name_space) {
//    R_object meta;
//    if (TYPE(name_space) == type_local_scope) {
//        meta = (R_object)str_dict_table_get(((R_module)name_space)->dictionary, name___metaclass__);
//        if (meta == 0) {
//            meta = (R_object)type_type;
//        }
//    } else {
//        R_Exception ex;
//        HOTPY_TRY(ex)
//            meta = (R_object)TYPE(OPERATOR_GETITEM)->call(OPERATOR_GETITEM, 2, name___metaclass__, name_space);
//        HOTPY_EXCEPT
//            meta = (R_object)type_type;
//        HOTPY_END_TRY
//    }
//    return meta;
    return (R_object)type_type;
}

//R_type type_new(R_type mcls, R_str classname, R_tuple bases, R_object dictionary) {
//    R_type new_class = gc_allocate(type);
//    R_Exception ex;
//    R_object iter, d_next;
//    R_dict_table dict;
//    int i, len;
//    SET_TYPE(new_class, mcls);
//    GVMT_NO_GC_BLOCK_BEGIN
//        new_class->lock = create_lock();
//     GVMT_NO_GC_BLOCK_END
//    do_inheritance(new_class, bases);
//    do_slots(new_class, dictionary);
//    new_class->__name__ = classname;
//    new_class->guards = new_dict(0);
//    if (IS_DICT_EXACT(dictionary)) {
//        dict = ((R_dict)dictionary)->__table;
//    } else {
//        dict = 0;
//       raise_char(type_InternalError, "TO DO - Implement type.__new__ with non-dict");
//    }
//    len = dict->length / 2;
//    for (i = 0; i < len; i++) {
//        if (ITEM(dict, 2*i + 1) != (R_object)NULL_R) {
//            R_str name = (R_str)ITEM(dict, 2*i);
//            R_object value = ITEM(dict, 2*i + 1);
//            TYPE_CHECK(name, type_str);
//            type_setattr((R_object)new_class, name, value);
//        }
//    }
//    return new_class;
//}

R_type _HotPy_type(R_tuple t, R_dict d) {
    if (d && !is_empty(d)) {
        raise_char(type_TypeError, "type() does not take keyword parameters");
    }
    if (LEN(t) == 1) {
        return TYPE(ITEM(t, 0));
    } else if (LEN(t) == 3) {
        R_object name = ITEM(t, 0);
        R_object supers = ITEM(t, 1);
        R_object mapping = ITEM(t, 2);
        if (IS_STR(name)) {
            if (IS_TUPLE(supers)) {
                return makeClass((R_str)name, (R_tuple)supers, mapping);
            } else {
                raise_char(type_TypeError, "type name must be a string");
            }
        } else {
            raise_char(type_TypeError, "type bases must a tuple");
        }
    } else {
         raise_char(type_TypeError, "type() take 1 or 3 parameters");
    }
    return 0;
}

R_type makeClass(R_str name, R_tuple supers, R_object names) {
    int i, len;
    R_dict dict;
    R_type new_class, super_type;
    R_list slots;
    new_class = gc_allocate(type);
    SET_TYPE(new_class, (R_type)get_metaclass((R_object)names));
    // new_class->ref_count = IMMORTAL;
    new_class->lock = GVMT_LOCK_INITIALIZER;
    new_class->__name__ = name;
    // Verify that all supers are in fact classes...
    do_inheritance(new_class, supers);
    do_slots(new_class, (R_object)names);
    new_class->length = sizeof(void*) * (new_class->shape[0] - new_class->shape[1] +
                    new_class->shape[2] - new_class->shape[3] + new_class->shape[4]);
    //new_class->__setattr__ = _HotPyObject_GenericSetAttr;
    //new_class->__getattribute__ = _HotPyObject_GenericGetAttr;
    new_class->guards = new_dict(0);
    new_class->keys_cache = empty_dict_keys;
    if (IS_DICT_EXACT(names)) {
        R_dict d = (R_dict)names;
        LOCK_DICT(d);
        len = d->values->keys->length;
        for (i = 0; i < len; i++) {
            R_str name = (R_str)ITEM(d->values->keys, i);
            if (ITEM(d->values, i) != (R_object)NULL_R) {
                if (TYPE(name) != type_str) {
                    UNLOCK_DICT(d);
                    raise_char(type_TypeError, "class member names must be strings");
                }
                type_setattr((R_object)new_class, name, ITEM(d->values, i));
            }
        }
        UNLOCK_DICT(d);
    } else {
        raise_char(type_InternalError, "TO DO - Implement type.__new__ with non-dictionary");
    }
    new_class->tp_name = malloc(name->length+2);
    py_print(new_class->tp_name, name);
    return new_class;
}

/** This is a rather slow implementation */
static intptr_t call_py_hash(R_object obj) {
    R_type klass = TYPE(obj);
    R_object callable = class_attribute(klass, name___hash__);
    call_func call;
    R_int i;
    R_tuple t;
    assert(callable != NULL);
    call = CALLABLE(callable);
    t = make_tuple(1);
    ITEM(t, 0) = obj;
    i = (R_int)call(callable, t, 0);
    if (IS_INT(i)) {
         return int_from_py_int(i);
    } else {
        R_str name = TYPE_NAME(obj);
        R_str msg = py_sprintf("__hash__ returned non-int (type %s)", name);
        raise_str(type_TypeError, msg);
        return 0;
    }
}

static int is_special_name(R_str name) {
    int len = LEN(name);
    return len > 4 &&
           name->text[1] == '_' &&
           name->text[len-1] == '_' &&
           name->text[0] == '_' &&
           name->text[len-2] == '_';
}

void type_setattr(R_object obj, R_str name, R_object value) {
    R_type class = (R_type)obj;
    TYPE_ASSERT(class, type);
    TYPE_ASSERT(name, str);
    if (IMMUTABLE_TYPE(class)) {
        R_str msg = py_sprintf("can't set attributes of built-in/extension type '%s'",
                           class->__name__);
        raise_str(type_TypeError, msg);
    }
    // Trigger any guards, updating them.
    if (!is_empty(class->guards)) {
        R_guard g = (R_guard)dict_get_str(class->guards, name);
        if (g) {
            trigger_guard(g);
            dict_set_str(class->guards, name, 0);
        }
    }
    if (is_special_name(name)) {
        if (string_equals(name, name___call__) ||
            string_equals(name, name___class__)) {
            // These should be settable,
            // __call__ pretty much always; __class__ if shapes & dict_offset match.
            raise_char(type_TypeError, "Read only attribute");
        } else if (string_equals(name, name___setattr__)) {
            fatal("Cannot handle __setattr__ yet");
        } else if (string_equals(name, name___getattribute__)) {
            LOCK_TYPE(class);
            if (value == 0) {
                class->__getattribute__ = 0;
            } else if (TYPE(value) == type_builtin_function &&
                        ((R_builtin_function)value)->executable ==
                        (behaviour)PY_FUNC(object_getattribute)) {
                class->__getattribute__ = _HotPyObject_GenericGetAttr;
            } else {
                class->__getattribute__ = override_getattribute;
                class->override_getattr = value;
            }
        } else if (string_equals(name, name___del__)) {
            LOCK_TYPE(class);
            class->flags |= FINALIZE_FLAG;
        } else if (string_equals(name, name___hash__)) {
            LOCK_TYPE(class);
            if (value == (R_object)None) {
                class->hash = HotPyObject_HashNotImplemented;
            } else {
                class->hash = call_py_hash;
            }
        } else {
            LOCK_TYPE(class);
        }
    }
    if (value == 0) {
        str_dict_table_delete(class->attributes, name);
    } else {
        class->attributes = str_dict_table_set(class->attributes, name, value);
    }
    UNLOCK_TYPE(class);
}

R_object PY_FUNC3(type_setattr, R_object object, R_object key, R_object value) {
    if (!isinstance(object, type_type)) {
        char* f = "descriptor '__setattr__' requires a 'type' object but received a '%s'";
        R_str msg = py_sprintf(f, TYPE_NAME(object));
        raise_str(type_TypeError, msg);
    }
    if (IS_STR(key)) {
        type_setattr(object, (R_str)key, value);
        return (R_object)None;
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                                TYPE_NAME(object));
        raise_str(type_TypeError, msg);
        return 0;
    }
}

R_object PY_FUNC2(type_delattr, R_object object, R_object key) {
    if (!isinstance(object, type_type)) {
        char* f = "descriptor '__delattr__' requires a 'type' object but received a '%s'";
        R_str msg = py_sprintf(f, TYPE_NAME(object));
        raise_str(type_TypeError, msg);
    }
    if (IS_STR(key)) {
        type_setattr(object, (R_str)key, NULL);
        return (R_object)None;
    } else {
        R_str msg = py_sprintf("attribute name must be string, not '%s'",
                                TYPE_NAME(object));
        raise_str(type_TypeError, msg);
        return 0;
    }
}

GETATTRIBUTE_FUNCTION(override_getattribute) {
    R_type klass = TYPE(object);
    R_object f = klass->override_getattr;
    R_tuple t;
    int i;
    for (i = 1; f == NULL; i++) {
        f = ((R_type)ITEM(klass->__mro__, i))->override_getattr;
    }
    t = make_tuple(2);
    ITEM(t, 0) = object;
    ITEM(t, 1) = (R_object)name;
    return CALLABLE(f)((R_object)f, t, 0);
}

R_object PY_FUNC2(isinstance, R_object o, R_type t) {
    return (R_object)booleans[isinstance(o, t)];
}

R_object _HotPyObject_New(R_type cls) {
    intptr_t i, j, n, offset, size;
    R_object obj;
    TYPE_ASSERT(cls, type);
    if (cls->variable)
        fatal("Cannot create objects of varying size");
    assert(cls->shape[0] >= 1);
    assert(cls->shape[1] <= 0);
    assert(cls->shape[2] >= 0);
    assert(cls->shape[3] <= 0);
    assert(cls->shape[4] == 0);
    assert(cls->length == sizeof(void*) * (cls->shape[0] - cls->shape[1] + cls->shape[2] - cls->shape[3]));
    obj = (R_object)gvmt_malloc(cls->length);
    SET_TYPE(obj, cls);
    //assert(cls->shape[0] == 1);
    //offset = 1;
    //assert(cls->shape[1] < 0);
    //for (j = 0, n = -cls->shape[1]; j < n; j++) {
    //    ((intptr_t*)obj)[offset+j] = 0;
    //}
    //offset += n;
    //assert(cls->shape[2] >= 0);
    //n = cls->shape[2];
    //if (n > 0) {
    //    for (j = 0; j < n; j++) {
    //        ((R_NoneType*)obj)[offset+j] = None;
    //    }
    //    offset += n;
    //    assert(cls->shape[3] <= 0);
    //    n = -cls->shape[3];
    //    for (j = 0; j < n; j++) {
    //        ((intptr_t*)obj)[offset+j] = 0;
    //    }
    //}
    //gvmt_fully_initialized((GVMT_Object)obj);
    if (cls->dict_offset) {
        assert(cls->dict_offset > 0);
        assert(cls->keys_cache);
        FIELD(R_dict_values, obj, cls->dict_offset) =
                new_dict_values(cls->keys_cache);
    }
    if (cls->flags & FINALIZE_FLAG)
        gvmt_gc_finalizable((GVMT_Object)obj);
    return obj;
}


/********* Descriptors **********/

DESC_CALL_SIGNATURE(desc_call_type) {
    R_type cls = (R_type)callable;
    R_object new_func;
    R_object result;
    TYPE_ASSERT(callable, type);
    new_func = class_attribute(cls, name___new__);
    if (new_func == (R_object)object_new) {
        result = _HotPyObject_New(cls);
    } else {
        R_tuple c_t = tuple_prepend((R_object)cls, tuple);
        result = CALLABLE(new_func)(new_func, c_t, dict);
    }
    if (TYPE(result) == cls) {
        R_object init_func = class_attribute(cls, name___init__);
        call_func call = CALLABLE(init_func);
        R_tuple r_t = tuple_prepend((R_object)result, tuple);
        call(init_func, r_t, dict);
    }
    return result;
}

R_object class_attribute(R_type k, R_str name) {
    R_tuple mro = k->__mro__;
    int i, len = mro->length;
    for (i = 0; i < len; i++) {
        R_type cls = (R_type)ITEM(mro, i);
        R_object d = str_dict_table_get(cls->attributes, name);
        if (d) {
            return d;
        }
    }
    return 0;
}

R_object _special_fallback(R_object obj, R_str name) {
    // Use a dictionary.
    // E.g. name == '__len__' -> "object of type '%s' has no len()"
    // To do.
    raise_str(type_AttributeError, py_sprintf(
            "'%s' object has no attribute '%s'", TYPE_NAME(obj), name));
    return 0;
}

R_object _HotPyObject_LookupSpecial(R_object obj, R_str name) {
    R_object attr;
    R_tuple mro;
    int i;
    R_type t;
    attr = class_attribute(TYPE(obj), name);
    if (attr) {
        // attr will be a function most of the time
        if (IS_FUNCTION(attr)) {
            return _HotPy_bind_method(obj, attr);
        } else {
            R_tuple mro = TYPE(attr)->__mro__;
            // Check for descriptor.
            for (i = 0; ; i++) {
                t = (R_type)ITEM(mro, i);
                if (t == type_object)
                    break;
                if (t->get) {
                    return t->get(attr, obj, TYPE(obj));
                }
            }
            return attr;
        }
    } else {
        return _special_fallback(obj, name);
    }
}

R_object type_getattribute(R_object type, R_str name) {
    R_type t, cls = (R_type)type;
    R_type meta_cls = TYPE(cls);
    R_object meta_descriptor, result;
    R_str msg;
    TYPE_ASSERT(name, str);
    TYPE_ASSERT(cls, type);
    meta_descriptor = class_attribute(meta_cls, name);
    if (meta_descriptor) {
        R_tuple mro = TYPE(meta_descriptor)->__mro__;
        int i;
        // Check for overriding descriptor.
        for (i = 0; ; i++) {
            t = (R_type)ITEM(mro, i);
            if (t == type_object)
                break;
            if (t->set) {
                assert("If type has set it must have get" && t->get);
                return t->get(meta_descriptor, (R_object)cls, meta_cls);
            }
        }
    }
    result = class_attribute(cls, name);
    if (result) {
        // Check for descriptor.
        R_tuple mro = TYPE(result)->__mro__;
        int i;
        for (i = 0; ; i++) {
            t = (R_type)ITEM(mro, i);
            if (t == type_object)
                break;
            if (t->get)
                return t->get(result, NULL, cls);
        }
        return result;
    }
    if (meta_descriptor) {
        R_tuple mro = TYPE(meta_descriptor)->__mro__;
        int i;
        // Check for non-overriding descriptor.
        for (i = 0; ; i++) {
            t = (R_type)ITEM(mro, i);
            if (t == type_object)
                break;
            if (t->get) {
                assert("If type has set it must have get" && t->get);
                return t->get(meta_descriptor, (R_object)cls, meta_cls);
            }
        }
    }
    msg = py_sprintf("type '%s' has no attribute '%s'", cls->__name__, name);
    raise_str(type_AttributeError, msg);
    return 0;
}

R_object PY_FUNC2(type_getattribute, R_object object, R_object key) {
    if (!isinstance(object, type_type)) {
        char* fmt =
            "descriptor '__getattribute__' requires a 'type' object but received a '%s'";
        raise_str(type_TypeError, py_sprintf(fmt, TYPE_NAME(object)));
    }
    if (IS_STR(key)) {
        return type_getattribute(object, (R_str)key);
    } else {
        char* fmt = "attribute name must be string, not '%s'";
        raise_str(type_TypeError, py_sprintf(fmt, TYPE_NAME(object)));
        return 0;
    }
}

R_object PY_FUNC1(type_str, R_type cls) {
    TYPE_ASSERT(cls, type);
    return (R_object)py_sprintf("<class '%s'>", cls->__name__);
}

R_object PY_FUNC1(dict_proxy_iter, R_dict_proxy d) {
    R_dict_proxy_iterator it;
    TYPE_ASSERT(d, dict_proxy);
    it = gc_allocate(dict_proxy_iterator);
    SET_TYPE(it, type_dict_proxy_iterator);
    it->table = d->type->attributes;
    it->next = 0;
    return (R_object)it;
}

R_object PY_FUNC1(dict_proxy_iter_next, R_dict_proxy_iterator it) {
    TYPE_ASSERT(it, dict_proxy_iterator);
    while (it->next < it->table->length) {
        int this = it->next;
        it->next += 2;
        if (ITEM(it->table, this) != (R_object)NULL_R)  {
            R_object result = ITEM(it->table, this);
            assert(isinstance(result, type_str));
            return result;
        }
    }
    stop_iteration();
    return 0;
}

R_str type_slot_name(R_type t, int offset) {
    R_dict_table table = t->attributes;
    uint32_t i, len = table->length >> 1;
    for (i = 0; i < len; i++) {
        R_object attr = table->array[i*2+1];
        if (TYPE(attr) == type_slot_descriptor &&
            ((R_slot_descriptor)attr)->offset == offset) {
            return ((R_slot_descriptor)attr)->name;
        }
    }
    return NULL;
}

int add_guard_for_type_attribute(R_type t, R_str name, R_trace trace,
                                  R_object expected) {
    R_guard g;
    R_tuple mro = t->__mro__;
    int i, limit = mro->length;
    for (i = 0; i < limit; i++) {
        R_type t = (R_type)ITEM(mro, i);
        R_object attr;
        if (IMMUTABLE_TYPE(t)) {
            attr = str_dict_table_get(t->attributes, name);
            if (attr)
                return attr == expected;
        } else {
            LOCK_TYPE(t);
            attr = str_dict_table_get(t->attributes, name);
            if (attr && attr != expected) {
                UNLOCK_TYPE(t);
                return 0;
            }
            add_attribute_guard(t, name, trace);
            UNLOCK_TYPE(t);
            if (attr) {
                assert(attr == expected);
                return 1;
            }
        }
    }
    assert(expected == NULL);
    return 1;
}

