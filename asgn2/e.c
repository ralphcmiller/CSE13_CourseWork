#include "mathlib.h"

#include <stdio.h>

int e_terms(void) {

    static int counter;
    counter += 1;
    return counter;
}

double e(void) {
    double term = 1.0;
    double numerator = 1.0;
    double denominator = 1.0;
    double sum = 0.0;
    for (double k = 1.0; absolute(term) > EPSILON; k += 1.0) {
        denominator = denominator * k;
        term = numerator / denominator;
        sum += term;
        e_terms();
    }
    sum += 1;
    //printf("%f", sum);
    return sum;
}
