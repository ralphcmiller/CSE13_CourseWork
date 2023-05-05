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
    FILE *pbfile = fopen("rsa.pub", "r");
    FILE *outfile = stdout;
    FILE *infile = stdin;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': do_display = true; break;
        case 'v': verbose = true; break;
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pbfile = fopen(optarg, "r"); break;
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
        printf("   -i             input file of data to encrypt (default: stdin).\n");
        printf("   -o             Output file for encrypted data (default: stdout).\n");
        printf("   -n pbfile      Specifies public key file (default: rsa.pub).\n");
    }
    // Read public key
    mpz_t n, e, s;
    mpz_init(n);
    mpz_init(e);
    mpz_init(s);
    char username[1000];

    rsa_read_pub(n, e, s, username, pbfile);
    // If verbose is enabled print
    if (verbose == true) {
        printf("Username:      %s\n", username);
        gmp_printf("Signature:     %Zd\n", s);
        gmp_printf("Public mod n:  %Zd\n", n);
        gmp_printf("Public exp e:  %Zd\n", e);
    }
    // Convert read in username into mpz_t
    mpz_t name;
    mpz_init(name);

    mpz_set_str(name, username, 62);

    // Verify signature
    if (rsa_verify(name, s, e, n) == false) {
        printf("EXIT\n");
    }

    // Encrypt file
    rsa_encrypt_file(infile, outfile, n, e);

    // Cleanup and close

    fclose(infile);
    fclose(outfile);
    fclose(pbfile);

    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(s);
    mpz_clear(name);

    return 0;
}
