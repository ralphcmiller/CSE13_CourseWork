#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
    int opt = 0;
    bool do_e, do_n, do_euler, do_mad, do_viete, do_bbp, do_display, do_stats;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            do_e = true;
            do_n = true;
            do_euler = true;
            do_mad = true;
            do_viete = true;
            do_bbp = true;
            break;
        case 'e': do_e = true; break;
        case 'n': do_n = true; break;
        case 'r': do_euler = true; break;
        case 'm': do_mad = true; break;
        case 'v': do_viete = true; break;
        case 'b': do_bbp = true; break;
        case 'h': do_display = true; break;
        case 's': do_stats = true; break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }
    if (!do_e && !do_euler && !do_n && !do_mad && !do_viete && !do_bbp) {
        do_display = true;
    }
    if (do_display) {
        printf("SYNOPSIS\n   A test harness for the small numerical library.\n\n");
        printf("USEAGE\n   %s -[aebmrvnsh]\n\n", argv[0]);
        printf("OPTIONS\n  -a   Runs all tests.\n  -e   Runs e test.\n  -b   Runs BBP pi test.\n  "
               "-m   Runs Madhava pi test.\n  -r   Runs Euler pi test.\n  -v   Runs Viete pi "
               "test.\n  -n   Runs Newton square root tests.\n  -s   Print verbose statistics.\n  "
               "-h   Display program synopsis and useage.\n");
    }
    if (do_e) {
        double x = 1;
        double e_0 = e(); //holds approximation value
        double difference = e_0 - exp(x);

        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e_0, exp(x), difference);
        if (do_stats) {

            printf("e() terms = %d\n",
                e_terms()
                    - 1); // the -1 is correcting the term counter as it is called one more time after the approximation, falsely adding + 1 to the term counter.
        }
    }
    if (do_euler) {
        double pi = M_PI;
        double euler_0 = pi_euler(); //holds approximation value
        double difference = fabs(euler_0 - pi);
        printf(
            "pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", euler_0, pi, difference);
        if (do_stats) {
            printf("pi_euler() terms = %d\n", pi_euler_terms() - 1); // see do_e
        }
    }
    if (do_bbp) {
        double pi = M_PI;
        double bbp_0 = pi_bbp(); //holds approximation value
        double difference = fabs(bbp_0 - pi);
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", bbp_0, pi, difference);
        if (do_stats) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms() - 1); // see do_e
        }
    }
    if (do_mad) {
        double pi = M_PI;
        double mad_0 = pi_madhava(); //holds approximation value
        double difference = fabs(mad_0 - pi);
        printf(
            "pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", mad_0, pi, difference);
        if (do_stats) {
            printf("pi_madhava() terms = %d\n", pi_madhava_terms() - 1); // see do_e
        }
    }
    if (do_viete) {
        double pi = M_PI;
        double viete_0 = pi_viete(); //holds approximation value
        double difference = fabs(viete_0 - pi);
        printf(
            "pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", viete_0, pi, difference);
        if (do_stats) {
            printf("pi_viete() terms = %d\n", pi_viete_factors() - 1); // see do_e
        }
    }
    if (do_n) {
        for (double x = 0.0; x <= 10.0; x += 0.1) {
            double newton_0 = sqrt_newton(x);
            double difference = fabs(newton_0 - sqrt(x));
            printf("sqrt_newton(%f) = %16.15lf, sqrt(%f) = %16.15lf, diff = %16.15lf\n", x,
                newton_0, x, sqrt(x), difference);
            if (do_stats) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }
}
