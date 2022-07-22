#ifndef HOTPY_RANGEOBJECT_H
#define HOTPY_RANGEOBJECT_H

GVMT_OBJECT(range) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(int)* _begin;    /* read-only */
    GVMT_OBJECT(int)* _end;    /*  read-only */
    GVMT_OBJECT(int)* _step;    /*  read-only */
};

GVMT_OBJECT(range_iterator) {
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(int)* next;    
    GVMT_OBJECT(int)* end;    
    GVMT_OBJECT(int)* step;    
};

GVMT_OBJECT(int_iterator) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    GVMT_OBJECT(int)* next;   
    GVMT_OBJECT(int)* end;    
};


#endif
