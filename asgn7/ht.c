#include "ht.h"
#include "node.h"
#include "salts.h"
#include "speck.h"
#include <stdio.h>
#include <stdlib.h>

// For statistics
uint64_t lookups;

// Struct Def for Hash Table
// Code from Asgn Doc
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Constructor for Hash Table
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        // Num of binary search trees table can index upto
        ht->size = size;

        // Add flavor
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;

        // Allocate memory for trees
        ht->trees = (Node **) calloc(size, sizeof(Node));
        // Initialize all BSTs
        for (uint32_t i = 0; i < size; i++) {
            ht->trees[i] = bst_create();
        }
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Destructor for Hash Table
// Need to check if everything is freed
void ht_delete(HashTable **ht) {
    //free all bsts
    for (uint32_t i = 0; i < (*ht)->size; i++) {
        if ((*ht)->trees[i] != NULL) {
            bst_delete(&(*ht)->trees[i]);
        }
    }

    if (*ht && (*ht)->trees) {
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}

// Returns HT size
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Searches for a node in HT that contains oldspeak
// Calculates lookup index on HT by hashing the passed in oldspeak
// If the node is found the pointer to the node is returned
// Else a NULL pointer is returned
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    //find the index of trees to lookup
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);

    // Increment lookups
    lookups += 1;

    return bst_find(ht->trees[index], oldspeak);
}

// Inserts the specified oldspeak and its corresponding newspeak translation
// into the hash table.
// Calculates index to insert to by hashing the passed in oldspeak
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    //find the index of trees to insert into
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);

    // if tree is empty at index create and add a node
    // else insert old/new speak into root of the index'd tree
    if (ht->trees[index] == NULL) {
        ht->trees[index] = node_create(oldspeak, newspeak);
    } else {
        ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    }

    // Increment lookups
    lookups += 1;
}

// Returns the num of non-NULL binary search trees in the HT
uint32_t ht_count(HashTable *ht) {
    //init vars
    uint32_t counter = 0;
    //go through all binary search trees
    //if the tree is non-NULL increment counter
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            counter += 1;
        }
    }
    return counter;
}

// Returns the avg binary search tree size
// Computed as sum of all BST sizes / # of non-NULL BSTs
double ht_avg_bst_size(HashTable *ht) {
    //get the sum of all BST sizes
    //and # of non-Null BSTs
    double sum = 0;
    double num_non = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            sum += bst_size(ht->trees[i]);
            num_non += 1;
        }
    }
    // Compute avg
    double result = sum / num_non;
    return result;
}

// Returns avg BST height
// Computed as sum of all BST heights / # non-NULL BSTs
double ht_avg_bst_height(HashTable *ht) {
    //get the sum of all BST heights
    //and # of non-Null BSTs
    double sum = 0;
    double num_non = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            sum += bst_height(ht->trees[i]);
            num_non += 1;
        }
    }
    // Compute avg
    double result = sum / num_non;
    return result;
}

// Debug function to print out contents of HT
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->trees[i] != NULL) {
            bst_print(ht->trees[i]);
            printf("---------------\n");
        } else {
            //printf("BST is empty\n---------------\n");
        }
    }
}
