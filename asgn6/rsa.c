#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>

// Creates two parts of rsa key, primes p q, product n, and public exponent e
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    // First decided num of bits that goto each prime
    // such that log_2(n) >= nbits
    // mpz_sizeinbase(x,2)
    uint64_t pbits = (nbits / 4) + (rand() % (nbits / 2));
    uint64_t qbits = nbits - pbits;
    // Create primes p and q using make_prime

    make_prime(p, pbits + 1, iters);
    make_prime(q, qbits + 1, iters);
    // Compute n = p * q
    mpz_mul(n, p, q);
    // Compute p(n) = (p-1)(q-1)
    mpz_t p_1, q_1, p_n;
    mpz_init(q_1);
    mpz_init(p_1);
    mpz_init(p_n);

    mpz_sub_ui(p_1, p, 1); //p_1 = p-1

    mpz_sub_ui(q_1, q, 1); //q_1 = q-1

    mpz_mul(p_n, p_1, q_1); //p(n) = (p-1)*(q-1);

    // Find a suitable public exponent e
    mp_bitcnt_t bits = nbits;
    mpz_t check;
    mpz_init(check);
    bool found = false;
    while (found == false) {
        //generate random e
        mpz_urandomb(e, state, bits);
        //check if it is coprime to n
        gcd(check, e, p_n);
        //if gcd of e and n is 1
        if (mpz_cmp_ui(check, 1) == 0) {
            found = true;
            mpz_clear(check);
        }
    }
    mpz_clear(p_1);
    mpz_clear(q_1);
    mpz_clear(p_n);
}

// Writes public rsa key to pbfile
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    //write n,e,s, and username
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
}

// Reads a public RSA key from pbfile
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    //get n,e,s
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    //get username
    fscanf(pbfile, "%s\n", username);
}

// Creates a new RSA private key d, given primes p and q, and exponent e
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    //p-1 * q-1 to compute totient of n
    // find some d that is the inverse of e, mod n
    mpz_t p_1, q_1;
    mpz_init(p_1);
    mpz_init(q_1);
    mpz_sub_ui(p_1, p, 1);
    mpz_sub_ui(q_1, q, 1);

    mpz_t n_p;
    mpz_init(n_p);
    mpz_mul(n_p, p_1, q_1);
    mod_inverse(d, e, n_p);
    mpz_clear(p_1);
    mpz_clear(q_1);
}

// Writes a private RSA key to pvfile
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    //write n,d
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}

// Reads a private RSA key from pvfile
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    //read n,d
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
}

// Preforms RSA encryptions, computing cypgertext c by encrypting message m
// using public exponent e and modulus n.
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    // E(m) = c = m^e mod n
    pow_mod(c, m, e, n);
}

// Encrypts the contents of infile, writing the encrypted conentnts to outfile
// Encrypted in blocks
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    //calculate block size
    // k = floor[(log_2(n) - 1)/8]
    size_t n_inbase = mpz_sizeinbase(n, 2);
    n_inbase -= 1;
    uint64_t k = n_inbase / 8;
    //Dynamically allocate an array of size k, of type uint8_t
    uint8_t *buffer = (uint8_t *) calloc(k, sizeof(uint8_t));
    //prepend k with 0xFF
    buffer[0] = 0xFF;
    //scan in bytes and convert to mpz_t
    fread(buffer, sizeof(uint8_t), k - 1, infile);
    mpz_t m, a;
    mpz_init(m);
    mpz_init(a);
    mpz_import(m, k, 1, 8, 1, 0, a);
    mpz_t c;
    mpz_init(c);
    rsa_encrypt(c, m, e, n);
    //print cyphertext c to outfile
    gmp_fprintf(outfile, "Zx\n", c);

    mpz_clear(m);
    mpz_clear(c);
    mpz_clear(a);
}

// Preforms an RSA decryption, computing messagfe m by decrypting
// cyphertext c using private key d and public modulus n
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    //D(c) = m = c^d mod n
    pow_mod(m, c, d, n);
}

// Decrypts the contents of infile, writing the contents to outfile
// Decrypted in blocks
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    //calculate block size
    // k = floor[(log_2(n) - 1)/8]
    size_t n_inbase = mpz_sizeinbase(n, 2);
    n_inbase -= 1;
    uint64_t k = n_inbase / 8;
    //Dynamically allocate an array of size k, of type uint8_t
    uint8_t *buffer = (uint8_t *) calloc(k, sizeof(uint8_t));
    //scan in hex strings
    //loop to some condition
    mpz_t c, m, a;
    mpz_init(c);
    mpz_init(m);
    mpz_init(a);
    void *j = NULL;
    gmp_fscanf(infile, "%Zx\n", c);

    rsa_decrypt(m, c, d, n);
    mpz_export(buffer, j, 1, 8, 1, 0, a);
    fwrite(buffer, 8, k, outfile); //to write out decrypted bytes

    mpz_clear(c);
    mpz_clear(m);
    mpz_clear(a);
}

// Performs RSA signing, producing signature s by signing message m using
// private key d and public modulus n
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    //S(m) = s = m^d % n
    pow_mod(s, m, d, n);
}

// Performs RSA verification, returning true if signature s is verified
// and false otherwise
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    //initialize check var
    mpz_t verify;
    mpz_init(verify);
    //call pow_mod on s^e % n
    pow_mod(verify, s, e, n);

    //if verify == m
    if (mpz_cmp(verify, m) == 0) {
        mpz_clear(verify);
        return true;
    } else {
        mpz_clear(verify);
        return false;
    }
}
