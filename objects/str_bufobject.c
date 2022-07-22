#include "hotpy.h"

/** String buffer */

void resize_array(R_str_buf buf, int min_size) {
    R_char_array new_array, old_array = buf->array;
    int i, old_size = old_array->length;
    int new_size = old_size << 1;
    while (new_size < min_size)
        new_size <<= 1;
    new_array = gc_allocate_var(char_array, chars, new_size);
    SET_TYPE(new_array, type_char_array);
    ZERO_REF_COUNT(new_array);
    new_array->length = new_size;
    old_array = buf->array;
    for (i = 0; i < old_size; i++) {
        Py_UNICODE c = old_array->chars[i];
        new_array->chars[i] = c;  
    }
    gvmt_fully_initialized((GVMT_Object)new_array);
    buf->array = new_array;
}

void str_buf_append_str(R_str_buf buf, R_str s) {
    int i, size = buf->size;
    int slen = s->length;
    TYPE_ASSERT(buf, str_buf);
    TYPE_ASSERT(s, str);
    if (size + slen > buf->array->length) {
        resize_array(buf, size + slen); 
    }
    for(i = 0; i < slen; i++) {
        Py_UNICODE c = s->text[i];
        buf->array->chars[i+size] = c;
    }
    buf->size += slen;
}

R_object PY_FUNC2(str_buf_append, R_str_buf buf, R_str s) {
    str_buf_append_str(buf, s);
    return (R_object)None;
}

void str_buf_append_chars(R_str_buf buf, Py_UNICODE* chars, size_t count) {
    int i, size = buf->size;
    TYPE_ASSERT(buf, str_buf);
    if (size + count > buf->array->length) {
        resize_array(buf, size + count); 
    }
    for(i = 0; i < count; i++) {
        buf->array->chars[i+size] = chars[i];
    }
    buf->size += count;
}

R_str str_buf_to_str(R_str_buf buf) {
    return hotpy_str_from_char_array(buf->array, buf->size);
}

R_object PY_FUNC1(str_buf___str__, R_str_buf buf) {
    return (R_object)hotpy_str_from_char_array(buf->array, buf->size);
}

R_object PY_FUNC1(str_buf___len__, R_str_buf buf) {
    return (R_object)R_int_from_ssize_t(buf->size);
}

R_object PY_FUNC1(str_buf_init, R_str_buf buf) {
    R_char_array array = gc_allocate_var(char_array, chars, 16);
    SET_TYPE(array, type_char_array);
    ZERO_REF_COUNT(array);
    array->length = 16;
    gvmt_fully_initialized((GVMT_Object)array);
    buf->size = 0;
    buf->array = array;
    return (R_object)None;
}

R_str_buf make_str_buf(void) {
    R_str_buf buf = gc_allocate(str_buf);
    SET_TYPE(buf, type_str_buf);
    PY_str_buf_init(buf);
    return buf;
}

