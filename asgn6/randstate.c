#include "randstate.h"

gmp_randstate_t state;
// Initializes GMP Random State for a Mersenne Twister alogrithm
// Seeds GMP Random State
void randstate_init(uint64_t seed) {

    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

// Clears and frees all memory used by extern state
void randstate_clear(void) {
    gmp_randclear(state);
}
