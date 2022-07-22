/* This file is automatically generated */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <alloca.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include "hotpy.h"

#include "operators.h"
extern int isnan(double d);

extern double pow(double d1, double d2);

R_object PY_FUNC1(pos_float, R_object o) {
    return o;
}

R_object PY_FUNC1(NoneType_bool, R_object o) {
    return (R_object)FALSE;
}

R_object PY_FUNC2(no_add, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for +: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(is_, R_object x, R_object y) {
    return (R_object)booleans[x == y];   
}

R_object PY_FUNC2(is_not, R_object x, R_object y) {
    return (R_object)booleans[x != y];   
}

R_object PY_FUNC2(unorderable_types, R_object x, R_object y) {
    R_str msg = py_sprintf("unorderable types: %s %s", 
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_sub, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for -: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_mul, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for *: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_truediv, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for /: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_mod, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for %%: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_pow, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for **: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_and_, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for &: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_or_, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for |: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_xor, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for ^: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_lshift, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for <<: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_rshift, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for >>: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(no_floordiv, R_object x, R_object y) {
    R_str msg = py_sprintf("unsupported operand types for //: '%s' and '%s'",
                           TYPE_NAME(x), TYPE_NAME(y));
    raise_str(type_TypeError, msg);
    return 0;
}

R_object PY_FUNC2(tracing_function, R_operator op, R_function f) {
    op->python_equivalent = f;
    return (R_object)None;
}

R_object PY_FUNC2(mod_int_float, R_int p0, R_float p1) {
    double v0 = double_from_py_int(p0);
    double v1 = p1->value;
    double quot, rem;
    R_float result;
    if (v1 == 0.0) zero_division_error("float modulo");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    rem = v0 - quot * v1;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = rem;
    return (R_object)result;
}

R_object PY_FUNC2(mod_float_int, R_float p0, R_int p1) {
    double v0 = p0->value;
    double v1 = double_from_py_int(p1);
    double quot, rem;
    R_float result;
    if (v1 == 0.0) zero_division_error("float modulo");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    rem = v0 - quot * v1;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = rem;
    return (R_object)result;
}

R_object PY_FUNC2(mod_float_float, R_float p0, R_float p1) {
    double v0 = p0->value;
    double v1 = p1->value;
    double quot, rem;
    R_float result;
    if (v1 == 0.0) zero_division_error("float modulo");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    rem = v0 - quot * v1;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = rem;
    return (R_object)result;
}

R_object PY_FUNC2(floordiv_int_float, R_int p0, R_float p1) {
    double v0 = double_from_py_int(p0);
    double v1 = p1->value;
    double quot;
    R_float result;
    if (v1 == 0.0) zero_division_error("float division");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = quot;
    return (R_object)result;
}

R_object PY_FUNC2(floordiv_float_int, R_float p0, R_int p1) {
    double v0 = p0->value;
    double v1 = double_from_py_int(p1);
    double quot;
    R_float result;
    if (v1 == 0.0) zero_division_error("float division");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = quot;
    return (R_object)result;
}

R_object PY_FUNC2(floordiv_float_float, R_float p0, R_float p1) {
    double v0 = p0->value;
    double v1 = p1->value;
    double quot;
    R_float result;
    if (v1 == 0.0) zero_division_error("float division");
    GVMT_NO_GC_BLOCK_BEGIN 
    quot = floor(v0/v1);
    GVMT_NO_GC_BLOCK_END
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = quot;
    return (R_object)result;
}

extern R_object PY_FUNC2(add_int_int, R_int i0, R_int i1); 
R_object PY_FUNC2(add_int_float, R_int p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    value = double_from_py_int(p0) + p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(add_float_int, R_float p0, R_int p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    value = p0->value + double_from_py_int(p1);
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(add_float_float, R_float p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    value = p0->value + p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

extern R_object PY_FUNC2(sub_int_int, R_int i0, R_int i1); 
R_object PY_FUNC2(sub_int_float, R_int p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    value = double_from_py_int(p0) - p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(sub_float_int, R_float p0, R_int p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    value = p0->value - double_from_py_int(p1);
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(sub_float_float, R_float p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    value = p0->value - p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

extern R_object PY_FUNC2(mul_int_int, R_int i0, R_int i1); 
R_object PY_FUNC2(mul_int_float, R_int p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    value = double_from_py_int(p0) * p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(mul_float_int, R_float p0, R_int p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    value = p0->value * double_from_py_int(p1);
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(mul_float_float, R_float p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    value = p0->value * p1->value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

extern R_object PY_FUNC2(and__int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(or__int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(xor_int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(rshift_int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(mod_int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(lshift_int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(floordiv_int_int, R_int i0, R_int i1); 
extern R_object PY_FUNC2(truediv_int_int, R_int i0, R_int i1); 
R_object PY_FUNC2(truediv_int_int, R_int p0, R_int p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    value = double_from_py_int(p1);
    if (value == 0.0) zero_division_error("integer division");
    value = double_from_py_int(p0) / value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(truediv_int_float, R_int p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    value = p1->value;
    if (value == 0.0) zero_division_error("float division");
    value = double_from_py_int(p0) / value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(truediv_float_int, R_float p0, R_int p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    value = double_from_py_int(p1);
    if (value == 0.0) zero_division_error("float division");
    value = p0->value / value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(truediv_float_float, R_float p0, R_float p1) {
    R_float result;
    double value;
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    value = p1->value;
    if (value == 0.0) zero_division_error("float division");
    value = p0->value / value;
    result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = value;
    return (R_object)result;
}

R_object PY_FUNC2(eq_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 == i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp == 0]; 
}

R_object PY_FUNC2(eq_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp == 0]; 
}

R_object PY_FUNC2(eq_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp == 0]; 
}

R_object PY_FUNC2(eq_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 == d1];
}

R_object PY_FUNC2(ne_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 != i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp != 0]; 
}

R_object PY_FUNC2(ne_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp != 0]; 
}

R_object PY_FUNC2(ne_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp != 0]; 
}

R_object PY_FUNC2(ne_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 != d1];
}

R_object PY_FUNC2(lt_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 < i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp < 0]; 
}

R_object PY_FUNC2(lt_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp < 0]; 
}

R_object PY_FUNC2(lt_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp < 0]; 
}

R_object PY_FUNC2(lt_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 < d1];
}

R_object PY_FUNC2(gt_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 > i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp > 0]; 
}

R_object PY_FUNC2(gt_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp > 0]; 
}

R_object PY_FUNC2(gt_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp > 0]; 
}

R_object PY_FUNC2(gt_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 > d1];
}

R_object PY_FUNC2(le_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 <= i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp <= 0]; 
}

R_object PY_FUNC2(le_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp <= 0]; 
}

R_object PY_FUNC2(le_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp <= 0]; 
}

R_object PY_FUNC2(le_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 <= d1];
}

R_object PY_FUNC2(ge_int_int, R_int p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, int);
    i0 = (intptr_t)gvmt___untag__((GVMT_Object)p0);
    i1 = (intptr_t)gvmt___untag__((GVMT_Object)p1);
    if (i0 & i1 & 1)
        return (R_object)booleans[i0 >= i1];
    else                       
        cmp = py_int_int_compare(p0, p1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp >= 0]; 
}

R_object PY_FUNC2(ge_int_float, R_int p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, int);
    TYPE_ASSERT(p1, float);
    d1 = p1->value;
    if (d1 != d1) return (R_object)FALSE;
    cmp = py_int_double_compare(p0, d1);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp >= 0]; 
}

R_object PY_FUNC2(ge_float_int, R_float p0, R_int p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, int);
    d0 = p0->value;
    if (d0 != d0) return (R_object)FALSE;
    cmp = -py_int_double_compare(p1, d0);
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[cmp >= 0]; 
}

R_object PY_FUNC2(ge_float_float, R_float p0, R_float p1) {
    int cmp;
    intptr_t i0, i1;
    double d0, d1;
    GVMT_NO_GC_BLOCK_BEGIN 
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    d0 = p0->value;
    d1 = p1->value;
    if (d0 != d0 || d1 != d1) return (R_object)FALSE;
    GVMT_NO_GC_BLOCK_END
    return (R_object)booleans[d0 >= d1];
}

R_object PY_FUNC2(lt_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object)booleans[string_compare(p0, p1) < 0];
}

R_object PY_FUNC2(gt_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object)booleans[string_compare(p0, p1) > 0];
}

R_object PY_FUNC2(le_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object)booleans[string_compare(p0, p1) <= 0];
}

R_object PY_FUNC2(ge_str_str, R_str p0, R_str p1) {
    TYPE_ASSERT(p0, str);
    TYPE_ASSERT(p1, str);
    return (R_object)booleans[string_compare(p0, p1) >= 0];
}

R_object PY_FUNC2(cmp_float_float, R_float p0, R_float p1) {
    TYPE_ASSERT(p0, float);
    TYPE_ASSERT(p1, float);
    if (p0->value == p1->value) {
        return (R_object)PY_SMALL_INT(0);
    } else {
        if (p0->value < p1->value) {
            return (R_object)PY_SMALL_INT(-1);
        } else {
            return (R_object)PY_SMALL_INT(1);
        }
    }
}

