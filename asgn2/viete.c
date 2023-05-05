#include "mathlib.h"

#include <stdio.h>
int pi_viete_factors(void) {
    static int counter;
    counter += 1;
    return counter;
}
double pi_viete(void) {
    double term = 0;
    double numerator = 0;
    double denominator = 2.0;
    double sum = 1.0;
    //the numerator as the amount of terms -> inf gets really close to 2, so when it is within one EPSILON of 2, the approximation will end
    while (2 - numerator > EPSILON) {

        numerator = sqrt_newton(2 + numerator); // sqrt(2 + previous numerator)

        term = numerator / denominator;

        sum = sum * term; // product notation

        pi_viete_factors();
    }
    sum = 2 / sum; //solving for pi
    return sum;
}
