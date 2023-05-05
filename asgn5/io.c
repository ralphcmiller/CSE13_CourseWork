#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include "defines.h"
#include "io.h"

extern uint64_t bytes_read;
extern uint64_t bytes_written;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int num_read = 0; //holds num of bytes read
    int test_for_zero = -1; //tests for EOF to exit loop
    while (num_read < nbytes && test_for_zero != 0) {
        test_for_zero = read(infile, buf, nbytes);
        num_read += test_for_zero;
    }
    return num_read;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int num_write = 0; //holds num of bytes written
    ssize_t test_for_zero = -1; //tests for EOF to exit loop
    while (num_write < nbytes && test_for_zero != 0) {
        test_for_zero = write(outfile, buf, nbytes);
        num_write += test_for_zero;
    }
    return num_write;
}
static uint8_t buf[BLOCK];
static int index = 0; //represents amount of bits read
static int end = -1; //tracks where to stop reading bits
bool read_bit(int infile, uint8_t *bit) {

    //fill buffer if empty
    if (index == 0) {
        int bytes = read_bytes(infile, buf, BLOCK);
        if (bytes < BLOCK) {
            //set end to amount of bits to read + 1
            end = (bytes * 8) + 1;
        }
    }
    //return a bit out of the buffer
    *bit = (buf[index / 8] >> (index % 8)) & 1;
    //increment index by one
    index += 1;
    if (index / 8 == BLOCK) {
        //if index has read the amount of bits set index to 0
        index = 0;
    }
    //return t or f depending on if there are any more bits to return
    return index != end;
}
//buffers a code
//when buffer is full write code
//code translated from Eugenes psuedocode in section
static int setup = 0;
void clear_buff(int setup) {
    if (setup == 0) {
        for (int i = 0; i < BLOCK; i += 1) {
            buf[i] = 0;
            index = 0;
        }
    }
}
void write_code(int outfile, Code *c) {
    //clear buffer if first time
    if (setup == 0) {
        clear_buff(setup);
    }
    setup = 1;

    //iterate over all bits in the code
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        //buffer each bit
        //T = 1, F = 0
        if (code_get_bit(c, i) == true) {
            //set bit at buffer index
            buf[index / 8] |= (1 << (index % 8));
        } else {
            //clr bit at buffer index
            buf[index / 8] &= ~(1 << (index % 8));
        }
        //increment index
        index += 1;
        //if buffer is full
        if (index == BLOCK) {
            flush_codes(outfile);
        }
    }
}
//flush bits in code if buf is not empty
void flush_codes(int outfile) {
    if (index > 0) {
        //convert index to # of bytes using some cool math :)
        //does int division, then if the statement in ()
        //is true it'll add 1, otherwise itll add 0!
        int bytes = index / 8 + (index % 8 != 0);
        //write bytes to outfile
        write_bytes(outfile, buf, bytes);
        index = 0;
    }
}
