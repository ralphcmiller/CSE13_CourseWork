#include "code.h"
#include "defines.h"
#include <stdbool.h>
#include <stdio.h>
//sets top to 0
//0s out array of bits
Code code_init(void) {
    Code c;
    c.top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i += 1) {
        c.bits[i] = 0;
    }
    return c;
}
//returns the number of bits pushed onto Code
uint32_t code_size(Code *c) {
    return c->top;
}
//returns true if code is empty
//retunrs false otherwise
bool code_empty(Code *c) {
    //if the code size is 0 its empty!
    if (code_size(c) == 0) {
        return true;
    }
    return false;
}
//returns true if code is full
//returns false otherwise
bool code_full(Code *c) {
    //if the code size equals capacity its full!
    if (code_size(c) == ALPHABET) {
        return true;
    }
    return false;
}

//uses OR operator to set bit at index i
//returns false if i is out of range ie bigger than ASCII char set
//returns true if sucessful
bool code_set_bit(Code *c, uint32_t i) {
    //check range of i
    if (i >= ALPHABET) {
        return false;
    }

    c->bits[i / 8] |= (1 << (i % 8));
    return true;
}
//uses AND operator to clear bit at index i
//returns false if i is out of range ie bigger than ASCII char set
//returns true if sucessful
bool code_clr_bit(Code *c, uint32_t i) {
    //check range of i
    if (i >= ALPHABET) {
        return false;
    }
    c->bits[i / 8] &= ~(1 << (i % 8));
    return true;
}
//returns false if i is out of range or if bit is 0
//returns true if bit is 1
bool code_get_bit(Code *c, uint32_t i) {
    //check range of i
    if (i >= ALPHABET) {
        return false;
    }
    //set tmp byte to value of bit at i
    uint8_t tmp = (c->bits[i / 8] >> (i % 8)) & 1;
    if (tmp == 1) {
        //bit is 1
        return true;
    }
    //otherwise bit is 0
    return false;
}
//push a bit onto the code stack
//returns false if code stack is full
//returns true for success
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c) == true) {
        return false;
    }
    //if our bit is 1 call set_bit at top
    if (bit == 1) {
        code_set_bit(c, c->top);
    }
    //if our bit is 0 we just increment top
    c->top += 1;
    return true;
}
//pops a bit off the code stack setting it to the address of bit
//returns false if code stack is empty
//returns true for success
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c) == true) {
        return false;
    }
    //decrement top before popping
    c->top -= 1;
    //if the bit to be popped is 1
    //we needs to also clear the bit
    if (code_get_bit(c, c->top) == true) {
        *bit = 1;
        code_clr_bit(c, c->top);
        return true;
    }
    //otherwise the bit is 0
    *bit = 0;
    return true;
}
//debug function
//prints out every bit and index of bits[i]
//as well as code_size
void code_print(Code *c) {
    if (code_size(c) >= 0) {
        for (int i = ALPHABET - 1; i >= 0; i -= 1) {
            if ((i + 1) % 8 == 0) {
                if (i != ALPHABET - 1) {
                    printf("] index:%d\n", ((i + 1) / 8));
                }
                printf("[");
            }
            if (((i + 1) % 4) == 0 && i != ALPHABET - 1) {
                if ((i + 1) % 8 == 0) {
                } else {
                    printf(" ");
                }
            }
            if (code_get_bit(c, i) == true) {
                printf("1");
            }
            if (code_get_bit(c, i) == false) {
                printf("0");
            }
        }
        printf("] index:0\n");
        printf("Code Size:%u\n", code_size(c));
    }
}
