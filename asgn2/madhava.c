#include "mathlib.h"

#include <stdio.h>
int pi_madhava_terms(void) {
    static int counter;
    counter += 1;
    return counter;
}

//this summation is setup as the sum of: 1 / (-3)^k * (2k+1)
double pi_madhava(void) {
    double d_0 = 0; //d_0: (-3)^k
    double d_1; //d_1: (2k+1)
    double denominator; // d_0 * d_1
    double term = 1.0;
    double sum = 0.0;

    for (double k = 0.0; absolute(term) > EPSILON; k += 1.0) {

        if (d_0 == 0) { //case for k = 0 where to avoid 1 / 0 = inf
            d_0 = 1;
        } else {
            d_0 = d_0 * -3;
        }

        d_1 = 2 * k + 1;
        denominator = d_0 * d_1;
        term = 1 / denominator;
        sum += term;
        pi_madhava_terms();
    }
    sum = sum * sqrt_newton(12); // solving for pi, sum = pi/sqrt(12)

    return sum;
}
