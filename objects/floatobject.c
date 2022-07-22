#include "hotpy.h"
#include <math.h>
#include <errno.h>
#include <malloc.h>

extern int isnan(double d);
extern int isinf(double d);
extern double frexp(double d, int* i);
extern double ldexp(double d, int i);

R_float make_float(void) { 
    R_float result = gc_allocate(float);
    SET_TYPE(result, type_float);
    return result;
}

intptr_t hotpy_float_hash(R_float f) {
    double v = f->value;   
	double intpart, fractpart;
	int expo;
	intptr_t hipart;
	intptr_t x;		/* the final hash value */
    GVMT_NO_GC_BLOCK_BEGIN 
        fractpart = modf(v, &intpart);
        if (fractpart == 0.0) {
            if (intpart > LONG_MAX || -intpart > LONG_MAX) {
                R_int plong;
                if (isinf(intpart))
                    v = v < 0 ? -271828.0 : 314159.0;
                plong = py_int_from_double(intpart);
                x = hotpy_int_hash(plong);
            } else {
                /* Fits in a C long == a Python int, so is its own hash. */
                x = (intptr_t)intpart;
            }
        } else {
            v = frexp(v, &expo);
            v *= 2147483648.0;	/* 2**31 */
            hipart = (long)v;	/* take the top 32 bits */
            v = (v - (double)hipart) * 2147483648.0; /* get the next 32 bits */
            x = hipart + (long)v + (expo << 15);
        }
    GVMT_NO_GC_BLOCK_END
    return x;
}

R_object PY_FUNC1(__bool___float, R_float f) { 
    TYPE_ASSERT(f, float);
    return (R_object)booleans[f->value != 0.0];
}

R_str float_to_str(R_float f) { 
    char buf[32];
    TYPE_ASSERT(f, float);
    sprintf(buf, "%f", f->value);
    return string_from_c_string(buf);
}

R_object PY_FUNC1(__str___float, R_float f) { 
    return (R_object)float_to_str(f);
}

R_float to_float(R_object obj) {
    R_float f;
    if (TYPE(obj) == type_float) {
        f = (R_float)obj;
    } else {
        f = (R_float)vm_call_special(obj, name___float__);
        if (f == 0 || !isinstance(f, type_float)) {
            R_str msg = py_sprintf("%%d format: a number is required, not %s",
                                   TYPE_NAME(obj));
            raise_str(type_TypeError, msg);
        }
    }
    return f;
}

R_object PY_FUNC1(neg_float, R_float f) {
    double d0 = f->value;
    R_float n = make_float();
    TYPE_ASSERT(f, float);
    n->value = -d0;
    return (R_object)n;
}

R_object PY_FUNC2(pow_int_float, R_int i0, R_float f1) {
    long p0 = double_from_py_int(i0);
    double p1 = f1->value;
    double result;
    R_float f;
    GVMT_NO_GC_BLOCK_BEGIN
        result = pow(p0, p1);
    GVMT_NO_GC_BLOCK_END
    f = make_float();
    f->value = result;
    return (R_object)f;
}

R_object PY_FUNC2(pow_float_int, R_float f0, R_int i1) {
    double p0 = f0->value;
    double p1 = double_from_py_int(i1);
    double result;
    R_float f;
    GVMT_NO_GC_BLOCK_BEGIN
        result = pow(p0, p1);
    GVMT_NO_GC_BLOCK_END
    f = make_float();
    f->value = result;
    return (R_object)f;
}

R_object PY_FUNC2(pow_float_float, R_float f0, R_float f1) {
    double p0 = f0->value;
    double p1 = f1->value;
    double result;
    R_float f;
    GVMT_NO_GC_BLOCK_BEGIN
        result = pow(p0, p1);
    GVMT_NO_GC_BLOCK_END
    f = make_float();
    f->value = result;
    return (R_object)f;
}

R_object zero_division_error(char* msg) {
    raise_char(type_ZeroDivisionError, msg);
    return NULL;
}

extern double os_strtod(const char* s, double* d);

R_float parse_float(R_str s, R_type overflow) {
    R_float f;
    double d;
    char* buffer = malloc(LEN(s)+1);
    int err;
    py_print(buffer, s);
    buffer[LEN(s)] = 0;
    err = os_strtod(buffer, &d);
    free(buffer);
    if (err) {
        R_str msg;
        if (err == EINVAL) {
            msg = py_sprintf("could not convert string to float: %s", s);
            raise_str(type_ValueError, msg);
        } if (fabs(d) >= 1.0 && overflow != NULL) {
            assert(err == ERANGE);
            msg = py_sprintf("value too large to convert to float: %s", s);
            raise_str(overflow, msg);
        }
    }
    f = make_float();
    f->value = d;
    return f;
}


