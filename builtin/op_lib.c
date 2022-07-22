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

