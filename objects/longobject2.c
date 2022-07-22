#include "hotpy.h"
#include <math.h>
#include <assert.h>
#include "intobject.h"
#include "longobject.h"
#include <malloc.h>
#include "function.h"

#define ASSERT_IS_LONG(i) assert((!gvmt_is_tagged(i)) && ((i)->ob_type == type_int))

/** Most significant digit is length -1  */

#if WORD_SIZE == 8
typedef uint32_t digit_t;
# define DIGIT_BITS 32
# define SIGN_SHIFT 63
#else
typedef uint16_t digit_t;
# define DIGIT_BITS 16
# define SIGN_SHIFT 31
#endif
   
GVMT_OBJECT(int) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    size_t length;   
    int8_t sign; // 0 is positive, 1 is negative
    digit_t digits[]; 
};

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

static R_int gc_allocate_int(int size) {
    assert(size > 0);
    return gc_allocate_var(int, digits, size);
}

// /0/1 style for sign.
#define NEGATE(i) (i)->sign = 1-(i)->sign
#define IS_NEGATIVE(i) ((i)->sign)
#define PROD_SIGN(i, j) ((i)->sign^(j)->sign)
#define POSITIVE 0
#define NEGATIVE 1

static R_int py_int_from_overflow(intptr_t i) {
    R_int result;
    result = gc_allocate_int(2);
    SET_TYPE(result, type_int);
    result->length = 2;
    result->sign = i;
    i = abs(i);
    if (i < 0) {
        result->sign = POSITIVE;
        i = -i;  
    } else {
        result->sign = NEGATIVE; 
    }
    assert(i != 0);   
    result->l_digit[1] = i;
    result->l_digit[0] = i >> DIGIT_BITS;
    CHECK(result);
    return result;
}

static R_int long_copy(R_int src) {
    size_t i, len = LEN(src);
    R_int result = gc_allocate_int(src->length);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = src->length;
    gvmt_fully_initialized((GVMT_Object)result);
    result->sign = src->sign;
    for (i = 0; i < len; i++) {
        result->digits[i] = result->digits[i];
    }
    return result;
}    

R_int hotpy_long_invert(R_int i) {
    R_int x;
    ASSERT_IS_LONG(i);
    if (NEGATIVE(x)) {
        x = sub1(i);
        x->sign = POSITIVE;
    } else {
        x = add1(i);
        x->sign = NEGATIVE;
    }
    return x;
}

R_int hotpy_long_xor(R_int a, R_int b) {
    R_int result;
    digit_t inv, inv_a, inv_b;
    size_t len_a = LEN(a), len_b = LEN(b);
    size_t i, ndigits = max(len_a, len_b);
    size_t shorter = min(len_a, len_b);
    if (IS_NEGATIVE(a)) {
        inv_a = (digit_t)-1;
        a = hotpy_long_invert(a);
    } else {
        inv_a = 0;    
    }
    if (IS_NEGATIVE(b)) {
        inv_b = (digit_t)-1;
        b = hotpy_long_invert(b);
    } else {
        inv_b = 0;    
    }
    if (inv_a != inv_b) {
        inv_a = ~inv_a;
    }
    inv = inv_a ^ inv_b;
    result = gc_allocate_int(ndigits);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = ndigits;
    for (i = 0; i < shorter; ++i) {
        result->digits[i] = a->digits[i] ^ b->l_digit[i] ^ inv;
    }
    if (LEN(a) > LEN(b)) {
        for (; i < ndigits; ++i) { 
            result->digits[i] = a->digits[i] ^ inv;
        }
    } else {
         for (; i < ndigits; ++i) {  
             result->digits[i] = b->digits[i] ^ inv;
         }
    }
    if (inv) {
        result = hotpy_long_invert(result);
    } else {
        result = long_normalise(result);   
    }
    return result;
}

/****** TO HERE ***********/

R_int py_long_from_double(int sign, double frac, size_t ndigits) {
    int i;
    R_int result = gc_allocate_int(ndigits);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = ndigits;
    GVMT_NO_GC_BLOCK_BEGIN
        for (i = ndigits; --i >= 0; ) {
            int bits = (int)frac;
            result->digits[i] = ((int32_t)bits);
            frac = frac - (double)bits;
            frac = ldexp(frac, 32);
        }
    GVMT_NO_GC_BLOCK_END
    if (sign < 0)
        result->sign = NEGATIVE;
    CHECK(result);
    return result;
}

double double_from_py_long(R_int l) {
	double result;
    size_t len = l->length;
    ASSERT_IS_LONG(l);
	if (len == 1)
	    return (double)l->digits[0];
	else
        assert(0 && "To do");
    return 0;
}

intptr_t int_from_py_long(R_int l) {
    ASSERT_IS_LONG(l);
    if (l->length == 1) {
        return (int32_t)l->digits[0];
    } else {
        if (sizeof(intptr_t) == 8 && (l->length == 2)) {
            return l->digits[0] | (((int64_t)l->digits[1]) << 32);
        } else {
            raise_char(type_OverflowError, 
                       "Python int too large to convert to machine int");
            return 0;
        }
    }
}

static R_int _add1(R_int a, uintptr_t b) {
    int i;
    R_int result;
    size_t len = a->length;
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = len;
    gvmt_fully_initialized((GVMT_Object)result);
    for (i = 0; i < len; i++) { 
        uintptr_t t = ((uintptr_t)a->digits[i]) + b;
        b = t >> DIGIT_BITS;
        result->digits[i] = t;
    }
    if (b) {
        return long_extend(result, b);
    } else {
        assert(IS_NORMALISED(result));
        return result;
    }
}

R_int hotpy_long_add1(R_int a, intptr_t b) {
    if (IS_NEGATIVE(a)) {
        if (b < 0) {
            return _add1(a, ~((uintptr)b)+1);
        } else {
            return _sub1(a, b);
        }
    } else {
        if (b < 0) {
            return _sub1(a, ~((uintptr)b)+1);
        } else {
            return _add1(a, b);  
        }
    }
}

R_int add_long_long(R_int a, R_int b) {
    ASSERT_IS_LONG(a);
    ASSERT_IS_LONG(b);
    if (IS_NEGATIVE(a)) {
        if (IS_NEGATIVE(b)) {
            R_int sum = _long_add(a, b);
            sum->sign = NEGATIVE;
            return sum;
        } else {
            return _long_sub(b, a);   
        }
    } else {
        if (IS_NEGATIVE(b)) {
            return _long_sub(a, b);
        } else {
            return _long_add(a, b); 
        }
    }
}

/** Add abs(a) to abs(b). Result is positive */
static R_int _add_long(R_int a, R_int b) {
    R_int result;
    size_t len_a, len_b;
    int sign = SIGN(a);
    uintptr_t carry;
    if (a->length < b->length) {
        R_int tmp;
        tmp = a;
        a = b;
        b = tmp;
    }
    assert(a->length >= b->length);
    len_a = a->length;
    len_b = b->length;
    result = gc_allocate_int(len_a);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = len_a;
    result->sign = POSITIVE;
    gvmt_fully_initialized((GVMT_Object)result);  
    carry = 0;
    for (i = 0; i < len_b; i++) { 
        uintptr_t t = carry + a->digits[i] + b->digits[i];
        carry = t >> DIGIT_BITS;
        result->digits[i] = t;
    }
    for (; i < len_a; i++) {
        uintptr_t t = carry + a->digits[i];
        carry = t >> DIGIT_BITS;
        result->digits[i] = t;  
    }
    if (carry)
        return long_extend(result, carry);
    else 
        return long_normalise(result);
}
    
/** abs(a) - abs(b) */
static R_int _sub_long(R_int a, R_int b) {
    R_int result;
    size_t len_a = a->length, len_b = b->length;
    int sign = POSITIVE;
    intptr_t borrow;
    if (len_a < len_b) {
        R_int tmp;
        size_t len_tmp;
        tmp = a;
        a = b;
        b = tmp;
        sign = NEGATIVE;
        len_tmp = len_a
        len_a = len_b;
        len_b = len_tmp;
    } else if (len_a == len_b) {
        i = len_a-1;
        while (a->digits[i] == b->digits[i]) {
            if (i == 0) 
                return PY_SMALL_INT(0);
            --i;
        }
        len_a = len_b = i+1;
        if (a->digits[i] < b->digits[i]) {
            R_int tmp;  
            tmp = a;
            a = b;
            b = tmp;
            sign = NEGATIVE;
        }
    }
    result = gc_allocate_int(len_a);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = len_a;
    result->sign = sign;
    gvmt_fully_initialized((GVMT_Object)result);  
    borrow = 0;
    for (i = 0; i < len_b; i++) {
        intptr_t t = a->digits[i] - borrow - b->digits[i];
        borrow = ((uintptr_t)t) >> SIGN_SHIFT;
        result->digits[i] = t;
    }
    for (; i < len_a; i++) {
        intptr_t t = a->digits[i] - borrow;
        borrow = ((uintptr_t)t) >> SIGN_SHIFT;
        result->digits[i] = t;
    }
    assert(borrow == 0);
    return long_normalise(result);
}

R_int sub_long_long(R_int a, R_int b) {
    ASSERT_IS_LONG(a);
    ASSERT_IS_LONG(b);
    if (IS_NEGATIVE(a)) {
        if (IS_NEGATIVE(b)) {
            return _long_sub(b, a);
        } else {
            R_int sum = _long_add(a, b);
            sum->sign = NEGATIVE;
            return sum;   
        }
    } else {
        if (IS_NEGATIVE(b)) {
            return _long_add(a, b);
        } else {
            return _long_sub(a, b);
        }
    }
}

R_int py_int_from_int64(int64_t val) {
    R_int result;
    if (val == (int32_t)val) {
        return R_int_from_ssize_t((int32_t)val);
    }
    result = gc_allocate_int(2);
    SET_TYPE(result, type_int);
    result->length = 2;
    result->digits[0] = (int32_t)val;
    result->digits[1] = (int32_t)(val >> 32);
    CHECK(result);
    return result;    
}

/** Age-old multiplication algorithm 
 * Algortihm from "Hacker's Delight" */
static R_int standard_multiply(R_int a, R_int b) {
    R_int result;
    unsigned i, alen = a->length;
    unsigned j, blen = b->length;
    unsigned tlen = alen + blen;
    ASSERT_IS_LONG(a);
    ASSERT_IS_LONG(b);
    result = gc_allocate_int(alen + blen);
    SET_TYPE(result, type_int);
    result->length = alen + blen;
    result->sign = PROD_SIGN(a, b);
    for (j = 0; j < blen; j++) {
        digit_t k = 0;
        for (i = 0; i < alen; i++) {
            uintptr_t t = a->digits[i] * b->digits[j] + 
                          result->digits[i+j] + k;
            result->digits[i+j] = t;
            k = t >> DIGIT_BITS;
        }
        result->digits[j+alen] = k;
    } 
    return long_normalise(result);
}

static void copy_int_into_array(R_int a, uint32_t* un) {
    int i, len = a->length;
    if (IS_NEGATIVE(a)) {
        int c = 1;
        for (i = 0 ; i < len; i++) {   
            uint32_t d = a->digits[i];
            un[i] = ~d + c;
            c = (d == 0) & c;
        }
        assert(c == 0);
        un[len] = c;
    } else {
        for (i = 0 ; i < len; i++) {   
            uint32_t d = a->digits[i];
            un[i] = d;
        }
    }
}

static R_int make_int_from_array(uint32_t *a, int len) {
    int i, xlen, n;
    R_int x;
    if (len == 0)
        return PY_SMALL_INT(0);
    x = gc_allocate_int(len);
    SET_TYPE(x, type_int);
    ZERO_REF_COUNT(x);
    x->length = len;
    gvmt_fully_initialized((GVMT_Object)x);
    for(i = 0; i < len; i++) {
        x->digits[i] = a[i];
    }
    x = NORMALISE(x);
    return x;
}

#define read32(a) (((a)[0]<<24) | ((a)[1]<<16) | ((a)[2]<<8) | (a)[3])


R_int int_from_byte_array(uint8_t *a, int len) {
    int i, words = (len+3)/4;
    uint32_t *a32 = alloca(words*4);
    R_int x;
    if (len <= 4) {
        int j, t = 0;
        for (j = 0; j < len; j++) {
             t <<= 8;
             t |= a[j];
        }
        // Sign extend
        t = (t << ((4 - len)*8)) >> ((4 - len)*8);
        return R_int_from_ssize_t(t);
    } else {
        int remainder = len&3;
        uint8_t *little_end = a + len - 4;
        x = gc_allocate_int(words);
        SET_TYPE(x, type_int);
        ZERO_REF_COUNT(x);
        x->length = words;
        gvmt_fully_initialized((GVMT_Object)x);
        for (i = 0; i < words-1; i++) {
            x->digits[i] = read32(little_end-i*4);
        }
        if (remainder) {
            int j, t = 0;
            for (j = 0; j < remainder; j++) {
                 t <<= 8;
                 t |= a[j];
            }
            //Sign extend.
            x->digits[i] = (t << ((4 - remainder)*8)) >> ((4 - remainder)*8);
        } else {
            x->digits[i] = read32(little_end-i*4);  
        }
        x = NORMALISE(x);
        return x;
    }
}

#define REQUIRED_MALLOC(n) ((n) >= 128)

#define ALLOCATE(n) REQUIRED_MALLOC(n) ? malloc(sizeof(int32_t)*(n)) : alloca(sizeof(int32_t)*(n))
#define DEALLOCATE(a, n) if REQUIRED_MALLOC(n) free(a)

R_int mul_long_long(R_int a, R_int b) {
    R_int result;
    if (a->length + b->length == 2) {
        int64_t val = ((int64_t)a->digits[0]) * b->digits[0];
        return py_int_from_int64(val);
    } else {
//        return (R_object)standard_multiply(a, b);   
        R_int prod;
        int len, m_a, m_b, m_x;
        uint32_t* a_array, *b_array, *x_array;
        a_array = ALLOCATE(a->length+1);
        b_array = ALLOCATE(b->length+1);
        x_array = ALLOCATE(a->length + b->length+2);
        copy_int_into_array(a, a_array);
        copy_int_into_array(b, b_array);
        len = hotpy_mpz_mul(a->length, a_array, b->length, b_array, x_array);
        assert(len <= (a->length + b->length));
        if (((int32_t)x_array[len-1]) < 0) {
            x_array[len] = 0;
            len ++;
        }
        prod = make_int_from_array(x_array, len);
        assert(!IS_NEGATIVE(prod));
        DEALLOCATE(a_array, a->length+1);
        DEALLOCATE(b_array, b->length+1);
        DEALLOCATE(x_array, a->length + b->length+2);
        if (IS_NEGATIVE(a) ^ IS_NEGATIVE(b))
            return negate_inplace(prod);
        else
            return prod;
    }
}

R_int _mul1((R_int a, digit_t b) {
    R_int prod;  
    size_t len_a = a->length;
    uintptr_t k;
    if (((uintptr_t)(a->digits[len_a-1] + 1))*b > (digit)-1) 
        len = len_a + 1;
    else 
        len = len_a;
    prod = gc_allocate_int(len);
    SET_TYPE(prod, type_int);
    ZERO_REF_COUNT(prod);
    prod->length = len;
    gvmt_fully_initialized((GVMT_Object)prod);
    k = 0;
    for (i = 0; i < len_a; i++) {
        uintptr_t t = ((uintptr_t)a->digits[i]) * b + k;
        prod->digits[i] = t;
        k = t >> DIGIT_BITS;
    }
    if (len > len_a) {
        assert(((digit_t)k) == k);
        prod->digits[len_a] = k;
    } else {
        assert(k == 0);
    }
    return long_normalise(prod);
}

R_int hotpy_mul_int(intptr_t a, intptr_t b) {
    int sign = (a<0)^(b<0);
    digit_t d0, d1, d2, d3;   
    digit ma, la, mb, lb;
    uintptr_t k, t, p0, p1a, p1b, p2;
    a = abs(a);
    b = abs(b);
    ma = a >> DIGIT_BITS;
    la = a;
    mb = b >> DIGIT_BITS;
    lb = b;
    /* Get products */
    p0 = la * lb;
    p1a = ma * lb;
    p1b = la * mb;
    p2 = ma * mb;
    /* Fill in digits */
    d0 = p0;
    k = p0 >> DIGIT_BITS;
    t = p1a + k;
    d1 = t;
    k = t >> DIGIT_BITS;
    t = p2 + k;
    d2 = t;
    k = t >> DIGIT_BITS;
    d3 = k;
    t = p1b + d1;
    d1 = t;
    k = t >> DIGIT_BITS;
    t = d2 + k;
    d2 = t;
    k = t >> DIGIT_BITS;
    d3 += k;
    if (d3 == 0) {
        if (d2 == 0) {
            // Fits into word
            if (d1 >= (1 << (DIGIT_BITS-2))) {
                R_int result = gc_allocate_int(3);
                SET_TYPE(result, type_int);
                result->length = 3;
                result->sign = sign ? POSITIVE : NEGATIVE;
                result->digits[0] = d0;
                result->digits[1] = d1;
                return result;
            } else {
                intptr_t i = (d1 << DIGIT_BITS) | d0;
                if (sign)
                    i = -i;
                return PY_SMALL_INT(i);   
            }
        } else { 
            R_int result = gc_allocate_int(3);
            SET_TYPE(result, type_int);
            result->length = 3;
            result->sign = sign ? POSITIVE : NEGATIVE;
            result->digits[0] = d0;
            result->digits[1] = d1;
            result->digits[2] = d2;
            gvmt_fully_initialized((GVMT_Object)result);
            return result;
        }
    } else {
        R_int result = gc_allocate_int(4);
        SET_TYPE(result, type_int);
        result->length = 4;
        result->sign = sign ? POSITIVE : NEGATIVE;
        result->digits[0] = d0;
        result->digits[1] = d1;
        result->digits[2] = d2;
        result->digits[3] = d3;
        gvmt_fully_initialized((GVMT_Object)result);  
        return result;
    }
}

static R_int wlshift(R_int a, int words) {
    int i, len = a->length + words;
    R_int result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = words; i < len; i++) {
        result->digits[i] = a->digits[i-words];
    }
    return result;
}

R_int long_left_shift(R_int a, uintptr_t lsh) {
    int i, words, len, alast;
    digit_t l_bits, r_bits, r_mask;
    R_int result;
    l_bits = lsh & (DIGIT_BITS-1);
    words = (lsh+(DIGIT_BITS-1)) >> LOG_DIGIT_BITS;
    if (l_bits == 0)
        return wlshift(a, words);
    r_bits = DIGIT_BITS - l_bits;
    r_mask = ((digit_t)-1) >> r_bits;
    len = a->length + words;
    alast = a->length - 1;
    assert(lsh > 0);
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    result->sign = a->sign;
    result->digits[words-1] = (a->digits[0] << l_bits);
    for (i = words; i < len-1; i++) {
        result->digits[i] = ((a->digits[i-words-1] >> r_bits) & r_mask) |
                            ((a->digits[i-words] << l_bits));
    }
    result->digits[len-1] = ((a->digits[alast] >> r_bits) & r_mask);
    result = NORMALISE(result);
    return result;
}


extern void divmnu(uint16_t q[], uint16_t un[], uint16_t vn[], int m, int n);

static void fill_array(R_int a, uint16_t* un) {
    int i, len = a->length;
    for (i = 0 ; i < len; i++) {   
        uint32_t d = a->digits[i];
        un[i*2] = d;
        un[i*2+1] = d >> 16;
    }
}

extern void negate_array(uint16_t un[], int len);

#define non_zero_array(a, l) (((l)!=0)||((a)[0]!=0))

/** Unsigned divmod by single digit: abs(u) / n */ 
digit_t divmod1(R_int u, digit_t n, R_int* q) {
    int32_t k = 0;
    intptr_t rem;
    int j, ulen = u->length;
    R_int quot = gc_allocate_int(ulen);
    SET_TYPE(quot, type_int);
    quot->length = ulen;
    for(j = ulen-1; j >= 0; j--) {
        digit_t hi;
        rem = (rem << DIGIT_BITS) + u->digits[j];
        quot->digits[j] = rem / n;
        rem -= (intptr_t)hi * n;
    }
    if (LEN(quot) == 1)
        quot = PY_SMALL_INT(quot->digits[0]);   
    else
        quot = NORMALISE(quot);
    *q = quot;
    return k;
}
 
R_int floordiv_long_long(R_int u, R_int v) {
    R_int quot;
    ASSERT_IS_LONG(u);
    ASSERT_IS_LONG(v);
    if (v->length == 1) {
        digit_t rem;
        if (v->digits[0] == 0)
            zero_division_error("integer division");
        rem = divmod1(u, v->digits[0], &quot);
        if (PROD_SIGN(u, v) == NEGATIVE) {
            quot->sign = NEGATIVE;   
            if (rem)
                increment_inplace(quot);
        }
    } else {
        R_int rem = _long_divmod(u, v, &quot);
        if (PROD_SIGN(u, v) == NEGATIVE) {
            quot->sign = NEGATIVE;   
            if (rem == PY_SMALL_INT(0)) {
                increment_inplace(quot); 
            }
        }
    }
    return quot;
}
  
R_int mod_long_long(R_int u, R_int v) {
    if (u->length + v->length == 2) {
        int ui, vi, q, val;
        if (v->digits[0] == 0)
            zero_division_error("integer modulo");
        ui = u->digits[0];
        vi = v->digits[0];
        q = ui/vi;
        if (q < 0) {
            if (vi*q == ui)
                return PY_SMALL_INT(0);
            else 
                q--;
        }
        val = ui - q * vi;
        return R_int_from_ssize_t(val);
    } else if (v->length == 1) {
        R_int quot;
        int32_t k = divmod1(u, v->digits[0], &quot);
        return R_int_from_ssize_t(k);
    } else {
        R_int rem;
        int qlen, len, sign;
        uint32_t* u_array, *v_array, *r_array;
        u_array = ALLOCATE(u->length+1);
        v_array = ALLOCATE(v->length+1);
        r_array = ALLOCATE(v->length+1);
        copy_int_into_array(u, u_array);
        copy_int_into_array(v, v_array);
        sign = IS_NEGATIVE(u) ^ IS_NEGATIVE(v);
        len = hotpy_mpz_mod(u->length, u_array, v->length, v_array, r_array, sign);
        rem = make_int_from_array(r_array, len);
        if (IS_NEGATIVE(v))
            rem = negate_inplace(rem);
        DEALLOCATE(u_array, u->length+1);
        DEALLOCATE(v_array, v->length+1);
        DEALLOCATE(r_array, v->length+1);
        return rem;
    }
}



static R_int wrshift(R_int a, int words) {
    int i, len = a->length - words;
    R_int result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = 0; i < len; i++) {
        result->digits[i] = a->digits[i+words];
    }
    return result;
}

R_int long_right_shift(R_int a, int rsh) {
    R_int result;
    int i, words, r_bits, l_bits, r_mask, len, alast;
    words = rsh >> 5;
    r_bits = rsh & 31;
    len = a->length - words;
    if (len <= 0)
        return PY_SMALL_INT(0);
    if (r_bits == 0)
        return wrshift(a, words);
    l_bits = 32 - r_bits;
    r_mask = 0xffffffff >> r_bits;
    alast = a->length - 1;
    assert(rsh > 0);
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = 0; i < len-1; i++) {
        result->digits[i] = ((a->digits[i+words] >> r_bits) & r_mask) |
                            ((a->digits[i+words+1] << l_bits));
    }
    if (IS_NEGATIVE(a)) {
        result->digits[len-1] = ((a->digits[alast] >> r_bits) & r_mask) | 
                                (-1 << l_bits);
    } else {
        result->digits[len-1] = ((a->digits[alast] >> r_bits) & r_mask);
    }
    result = NORMALISE(result);
    return result;
}

int py_long_double_compare(R_int l, double d) {
    if (l->length == 1) {
        int i = l->digits[0];
        if (i > d)
            return 1;
        else if (i == d)
            return 0;
        else
            return -1;
    }
    assert(0 && "TO DO");
    return 0;

//    int fsign = d == 0.0 ? 0 : d < 0.0 ? -1 : 1;
//    int lsign = l->sign;
//    int i, exponent, nbits, len;
//    double mant;
//    int64_t x, y;
//    assert(!isnan(d));
//    if (isinf(d))
//        return -fsign;
//    if (fsign != lsign) {
//        return lsign - fsign;
//    }
//    if (l->length > 1000) /* |l| is larger than any possible d except inf */
//        return lsign;
//    nbits = number_of_bits(l);
//    if (nbits <= 48) {
//        double ld = double_from_py_int(l);
//        assert(!isnan(ld));
//        if (ld > d)
//            return 1;
//        else if (ld == d)
//            return 0;
//        else
//            return -1;
//    }
//    if (fsign < 0) {
//        d = -d;
//    }
//    mant = frexp(d, &exponent);
//    if (exponent < nbits)
//        return -1;
//    if (exponent > nbits) 
//        return 1;
//    x = (int64_t)ldexp(mant, 63);
//    y = 0;
//    len = l->length;
//    for (i = 1; i <= len && i <= 7; i++) {
//        y <<= SHIFT;
//        y |= l->digits[len-i];
//    }
//    for( ;i <= 7; i++)
//        y <<= SHIFT;
//    if (y > x)
//        return fsign;
//    else if (y == x)
//        return 0;
//    else
//        return -fsign;
}


int py_long_long_compare(R_int a, R_int b) {
    int len_a = a->length, len_b = b->length;
    if (len_a == len_b) {
        int i;
        int32_t ms_a = a->digits[len_a-1];
        int32_t ms_b = b->digits[len_a-1];
        if (ms_a < ms_b)
            return -1;
        else if (ms_a > ms_b)
            return 1;
        for (i = len_a -2; i >= 0; --i) {
            if (a->digits[i] != b->digits[i]) {
                if (a->digits[i] < b->digits[i])
                    return -1;
                else
                    return 1;
            }
        }
        return 0;
    }
    if (len_a > len_b) {
        if (IS_NEGATIVE(a))
            return -1;
        else
            return 1;
    } else {
        if (IS_NEGATIVE(b))
            return 1;
        else
            return -1;
    }
}

int py_long_compare1(R_int a, int32_t b) {
    if (LEN(a) > 1) {   
       if (IS_NEGATIVE(a))
            return -1;
        else
            return 1;  
    } else {
        int ai = a->digits[0];   
        if (ai < b)
            return -1;
        else if (ai > b)
            return 1;
        else
            return 0;  
    }
}

intptr_t hotpy_long_hash(R_int l) {
    uintptr_t hash = 0;
    int i;
    uintptr_t len = l->length;
    for (i = 0; i < len; i++) {
        hash ^= l->digits[i];
    }
    return hash;
}

R_int negate_long(R_int i) {
    uintptr_t len = i->length;
    R_int result;
    int j;
    ASSERT_IS_LONG(i);
    if (len == 1) {
        int val = i->digits[0];
        if (val != MIN_INTEGER)
            return R_int_from_ssize_t(-val);
    }
    return negate(i);
}

R_int invert_long(R_int i) {
    uintptr_t len = i->length;
    R_int result;
    int j;
    ASSERT_IS_LONG(i);
    if (len == 1) {
        int val = i->digits[0];
        return R_int_from_ssize_t(~val);
    } else {
        result = gc_allocate_int(len);
        SET_TYPE(result, type_int);
        result->length = len;
        for (j = 0; j < len; j++) {
            result->digits[j] = ~(i->digits[j]);
        }
        return result;
    }
}

R_int and_long_long(R_int a, R_int b) {
    R_int result;
    int i, alen, len, sign;
    if (a->length + b->length == 2) {
        int val = a->digits[0] & b->digits[0];
        return R_int_from_ssize_t(val);
    }
    if (a->length > b->length) {
        R_int t;
        t = a;
        a = b;
        b = t;
    }
    alen = a->length;
    if (IS_NEGATIVE(a)) {
        len = b->length;
    } else {
        len = alen;
    }
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = 0; i < alen; i++) {
        result->digits[i] = (a->digits[i] & b->digits[i]);
    }
    for (; i < len; i++) {   
        result->digits[i] = b->digits[i];
    }
    result = NORMALISE(result);
    return result;
}

R_int and_long1(R_int a, int b) {
    R_int result;
    int i, len = a->length;
    if (b >= 0) {
        int x = a->digits[0] & b;
        return (R_int)gvmt_tag((x << 1)+1);
    }
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    result->digits[0] = a->digits[0] & b;
    for (i = 1; i < len; i++) {
        result->digits[i] = a->digits[i];
    }
    result = NORMALISE(result);
    return result;
}

R_int or_long_long(R_int a, R_int b) {
    R_int result;
    int i, len, alen;
    if (a->length + b->length == 2) {
        int val = a->digits[0] | b->digits[0];
        return R_int_from_ssize_t(val);
    }
    if (a->length > b->length) {
        R_int t;
        t = a;
        a = b;
        b = t;
    }
    alen = a->length;
    if (IS_NEGATIVE(a)) {
        len = alen;
    } else {
        len = b->length;
    }
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = 0; i < alen; i++) {   
        result->digits[i] = (a->digits[i] | b->digits[i]);
    }
    for (; i < len; i++) {   
        result->digits[i] = b->digits[i];
    }
    result = NORMALISE(result);
    return result;
}

R_int or_long1(R_int a, int b) {
    R_int result;
    int i, len = a->length;
    if (b < 0) {
        int x = a->digits[0] | b;
        return (R_int)gvmt_tag((x << 1)+1);
    }
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    result->digits[0] = a->digits[0] | b;
    for (i = 1; i < len; i++) {
        result->digits[i] = a->digits[i];
    }
    result = NORMALISE(result);
    return result;
}

R_int xor_long_long(R_int a, R_int b) {
    R_int result;
    int i, len, alen,ext;
    if (a->length + b->length == 2) {
        int val = a->digits[0] ^ b->digits[0];
        return R_int_from_ssize_t(val);
    }
    if (a->length > b->length) {
        R_int t;
        t = a;
        a = b;
        b = t;
    }
    alen = a->length;
    len = b->length;
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    for (i = 0; i < alen; i++) {   
        result->digits[i] = (a->digits[i] ^ b->digits[i]);
    }
    if (IS_NEGATIVE(a)) {
        ext = -1;
    } else {
        ext = 0;
    }
    for (; i < len; i++) {   
        result->digits[i] = b->digits[i] ^ ext;
    }
    result = NORMALISE(result);
    return result;
}

R_int xor_long1(R_int a, int b) {
    R_int result;
    int i, ext, len = a->length;
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    ext = -(b < 0);
    result->digits[0] = a->digits[0] ^ b;
    for (i = 1; i < len; i++) {   
        result->digits[i] = a->digits[i] ^ ext;
    }
    result = NORMALISE(result);
    return result;
}

static char digit_chars[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static R_str int_binary_str(R_int a) {
    assert(0 && "TO DO");   
    return 0;
}

static R_str int_hex_str(R_int a) {
    assert(0 && "TO DO");
    return 0;
}

R_str long_to_str(R_int a, int32_t radix) {
    R_str s;
    GVMT_NO_GC_BLOCK_BEGIN
    if (radix == 2) {
        s = int_binary_str(a);
    } else if (radix == 16) { 
        s = int_hex_str(a);
    } else if (a->length == 1) { 
        char buf[16];
        sprintf(buf, "%d", a->digits[0]);
        s = string_from_c_string(buf);
    } else { 
        R_int q; 
        int sign = 0;
        double char_per_word = log(0xffffffff)/log(radix);
        int i, power = 1;
        int char_estimate = char_per_word*(a->length+1);
        char buf[100];
        char* out;
        void *p;
        int32_t radix_power = radix;
        while (radix_power*radix < (1 << 16)) {
            radix_power *= radix;
            power++;
        }
        if (char_estimate >= 100) {
            p = malloc(char_estimate+1);
            out = ((char*)p) + char_estimate;
        } else {
            p = 0;
            out = buf + char_estimate;
        }
        *out = '\0';
        if (IS_NEGATIVE(a)) {
            q = negate(a);
            sign = 1;
        } else {
            q = a;
        }
        do {
            int32_t rem = divmod1(q, radix_power, &q);
            for (i = 0; i < power; i++) {
                --out;
                *out = digit_chars[rem % radix];
                rem /= radix;
            }
            gvmt_gc_safe_point(); 
        } while (!gvmt_is_tagged(q));
        { 
            int quot = as_int(q);
            while (quot) {
                int32_t rem = quot % radix_power;
                quot /= radix_power;
                for (i = 0; i < power; i++) {
                    --out;
                    *out = digit_chars[rem % radix];
                    rem /= radix;
                }
            }
        }
        while(*out == '0')
            out++;
        if (sign) {
            --out;
            *out = '-';
        }
        s = string_from_c_string(out);
        if (p) {
            assert(out >= (char*)p);
            free(p);
        } else {
            assert(out >= buf);  
        }
    }
    GVMT_NO_GC_BLOCK_END
    return s;
}


