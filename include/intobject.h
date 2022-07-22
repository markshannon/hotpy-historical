#ifndef HOTPY_INTOBJECT_H
#define HOTPY_INTOBJECT_H

GVMT_OBJECT(int) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    uint32_t length;   
    uint32_t digits[]; 
};

typedef GVMT_OBJECT(int) *R_int;
extern R_type type_int;

#define PY_SMALL_INT(i) ((R_int)gvmt_tag((((intptr_t)(i))<<1)+1))

intptr_t int_from_py_long(R_int l);

/** Returns 1 if this int is one. Useful for range & slice */
//int py_int_is_one(R_int i);
//#define py_int_is_one(i) ((LEN(i) == 1) && (i)->digits[0] == 1)
#define py_int_is_one(i) (i == (R_int)gvmt_tag(3))

double double_from_py_long(R_int l);

#define double_from_py_int(l) (gvmt_is_tagged(l) ? \
    ((double)(((intptr_t)gvmt_untag(l)) >> 1)) : \
        double_from_py_long(l))

R_int py_int_from_double(double d);
R_int py_int_from_int64(int64_t val);

R_object PY_FUNC1(int_to_float, R_int l);

R_int HotPyLong_FromSsize_t(intptr_t i);

int py_int_double_compare(R_int l, double d);

int py_int_int_compare(R_int a, R_int b);

intptr_t hotpy_int_hash(R_int l);

R_int HotPyNumber_Index(R_object o);

R_object PY_FUNC2(pow_int_int, R_int i0, R_int i1);

R_str int_to_str(R_int a, int32_t radix);

R_object PY_FUNC2(add_int_int, R_int a, R_int b);

R_object PY_FUNC2(sub_int_int, R_int a, R_int b);

#define NON_ZERO(i) (i != (R_int)gvmt_tag(1))

R_int _boxed_int_from_ssize_t(intptr_t i);

/** Warning - These macros evaluate their argument more than once.
 */
#define R_int_from_ssize_t(i)  ((((intptr_t)((i) + 0x40000000)) >= 0) ? \
    ((R_int)gvmt_tag(((i)<<1)+1)) : _boxed_int_from_ssize_t(i))

R_int HotPyLong_FromSize_t(size_t v);

#define int_from_py_int(i) (((intptr_t)gvmt_is_tagged(i)) ? \
     (((intptr_t)gvmt_untag(i)) >> 1) : int_from_py_long(i))

/** Make an int from a (big-endian) signed byte-array. */
R_int int_from_byte_array(uint8_t *a, int len);

extern int8_t ascii_digits[128];

R_object parse_int(R_str, int radix);

#endif // INTOBJECT_H

