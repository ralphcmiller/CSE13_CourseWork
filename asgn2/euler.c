#include "mathlib.h"

#include <stdio.h>

int pi_euler_terms(void) {
    static int counter;
    counter += 1;
    return counter;
}

double pi_euler(void) {
    double term = 1.0;
    double numerator = 1.0;
    double denominator = 0.0;
    double sum = 0.0;

    // to get a more accurate approximation of pi, you would need to keep iterating past abs(term) > EPSILON.

    for (double k = 1.0; absolute(term) > EPSILON; k += 1.0) {
        denominator = k * k; // k squared
        term = numerator / denominator;
        sum += term;
        pi_euler_terms();
    }
    sum = 6 * sum;
    sum = sqrt_newton(sum);
    return sum;
}
