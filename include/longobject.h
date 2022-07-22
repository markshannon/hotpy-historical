#ifndef HOTPY_LONGOBJECT_H
#define HOTPY_LONGOBJECT_H

#define IS_NEGATIVE(a) (((int32_t)(a)->digits[(a)->length-1]) < 0)

#define as_int(i) (((intptr_t)gvmt_untag(i)) >> 1)

R_int negate_inplace(R_int l);

R_int HotPyLong_FromSsize_t(intptr_t i);

R_int py_long_from_double(int sign, double frac, size_t ndigits);

R_int add_long1(R_int a, int32_t b);
R_int add_long_long(R_int a, R_int b);
R_int sub_long_long(R_int a, R_int b);

R_int long_left_shift(R_int a, int lsh);
R_int long_right_shift(R_int a, int rsh);

int py_long_long_compare(R_int a, R_int b);
int py_long_compare1(R_int a, int32_t b);
int py_long_double_compare(R_int l, double d);

intptr_t hotpy_long_hash(R_int l);

R_int negate_long(R_int i);
R_int invert_long(R_int i);

R_int mul_long_long(R_int a, R_int b);
R_int mul_long1(R_int a, int32_t b);

R_int floordiv_long_long(R_int u, R_int v);
int32_t divmod1(R_int u, int32_t n, R_int* q);
R_int mod_long_long(R_int a, R_int b);

R_int and_long_long(R_int a, R_int b);
R_int and_long1(R_int a, int32_t b);

R_int or_long_long(R_int a, R_int b);
R_int or_long1(R_int a, int32_t b);
      
R_int xor_long_long(R_int a, R_int b);
R_int xor_long1(R_int a, int32_t b);

R_int pow_long(R_int power, intptr_t exp);

R_str long_to_str(R_int a, int32_t radix);

#endif // INTOBJECT_H

