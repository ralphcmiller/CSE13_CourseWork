#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
// Sets Perms of pvfile to 0600
void set_perms(FILE *pvfile) {
    struct stat buf;
    int fd = fileno(pvfile);
    fstat(fd, &buf);
    buf.st_mode = 0600;
    fchmod(fd, buf.st_mode);
}
#define OPTIONS "hvb:c:s:n:d:"
int main(int argc, char **argv) {
    //initialize getopt variables
    int opt = 0;
    bool do_display = false;
    bool verbose = false;
    uint64_t seed = time(NULL);
    uint64_t bits = 256;
    uint64_t iters = 50;

    FILE *pbfile = fopen("rsa.pub", "w");
    FILE *pvfile = fopen("rsa.priv", "w");
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': do_display = true; break;
        case 'v': verbose = true; break;
        case 'b': bits = atoi(optarg); break;
        case 'c': iters = atoi(optarg); break;
        case 's': seed = atoi(optarg); break;
        case 'n': pbfile = fopen(optarg, "w"); break;
        case 'd': pvfile = fopen(optarg, "w"); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }

    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   Generates an RSA public/private key pair.\n\n");
        printf(
            "USEAGE\n   %s [-hv] [-b bits] [-i iters] [-s seed] -n pbfile -d pvfile\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h             Program help and usage.\n");
        printf("   -v             Enables verbose output.\n");
        printf("   -b             Specifies the minimum bits needed for public mod n.\n");
        printf(
            "   -i             Specifies the num of Miller-Raben iterations for testing primes.\n");
        printf("   -s             Specifies random seed for rand state initialization (default: "
               "time(NULL)).\n");
        printf("   -n pbfile      Specifies public key file (default: rsa.pub).\n");
        printf("   -d pvfile      Specifies private key file (default: rsa.priv).\n");
    }
    // GENERATING KEYS //

    //First set permissions on pvfile
    set_perms(pvfile);

    //Next initialize the random state using set seed
    randstate_init(seed);

    //Make public and private keys
    mpz_t p, q, n, e;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(e);
    rsa_make_pub(p, q, n, e, bits, iters);

    mpz_t d;
    mpz_init(d);
    rsa_make_priv(d, e, p, q);
    //Get current users name as a string
    char *username = getenv("USER");

    //Convert username to mpz_t and compute signature
    mpz_t name;
    mpz_init(name);
    mpz_set_str(name, username, 62);

    mpz_t s;
    mpz_init(s);
    rsa_sign(s, name, d, n);
    //Write public and private key to respective files
    rsa_write_priv(n, d, pvfile);
    rsa_write_pub(n, e, s, username, pbfile);

    //If Verbose is enabled print output
    if (verbose == true) {
        printf("Username:      %s\n", username);
        gmp_printf("Signature:     %Zd\n", s);
        gmp_printf("Prime p:       %Zd\n", p);
        gmp_printf("Prime q:       %Zd\n", q);
        gmp_printf("Public mod n:  %Zd\n", n);
        gmp_printf("Public exp e:  %Zd\n", e);
        gmp_printf("Private key d: %Zd\n", d);
    }

    //Close files and clean up
    fclose(pvfile);
    fclose(pbfile);

    randstate_clear();

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(s);
    mpz_clear(d);
    mpz_clear(name);

    return 0;
}
