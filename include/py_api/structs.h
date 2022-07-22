#ifndef HOTPY_API_STRUCTS_H
#define HOTPY_API_STRUCTS_H

GVMT_OBJECT(binary_pred_api) {        
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    objobjproc func;    /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(binary_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    binaryfunc func;    /* pointer */
    GVMT_OBJECT(str)* name;   
};

GVMT_OBJECT(unary_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    unaryfunc func;    /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(objint_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    ssizeargfunc func;    /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(objintobj_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    ssizeobjargproc func;     /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(getset_wrapper) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    getter getter;    /* pointer */
    setter setter;    /* pointer */
    void* closure;    /* pointer */
    GVMT_OBJECT(str)* name; 
    GVMT_OBJECT(str)* __doc__;    /* read-only */
};

GVMT_OBJECT(len_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    lenfunc func;    /* pointer */
};

GVMT_OBJECT(new_api) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    newfunc func;    /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(binary_r_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    binaryfunc func;    /* pointer */
    GVMT_OBJECT(str)* name;  
};

GVMT_OBJECT(unary_pred_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    inquiry func;    /* pointer */
    GVMT_OBJECT(str)* name;
};

GVMT_OBJECT(setitem_api) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    objobjargproc func;    /* pointer */
    GVMT_OBJECT(str)* name;  
};

GVMT_OBJECT(compare_api) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t op;  
    GVMT_OBJECT(str)* name;  
    richcmpfunc func;    /* pointer */
};

GVMT_OBJECT(hash_api) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    hashfunc func;    /* pointer */
    GVMT_OBJECT(str)* name; 
};

GVMT_OBJECT(method_api) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    int32_t parameter_format;
    GVMT_OBJECT(str)* __name__;    /* read-write */
    GVMT_OBJECT(str)* __doc__;    /* read-write */
    GVMT_OBJECT(tuple)* paramNames;  /* read-only */
    GVMT_OBJECT(tuple)* defaults;    /* read-only */
    void* func;    /* pointer */
    int32_t flags; 
};






#endif
