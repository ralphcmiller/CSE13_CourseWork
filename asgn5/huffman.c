#include "huffman.h"
#include "pq.h"
#include "io.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
//goes through histogram
//for each element in the histogram with a value
//greater than 1, create a node and add it to the pq
Node *build_tree(uint64_t hist[static ALPHABET]) {
    //get capacity for priority queue
    uint32_t capacity = 0;
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            capacity += 1;
        }
    }
    //initalize pq
    PriorityQueue *q = pq_create(capacity);

    //go through histogram
    for (int i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            //get symbol
            uint8_t symbol = i;
            //create node for symb and its freq
            Node *n = node_create(symbol, hist[i]);
            //put node in pq
            enqueue(q, n);
        }
    }
    //once we have all our nodes in the pq
    //we want to create parent nodes until there is only 1 root left
    Node *left = NULL;
    Node *right = NULL;
    while (pq_size(q) > 1) {
        dequeue(q, &left);
        dequeue(q, &right);
        Node *parent = node_join(left, right);
        enqueue(q, parent);
    }
    //dequeue the root node from the pq
    Node *root;
    dequeue(q, &root);
    //free memory
    pq_delete(&q);
    return root;
}

// A DFS algorithm that traverses a huffman tree
// Checks to see if a node has child nodes to determine if it is a leaf
// If the node is not a leaf, push a direction onto the Code
// If the node is a leaf, pop a direction and go up the recursive call chain
// DFS is done when all leaves are found
static int setup = 0;
static Code travel;
//checks if build_trees has been ran before
//if not initalize Code
Code build_init(int setup) {
    if (setup == 0) {
        travel = code_init();
    }
    return travel;
}
void build_codes(Node *root, Code table[static ALPHABET]) {
    travel = build_init(setup);
    setup = 1;
    //initalize pointer so we can use pop()
    uint8_t bit;
    //traverse huffman tree starting at root
    //every time we go left push a 0
    //every time we go right push a 1
    if (root != NULL) {
        if (root->left == NULL) {
            //node is a left leaf
            table[root->symbol] = travel;
        } else {
            //node is not a left leaf
            code_push_bit(&travel, 0);
            //build code to the left recursivly
            build_codes(root->left, table);
            //after a left leaf is found we pop code
            code_pop_bit(&travel, &bit);
            //then travel down right side
            code_push_bit(&travel, 1);
            //build code to the right recursivly
            build_codes(root->right, table);
            //after a right leaf is found we pop code
            code_pop_bit(&travel, &bit);
        }
    }
}
//Similar to the build_code algorithm, this function
//does a post order traversal of the huffman tree
//assigning each leaf an L and its symbol, and each
//interior node an I
void dump_tree(int outfile, Node *root) {
    //initalize buffer
    uint8_t buf[2];
    //start at root node
    if (root != NULL) {
        //it is a root so recursivly call dump_tree
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if (root->left == NULL && root->right == NULL) {
            //it is a leaf
            buf[0] = 'L';
            buf[1] = root->symbol;
            write_bytes(outfile, buf, 2);
        } else {
            //it is an interior node
            uint8_t inter = 'I';
            write_bytes(outfile, &inter, 1);
        }
    }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);
    Node *root = NULL;
    Node *left = NULL;
    Node *right = NULL;
    //iterate over contents of tree from 0 -> nbytes
    for (int i = 0; i < nbytes; i += 2) {
        if (tree[i] == 'L') {
            //element is a leaf so create a node and push it
            Node *n = node_create(tree[i + 1], 0);
            stack_push(s, n);
        }
        if (tree[i] == 'I') {
            //element is an interior, create a node and push it
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *parent = node_join(left, right);
            stack_push(s, parent);
            i -= 1;
        }
    }
    stack_pop(s, &root);
    return root;
}

void delete_tree(Node **root);
