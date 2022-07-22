#ifndef HOTPY_HASHTABLE_H
#define HOTPY_HASHTABLE_H

GVMT_OBJECT(int_vector) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uintptr_t length;  
    int32_t vector[]; 
};                    

GVMT_OBJECT(hashtable) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t size;   
    uint32_t load;   
    uint32_t mask;   
    GVMT_OBJECT(int_vector)* codes;
    GVMT_OBJECT(array)* values;    
};

R_hashtable new_empty_hashtable(void);

R_object hashtable_get(R_hashtable hash, int code);

void hashtable_set(R_hashtable hash, int code, R_object value);

#endif // HOTPY_HASHTABLE_H

