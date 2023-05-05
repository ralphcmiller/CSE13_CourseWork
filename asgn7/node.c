#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constructor function
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->left = NULL;
    n->right = NULL;

    if (oldspeak == (char *) NULL) {
        n->oldspeak = NULL;
    } else {
        n->oldspeak = strdup(oldspeak);
    }
    if (newspeak == (char *) NULL) {
        n->newspeak = NULL;
    } else {
        n->newspeak = strdup(newspeak);
    }
    return n;
}

//Destructor function
void node_delete(Node **n) {
    free((*n)->oldspeak);
    free((*n)->newspeak);
    free(*n);
    *n = NULL;
}

//debug function
//prints a nodes symbol and frequency
void node_print(Node *n) {
    if (n != NULL) {
        // If node contains old AND newspeak
        if (n->oldspeak != NULL && n->newspeak != NULL) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        }
        if (n->oldspeak != NULL && n->newspeak == NULL) {
            printf("%s\n", n->oldspeak);
        }
        if (n->oldspeak == NULL && n->newspeak == NULL) {
            printf("Error: old and newspeak fields are both NULL\n");
        }
    }
}
