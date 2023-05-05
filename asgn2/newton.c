#include "mathlib.h"

#include <stdio.h>
static int counter_0, counter_1 = 0;
int sqrt_newton_iters(void) {
    counter_1 = counter_0;
    //printf("%d\n", counter_1);
    return counter_1;
}
double sqrt_newton(double x) {
    //code provided for in assignment document
    double z = 0.0;
    double y = 1.0;
    counter_0 = 0;
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
        counter_0 += 1;
    }
    //printf("counter_0 %d\n", counter_0);
    sqrt_newton_iters();
    return y;
}
