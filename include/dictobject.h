#ifndef HOTPY_DICTOBJECT_H
#define HOTPY_DICTOBJECT_H


/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(dict_values) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    intptr_t size;  /* Put size here to ensure length is correct offset */
    uintptr_t length;
    GVMT_OBJECT(dict_keys)* keys; 
    GVMT_OBJECT(object)* array[]; 
};

GVMT_OBJECT(dict_values_iterator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t next; 
    int32_t size; 
    GVMT_OBJECT(dict)* dict;  
    GVMT_OBJECT(dict_values)* values; 
};

/* Not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(dict_keys) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    intptr_t load;
    uintptr_t length;
    int32_t used;   
    GVMT_OBJECT(object)* array[];  
};

GVMT_OBJECT(dict) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t lock;  
    GVMT_OBJECT(dict_values)* values;   
};

GVMT_OBJECT(dict_iterator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t next;   
    GVMT_OBJECT(dict_keys)* keys;
};

// Although this has extra1/2 before the array, in fact the values
// are laid out as per dict_values with the guards at the end.
/* Like dict_values, not visible at Python level, so no need for a ref-count */
GVMT_OBJECT(module_dict_values) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    intptr_t size;
    uintptr_t length;           
    GVMT_OBJECT(dict_keys)* keys;
    GVMT_OBJECT(object)* extra1; 
    GVMT_OBJECT(object)* extra2; 
    GVMT_OBJECT(object)* array[];
};                     

#define is_immutable_keys(k) ((k)->load == (k)->used)
                                    
R_dict new_dict(int capacity);
                    
R_dict new_module_dict(int capacity);

R_object PY_FUNC3(new_dict, R_type cls, R_object other, R_object d);

R_object PY_FUNC1(dict_clear, R_dict d);

R_object PY_FUNC1(dict_values, R_dict dict);

R_object PY_FUNC2(getitem_dict, R_dict d, R_object key);

R_object PY_FUNC3(setitem_dict, R_dict d, R_object key, R_object value);

R_object PY_FUNC3(setitem_dict_probable_str, R_dict d, R_object key, R_object value);

R_object PY_FUNC2(dict_contains, R_dict d, R_object key);

R_object PY_FUNC2(delitem_dict_object, R_dict d, R_object key);

R_dict dict_copy(R_dict d);

R_dict new_empty_dict(void);

R_object dict_remove_named_item(R_dict d, R_str key);

R_dict new_dict_with_keys(R_dict_keys keys);

#ifdef NO_LOCKING

#define LOCK_DICT(d) ((void)0)
#define UNLOCK_DICT(d) ((void)0)

#else

#define LOCK_DICT(d) if (hotpy_multi_threaded) gvmt_lock_internal((GVMT_Object)(d), offsetof(GVMT_OBJECT(dict), lock))
#define UNLOCK_DICT(d) if (hotpy_multi_threaded) gvmt_unlock_internal((GVMT_Object)(d), offsetof(GVMT_OBJECT(dict), lock))

#endif

#ifdef READ_LOCKING
                
#define READ_DICT_LOCK(l) LOCK_DICT(l)
#define READ_DICT_UNLOCK(l) UNLOCK_DICT(l)

#else

#define READ_DICT_LOCK(l) ((void)0)
#define READ_DICT_UNLOCK(l) ((void)0)

#endif

R_dict_keys dict_keys_copy(R_dict_keys k);
R_dict_values dict_values_copy(R_dict_values old, R_dict_keys k);

#endif
