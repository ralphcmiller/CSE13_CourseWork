#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

// Bloom Filter Module
// An array of bit vectors
// Hash's a string 3 times, setting bits in array
// A probabalistic method of determining if a word is banned

// Struct Definition
// Code from Asgn Doc
struct BloomFilter {
    uint64_t primary[2]; // Primary hash function salt
    uint64_t secondary[2]; // Secondary hash function salt
    uint64_t tertiary[2]; // Tertiary hash function salt
    BitVector *filter;
};

// Constructor for Bloom filter
// Creates BitVector filter
// Adds salts to struct
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    bf->filter = bv_create(size);

    // Add flavor
    bf->primary[0] = SALT_PRIMARY_LO;
    bf->secondary[0] = SALT_SECONDARY_LO;
    bf->tertiary[0] = SALT_TERTIARY_LO;

    bf->primary[1] = SALT_PRIMARY_HI;
    bf->secondary[1] = SALT_SECONDARY_HI;
    bf->tertiary[1] = SALT_TERTIARY_HI;

    return bf;
}

// Destructor for Bloom filter
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);
    free(*bf);
    *bf = NULL;
    return;
}

// Returns size of Bloom filter
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Inserts oldspeak into BF
// by hashing with each of the three salts using the SPECK cipher
// then setting the bits at those indicies in the filter vector
void bf_insert(BloomFilter *bf, char *oldspeak) {
    //Initalize Vars
    uint32_t index = 0;
    //First Hash
    index = hash(bf->primary, oldspeak) % bv_length(bf->filter);
    bv_set_bit(bf->filter, index);

    //Second Hash
    index = hash(bf->secondary, oldspeak) % bv_length(bf->filter);
    bv_set_bit(bf->filter, index);

    //Third Hash
    index = hash(bf->tertiary, oldspeak) % bv_length(bf->filter);
    bv_set_bit(bf->filter, index);
}

// Probes the Bloom filter for oldspeak
// If all three index are set in the BF return true
// Else return false
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    //Initalize Vars
    uint32_t index = 0;
    uint8_t counter = 0;
    //First Hash
    index = hash(bf->primary, oldspeak) % bv_length(bf->filter);
    if (bv_get_bit(bf->filter, index) == true) {
        counter += 1;
    }

    //Second Hash
    index = hash(bf->secondary, oldspeak) % bv_length(bf->filter);
    if (bv_get_bit(bf->filter, index) == true) {
        counter += 1;
    }

    //Third Hash
    index = hash(bf->tertiary, oldspeak) % bv_length(bf->filter);
    if (bv_get_bit(bf->filter, index) == true) {
        counter += 1;
    }

    //Check if all three hash's returned true

    if (counter == 3) {
        // Oldspeak is in BF
        return true;
    }

    // Oldspeak not in BF
    return false;
}
// Returns the amount of set bits in BF
uint32_t bf_count(BloomFilter *bf) {
    //Initialize Vars
    uint32_t count = 0;
    //loop through filter and count set bits
    for (uint32_t i = 0; i < bv_length(bf->filter) * 8; i++) {
        if (bv_get_bit(bf->filter, i) == true) {
            count += 1;
        }
    }
    return count;
}
// Debug function that prints out Bloom filter
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
