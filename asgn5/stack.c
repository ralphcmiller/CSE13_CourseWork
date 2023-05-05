#include "stack.h"
#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//Stack struct included in Asgn doc
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

//Constructor function for stack
//included in asgn doc
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}
//Destructor function for stack
//Included in asgn doc
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}
//returns num of items in the stack
uint32_t stack_size(Stack *s) {
    return s->top;
}
//returns true if the stack is empty
//returns false otherwise
bool stack_empty(Stack *s) {
    //if the stack size is 0 its empty!
    if (stack_size(s) == 0) {
        return true;
    }
    return false;
}
//returns ture if stack is full
//returns false otherwise
bool stack_full(Stack *s) {
    //if the stack size equals capacity its full!
    if (stack_size(s) == s->capacity) {
        return true;
    }
    return false;
}
//pushes an node n on to the top of the stack
//returns true if it was a sucess
//returns false if the stack is full
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s) == true) {
        return false;
    }
    //put n in top index on items array
    s->items[s->top] = n;
    //increment top to next postion
    s->top += 1;
    return true;
}
//pops an item off the stack
//returns true if pop was sucessful
//returns false if stack is empty
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s) == true) {
        return false;
    }
    //decrement top to its prev position
    s->top -= 1;
    //passes value of item to pointer n
    *n = s->items[s->top];
    //set value of item to 0 now that it is popped
    s->items[s->top] = 0;
    return true;
}

// debug function
// prints symbol and freq of nodes in stack
// prints value of top of stack
void stack_print(Stack *s) {

    for (uint32_t i = 0; i < s->capacity; i += 1) {
        if (s->items[i] != NULL) {
            uint32_t freq = s->items[i]->frequency;
            uint32_t symb = s->items[i]->symbol;
            printf("%c:%u, ", symb, freq);
        }
    }
    if (stack_empty(s) == true) {
        printf("There are no nodes in the stack!");
    }
    printf("\nTop:%u\n", s->top);
}
