#include "hotpy.h"
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <malloc.h>
#include "attributes.h"

/** String object and related code */

extern GVMT_OBJECT(type) _str;

GVMT_OBJECT(str) empty_string_object = {
    &_str,
    IMMORTAL,
    0,
    0
};

int string_equals_function(R_str s1, R_str s2) {
    if (s1->length != s2->length) {
        return 0;
    } else {
        int i;
        for(i = 0; i < s1->length; i++) {
            if (s1->text[i] != s2->text[i])
                return 0;
        }
        return 1;
    }
}

int string_compare(R_str s1, R_str s2) {
    int i, len1, len2, len_cmp;
    len1 = s1->length;
    len2 = s2->length;
    for (i = 0; i < len1 && i < len2; i++) {
        Py_UNICODE c1 = s1->text[i];
        Py_UNICODE c2 = s2->text[i];
        if(c1 != c2) {
            return (c1 < c2) ? -1 : 1;
        }
    }
    if (len1 < len2)
        return -1;
    return len1 != len2;
}

/** These two functions are wrong. Need to decode stuff first
  *  Should rename them to fprint_ascii or similar. */
int py_fprint(FILE* file, R_str s) {
    char *buf;
    int err;
    if (LEN(s) > 200)
        buf = malloc(LEN(s)+1);
    else
        buf = alloca(LEN(s)+1);
    py_print(buf, s);
    err = fwrite(buf, 1, s->length, file);
    if (LEN(s) > 200)
        free(buf);
    // Should raise exception on error.
    return err;
}

int py_print(char* buf, R_str s) {
    int i, l = s->length;
    for (i = 0; i < l; i++)
        buf[i] = s->text[i];
    buf[l] = 0;
    return l;
}

R_object PY_FUNC2(getitem_str, R_str string, R_object o) {
    intptr_t i;
    TYPE_ASSERT(string, str);
    if (IS_INT(o)) {
        i = sequence_index((R_int)o, string->length);
    } else if (IS_SLICE(o)) {
        R_slice s = (R_slice)o;
        int start, stop, step, len;
        len = decode_slice(s, string->length, &start, &stop, &step);
        if (step == 1) {
            return (R_object) substring(string, start, stop);
        } else {
            assert(0 && "To do");
            return (R_object)NotImplemented;
        }
    } else {
        i = sequence_index(HotPyNumber_Index(o), string->length);
    }
    return (R_object)single_character(string->text[i]);
}

R_object PY_FUNC1(print_string, R_str str) {
    py_fprint(stdout, str);
    return (R_object)None;
}

intptr_t hotpy_str_hash(R_str str) {
    TYPE_ASSERT(str, str);
    return str->hashcode;
}

R_object PY_FUNC1(__hash___str, R_str str) {
    TYPE_ASSERT(str, str);
    return (R_object)R_int_from_ssize_t(str->hashcode);
}

static void string_hash(R_str string) {
    int i, hash = 0;
    int len = string->length;
    for (i = 0; i < len; i++) {
        hash = ((hash << 5) | ((hash >> 27) & 0x1f)) + (uint8_t)string->text[i];
    }
    string->hashcode = hash;
}

int print_i(R_str s, int offset, int value, int width) {
    char buf[16];
    char fmt[8];
    int i, len;
    /* TO DO - make this less inefficient */
    sprintf(fmt, "%%%dd", width);
    len = sprintf(buf, fmt, value);
    for (i = 0; i < len; i++)
        s->text[offset + i] = buf[i];
    return len;
}

int py_vstr_len(const char* format, GVMT_Value* ap) {
    int len = 0;
    const char* f = format;
    char buf[16];
    while(*f) {
        if (*f == '%') {
            if (f[1] >= '0' && f[1] <= '9')
                fatal("TO DO - widths");
            if(f[1] == 's') {
                R_str s = (R_str)gvmt_stack_read_object(ap++);
                TYPE_ASSERT(s, str);
                len += s->length;
            } else if (f[1] == 'i') {
                int val;
                R_int i = (R_int)gvmt_stack_read_object(ap++);
                val = int_from_py_int(i);
                TYPE_ASSERT(i, int);
                len += sprintf(buf, "%d", val);
            } else if (f[1] == '%') {
                len ++;
            } else {
                fatal("Unknown format char: %c\n", f[1]);
            }
            f += 2;
        } else {
            f++;
            len++;
        }
    }
    return len;
}

R_str py_sprintf(const char* format, ...) {
	GVMT_Value *ap;
    int i = 0, len;
    R_str out;
    const char *f;
    ap = gvmt_stack_top();
    len = py_vstr_len(format, ap);
	va_end(ap);
    out = gc_allocate_str(len);
    SET_TYPE(out, type_str);
    out->length = len;
    f = format;
    while(*f && i < len) {
        if (*f == '%') {
            int width = 0;
            while(f[1] >= '0' && f[1] <= '9') {
                width = width * 10 + (f[1] - '0');
                ++f;
            }
            if (width > len - i)
                width = len - i;
            if(f[1] == 's') {
                int j, length;
                R_str s = (R_str)gvmt_stack_read_object(ap++);
                TYPE_ASSERT(s, str);
                length = s->length;
                if (width) {
                    length = s->length > width ? width : s->length;
                    for (i = length; i < width; i++) {
                        s->text[i++] = ' ';
                    }
                }
                for (j = 0; j < length; j++)
                    out->text[i+j] = s->text[j];
                i += length;
            } else if (f[1] == 'i') {
                R_int val = (R_int)gvmt_stack_read_object(ap++);
                TYPE_ASSERT(val, int);
                i += print_i(out, i, int_from_py_int(val), width);
            } else if (f[1] == '%') {
                out->text[i] = '%';
                i++;
            } else {
                fatal("Unknown format char: %c\n", f[1]);
            }
            f += 2;
        } else {
            out->text[i++] = *f++;
        }
    }
    string_hash(out);
    return out;
}

/** Returns number of bytes required for string s
    converted into bytes using specified encoding */
int string_encoded_length(R_str s, GVMT_Object encoding);

R_str string_from_utf8_bytes(int count, unsigned char* data) {
    // Count is number of characters, not number of bytes.
    R_str result = gc_allocate_str(count);
    int i;
    SET_TYPE(result, type_str);
    result->length = count;
    for (i = 0; i < count; i++) {
        if (data[0] < 0x80) {
            result->text[i] = data[0];
            data++;
        } else if (data[0] < 0xE0) {
            if (data[0] < 0xC2 || (data[1] & 0xC0) != 0x80)
                raise_str(type_TypeError, py_sprintf("Illegal bytes: %d %d", data[0], data[1]));
            result->text[i] = ((data[0] & 0x1f) << 6) | (data[1] & 0x3f);
            data += 2;
        } else if (data[0] < 0xF0) {
            if ((data[1] & 0xC0) != 0x80 || (data[2] & 0xC0) != 0x80)
                raise_str(type_TypeError, py_sprintf("Illegal bytes: %d %d %d", data[0], data[1], data[2]));
            result->text[i] = ((data[0] & 0xf) << 12) | ((data[1] & 0x3f) << 6) | (data[2] & 0x3f);
            data += 3;
        } else {
            raise_str(type_TypeError, py_sprintf("Illegal byte: %d", data[0]));
        }
    }
    string_hash(result);
    return result;
}

R_str string_from_utf8_string(unsigned char* chars, int length) {
    R_str result = gc_allocate_str(length);
    int i;
    unsigned char* data = chars;
    unsigned char* end = chars+length;
    SET_TYPE(result, type_str);
    result->length = length;
    for (i = 0; data < end; i++) {
        if (data[0] < 0x80) {
            result->text[i] = data[0];
            data++;
        } else if (data[0] < 0xE0) {
            if (data[0] < 0xC2 || (data[1] & 0xC0) != 0x80)
                raise_str(type_TypeError, py_sprintf("Illegal bytes: %d %d", data[0], data[1]));
            result->text[i] = ((data[0] & 0x1f) << 6) | (data[1] & 0x3f);
            data += 2;
        } else if (data[0] < 0xF0) {
            if ((data[1] & 0xC0) != 0x80 || (data[2] & 0xC0) != 0x80)
                raise_str(type_TypeError, py_sprintf("Illegal bytes: %d %d %d", data[0], data[1], data[2]));
            result->text[i] = ((data[0] & 0xf) << 12) | ((data[1] & 0x3f) << 6) | (data[2] & 0x3f);
            data += 3;
        } else {
            raise_str(type_TypeError, py_sprintf("Illegal byte: %d", data[0]));
        }
    }
    if (data > end)
        raise_str(type_TypeError, py_sprintf("Partial character at end of text"));
    result->length = i;
    // GC will recycle end of allocated area
    string_hash(result);
    return result;
}

R_str decode_utf8_bytes(R_bytes b) {
    uint8_t* buffer;
    void* ptr;
    ptr = gvmt_pin((GVMT_Object)b);
    buffer = ((uint8_t*)ptr) + offsetof(GVMT_OBJECT(bytearray), bytes);
    return string_from_utf8_string(buffer, b->length);
}

R_str format_as_int(R_object obj) {
    R_int i;
    if (IS_INT(obj)) {
        i = (R_int)obj;
    } else {
        i = (R_int)vm_call_special(obj, name___int__);
        if (i == 0 || !IS_INT(i)) {
            R_str msg = py_sprintf("%%d format: a number is required, not %s",
                                   TYPE_NAME(obj));
            raise_str(type_TypeError, msg);
        }
    }
    return int_to_str(i, 10);
}

R_str format_as_float(R_object obj) {
    return float_to_str(to_float(obj));
}

extern intptr_t fix_limit(intptr_t val, intptr_t length);

R_object PY_FUNC2(str_join_list, R_str s, R_list l) {
    int i, length = 0, slen = s->length;
    int l_size = LIST_SIZE(l);
    R_str x, result;
    R_array a = l->array;
    if (l_size == 0)
        return (R_object)empty_string;
    for (i = 0; i < l_size; i++) {
        x = (R_str)ITEM(a, i);
        TYPE_CHECK(x, type_str);
        length += x->length;
    }
    length += (l_size - 1) * slen;
    result = gc_allocate_str(length);
    SET_TYPE(result, type_str);
    result->length = length;
    length = 0;
    for (i = 0; ; ) {
        int j, len;
        x = (R_str)ITEM(a, i);
        len = x->length;
        for (j = 0; j < len; j++) {
            result->text[length+j] = x->text[j];
        }
        length += len;
        i++;
        if (i == l_size) {
            assert(length == result->length);
            string_hash(result);
            return (R_object)result;
        }
        for (j = 0; j < slen; j++) {
            result->text[length+j] = s->text[j];
        }
        length += slen;
    } while (1);
    return 0;
}

/* Returns the length of the format substring
 * TO DO - Do this properly.
*/
int do_format(R_str format, int index, R_object object, R_str* out) {
    R_object tmp;
    switch(format->text[index]) {
    case 's':
        *out = HotPyObject_Str(object);
        return 1;
    case 'd':
        *out = format_as_int(object);
        return 1;
    case 'g': case 'f':
        *out = format_as_float(object);
        return 1;
    default:
        fatal("Don't format character '%c'\n", format->text[index]);
    }
    return 0;
}

/* Keep this simple, use a list to gather substrings */
R_str string_format_tuple(R_str str, R_tuple tuple) {
    R_list strings = make_list(tuple->length *2);
    R_str result;
    int i, substring_start = 0;
    int index = 0, slen = str->length;
    for (i = 0; i < slen; i++) {
        if (str->text[i] == '%') {
            R_str s;
            if (i < slen-1 && str->text[i+1] == '%') {
                list_append(strings, (R_object)single_character('%'));
                i++;
            } else {
                if (index >= tuple->length) {
                    raise_char(type_TypeError, "Not enough arguments for format string");
                }
                if (i != substring_start) {
                    list_append(strings, (R_object)substring(str, substring_start, i));
                }
                i += do_format(str, i+1, ITEM(tuple, index), &s);
                index++;
                list_append(strings, (R_object)s);
                substring_start = i+1;
            }
        }
    }
    if (i != substring_start) {
        list_append(strings, (R_object)substring(str, substring_start, i));
    }
    if (index < tuple->length) {
        raise_char(type_TypeError, "Not all arguments converted during string formatting");
    }
    return (R_str)PY_str_join_list(strings, empty_string);
}

R_str single_character(int code) {
    R_str result = gc_allocate_str(1);
    SET_TYPE(result, type_str);
    result->length = 1;
    result->text[0] = code;
    result->hashcode = code;
    return result;
}

R_object PY_FUNC1(chr, R_int i) {
    intptr_t p0 = int_from_py_int(i);
    TYPE_ASSERT(i, int);
    if (((unsigned)p0) > 255) {
        raise_char(type_ValueError, "chr() arg not in range(256)");
    }
    return (R_object)single_character(p0);
}

/** This is not locale specific so c_string had better be ASCII */
R_str string_from_c_string(const char* c_string) {
    int i, len;
    R_str result;
    for (len = 0; c_string[len]; len++);
    result = gc_allocate_str(len);
    SET_TYPE(result, type_str);
    result->length = len;
    for (i = 0; i < len; i++) {
       result->text[i] = c_string[i];
    }
    string_hash(result);
    return result;
}

R_str string_from_chars(Py_UNICODE* chars, int count) {
    int i;
    R_str result = gc_allocate_str(count);
    SET_TYPE(result, type_str);
    result->length = count;
    for (i = 0; i < count; i++) {
       result->text[i] = chars[i];
    }
    string_hash(result);
    return result;
}

R_str substring(R_str str, int start, int end) {
    int i, low, high;
    R_str result;
    GVMT_NO_GC_BLOCK_BEGIN
        low = fix_limit(start, str->length);
        high = fix_limit(end, str->length);
    GVMT_NO_GC_BLOCK_END
    result = gc_allocate_str(high - low);
    SET_TYPE(result, type_str);
    result->length = high - low;
    for (i = low; i < high; i++)
        result->text[i-low] = str->text[i];
    string_hash(result);
    return result;
}

R_str hotpy_str_from_char_array(R_char_array chars, size_t len) {
    int i;
    R_str result;
    assert(len <= LEN(chars));
    result = gc_allocate_str(len);
    SET_TYPE(result, type_str);
    result->length = len;
    for (i = 0; i < len; i++)
        result->text[i] = chars->chars[i];
    string_hash(result);
    return result;
}

R_str string_concat(R_str s1, R_str s2) {
    R_str result;
    int i, len1 = s1->length;
    int len2 = s2->length;
    result = gc_allocate_str(len1 + len2);
    SET_TYPE(result, type_str);
    result->length = len1 + len2;
    for (i = 0; i < len1; i++) {
        Py_UNICODE ch = s1->text[i];
        result->text[i] = ch;
    }
    for (i = 0; i < len2; i++) {
        Py_UNICODE ch = s2->text[i];
        result->text[len1 + i] = ch;
    }
    string_hash(result);
    return result;
}

R_object PY_FUNC2(add_str_str, R_str s0, R_str s1) {
    TYPE_ASSERT(s1, str);
    TYPE_ASSERT(s0, str);
    return (R_object)string_concat(s0, s1);
}

R_object PY_FUNC2(cmp_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object)PY_SMALL_INT(string_compare(p0, p1));
}

R_object PY_FUNC2(ne_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object) booleans[1-string_equals(p0, p1)];
}

R_object PY_FUNC2(eq_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object) booleans[string_equals(p0, p1)];
}

R_object PY_FUNC2(mod_str_tuple, R_str str, R_tuple tuple) {
    TYPE_ASSERT(str, str);
    TYPE_ASSERT(tuple, tuple);
    return (R_object)string_format_tuple(str, tuple);
}

R_object PY_FUNC3(substring, R_str string, R_int start, R_int end) {
    TYPE_ASSERT(string, str);
    TYPE_ASSERT(start, int);
    TYPE_ASSERT(end, int);
    return (R_object)substring(string, int_from_py_int(start), int_from_py_int(end));
}

R_object PY_FUNC2(mul_str_int, R_str str, R_int x) {
    R_str result;
    int i, j, count, len = str->length;
    count = int_from_py_int(x);
    if (count < 0)
        return (R_object)empty_string;
    result = gc_allocate_str(len * count);
    SET_TYPE(result, type_str);
    result->length = len * count;
    for (i = 0; i < count; i++) {
        for (j = 0; j < len; j++) {
            Py_UNICODE ch = str->text[j];
            result->text[i*len+j] = ch;
        }
    }
    string_hash(result);
    return (R_object)result;
}

/********** striterator methods ******/

R_object PY_FUNC1(str_iter, R_str s) {
    R_str_iterator it = gc_allocate(str_iterator);
    SET_TYPE(it, type_str_iterator);
    it->next = 0;
    it->str = s;
    return (R_object)it;
}

R_object PY_FUNC1(str_iter_next, R_str_iterator self) {
    R_str s;
    if (self->next >= LEN(self->str))
        stop_iteration();
    s = single_character(self->str->text[self->next]);
    self->next++;
    return (R_object)s;
}


