#ifndef HOTPY_BYTEARRAYOBJECT_H
#define HOTPY_BYTEARRAYOBJECT_H

GVMT_OBJECT(bytearray) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uintptr_t length;    
    uint8_t bytes[];  
};

R_bytearray make_bytearray(size_t size);

R_bytelist make_bytelist(size_t capacity);

void bytelist_append(R_bytelist list, uint8_t byte);
void bytelist_append_nolock(R_bytelist list, uint8_t byte);
void bytelist_append_block_nolock(R_bytelist list, uint8_t *bytes, size_t count);

void bytelist_append_long(R_bytelist list, int32_t value);

void bytelist_append_short(R_bytelist list, int16_t value);

int bytelist_pop(R_bytelist list);

void bytelist_clear(R_bytelist list);

#endif // HOTPY_BYTEARRAYOBJECT_H

