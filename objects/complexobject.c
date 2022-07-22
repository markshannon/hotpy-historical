#include <assert.h>
#include <stdlib.h>
#include "hotpy.h"


R_object PY_FUNC1(neg_complex, R_complex c) {
    R_complex neg = gc_allocate(complex);
    SET_TYPE(neg, type_complex);
    neg->real = -c->real;
    neg->imag = -c->imag;
    return (R_object)neg;
}

R_object PY_FUNC1(add_complex, R_complex a, R_complex b) {
    R_complex r = gc_allocate(complex);
    SET_TYPE(r, type_complex);
	r->real = a->real + b->real;
	r->imag = a->imag + b->imag;
    return (R_object)r;
}

R_object PY_FUNC1(add_complex, R_complex a, R_complex b) {
    R_complex r = gc_allocate(complex);
    SET_TYPE(r, type_complex);
	r->real = a->real - b->real;
	r->imag = a->imag - b->imag;
    return (R_object)r;
}

R_object PY_FUNC1(mul_complex, R_complex a, R_complex b) {
    R_complex r = gc_allocate(complex);
    SET_TYPE(r, type_complex);
	r->real = a->real*b->real - a->imag*b->imag;
	r->imag = a->real*b->imag + a->imag*b->real;
    return (R_object)r;
}

R_object PY_FUNC1(div_complex, R_complex a, R_complex b) {
    R_complex r = gc_allocate(complex);
    double areal = a->real;
    double aimag = a->imag;
    double breal = b->real;
    double bimag = b->imag;
	double abs_breal = breal < 0 ? -breal : breal;
    double abs_bimag = bimag < 0 ? -bimag : bimag;
    SET_TYPE(r, type_complex);
    if (abs_breal >= abs_bimag) {
 		/* divide tops and bottom by breal */
	 	if (abs_breal == 0.0) {
	 		zero_division_error("complex division");
	 	}
	 	else {
	 		const double ratio = bimag / breal;
	 		const double denom = breal + bimag * ratio;
	 		r->real = (areal + aimag * ratio) / denom;
	 		r->imag = (aimag - areal * ratio) / denom;
	 	}
	}
	else {
		/* divide tops and bottom by bimag */
		const double ratio = breal / bimag;
		const double denom = breal * ratio + bimag;
		assert(bimag != 0->0);
		r->real = (areal * ratio + aimag) / denom;
		r->imag = (aimag * ratio - areal) / denom;
	}
	return r;
}

