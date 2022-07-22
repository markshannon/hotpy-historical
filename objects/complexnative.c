#include <math.h>

/** This code copied directly from CPython sources 
 *  See python.org for copyright. */

typedef struct {
    double real;
    double imag;
} Py_complex;

static Py_complex c_1 = {1., 0.};

Py_complex
c_sum(Py_complex a, Py_complex b)
{
	Py_complex r;
	r.real = a.real + b.real;
	r.imag = a.imag + b.imag;
	return r;
}

Py_complex
c_diff(Py_complex a, Py_complex b)
{
	Py_complex r;
	r.real = a.real - b.real;
	r.imag = a.imag - b.imag;
	return r;
}

Py_complex
c_neg(Py_complex a)
{
	Py_complex r;
	r.real = -a.real;
	r.imag = -a.imag;
	return r;
}

Py_complex
c_prod(Py_complex a, Py_complex b)
{
	Py_complex r;
	r.real = a.real*b.real - a.imag*b.imag;
	r.imag = a.real*b.imag + a.imag*b.real;
	return r;
}

Py_complex
c_quot(Py_complex a, Py_complex b)
{
	/******************************************************************
	This was the original algorithm.  It's grossly prone to spurious
	overflow and underflow errors.  It also merrily divides by 0 despite
	checking for that(!).  The code still serves a doc purpose here, as
	the algorithm following is a simple by-cases transformation of this
	one:

	Py_complex r;
	double d = b.real*b.real + b.imag*b.imag;
	if (d == 0.)
		errno = EDOM;
	r.real = (a.real*b.real + a.imag*b.imag)/d;
	r.imag = (a.imag*b.real - a.real*b.imag)/d;
	return r;
	******************************************************************/

	/* This algorithm is better, and is pretty obvious:  first divide the
	 * numerators and denominator by whichever of {b.real, b.imag} has
	 * larger magnitude.  The earliest reference I found was to CACM
	 * Algorithm 116 (Complex Division, Robert L. Smith, Stanford
	 * University).  As usual, though, we're still ignoring all IEEE
	 * endcases.
	 */
	 Py_complex r;	/* the result */
 	 const double abs_breal = b.real < 0 ? -b.real : b.real;
	 const double abs_bimag = b.imag < 0 ? -b.imag : b.imag;

	 if (abs_breal >= abs_bimag) {
 		/* divide tops and bottom by b.real */
	 	if (abs_breal == 0.0) {
	 		errno = EDOM;
	 		r.real = r.imag = 0.0;
	 	}
	 	else {
	 		const double ratio = b.imag / b.real;
	 		const double denom = b.real + b.imag * ratio;
	 		r.real = (a.real + a.imag * ratio) / denom;
	 		r.imag = (a.imag - a.real * ratio) / denom;
	 	}
	}
	else {
		/* divide tops and bottom by b.imag */
		const double ratio = b.real / b.imag;
		const double denom = b.real * ratio + b.imag;
		assert(b.imag != 0.0);
		r.real = (a.real * ratio + a.imag) / denom;
		r.imag = (a.imag * ratio - a.real) / denom;
	}
	return r;
}

Py_complex
c_pow(Py_complex a, Py_complex b)
{
	Py_complex r;
	double vabs,len,at,phase;
	if (b.real == 0. && b.imag == 0.) {
		r.real = 1.;
		r.imag = 0.;
	}
	else if (a.real == 0. && a.imag == 0.) {
		if (b.imag != 0. || b.real < 0.)
			errno = EDOM;
		r.real = 0.;
		r.imag = 0.;
	}
	else {
		vabs = hypot(a.real,a.imag);
		len = pow(vabs,b.real);
		at = atan2(a.imag, a.real);
		phase = at*b.real;
		if (b.imag != 0.0) {
			len /= exp(at*b.imag);
			phase += b.imag*log(vabs);
		}
		r.real = len*cos(phase);
		r.imag = len*sin(phase);
	}
	return r;
}

static Py_complex
c_powu(Py_complex x, long n)
{
	Py_complex r, p;
	long mask = 1;
	r = c_1;
	p = x;
	while (mask > 0 && n >= mask) {
		if (n & mask)
			r = c_prod(r,p);
		mask <<= 1;
		p = c_prod(p,p);
	}
	return r;
}

static Py_complex
c_powi(Py_complex x, long n)
{
	Py_complex cn;

	if (n > 100 || n < -100) {
		cn.real = (double) n;
		cn.imag = 0.;
		return c_pow(x,cn);
	}
	else if (n > 0)
		return c_powu(x,n);
	else
		return c_quot(c_1,c_powu(x,-n));

}

double
c_abs(Py_complex z)
{
	/* sets errno = ERANGE on overflow;  otherwise errno = 0 */
	double result;

    /* C99 rules: if either the real or the imaginary part is an
       infinity, return infinity, even if the other part is a
       NaN. */
	if (isinf(z.real))
	    return INFINITY;
	if (!isfinite(z.imag))
	    return z.imag;
	if (isnan(z.real))
	    return NAN;
	result = hypot(z.real, z.imag);
	if (!isfinite(result))
		errno = ERANGE;
	else
		errno = 0;
	return result;
}

