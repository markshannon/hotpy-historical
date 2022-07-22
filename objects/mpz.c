#include <gmp.h>
#include <inttypes.h>

unsigned hotpy_mpz_div(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* q, int round_up) {
    mpz_t n, d, x;
    unsigned xlen;
    mpz_init (n);
    mpz_init (d);
    mpz_init (x);
    mpz_import (n, ulen, -1, sizeof(int32_t), 0, 0, u);
    mpz_import (d, vlen, -1, sizeof(int32_t), 0, 0, v);
    if (round_up)
        mpz_cdiv_q (x, n, d);
    else
        mpz_fdiv_q (x, n, d);
    mpz_export (q, &xlen, -1, sizeof(int32_t), 0, 0, x); 
    mpz_clear (n);
    mpz_clear (d);
    mpz_clear (x);
    return xlen;
}

unsigned hotpy_mpz_mod(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* r, int round_up) {
    mpz_t n, d, x;
    unsigned xlen;
    mpz_init (n);
    mpz_init (d);
    mpz_init (x);
    mpz_import (n, ulen, -1, sizeof(int32_t), 0, 0, u);
    mpz_import (d, vlen, -1, sizeof(int32_t), 0, 0, v);
    if (round_up)
        mpz_cdiv_r (x, n, d);
    else
        mpz_fdiv_r (x, n, d);    
    mpz_export (r, &xlen, -1, sizeof(int32_t), 0, 0, x); 
    mpz_clear (n);
    mpz_clear (d);
    mpz_clear (x);
    return xlen;
}

unsigned hotpy_mpz_mul(int ulen, uint32_t* u, int vlen, uint32_t* v, uint32_t* p) {
    mpz_t a, b, x;
    unsigned xlen;
    mpz_init (a);
    mpz_init (b);
    mpz_init (x);
    mpz_import (a, ulen, -1, sizeof(int32_t), 0, 0, u);
    mpz_import (b, vlen, -1, sizeof(int32_t), 0, 0, v);
    mpz_mul (x, a, b);
    mpz_export (p, &xlen, -1, sizeof(int32_t), 0, 0, x); 
    mpz_clear (a);
    mpz_clear (b);
    mpz_clear (x);
    return xlen;
}

unsigned hotpy_mpz_mul_ui(int ulen, uint32_t* u, unsigned v, uint32_t* p) {
    mpz_t a, x;
    unsigned xlen;
    mpz_init (a);
    mpz_init (x);
    mpz_import (a, ulen, -1, sizeof(int32_t), 0, 0, u);
    mpz_mul_ui (x, a, v);
    mpz_export (p, &xlen, -1, sizeof(int32_t), 0, 0, x); 
    mpz_clear (a);
    mpz_clear (x);
    return xlen;
}


