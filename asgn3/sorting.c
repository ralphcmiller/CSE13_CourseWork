#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//bitmasking function to bitmask randomly generated elements to 30 bits
uint32_t bitmask_30(uint32_t num) {
    uint32_t bitmask = 0x3FFFFFFF;
    return bitmask & num;
}

//generating dynamic array, sized by legnth specified, with each element holding 32 bits.
//code from Eugenes section for allocating memory with an empty array using  calloc
uint32_t *generate_array(uint32_t n) {
    uint32_t *generated_A = (uint32_t *) calloc(n, sizeof(uint32_t));
    for (uint32_t i = 0; i < n; i += 1) { //populate array using rand() and bitmask_30()
        generated_A[i] = bitmask_30(rand());
    }
    return generated_A;
}

#define OPTIONS "haeisqn:p:r:"
typedef enum { INSERTION, HEAP, QUICK, SHELL, NUM_SORTS } Sorts;

void (*fn_pointer[])(Stats *stats, uint32_t *A, uint32_t n) = {
    insertion_sort, heap_sort, quick_sort, shell_sort
}; //array of pointers to sort functions, big brain idea from Mr.Long in the discord.

const char *names[] = { "Insertion Sort", "Heap Sort", "Quick Sort", "Shell Sort" };

int main(int argc, char **argv) {

    // set default values for array elements, elements to print, and random seed.
    uint32_t n, elements, seed;
    n = 100; //size of array
    elements = n;
    seed = 13371453;

    Stats stats;
    stats.moves = 0;
    stats.compares = 0;
    Set s = empty_set();

    int opt = 0;
    bool do_display;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            s = insert_set(INSERTION, s);
            s = insert_set(HEAP, s);
            s = insert_set(SHELL, s);
            s = insert_set(QUICK, s);
            break;
        case 'i': s = insert_set(INSERTION, s); break;
        case 'e': s = insert_set(HEAP, s); break;
        case 's': s = insert_set(SHELL, s); break;
        case 'q': s = insert_set(QUICK, s); break;
        case 'h': do_display = true; break;
        case 'n': n = abs(atoi(optarg)); break;
        case 'p':
            elements = atoi(optarg);
            if (elements > n) {
                printf("Too many elements to print, defaulting to: %u\n", n);
                elements = n;
            }
            break;
        case 'r': seed = atoi(optarg); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }

    //setting srand
    srand(seed);

    // do stuff *//

    if (s == 0) { //if there is no input print out program help
        do_display = true;
    }

    for (Sorts x = INSERTION; x < NUM_SORTS; x += 1) {
        if (member_set(x, s)) {
            do_display = false;
            reset(&stats); //reset statisitcs at before calling each sort function.
            uint32_t *A = generate_array(n); //generate array to sort
            fn_pointer[x](&stats, A, n); // big brain idea suggested by Mr.Long.com in the discord.
            printf("%s, %u elements, %" PRIu64 " moves, %" PRIu64 " compares\n", names[x], n,
                stats.moves, stats.compares);

            //print out array as a table of 5 elements
            int table = 0;
            if (elements != 0) { //if # elements == 0 dont print.
                for (uint32_t i = 0; i < n; i += 1) {
                    printf("%13" PRIu32, A[i]);
                    table += 1;
                    if (table == 5) {
                        printf("\n");
                        table = 0;
                    }
                }
            }
            free(A); //free memory allocated by generate_array();
        }
    }

    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms\n\n");
        printf("USEAGE\n   %s -[haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -h              display program help and usage.\n");
        printf("   -a              enable all sorts.\n");
        printf("   -e              enable Heap Sort.\n");
        printf("   -i              enable Insertion Sort.\n");
        printf("   -s              enable Shell Sort.\n");
        printf("   -q              enable Quick Sort.\n");
        printf("   -n length       specify number of array elements (default: 100).\n");
        printf("   -p elements     specify number of elements to print (default: 100).\n");
        printf("   -r seed         specify random seed (default: 13371453).\n");
    }
    return 0;
}
