#include "quick.h"

#include <stdio.h>
// translated from the asgn doc python psuedocode
uint32_t partition(Stats *stats, uint32_t *A, uint32_t low, uint32_t hi) {
    uint32_t i = low - 1;
    for (uint32_t j = low; j < hi; j += 1) {
        if (cmp(stats, A[j - 1], A[hi - 1]) == -1) {
            i += 1;
            swap(stats, &A[i - 1], &A[j - 1]);
        }
    }
    swap(stats, &A[i], &A[hi - 1]);
    return i + 1;
}
void quick_sorter(Stats *stats, uint32_t *A, uint32_t low, uint32_t hi) {
    if (low < hi) {
        uint32_t p = partition(stats, A, low, hi);
        quick_sorter(stats, A, low, p - 1);
        quick_sorter(stats, A, p + 1, hi);
    }
}
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    quick_sorter(stats, A, 1, n);
}
