#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "hotpy.h"

R_object PY_FUNC0(clock) {
    R_float result = gc_allocate(float);
    SET_TYPE(result, type_float);
    result->value = high_res_time();
    return (R_object)result;
}

R_object PY_FUNC0(time) {
    R_float result = gc_allocate(float);
    double c = clock();
    SET_TYPE(result, type_float);
    result->value = c/CLOCKS_PER_SEC;
    return (R_object)result;
}

