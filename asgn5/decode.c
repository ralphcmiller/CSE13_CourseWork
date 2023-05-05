#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "io.h"
#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "node.h"
#define OPTIONS "hi:o:"
int infile = STDIN_FILENO;
int outfile = STDOUT_FILENO;
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
        printf("SYNOPSIS\n   Huffman tree decoding alogorithm\n\n");
        printf("USEAGE\n   %s [-h] [-i infile] [-o outfile]\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h             Program help and usage.\n");
        printf("   -i infile      Input file to decompress (default: stdin)\n");
        printf("   -o outfile     Output file to write to (default: stdout)\n");
    }

    //read in header, check if header matches magic number
    //we know the header is 16 bytes big so we want to read in 16 bytes

    uint8_t buffer[16];
    read_bytes(infile, buffer, 16);
    //since the magic number is in the first 4 bytes, put those into a test var
    uint32_t magic = MAGIC;
    uint32_t magic_test = 0;
    for (int i = 0; i < 4; i += 1) {
        magic_test += buffer[i] << (i * 8);
    }
    if (magic_test != magic) {
        //exit program
        fprintf(stderr, "\nEncoded file header does not match magic number\n");
    }

    //set permissions of outfile
    //get permissions from buffer
    uint16_t perms = 0;
    for (int i = 4; i < 6; i += 1) {
        perms += buffer[i] << (i * 8);
    }
    fchmod(outfile, perms);

    //get tree size
    uint16_t size = 0;
    for (int i = 6; i < 8; i += 1) {
        if (buffer[i] != 0) {
            size += buffer[i];
        }
    }
    //read the tree into the an array of tree sized long
    uint8_t tree[size];
    read_bytes(infile, tree, size);

    //we can also get the file size for later
    uint64_t file_size = 0;
    for (int i = 8; i < 16; i += 1) {
        file_size += buffer[i] << (i * 8);
    }
    //reconstruct tree using rebuild_tree
    Node *root = rebuild_tree(size, tree);

    //read the codes and traverse the tree to reconstruct the message
    //start at root, if bit returned is a 0, go left, if it is a 1 go right.
    //if node is a leaf, write the symbol, and start at root.
    //this is similar to the huffman.c function build codes.
    //we are done writing when the amount of symbols written is equal to the file size.
    uint64_t symbols_written = 0;
    uint8_t bit_return = 0;
    uint8_t symbol = 0;
    Node *root_copy = root;
    while (symbols_written != file_size) {
        read_bit(infile, &bit_return);
        if (bit_return == 0) {
            //go left
            root = root->left;
            //check if leaf
            if (root->left == NULL && root->right == NULL) {
                //it is a leaf, write to outfile
                symbol = root->symbol;
                write_bytes(outfile, &symbol, 1);
                root = root_copy;
                symbols_written += 1;
            }
        } else {
            //go right
            root = root->right;
            //check if leaf
            if (root->left == NULL && root->right == NULL) {
                //it is a leaf, write to outfile
                symbol = root->symbol;
                write_bytes(outfile, &symbol, 1);
                root = root_copy;
                symbols_written += 1;
            }
        }
    }
}
