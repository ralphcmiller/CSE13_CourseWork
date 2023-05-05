#include <stdio.h>
#include <stdlib.h>
#include "node.h"

//Constructor function
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->left = NULL;
    n->right = NULL;
    n->symbol = symbol;
    n->frequency = frequency;
    return n;
}

//Destructor function
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
}

//creates a parent node for left and right nodes
//sums left and right frequencies
//sets parent node symbol to $
//returns pointer to created node
Node *node_join(Node *left, Node *right) {
    uint64_t LR_sum = left->frequency + right->frequency;
    uint8_t symbol = '$';
    Node *parent = node_create(symbol, LR_sum);
    parent->left = left;
    parent->right = right;
    return parent;
}
//debug function
//prints a nodes symbol and frequency
void node_print(Node *n) {
    printf("%c:%lu\n", n->symbol, n->frequency);
    if (n->left != NULL || n->right != NULL) {
        printf("CHILDREN\nLEFT:%c:%lu ", n->left->symbol, n->left->frequency);
        printf("RIGHT:%c:%lu\n", n->right->symbol, n->right->frequency);
    }
}
