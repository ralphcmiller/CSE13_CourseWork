#include "insert.h"

#include "stats.h"

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //start sort at array index 1, iterate until size of array is reached.
    for (uint32_t k = 1; k < n; k += 1) {
        uint32_t temp = A[k]; //hold A[k] so that we can move the array without losing A[k]
        move(stats, k); //counts a move for stats
        uint32_t j = k;
        //find A[k]'s spot in array, de-iterate j move back in the array.
        while (j > 0 && cmp(stats, temp, A[j - 1]) == -1) {
            A[j] = A[j - 1];
            move(stats, j);
            j -= 1;
        }
        A[j] = temp; //set A[k]'s spot in array
        move(stats, j);
    }
}
