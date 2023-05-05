#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//Stack struct included in Asgn doc
struct Stack {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};

void stack_testprint(Stack *s) {
    for (uint32_t i = 0; i < s->capacity; i += 1) {
        //printf("%u ", s->items[i]);
    }
    //printf("\nTop:%u\n", s->top);
}

//Constructor function for stack
//included in asgn doc
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    stack_testprint(s);
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
    //if the stack size equals capacity - 1 its full!
    if (stack_size(s) == s->capacity) {
        return true;
    }
    return false;
}
//pushes an item x on to the top of the stack
//returns true if it was a sucess
//returns false if the stack is full
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s) == true) {
        return false;
    }
    //put x in top index on items array
    s->items[s->top] = x;
    //increment top to next postion
    s->top += 1;
    stack_testprint(s);
    return true;
}
//pops an item off the stack
//returns true if pop was sucessful
//returns false if stack is empty
bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s) == true) {
        return false;
    }
    //decrement top to its prev position
    s->top -= 1;
    //passes value of item to pointer x
    *x = s->items[s->top];
    //set value of item to 0 now that it is popped
    s->items[s->top] = 0;

    stack_testprint(s);
    return true;
}
//peek into the stack to see the element currently at top
//if the stack is empty return false
//return true if sucessful
bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s) == true) {
        return false;
    }
    //deincrement top
    s->top -= 1;
    //passes value of item to pointer x
    *x = s->items[s->top];
    //re-increment top
    s->top += 1;
    return true;
}
//copys the entire stack of src to dst
//including the value of top
void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t i = 0; i < src->capacity; i += 1) {
        dst->items[i] = src->items[i];
    }
    dst->top = src->top;
}
//included in asgn doc
//prints out the contents of the stack
//each vertex in the stack corresponds to the name of a city
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
