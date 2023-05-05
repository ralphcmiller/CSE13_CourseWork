#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "huffman.h"
#include "stack.h"
#include "node.h"
#include "pq.h"
#include "defines.h"
#include "code.h"
#include "io.h"
#include "header.h"
#define OPTIONS "hi:o:"
int infile = STDIN_FILENO;
int outfile = STDOUT_FILENO;
Header header_create(int leaves, int infile) {
    Header h;
    h.magic = MAGIC;
    h.tree_size = 3 * leaves - 1;
    struct stat buf;
    fstat(infile, &buf);
    h.file_size = buf.st_size;
    h.permissions = buf.st_mode;
    //change perms of outfile
    fchmod(outfile, buf.st_mode);
    return h;
}
int main(int argc, char **argv) {
    //initialize getopt variables
    int opt = 0;
    bool do_display = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': do_display = true; break;
        case 'i': infile = open(optarg, O_RDWR); break;
        case 'o': outfile = open(optarg, O_RDWR | O_CREAT | O_TRUNC); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }

    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   Huffman tree encoding alogorithm\n\n");
        printf("USEAGE\n   %s [-h] [-i infile] [-o outfile]\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h             Program help and usage.\n");
        printf("   -i infile      Input file to compress (default: stdin)\n");
        printf("   -o outfile     Output file to write to (default: stdout)\n");
    }

    //read through infile and construct a histogram//
    uint64_t histogram[ALPHABET];
    uint8_t buffer[BLOCK];
    //zero out histogram and buffer
    for (int i = 0; i < ALPHABET; i += 1) {
        histogram[i] = 0;
    }
    for (int i = 0; i < BLOCK; i += 1) {
        buffer[i] = 0;
    }

    int bytes_read = -1;
    //read infile
    //set index 0 & 255 to 1
    histogram[0] = 1;
    histogram[255] = 1;
    while (bytes_read != 0) {
        bytes_read = read_bytes(infile, buffer, BLOCK);
        if (bytes_read != 0) {
            for (int i = 0; i < BLOCK; i += 1) {
                //using the value of the buffer add 1 to
                //the frequency of that symbol
                if (buffer[i] != 0) {
                    histogram[buffer[i]] += 1;
                }
            }
        }
    }

    //build Huffman Tree and get root node//
    Node *root = build_tree(histogram);
    //construct a code table by traversing the Huffamn Tree//
    //initalize an array of code structs
    Code table[ALPHABET];
    for (int i = 0; i < ALPHABET; i += 1) {
        table[i] = code_init();
    }
    //pass root node and code table into build_codes
    build_codes(root, table);

    //get num of leaves
    int leaves = 0;
    for (int i = 0; i < ALPHABET; i += 1) {
        if (histogram[i] > 0) {
            leaves += 1;
        }
    }
    //construct header
    Header h = header_create(leaves, infile);

    //write header to outfile//
    write(outfile, &h, 16);

    //dump tree to outfile//
    dump_tree(outfile, root);

    //write code to outfile //

    //set infile to the beginning
    lseek(infile, 0, SEEK_SET);

    //zero out buffer
    for (int i = 0; i < BLOCK; i += 1) {
        buffer[i] = 0;
    }
    bytes_read = -1;

    //read infile
    while (bytes_read != 0) {
        bytes_read = read_bytes(infile, buffer, BLOCK);
        if (bytes_read != 0) {
            for (int i = 0; i < BLOCK; i += 1) {
                //for each bytes in the infile
                //write the code for that symbol
                if (buffer[i] > 0) {
                    write_code(outfile, &table[buffer[i]]);
                }
            }
        }
    }
    flush_codes(outfile);
    return 0;
}
