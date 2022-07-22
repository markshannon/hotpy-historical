#ifndef HOTPY_BYTESOBJECT_H
#define HOTPY_BYTESOBJECT_H

GVMT_OBJECT(bytes) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uintptr_t length;   
    uint8_t bytes[];   
};


R_bytes make_bytes(size_t size);

#endif // HOTPY_BYTESOBJECT_H

