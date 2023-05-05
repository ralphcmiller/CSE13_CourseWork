#include "pq.h"
#include <stdio.h>
#include <stdlib.h>
struct PriorityQueue {
    uint32_t front;
    uint32_t rear;
    uint32_t capacity;
    Node **items;
};

//----------------------------------------------------------------//

void swap(Node *x, Node *y) {
    Node t = *x;
    *x = *y;
    *y = t;
}

uint32_t min_child(PriorityQueue *q, uint32_t first, uint32_t last) {
    //get child index
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if (right <= last && q->items[right - 1]->frequency < q->items[left - 1]->frequency) {
        return right;
    }
    return left;
}
void fix_heap(PriorityQueue *q, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = min_child(q, mother, last);

    while (mother <= last / 2 && found == false) {
        if (q->items[mother - 1]->frequency > q->items[great - 1]->frequency) {
            swap(q->items[mother - 1], q->items[great - 1]);
            mother = great;
            great = min_child(q, mother, last);
        } else {
            found = true;
        }
    }
}

void build_heap(PriorityQueue *q, uint32_t n) {
    uint32_t first = 1;
    uint32_t last = n;
    for (uint32_t father = last / 2; father > first - 1; father -= 1) {
        fix_heap(q, father, last);
    }
}

//----------------------------------------------------------------------------//

//Constructor function
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *q = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (q) {
        q->front = 0;
        q->rear = 0;
        q->capacity = capacity;
        q->items = (Node **) calloc(capacity, sizeof(Node));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}
//Destructor function
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}
//Returns true if pq is empty
//Returns false otherwise
bool pq_empty(PriorityQueue *q) {
    if (q->front == 0 && q->rear == 0) {
        //if front and rear are at 0, its empty!
        return true;
    }
    return false;
}
//Returns true if pq is full
//Returns false otherwise
bool pq_full(PriorityQueue *q) {
    if (q->rear == q->capacity) {
        //if rear is at capacity, its full!
        return true;
    }
    return false;
}
//Returns size of pq
uint32_t pq_size(PriorityQueue *q) {
    //by returning the rear we can get the size of the pq!
    return q->rear;
}
//Enqueues a node into the pq
//If the pq is full return false
//Returns true if successful
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q) == true) {
        return false;
    }
    //put node in array at rear
    q->items[q->rear] = n;
    //increment rear
    q->rear += 1;
    return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q) == true) {
        return false;
    }
    //build min heap
    build_heap(q, q->rear);

    //decrement rear
    q->rear -= 1;

    //swap priority node at front to rear so it is ignored
    swap(q->items[0], q->items[q->rear]);

    //set pointer to priority node
    *n = q->items[q->rear];
    return true;
}

void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->capacity; i += 1) {
        uint32_t freq = q->items[i]->frequency;
        uint32_t symb = q->items[i]->symbol;
        printf("%c:%u, ", symb, freq);
    }
    if (pq_empty(q) == true) {
        printf("There are no nodes in the queue!");
    }
    printf("\nSize:%u\n", q->rear);
}
