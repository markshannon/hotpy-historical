#include "hotpy.h"
#include <math.h>
#include <assert.h>
#include "intobject.h"
#include "longobject.h"
#include <malloc.h>
#include "function.h"

extern GVMT_OBJECT(type) _int;
R_type type_int = &_int;

extern int isnan(double d);
extern int isinf(double d);

R_int py_int_from_double(double d) {
    R_int result;
    double frac;
	int i, ndigits, expo, sign = 1;  
    GVMT_NO_GC_BLOCK_BEGIN
        if (isinf(d))
            raise_char(type_OverflowError, 
                       "cannot convert float infinity to integer");
        if (isnan(d))
            raise_char(type_ValueError, 
                       "cannot convert float NaN to integer");
        if (d < 0) {
            sign = -1;
            d = -d;
        }
        frac = frexp(d, &expo);
        if (expo <= 0)
            return PY_SMALL_INT(0);
        ndigits = (expo-1) / 32 + 1;
        frac = ldexp(frac, ((expo-1) & 0x1f) + 1);
    GVMT_NO_GC_BLOCK_END
    if (ndigits == 1)
        return R_int_from_ssize_t((int)(sign*frac));
    else
        return py_long_from_double(sign, frac, ndigits);
}

R_object PY_FUNC1(int_to_float, R_int l) {
    R_float f = gc_allocate(float);
    SET_TYPE(f, type_float);
    f->value = double_from_py_int(l);
    return (R_object)f;
}

R_object PY_FUNC1(float_to_int, R_float f) {
    return (R_object)py_int_from_double(f->value);
}

R_object PY_FUNC2(add_int_int, R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            int sum = as_int(a) + as_int(b);
            return (R_object)R_int_from_ssize_t(sum);
        } else {
            return (R_object)add_long1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)add_long1(a, as_int(b));
        } else {
            return (R_object)add_long_long(a, b);
        }
    } 
}

R_object PY_FUNC2(sub_int_int, R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            int diff = as_int(a) - as_int(b);
            return (R_object)R_int_from_ssize_t(diff);
        } else {
            R_int s = add_long1(b, -as_int(a));
            s = negate_inplace(s);
            return (R_object)s;
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)add_long1(a, -as_int(b));
        } else {
            return (R_object)sub_long_long(a, b);
        }
    } 
}    

R_object PY_FUNC2(mul_int_int, R_int a, R_int b) {
    TYPE_ASSERT(a, int);
    TYPE_ASSERT(b, int);
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            return (R_object)py_int_from_int64(((int64_t)as_int(a)) * as_int(b));
        } else {
            return (R_object)mul_long1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)mul_long1(a, as_int(b));
        } else {
            return (R_object)mul_long_long(a, b);
        }
    } 
}

int8_t ascii_digits[] = {
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    10,
    11,
    12,
    13,
    14,
    15,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    10,
    11,
    12,
    13,
    14,
    15,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
    -1,
};

#define DIGIT(c) (((c) < 128) ? ascii_digits[c] : Py_UNICODE_TODIGIT(c))

#define SMALL_LIMIT (1<<30)

R_object parse_int(R_str s, int base) {
    int i, len = LEN(s);   
    R_int value;
    intptr_t small;
    uint16_t c;
    assert(len > 0);
    small = 0;
    i = 0;
    while (small*base < SMALL_LIMIT) {
        small *= base;
        c = s->text[i];
        i++;
        small += DIGIT(c);  
        if (i == len) {
            return (R_object)PY_SMALL_INT(small);
        }
    }
    value = _boxed_int_from_ssize_t(small);
    for ( ; i < len; i++) {
        assert(!gvmt_is_tagged(value));
        value = (R_int)mul_long1(value, base);
        c = s->text[i];
        value = (R_int)add_long1(value, DIGIT(c));
    }
    return (R_object)value;
}


R_object PY_FUNC2(lshift_int_int, R_int a, R_int b) {
    int lsh = int_from_py_int(b);
    if (lsh < 0)
        raise_char(type_ValueError, "negative shift count");
    if (gvmt_is_tagged(a)) {
        if (lsh < 32) 
            return (R_object)py_int_from_int64(((int64_t)as_int(a)) << lsh);
        else
            return (R_object)long_left_shift(_boxed_int_from_ssize_t(as_int(a)), lsh);
    } else {
        return (R_object)long_left_shift(a, lsh);
    } 
}

R_object PY_FUNC2(rshift_int_int, R_int a, R_int b) {
    int rsh = int_from_py_int(b);
    if (rsh < 0)
        raise_char(type_ValueError, "negative shift count");
    if (gvmt_is_tagged(a)) {
        if (rsh < 32) 
            return (R_object)gvmt_tag(((as_int(a) >> rsh) << 1) + 1);
        else
            return (R_object)PY_SMALL_INT(0);
    } else {
        return (R_object)long_right_shift(a, rsh);
    } 
}

//
//static R_int make_int_from_array(uint16_t *a, int len) {
//    int i, xlen, n;
//    R_int x;
//    while (len > 1 && a[len-1] == 0)
//        len = len - 1;
//    if (((int16_t)a[len-1]) < 0) {
//        xlen = (len+2)>>1;   
//    } else {
//        xlen = (len+1)>>1;
//    }
//    x = gc_allocate_int(xlen);
//    SET_TYPE(x, type_int);
//    x->length = xlen;
//    gvmt_fully_initialized((GVMT_Object)x);
//    if (len & 1) {
//        x->digits[xlen-1] = a[len-1];  
//    } else {
//        x->digits[xlen-1] = 0;
//    }
//    for(i = 0, n = len-1; i < n; i+=2) {
//        x->digits[i>>1] = a[i] | ((uint32_t)a[i+1]) << 16;
//    }
//    assert(!IS_NEGATIVE(x));
//    CHECK(x);
//    return x;
//}

//
//static void multiword_divmod(R_int a, R_int b, R_int *q, R_int *r) {
//    uint16_t *un, *vn, *qn;
//    int m, n, qlen, sign;
//    m = a->length << 1;
//    n = b->length << 1;
//    qlen = m - n + 1;
//    if (a->length > 100)
//        un = malloc(2*(m+1));
//    else
//        un = alloca(2*(m+1));
//    if (b->length > 100)
//        vn = malloc(2*n);
//    else
//        vn = alloca(2*n);
//    if (qlen > 100)
//        qn = malloc(2*qlen);
//    else
//        qn = alloca(2*qlen);
//    fill_array(a, un);
//    fill_array(b, vn);
//    if (IS_NEGATIVE(a)) {
//        negate_array(un, m);
//    }
//    if (IS_NEGATIVE(b)) {
//        negate_array(vn, n);
//    }
//    sign = IS_NEGATIVE(a) ^ IS_NEGATIVE(b);
//    while (un[m-1] == 0)
//        m = m - 1;
//    while (vn[n-1] == 0)
//        n = n - 1;
//    divmnu(qn, un, vn, m, n);
//    if (r) *r = make_int_from_array(un, n);
//    if (q) *q = make_int_from_array(qn, qlen);
//    if (IS_NEGATIVE(a)) {
//        if (IS_NEGATIVE(b)) {
//            if (r) negate_inplace(*r);
//        } else {
//            if (non_zero_array(un, n)) {
//                if (q) invert_inplace(*q);   
//                if (r) *r = (R_int)PY_sub_int_int(*r, b);
//            } else {
//                if (q) negate_inplace(*q);  
//            }
//        }
//    } else {
//        if (IS_NEGATIVE(b)) {
//            if (non_zero_array(un, n)) {
//                if (q) invert_inplace(*q);
//                if (r) *r = (R_int)PY_add_int_int(b, *r);
//            } else {
//                if (q) negate_inplace(*q);
//            }
//        } else {
//            // Do nothing, both positive.
//        }
//    }
//    if (a->length > 100)
//        free(un);
//    if (b->length > 100)
//        free(vn);
//    if (qlen > 100)
//        free(qn);
//    if (q) CHECK(*q);
//    if (r) CHECK(*r);
//}

R_object PY_FUNC2(floordiv_int_int, R_int u, R_int v) {
    int q, ui, vi;
    R_int quot;
    if (gvmt_is_tagged(u)) {
        ui = as_int(u);  
        if (gvmt_is_tagged(v)) {
            vi = as_int(v);
            goto int32_div;
        } else {
            if (LEN(v) == 1) {
                vi = v->digits[0];
                goto int32_div;
            } else {
                if (ui == 0)
                    return (R_object)PY_SMALL_INT(0);
                else if (IS_NEGATIVE(v) ^ (as_int(u)<0))
                    return (R_object)PY_SMALL_INT(-1);
                else
                    return (R_object)PY_SMALL_INT(0);
            }
        }
    } else {
        if (gvmt_is_tagged(v)) {
            intptr_t n = as_int(v);
            if (n == (int16_t)n) {
                divmod1(u, as_int(v), &quot);
                return (R_object)quot;
            } else {
                v = _boxed_int_from_ssize_t(n);
                return (R_object)floordiv_long_long(u, v); 
            }
        } else {
            return (R_object)floordiv_long_long(u, v);
        }
    } 
int32_div:
    if (vi == 0)
        zero_division_error("integer division");
    q = ui / vi;
    if (((ui < 0) ^ (vi < 0)) && vi*q != ui) 
        q--;
    if (q == 0x40000000)
        return (R_object)_boxed_int_from_ssize_t(q);
    else
        return (R_object)gvmt_tag((q << 1) + 1);
}

R_object PY_FUNC2(tagged_floordiv, R_int u, R_int v) {
    intptr_t rem, q, ui, vi;
    assert (gvmt_is_tagged(u) && gvmt_is_tagged(v));
    ui = as_int(u);
    vi = as_int(v);
    if (vi == 0)
        zero_division_error("integer division");
    q = ui / vi;
    if (((ui < 0) ^ (vi < 0)) && vi*q != ui)
        q--;
    if (q == 0x40000000)
        return (R_object)_boxed_int_from_ssize_t(q);
    else
        return (R_object)gvmt_tag((q << 1) + 1);
}

R_object PY_FUNC2(mod_int_int, R_int u, R_int v) {
    int rem, q, ui, vi;
    R_int quot;
    if (gvmt_is_tagged(u)) {
        ui = as_int(u);  
        if (gvmt_is_tagged(v)) {
            vi = as_int(v);
            goto int32_mod;
        } else {
            if (LEN(v) == 1) {
                vi = v->digits[0];
                goto int32_mod;
            } else {
                if (ui == 0)
                    return (R_object)PY_SMALL_INT(0);
                else if (IS_NEGATIVE(v) ^ (as_int(u)<0))
                    return (R_object)add_long1(v, as_int(u));
                else
                    return (R_object)u;
            }
        }
    } else {
        if (gvmt_is_tagged(v)) {
            divmod1(u, as_int(v), &quot);
            return (R_object)quot;
        } else {
            return (R_object)mod_long_long(u, v);
        }
    } 
int32_mod:
    if (vi == 0)
        zero_division_error("integer modulo");
    q = ui / vi;
    if (q < 0) {
        if (vi*q == ui)
            return (R_object)PY_SMALL_INT(0);
        else 
            q--;   
    }
    rem = ui - q * vi;
    return (R_object)R_int_from_ssize_t(rem);
}

R_object PY_FUNC2(tagged_mod, R_int u, R_int v) {
    intptr_t rem, q, ui, vi;
    ui = as_int(u);
    vi = as_int(v);
    assert (gvmt_is_tagged(u) && gvmt_is_tagged(v));
    if (vi == 0)
        zero_division_error("integer modulo");
    q = ui / vi;
    rem = ui - q * vi;
    if (rem != 0 && q < 0) {
        rem += vi;
    }
    return (R_object)PY_SMALL_INT(rem);
}    
 
int py_int_double_compare(R_int l, double d) {
    if (gvmt_is_tagged(l)) {
        int i = as_int(l);  
        if (i > d)
            return 1;
        else if (i == d)
            return 0;
        else
            return -1;
    } else {
        return py_long_double_compare(l, d);
    }
}

int py_int_int_compare(R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            if (a < b)
                return -1;
            else if (a > b)
                return 1;
            else
                return 0;
        } else {
            return -py_long_compare1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return py_long_compare1(a, as_int(b));
        } else {
            return py_long_long_compare(a, b);
        }
    } 
}
 
#define COUNTER_SHIFT (sizeof(uintptr_t)*8 - SHIFT)

intptr_t hotpy_int_hash(R_int l) {
    if (gvmt_is_tagged(l)) {
        return as_int(l); 
    } else {
        return hotpy_long_hash(l);
    }
}

R_object PY_FUNC1(int_hash, R_object self) {
    return self;   
}

R_object PY_FUNC1(neg_int, R_int i) {
    if (gvmt_is_tagged(i)) {
        return (R_object)gvmt_tag(-((intptr_t)gvmt_untag(i)) + 2);   
    } else {
        return (R_object)negate_long(i);
    }
}

R_object PY_FUNC1(invert_int, R_int i) {
    if (gvmt_is_tagged(i)) {
        return (R_object)gvmt_tag(((intptr_t)gvmt_untag(i)) ^ -2);   
    } else {
        return (R_object)invert_long(i);
    }
}

R_object PY_FUNC2(pow_int_int, R_int power, R_int i1) {
    intptr_t exp = int_from_py_int(i1);
    R_int result = (R_int)HotPyLong_FromSsize_t(1);
    if (exp < 0) {
        // To do ...
        // Use float pow   
        return (R_object)NotImplemented;
    }
    while (exp) {
        if (exp & 1)
            result = (R_int)PY_mul_int_int(result, power);
        power =(R_int) PY_mul_int_int(power, power);
        exp = exp >> 1;
    }
    return (R_object)result;
}

R_object PY_FUNC2(and__int_int, R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            return (R_object)gvmt_tag(((intptr_t)gvmt_untag(a)) &
                                      ((intptr_t)gvmt_untag(b)));
        } else {
            return (R_object)and_long1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)and_long1(a, as_int(b));
        } else {
            return (R_object)and_long_long(a, b);
        }
    } 
}

R_object PY_FUNC2(or__int_int, R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            return (R_object)gvmt_tag(((intptr_t)gvmt_untag(a)) |
                                      ((intptr_t)gvmt_untag(b)));
        } else {
            return (R_object)or_long1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)or_long1(a, as_int(b));
        } else {
            return (R_object)or_long_long(a, b);
        }
    } 
}

R_object PY_FUNC2(xor_int_int, R_int a, R_int b) {
    if (gvmt_is_tagged(a)) {
        if (gvmt_is_tagged(b)) {
            return (R_object)gvmt_tag((((intptr_t)gvmt_untag(a)) ^
                                      ((intptr_t)gvmt_untag(b))) + 1);
        } else {
            return (R_object)xor_long1(b, as_int(a));
        }
    } else {
        if (gvmt_is_tagged(b)) {
            return (R_object)xor_long1(a, as_int(b));
        } else {
            return (R_object)xor_long_long(a, b);
        }
    } 
}

extern char* _itoa(int value, char* str, int radix);

R_str int_to_str(R_int a, int32_t radix) {
    static char dig[] =
    "0123456789"
    "abcdefghijklmnopqrstuvwxyz";
    if (gvmt_is_tagged(a)) {
        char buf[36];
        char* out;
        unsigned val;
        if (radix == 10) {
            sprintf(buf, "%d", as_int(a));
            return string_from_c_string(buf);
        }
        out = buf+35;
        *out = 0;
        val = as_int(a);
        do {
            --out;
            *out = dig[val%radix];
            val /= radix;
        } while (val);
        return string_from_c_string(out);
    } else {
        return long_to_str(a, radix);  
    }
}

/* Attempt to coerce o to an integer for use as an index *
 * Returns an int if __index__ exists and is well behaved,
 * Raise an exception if __index__ otherwise */
R_int HotPyNumber_Index(R_object o) {
    R_int result;
    R_object d;
    if  (IS_INT(o))
        return (R_int)o;
    d = class_attribute(TYPE(o), name___index__);
    if (d) {
        R_tuple t = make_tuple1(o);
        call_func f = CALLABLE(d);
        result = (R_int)f(d, t, 0);
        if (IS_INT(result)) {
            return result;
        } else {
            R_str s = py_sprintf("__index__ returned non-int (type %s)", 
                                 TYPE_NAME(result));
            raise_str(type_TypeError, s);
        }
    } else {
        R_str s = py_sprintf("'%s' object cannot be interpreted as an integer",
                             TYPE_NAME(o));
        raise_str(type_TypeError, s);
    }
    return 0;
}

R_object PY_FUNC1(__bool___int, R_int i) {
    TYPE_ASSERT(i, int);
    return (R_object)booleans[NON_ZERO(i)];
}

R_object PY_FUNC1(pos_int, R_object o) {
    return o;
}

R_int HotPyLong_FromSsize_t(intptr_t v) {
    return R_int_from_ssize_t(v); 
}

R_int HotPyLong_FromSize_t(size_t v) {
    if (v < 0x40000000) {
        return (R_int)gvmt_tag(((v)<<1)+1);
    } else {
        return py_int_from_int64((int64_t)v);
    }
}


