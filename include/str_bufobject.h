#ifndef HOTPY_STR_BUF_OBJECT_H
#define HOTPY_STR_BUF_OBJECT_H
      
//To do 
// Rename str_buf to StringIO & implement
// full range of methods..

GVMT_OBJECT(char_array) {
    GVMT_OBJECT(type)* ob_type;    /*  read-only */
    uintptr_t ref_count;
    uintptr_t length;  
    uint8_t chars[];  
};                     

GVMT_OBJECT(str_buf) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    int32_t size;                      
    GVMT_OBJECT(char_array)* array;    
};

#endif
