#ifndef HOTPY_TYPEOBJECT_H
#define HOTPY_TYPEOBJECT_H


/************** Object layout ****************
 * The first field of any object is a reference to its class.
 * Instances of fixed size classes can contain any mixture of reference and
 * non-reference fields, but must be described by the shape vector in the class.
 * Instances of variable sized classes, have fixed first part followed by a
 * variable length vector, of one type (ref or non-ref).
 * Subclasses of variably sized classes, have their dictionary and lock
 * immediately following the vector.
 ****************/

GVMT_OBJECT(type) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t flags;
    char* tp_name;    /* pointer. For API compatibility (handy for debugging) */
    int32_t shape[5];
    int32_t variable;
    int32_t length;
    int32_t dict_offset;
    uint32_t unique_index;
    gvmt_write_func marshaller;    /* pointer */
    call_func call;  /* pointer */
    get_func get;    /* pointer */
    set_func set;    /* pointer */
    hash_func hash;    /* pointer */
    int32_t lock;
    getattribute_function __getattribute__;    /* pointer */
    setattr_function __setattr__; /* pointer */
    getattrofunc api_getattro;    /* pointer */
    setattrofunc api_setattro;    /* pointer */
    ternaryfunc api_call;    /* pointer */
    GVMT_OBJECT(dict_proxy)* __dict__;    /* read-only */
    GVMT_OBJECT(dict_table)* attributes;
    GVMT_OBJECT(str)* __name__;    /* read-only */
    GVMT_OBJECT(tuple)* __bases__;    /* read-only */
    GVMT_OBJECT(tuple)* __mro__;    /* read-only */
    GVMT_OBJECT(dict)* guards;
    GVMT_OBJECT(object)* override_getattr;
    GVMT_OBJECT(dict_keys)* keys_cache;
    GVMT_OBJECT(function)* surrogate_new;
};

GVMT_OBJECT(dict_proxy) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(type)* type;
};

GVMT_OBJECT(dict_proxy_iterator) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t next;
    GVMT_OBJECT(dict_table)* table;
};



#ifndef NDEBUG
#define TYPE_ASSERT(obj, cls) assert(is_subclass_of(TYPE(obj), type_##cls))
#else
#define TYPE_ASSERT(obj, cls)
#endif

#define TYPE_CHECK(obj, cls) {R_type t_1 = TYPE(obj), t_2 = (cls); \
        if(!(is_subclass_of(t_1, t_2))) raise_type_error(t_1, t_2); }

#define is_subclass_of(k1, k2) (((k1)==(k2))||is_proper_subclass_of((k1),(k2)))

int is_proper_subclass_of(R_type k1, R_type k2);

int _isinstance(R_object o, R_type t);
#define isinstance(o, t) _isinstance((R_object)o, t)

/** Returns the named descriptor, from class k.
 * If no descriptor returns NULL.
 */
R_object class_attribute(R_type k, R_str name);

R_type makeClass(R_str name, R_tuple supers, R_object dict);

R_type _HotPy_type(R_tuple t, R_dict d);

R_object _HotPyObject_New(R_type cls);

R_object _HotPyObject_LookupSpecial(R_object obj, R_str name);

get_func _HotPyObject_getter(R_object attr);

GETATTRIBUTE_FUNCTION(type_getattribute);

SETATTR_FUNCTION(type_setattr);

void lay_out(R_type new_class);

/** Defined by bootstrap VM */
extern int global_class_count;

R_str type_slot_name(R_type type, int offset);

int get_next_class_id(void);

#define FINALIZE_FLAG 1

/* These flags are used to determine if a type is a subclass. */
#define INT_SUBCLASS_FLAG		(1<<8)
#define LIST_SUBCLASS_FLAG	(1<<9)
#define TUPLE_SUBCLASS_FLAG	(1<<10)
#define BYTES_SUBCLASS_FLAG	(1<<11)
#define STR_SUBCLASS_FLAG	(1<<12)
#define DICT_SUBCLASS_FLAG	(1<<13)
#define BASE_EXC_SUBCLASS_FLAG	(1<<14)
#define TYPE_SUBCLASS_FLAG	(1<<15)

/* Is abstract */
#define ABSTRACT_FLAG (1<<16)

/* Doesn't allow subclassing. */
#define NO_SUBCLASS_FLAG (1<<17)

/* Is not modifiable, most built-in types. */
#define IMMUTABLE_FLAG (1<<18)

#define IMMUTABLE_TYPE(t) ((t)->flags & IMMUTABLE_FLAG)

#endif
