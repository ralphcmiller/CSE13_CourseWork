#include "numtheory.h"
#include "randstate.h"
#include <inttypes.h>
#include <stdlib.h>
// Computes the greatest common divisor of a and b
// Storing the gcd into g
void gcd(mpz_t g, mpz_t a, mpz_t b) {
    //initialize variables
    mpz_t temp, temp_a, temp_b;
    mpz_init(temp);
    mpz_init(temp_a);
    mpz_init(temp_b);
    mpz_set(temp_a, a);
    mpz_set(temp_b, b);
    while (mpz_cmp_ui(temp_b, 0) != 0) {
        mpz_set(temp, temp_b); // temp = b
        mpz_mod(temp_b, temp_a, temp_b); // b = a % b
        mpz_set(temp_a, temp); // a = temp
    }
    mpz_set(g, temp_a); // g = a
    //clear mpz types
    mpz_clear(temp);
    mpz_clear(temp_a);
    mpz_clear(temp_b);
}
// Computes the inverse of a mod n
// If inverse cannot be found i is set to 0
// A modular multiplicative inverse is found when:
// ((a % n) * (i % n)) % n equals 1
void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t r, r1, t, t1, q;
    mpz_init(r);
    mpz_init(r1);
    mpz_init(t);
    mpz_init(t1);
    mpz_init(q);

    //set r,r1 = n,a
    mpz_set(r, n);
    mpz_set(r1, a);
    //set t,t1 = 0,1
    mpz_set_ui(t, 0);
    mpz_set_ui(t1, 1);
    //while r1 != 0
    while (mpz_cmp_ui(r1, 0) != 0) {
        mpz_fdiv_q(q, r, r1);
        //make temps for r,r1
        mpz_t r_t, r1_t;
        mpz_init(r_t);
        mpz_init(r1_t);
        //set temps to r,r1
        mpz_set(r_t, r);
        mpz_set(r1_t, r1);
        // (r,r1) = (r1, r-q*r1)
        mpz_set(r, r1_t);

        mpz_mul(r1_t, q, r1);
        mpz_sub(r1, r_t, r1_t);

        // (t,t1) = (t1, t-q*t1)
        //make temps for t, t1
        mpz_t t_t, t1_t;
        mpz_init(t_t);
        mpz_init(t1_t);
        //set temps to t, t1
        mpz_set(t_t, t);
        mpz_set(t1_t, t1);

        //t = t1
        mpz_set(t, t1);

        mpz_mul(t1_t, q, t1);
        mpz_sub(t1, t_t, t1_t);

        //clear temps
        mpz_clear(r_t);
        mpz_clear(r1_t);
        mpz_clear(t_t);
        mpz_clear(t1_t);
    }
    //if r > 1
    if (mpz_cmp_ui(r, 1) > 0) {
        //no inverse
        //set o to 0
        mpz_set_ui(o, 0);

        //clear mpz_ts
        mpz_clear(r);
        mpz_clear(r1);
        mpz_clear(t);
        mpz_clear(t1);
        mpz_clear(q);
        return;
    }
    //if t < 0
    if (mpz_cmp_ui(t, 0) < 0) {
        //t = t+n
        mpz_add(t, t, n);
    }
    //set o to t
    mpz_set(o, t);
    //clear mpz_ts
    mpz_clear(r);
    mpz_clear(r1);
    mpz_clear(t);
    mpz_clear(t1);
    mpz_clear(q);
}
// Performs fast modular exponentiation
// Computes base raised to exponent power modulo modulus
// Storing result in o
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    //initialize mpz types
    mpz_t v, tmp, p, r_mod, r_mul, r_q, d_tmp;

    mpz_init(p);
    mpz_init_set(d_tmp, d);
    mpz_init(r_mod);
    mpz_init(r_mul);
    mpz_init(r_q);
    mpz_init_set_ui(tmp, 2); // tmp = 2
    mpz_init_set_ui(v, 1); // v = 1

    mpz_set(p, a); // p = a
    while (mpz_sgn(d_tmp) == 1) { // d > 0
        mpz_mod(r_mod, d_tmp, tmp);
        if (mpz_cmp_ui(r_mod, 1) == 0) { // tests for odd
            // v = (v * p) % n
            mpz_mul(r_mul, v, p); // result = (v * p)
            mpz_mod(v, r_mul, n); // v = result % n
        }
        // p = (p * p) % n
        mpz_mul(r_mul, p, p); // result = (p * p)
        mpz_mod(p, r_mul, n); // p = result % n
        mpz_fdiv_q(d_tmp, d_tmp, tmp); // d = (d / 2);
    }
    mpz_set(o, v); // o = v

    //clear mpz types
    mpz_clear(tmp);
    mpz_clear(v);
    mpz_clear(p);
    mpz_clear(r_mod);
    mpz_clear(r_mul);
    mpz_clear(r_q);
}

bool is_prime(mpz_t n, uint64_t iters) {
    //check for primes that are not allowed
    // ie 0, 2, ..ect

    mpz_t check_even;
    mpz_init(check_even);
    mpz_mod_ui(check_even, n, 2);

    if (mpz_cmp_ui(n, 0) == 0) {
        return false;
    }
    if (mpz_cmp_ui(n, 1) == 0) {
        return false;
    }
    if (mpz_cmp_ui(check_even, 0) == 0) {
        if (mpz_cmp_ui(n, 2) == 0) {
            return true;
        }
        return false;
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        return true;
    }

    // initialize mpz types
    uint64_t s_tmp = 0;
    mpz_t r, s, r_mod, top, bottom, y, j, s_1, n_1;
    mpz_init(s);
    mpz_init(s_1); //holds value of s-1
    mpz_init(top); // holds value of n - 1
    mpz_init(bottom);
    mpz_init(y);
    mpz_init(j);
    mpz_init(n_1);
    mpz_init_set_ui(r, 0); // r = 0
    mpz_init_set_ui(r_mod, 0); // r_mod = 0

    // write n - 1 = 2^s * r, such that r is odd
    // solve for r, check if odd, if not increment s by 1

    while (mpz_cmp_ui(r_mod, 1) != 0) { // tests for odd
        // r = (n-1) / (2^s)
        s_tmp += 1; // increment s by 1
        mpz_sub_ui(top, n, 1); // top = n - 1
        mpz_ui_pow_ui(bottom, 2, s_tmp); // bottom = 2^s
        mpz_fdiv_q(r, top, bottom); // r = top/bottom

        // get result for odd test
        mpz_mod_ui(r_mod, r, 2);
    }
    mpz_set_ui(s, s_tmp); // mpz_t s = s_tmp
    mpz_sub_ui(s_1, s, 1); // s_1 = s - 1;
    for (uint64_t i = 1; i < iters; i++) {
        //generate a random num in range [2,n-2]
        //get n into a ul
        uint64_t temp_n = mpz_get_ui(n);
        uint64_t num = rand() % temp_n - 4;
        num += 2;
        //set a to rand number
        mpz_t a;
        mpz_init_set_ui(a, num);
        pow_mod(y, a, r, n);
        mpz_set(n_1, n); //set n_1 to n-1
        mpz_sub_ui(n_1, n_1, 1);
        mpz_clear(a);
        //if y != 1 && y != n-1
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_1) != 0) {
            mpz_set_ui(j, 1); //set j to 1
            //while j < s && y != n-1
            while (mpz_cmp(j, s) < 0 && mpz_cmp(y, n_1) != 0) {
                mpz_t two;
                mpz_init_set_ui(two, 2);
                pow_mod(y, y, two, n);
                mpz_clear(two);
                //if y == 1
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clear(check_even);
                    mpz_clear(s);
                    mpz_clear(s_1);
                    mpz_clear(top);
                    mpz_clear(bottom);
                    mpz_clear(y);
                    mpz_clear(j);
                    mpz_clear(n_1);
                    mpz_clear(r);
                    mpz_clear(r_mod);
                    return false;
                }
                //j += 1
                mpz_add_ui(j, j, 1);
            }
            //if y != n-1
            if (mpz_cmp(y, n_1) != 0) {
                mpz_clear(check_even);
                mpz_clear(s);
                mpz_clear(s_1);
                mpz_clear(top);
                mpz_clear(bottom);
                mpz_clear(y);
                mpz_clear(j);
                mpz_clear(n_1);
                mpz_clear(r);
                mpz_clear(r_mod);
                return false;
            }
        }
    }
    mpz_clear(check_even);
    mpz_clear(s);
    mpz_clear(s_1);
    mpz_clear(top);
    mpz_clear(bottom);
    mpz_clear(y);
    mpz_clear(j);
    mpz_clear(n_1);
    mpz_clear(r);
    mpz_clear(r_mod);
    return true;
}

//generates a prime number p
//randomly generates a number using gmp functions, then tests with is prime.
//if sucessful p is set to this number
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    bool found = false;
    mp_bitcnt_t mbits = bits;
    mpz_t n;
    mpz_init(n);
    while (found == false) {
        mpz_urandomb(n, state, mbits);

        if (is_prime(n, iters) == true) {
            mpz_set(p, n);
            mpz_clear(n);
            found = true;
        }
    }
}
