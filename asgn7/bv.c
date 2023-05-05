#include "bv.h"
#include <stdio.h>
#include <stdlib.h>

// The ADT for a one-dimensional array of bits //

// BitVector Struct
// Code from Asgn Doc

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Constructor for BitVector
// Holds length bits
// If sufficient memort cannot be allocated
// the function returns null
// Otherwise the function returns a BitVector ptr
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        bv->vector = (uint8_t *) calloc((length / 8), sizeof(uint8_t));
        if (!bv->vector) {
            free(bv);
            bv = NULL;
        }
    }
    return bv;
}

// Destructor for BitVector
// Frees Memory Associated with the bit vector
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
    return;
}

// Returns the length of a bit vector
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Sets the ith bit in a bit vector
// If i is out of range, return false.
// Otherwise return true to indicate success.
bool bv_set_bit(BitVector *bv, uint32_t i) {
    // check if bv exsits
    if (bv == NULL || bv->vector == NULL) {
        return false;
    }
    // check range of i
    if (i >= (bv->length) || i < 0) {
        return false;
    }
    bv->vector[i / 8] |= (1 << (i % 8));
    return true;
}

// Clears the ith bit in a bit vector
// If i is out of range, return false.
// Otherwise return true to indicate success.
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    // check if bv exsits
    if (bv == NULL || bv->vector == NULL) {
        return false;
    }
    //check range of i
    if (i >= (bv->length) || i < 0) {
        return false;
    }
    bv->vector[i / 8] &= ~(1 << (i % 8));
    return true;
}

// Returns the ith bit in a bit vector
// If i is out of range, return false.
// Otherwise return true to indicate success.
bool bv_get_bit(BitVector *bv, uint32_t i) {
    // check if bv exsits
    if (bv == NULL || bv->vector == NULL) {
        return false;
    }
    //check range of i
    if (i >= (bv->length) || i < 0) {
        return false;
    }
    //set tmp byte to value of bit at i
    uint8_t tmp = (bv->vector[i / 8] >> (i % 8)) & 1;
    if (tmp == 1) {
        //bit is 1
        return true;
    }
    //otherwise bit is 0
    return false;
}

// A debug function to print the bits of a bitvector
void bv_print(BitVector *bv) {
    if (bv_length(bv) >= 0) {
        for (uint32_t i = (bv_length(bv)) - 1; i > 0; i -= 1) {
            if ((i + 1) % 8 == 0) {
                if (i != (bv_length(bv)) - 1) {
                    printf("] index:%d\n", ((i + 1) / 8));
                }
                printf("[");
            }
            if (((i + 1) % 4) == 0 && i != bv_length(bv) - 1) {
                if ((i + 1) % 8 == 0) {
                } else {
                    printf(" ");
                }
            }
            if (bv_get_bit(bv, i) == true) {
                printf("1");
            }
            if (bv_get_bit(bv, i) == false) {
                printf("0");
            }
        }
        if (bv_get_bit(bv, 0) == true) {
            printf("1");
        }
        if (bv_get_bit(bv, 0) == false) {
            printf("0");
        }
        printf("] index:0\n");
        printf("Vector Size:%u\n", bv_length(bv));
    }
}
