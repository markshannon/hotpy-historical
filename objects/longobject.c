#include "hotpy.h"
#include <math.h>
#include <assert.h>
#include "intobject.h"
#include "longobject.h"
#include <malloc.h>
#include "function.h"

#define ASSERT_IS_LONG(i) assert((!gvmt_is_tagged(i)) && ((i)->ob_type == type_int))
                                                             
/**** Use mpz for big numbers ********/

extern unsigned hotpy_mpz_div(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* q, int round_up);

extern unsigned hotpy_mpz_mod(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* r, int round_up);

extern unsigned hotpy_mpz_mul(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* r);

extern unsigned hotpy_mpz_mul_ui(int ulen, uint32_t* u, unsigned v, uint32_t* r);

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define MIN_INTEGER  (1 << 31)

#ifdef NDEBUG
# define CHECK(x)
# define gc_allocate_int(count) gc_allocate_var(int, digits, count)
#else
# define CHECK(x) check_int(x)

static R_int gc_allocate_int(int size) {
    assert(size > 0);
    return gc_allocate_var(int, digits, size);
}

#endif

#define overflow_add(sum, a, b) ((((sum)^(a))&((sum)^(b))) < 0) 
#define overflow_sub(diff, a, b) ((((a)^(b))&((diff)^(a))) < 0)

#define carry(sum, a) (((uint32_t)sum)<((uint32_t)a))
#define borrow(a, b) (((uint32_t)a)<((uint32_t)b)) 

static R_int negate(R_int a);

#define IS_NORMALISED(i)  \
    ((i)->length == 1 || (-((int32_t)(i)->digits[(i)->length-1]) != ((uint32_t)i->digits[(i)->length-2]) >> 31))

#define NORMALISE(i) (IS_NORMALISED(i) ? (i) : _normalise(i))

    
static void check_int(R_int i) {
    int j;
    assert(IS_INT(i));   
    assert(i->length > 0);
    assert(IS_NORMALISED(i));
}

/** Return a new normalised int from this unmormalised int. 
 *  (Must be used only by the macro )
 */
static R_int _normalise(R_int i) {
    R_int n;
    int j, len;
    assert(!IS_NORMALISED(i));
    ASSERT_IS_LONG(i);
    len = i->length;
    do {
        len--;
    } while (len > 1 && -((int32_t)i->digits[len-1]) == ((uint32_t)i->digits[len-2]) >> 31);
    if (len == 1) {
        int32_t x = i->digits[0];
        return R_int_from_ssize_t(x);
    } else {
        n = gc_allocate_int(len);
        SET_TYPE(n, type_int);
        n->length = len;
        for(j = 0; j < len; j++)
            n->digits[j] = i->digits[j];
        CHECK(n);
        return n;
    }
}

/* Since this has overflowed, digits[1] must be 
 * made the oposite sign from digits[0] */
static R_int py_int_from_overflow(intptr_t i) {
    R_int result;
    result = gc_allocate_int(2);
    SET_TYPE(result, type_int);
    result->length = 2;
    result->digits[0] = i;
    result->digits[1] = (i < 0)-1;
    CHECK(result);
    return result;
}

static R_int extend(R_int a, int32_t msw) {
    size_t i, len = LEN(a);
    R_int x = gc_allocate_int(len+1);
    SET_TYPE(x, type_int);
    ZERO_REF_COUNT(x);
    x->length = len+1;
    gvmt_fully_initialized((GVMT_Object)x);
    for (i = 0; i < len; i++) {
        x->digits[i] = a->digits[i];
    }
    x->digits[len] = msw;
    CHECK(x);
    return x;
}

R_int _boxed_int_from_ssize_t(intptr_t i) {
    R_int result;
    result = gc_allocate_int(1);
    SET_TYPE(result, type_int);
    result->length = 1;
    result->digits[0] = i;
    CHECK(result);
    return result;
}

R_int negate_inplace(R_int l) {
    int c, i, last, inv, len = l->length;
    ASSERT_IS_LONG(l);
    if (len == 1 && l->digits[0] != MIN_INTEGER) {
        int32_t d = l->digits[0];
        l->digits[0] = -d;
        return l;
    }
    c = 1;
    i = 0;
    do {
        int32_t t;
        inv = ~l->digits[i];
        t = inv + c;
        c = carry(t, inv);
        l->digits[i] = t;
        i++;
    } while(c && i < len);
    while (i < len) {
        inv = ~l->digits[i];
        l->digits[i] = inv;
        i++;
    }
    last = l->digits[len-1];
    if (overflow_add(last, inv, 0)) {
        return extend(l, 1-(last >> 31));   
    } else {
        return l;   
    }
}

static void invert_inplace(R_int l) {
    int j, len = l->length;
    ASSERT_IS_LONG(l);
    for (j = 0; j < len; j++) {
        l->digits[j] = ~(l->digits[j]);
    }
}

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
        result = negate_inplace(result);
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

R_int add_long1(R_int a, int32_t b) {
    int i;
    R_int result;
    int32_t digit = 0, ext = -(b < 0);
    size_t len = a->length;
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    ZERO_REF_COUNT(result);
    result->length = len;
    gvmt_fully_initialized((GVMT_Object)result);
    for (i = 0; i < len; i++) { 
        int64_t t = ((int64_t)a->digits[i]) + b;
        b = ((int32_t)(t >> 32)) + ext;
        digit = t;
        result->digits[i] = digit;
    }
    if (b != -(digit<0))
        return extend(result, b);
    else 
        return NORMALISE(result);
}

R_int add_long_long(R_int a, R_int b) {
    R_int result;
    ASSERT_IS_LONG(a);
    ASSERT_IS_LONG(b);
    if (a->length + b->length == 2) {
        int a0 = a->digits[0];
        int b0 = b->digits[0];
        int val = a0 + b0;
        if (overflow_add(val, a0, b0)) 
            return py_int_from_overflow(val);
        else       
            return _boxed_int_from_ssize_t(val);
    } else {
        int i, t = 0;
        int len, alen, last, c;
        int32_t ai = 0, bi = 0, ext;
        if (a->length > b->length) {
            R_int tmp;
            tmp = a;
            a = b;
            b = tmp;
        }
        len = b->length;
        alen = a->length;
        result = gc_allocate_int(len);
        SET_TYPE(result, type_int);
        ZERO_REF_COUNT(result);
        result->length = len;
        gvmt_fully_initialized((GVMT_Object)result);
        c = 0;
        for (i = 0; i < alen; i++) { 
            uint64_t t;
            t = a->digits[i];
            bi = b->digits[i];
            t = t + bi + c;
            c = t >> 32;
            result->digits[i] = t;
        }
        if (IS_NEGATIVE(a)) {
            for(; i < len; i++) {
                ai = -1;
                bi = b->digits[i];
                t = ai + c;
                c = carry(t, ai);
                t = t + bi;
                c |= carry(t, bi);
                result->digits[i] = t;
            }
            ext = -IS_NEGATIVE(b)-1+c;
        } else {
            for(; i < len; i++) {
                bi = b->digits[i];
                t = b->digits[i] + c;   
                c = carry(t, bi);
                result->digits[i] = t;
            }
            ext = -IS_NEGATIVE(b)+c;
        }
        last = result->digits[len-1];
        if ((-ext) != ((uint32_t)last) >> 31) {
            return extend(result, ext);
        } else {
            result = NORMALISE(result);
            return result;
        }
    }
}

R_int sub_long_long(R_int a, R_int b) {
    R_int result;
    if (a->length + b->length == 2) {
        int a0 = a->digits[0];
        int b0 = b->digits[0];
        int val = a0 - b0;
        if (overflow_sub(val, a0, b0))
            return py_int_from_overflow(val);
        else       
            return _boxed_int_from_ssize_t(val);
    } else {       
        int i, len, slen, c= 0;
        int32_t t;
        int32_t ai, bi;
        if (a->length > b->length) {
            len = a->length;
            slen = b->length;
        } else {
            len = b->length;
            slen = a->length;
        }
        result = gc_allocate_int(len);
        SET_TYPE(result, type_int);
        result->length = len;
        i = 0;
        do {
            int32_t t2;
            ai = a->digits[i];
            bi = b->digits[i];
            t2 = ai - c;
            c = borrow(ai, c);
            t = t2 - bi;   
            c |= borrow(t2, bi);
            result->digits[i] = t;
            i++;
        } while (i < slen);
        if (a->length > slen) {
            bi = -IS_NEGATIVE(b);
            for(; i < len; i++) {
                int32_t t2;
                ai = a->digits[i];
                t2 = ai - c;
                c = borrow(ai, c);
                t = t2 - bi;   
                c |= borrow(t2, bi);
                result->digits[i] = t;
            }
        } else {
            ai = -IS_NEGATIVE(a);
            for(; i < len; i++) {
                int32_t t2;
                bi = b->digits[i];
                t2 = ai - c;
                c = borrow(ai, c);
                t = t2 - bi;   
                c |= borrow(t2, bi);
                result->digits[i] = t;
            }
        }
        if (overflow_sub(t, ai, bi)) {
            R_int x = gc_allocate_int(len+1);
            SET_TYPE(x, type_int);
            x->length = len+1;
            for (i = 0; i < len; i++) {
                x->digits[i] = result->digits[i];
            }
            x->digits[len] = -(int32_t)(result->digits[len-1] >> 31);
            CHECK(x);
            return x;
        } else {
            result = NORMALISE(result);
            return result;
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
    ASSERT_IS_LONG(a);
    ASSERT_IS_LONG(b);
    result = gc_allocate_int(alen + blen);
    SET_TYPE(result, type_int);
    result->length = alen + blen;
    for (j = 0; j < blen; j++) {
        uint32_t k = 0;
        for (i = 0; i < alen; i++) {
            uint64_t t = a->digits[i];
            t = t * b->digits[j] + result->digits[i+j] + k;
            result->digits[i+j] = (int32_t)t;
            k = (int32_t)(t >> 32);
        }
        result->digits[j+alen] = k;
    } 
    // Correct for negative values.
    if (IS_NEGATIVE(a)) {
        uint64_t bor = 0;
        for (j = 0; j < blen; j++) {
            uint64_t t = result->digits[j+alen];
            t = t - b->digits[j] - bor;
            result->digits[j+alen] = (uint32_t)t;
            bor = (uint32_t)(t >> 63);
        }
    }
    if (IS_NEGATIVE(b)) {
        uint64_t bor = 0;
        for (i = 0; i < alen; i++) {
            uint64_t t = result->digits[i+blen];
            t = t - a->digits[i] - bor;
            result->digits[i+blen] = (uint32_t)t;
            bor = (uint32_t)(t >> 63);
        }
    }
    result = NORMALISE(result);
    return result;
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

R_int mul_long1(R_int a, int32_t b) {
    R_int prod;
    int len, m_a, m_b, m_x;
    uint32_t* a_array, *b_array, *x_array;
    if (b == 0)
        return PY_SMALL_INT(0);
    a_array = ALLOCATE(a->length+1);
    x_array = ALLOCATE(a->length + 3);
    copy_int_into_array(a, a_array);
    if (b < 0)
        len = hotpy_mpz_mul_ui(a->length, a_array, -b, x_array);
    else
        len = hotpy_mpz_mul_ui(a->length, a_array, b, x_array);
    assert(len <= (a->length + 1));
    if (((int32_t)x_array[len-1]) < 0) {
        x_array[len] = 0;
        len ++;
    }
    prod = make_int_from_array(x_array, len);
    assert(!IS_NEGATIVE(prod));
    DEALLOCATE(a_array, a->length+1);
    DEALLOCATE(x_array, a->length + 3);
    if (IS_NEGATIVE(a) ^ (b < 0))
        return negate_inplace(prod);
    else
        return prod;
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

R_int long_left_shift(R_int a, int lsh) {
    int i, words, l_bits, r_bits, r_mask, len, alast;
    R_int result;
    l_bits = lsh & 31;
    words = (lsh+31) >> 5;
    if (l_bits == 0)
        return wlshift(a, words);
    r_bits = 32 - l_bits;
    r_mask = 0xffffffff >> r_bits;
    len = a->length + words;
    alast = a->length - 1;
    assert(lsh > 0);
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    result->digits[words-1] = (a->digits[0] << l_bits);
    for (i = words; i < len-1; i++) {
        result->digits[i] = ((a->digits[i-words-1] >> r_bits) & r_mask) |
                            ((a->digits[i-words] << l_bits));
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

static R_int negate(R_int a) {
    R_int result; 
    int i, len, c;
    int32_t inv, t;
    len = a->length;
    if (len == 1 && a->digits[0] != MIN_INTEGER) {
        return _boxed_int_from_ssize_t(-(int32_t)a->digits[0]);
    }
    result = gc_allocate_int(len);
    SET_TYPE(result, type_int);
    result->length = len;
    c = 1;
    i = 0;
    do {
        inv = ~a->digits[i];
        t = inv + c;
        c = carry(t, inv);
        result->digits[i] = t;
        i++;
    } while(c && i < len);
    while(i < len) {
        inv = ~a->digits[i];
        t = inv;
        result->digits[i] = inv;
        i++;
    }    
    if (overflow_add(t, inv, 0)) {
        result = extend(result, 1-(result->digits[len-1] >> 31));
    }
    CHECK(result);
    return result;
}

int32_t divmod1(R_int u, int32_t n, R_int* q) {
    int32_t k = 0;
    int j, ulen = u->length, sign = 0;
    R_int quot = gc_allocate_int(ulen);
    SET_TYPE(quot, type_int);
    quot->length = ulen;
    if (IS_NEGATIVE(u)) {
        u = negate(u);
        sign = !sign;
    }
    if (n < 0) {
        assert(n != INT_MIN);
        n = -n;
        sign = !sign;
    }
    assert(n < (1 << 16) && n > 0);
    for(j = ulen-1; j >= 0; j--) {
        uint32_t d = u->digits[j];
        uint16_t bot, top = d >> 16;
        top = ((k<<16) + (d >> 16))/n;
        k = ((k<<16) + (d >> 16)) - top*n;
        bot = ((k<<16) + (d & 0xffff))/n;
        k = ((k<<16) + (d & 0xffff)) - bot*n;
        quot->digits[j] = (((uint32_t)top) << 16) | bot;
    }
    if (IS_NEGATIVE(u)) {
        k = -k;
    }
    if (sign) {
        if (k) {
            invert_inplace(quot);
            k = k+n;
        } else {
            negate_inplace(quot);
        }
    }
    if (LEN(quot) == 1)
        quot = R_int_from_ssize_t(quot->digits[0]);   
    else
        quot = NORMALISE(quot);
    *q = quot;
    return k;
}
 
R_int floordiv_long_long(R_int u, R_int v) {
    if (u->length + v->length == 2) {
        int ui, vi, val;
        if (v->digits[0] == 0)
            zero_division_error("integer division");
        ui = u->digits[0];
        vi = v->digits[0];
        val = ui/vi;
        if (((ui < 0) ^ (vi < 0)) && vi*val != ui) 
            val--;
        return R_int_from_ssize_t(val);
    } else if (v->length == 1 && ((int16_t)v->digits[0]) == (int32_t)v->digits[0]) {
        R_int quot;
        divmod1(u, v->digits[0], &quot);
        return quot;
    } else {
        R_int quot;
        int qlen, len, sign;
        uint32_t* u_array, *v_array, *q_array;
        u_array = ALLOCATE(u->length+1);
        v_array = ALLOCATE(v->length+1);
        qlen = u->length - v->length + 2;
        if (qlen < 1) qlen = 1;
        q_array = ALLOCATE(qlen);
        copy_int_into_array(u, u_array);
        copy_int_into_array(v, v_array);
        sign = IS_NEGATIVE(u) ^ IS_NEGATIVE(v);
        len = hotpy_mpz_div(u->length, u_array, v->length, v_array, q_array, sign);
        quot = make_int_from_array(q_array, len);
        if (sign)
            quot = negate_inplace(quot);
        DEALLOCATE(u_array, u->length+1);
        DEALLOCATE(v_array, v->length+1);
        DEALLOCATE(q_array, qlen);
        return quot;
    }
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


