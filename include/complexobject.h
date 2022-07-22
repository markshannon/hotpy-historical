#ifndef HOTPY_COMPLEX_OBJECT_H
#define HOTPY_COMPLEX_OBJECT_H

GVMT_OBJECT(complex) {
    GVMT_OBJECT(type)* ob_type;    /* read-only */
    uintptr_t ref_count;
    double real;
    double imag;
};

#endif
