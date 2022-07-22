#ifndef HOTPY_STROBJECT_H
#define HOTPY_STROBJECT_H

GVMT_OBJECT(str) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uintptr_t length;  
    uint32_t hashcode;
    uint16_t text[];  
};

GVMT_OBJECT(str_iterator) {
    GVMT_OBJECT(type)* ob_type;  /* read-only */
    uintptr_t ref_count;
    uint32_t next;  
    GVMT_OBJECT(str)* str;
};

/** This is semantically equivalent to string_equals_function, 
 *  but will be much faster for canonical strs where s1 == s2 
 *  or any string where s1 != s2 (since s1->hashcode != s2->hashcode with a very high
 *  probability. */            
#define string_equals(x,y) ((x)==(y) ? 1 : \
        (((x)->hashcode==(y)->hashcode) ? string_equals_function(x, y) : 0))

int string_compare(R_str s1, R_str s2);

R_str string_format_tuple(R_str str, R_tuple tuple);

R_str single_character(int code);

int string_equals_function(R_str s1, R_str s2);

R_str string_concat(R_str s1, R_str s2);

R_str substring(R_str s, int start, int end);

R_str string_from_c_string(const char* c_string);

R_str string_from_chars(Py_UNICODE* chars, int count);

R_str string_from_utf8_bytes(int count, unsigned char* data);

R_str decode_utf8_bytes(R_bytes b);

R_str canonical_string(R_str str);

R_str py_sprintf(const char* format, ...);

int py_print(char* buffer, R_str str);

int py_fprint(FILE* file, R_str str);

R_str hotpy_str_from_char_array(R_char_array chars, size_t len);

#define gc_allocate_str(n) \
((R_str)gvmt_malloc(offsetof(GVMT_OBJECT(str), text[0]) + (n)*sizeof(Py_UNICODE)))

R_str_buf make_str_buf(void);
void str_buf_append_str(R_str_buf buf, R_str s);
void str_buf_append_chars(R_str_buf buf, Py_UNICODE* chars, size_t count);

R_str str_buf_to_str(R_str_buf buf);

#endif
