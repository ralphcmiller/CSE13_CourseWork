#include "bst.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

uint64_t branches;

// Creates an empty tree
Node *bst_create(void) {
    Node *root = NULL;
    return root;
}

// Destructor for BST
// Uses postorder traversal to delete nodes
// Transposed code from Slide deck 18
void bst_delete(Node **root) {
    if (*root != NULL) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}

// Returns the height of the binary search tree rooted at root
uint32_t bst_height(Node *root) {
    // Traverse down each sub tree
    if (root == NULL) {
        return 0;
    } else {
        uint32_t l_count = bst_height(root->left);
        uint32_t r_count = bst_height(root->right);

        if (l_count > r_count) {
            return l_count + 1;
        } else {
            return r_count + 1;
        }
    }
}

// Returns the num of nodes in the tree
uint32_t bst_size(Node *root) {
    // get the size of the tree by traversing the entire tree

    // if root is null there is no node
    if (root == NULL) {
        return 0;
    }

    // count 1 node and go down recursive call chain until
    // all nodes are counted
    return 1 + bst_size(root->left) + bst_size(root->right);
}

// Searches for a node containing oldspeak in BST
// Transposed from finding a key, lecture slide deck 18
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}

// Inserts a new node containing the passed in old/newspeak into the BST
// Each left subtree contains oldspeak less than the parent
// Each right subtree contains oldspeak greater than the parent
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {

    //traverse tree and compare Nodes to find lexographic order
    //inserted node is less than root and has no left child
    if (strcmp(root->oldspeak, oldspeak) > 0) {
        if (root->left == NULL) {
            //Create new node
            Node *n = node_create(oldspeak, newspeak);
            //Connect node
            root->left = n;
            branches += 1;
        } else {
            // Go left
            branches += 1;
            bst_insert(root->left, oldspeak, newspeak);
        }
    }
    //inserted node is greater than root and has no right child
    if (strcmp(root->oldspeak, oldspeak) < 0) {
        if (root->right == NULL) {
            //Create new node
            Node *n = node_create(oldspeak, newspeak);
            //Connect node
            root->right = n;
            branches += 1;
        } else {
            //Go right
            branches += 1;
            bst_insert(root->right, oldspeak, newspeak);
        }
    }
    return root;
}
// Prints out each node in the BST through an inorder traversal
// Inorder traversal code modified from Slide deck 18
void bst_print(Node *root) {
    if (root != NULL) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}
