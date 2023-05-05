#include "mathlib.h"

#include <stdio.h>
int pi_bbp_terms(void) {
    static int counter;
    counter += 1;
    return counter;
}
double pi_bbp(void) {
    double in_0, in_1, in_2, in_3; // hold individual calculations on the inside of the equation
    double outer = 1.0; // holds value for 1/16^k
    double inside; // holds entire calculation of inside of the equation
    double power_16 = 0.0; //holds previous value for exp multiplaction
    double term = 1.0;
    double sum = 0.0;
    for (double k = 0.0; outer > EPSILON; k += 1) {

        in_0 = 4 / (8 * k + 1); // individual calcs
        in_1 = 2 / (8 * k + 4);
        in_2 = 1 / (8 * k + 5);
        in_3 = 1 / (8 * k + 6);

        inside = in_0 - in_1 - in_2 - in_3; //inside calc

        if (power_16 == 0) { //case for k = 0 where to avoid 1 / 0 = inf
            power_16 = 1;
        } else {
            power_16 = power_16 * 16;
        }

        outer = 1 / power_16;
        term = outer * inside;
        sum += term;
        pi_bbp_terms();
    }
    return sum;
}
