#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"

#define OPTIONS "hvi:o:n:"
int main(int argc, char **argv) {
    //initialize getopt variables
    int opt = 0;
    bool do_display = false;
    bool verbose = false;
    FILE *pvfile = fopen("rsa.priv", "r");
    FILE *outfile = stdout;
    FILE *infile = stdin;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': do_display = true; break;
        case 'v': verbose = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pvfile = fopen(optarg, "r"); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }

    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   Encrypts data using RSA encryption.\n   Encrypted data is decrypted "
               "by the decrypt program.\n\n");
        printf("USEAGE\n   %s [-hv] [-i infile] [-o outfile] -n pbubkey\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h             Program help and usage.\n");
        printf("   -v             Enables verbose output.\n");
        printf("   -i             Input file of encrypted data (default: stdin).\n");
        printf("   -o             Output file for decrypted data (default: stdout).\n");
        printf("   -n pvfile      Specifies private key file (default: rsa.priv).\n");
    }

    // Read private key
    mpz_t n, d;
    mpz_init(n);
    mpz_init(d);
    rsa_read_priv(n, d, pvfile);

    // If verbose is enabled print
    if (verbose == true) {
        gmp_printf("Public mod n:  %Zd\n", n);
        gmp_printf("Private key d:  %Zd\n", d);
    }

    // Decrypt file
    rsa_decrypt_file(infile, outfile, n, d);

    // Clean up and close
    fclose(infile);
    fclose(outfile);
    fclose(pvfile);

    mpz_clear(n);
    mpz_clear(d);

    return 0;
}
