#include "hotpy.h"
#include <math.h>

R_object PY_FUNC1(math_sqrt, R_object o) {
    R_float f = to_float(o);
    R_float s = make_float();
    GVMT_NO_GC_BLOCK_BEGIN 
        s->value = sqrt(f->value);
    GVMT_NO_GC_BLOCK_END
    return (R_object)s;
}

