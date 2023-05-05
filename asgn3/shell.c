#include "shell.h"

#include "stats.h"

#include <math.h>

int largest_k_generator(uint32_t n) {
    //calculate largest k
    // log(2n+3) / log(3)
    double numerator = log(2 * n + 3);
    double denominator = log(3);
    int k_largest = numerator / denominator;
    return k_largest;
}
int gap_generator(int k) {
    //calculate gap sequence
    int gap;
    int power_3 = pow(3, k); //calculate 3^k
    double numerator = power_3 - 1;
    double denominator = 2;
    gap = numerator / denominator;
    return gap;
}
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //set k to 0 as we want to start with the largest gap
    int k = 0;
    int k_largest = largest_k_generator(n);
    uint32_t gap = gap_generator(k_largest - k);
    while (gap > 1) { //loop through gap until gap = 0
        gap = gap_generator(k_largest - k);
        for (uint32_t i = gap; i < n; i += 1) {
            uint32_t temp = A[i];
            move(stats, i);
            uint32_t j = i; //set j to gap size
            while (j >= gap && cmp(stats, temp, A[j - gap]) == -1) {
                A[j] = A[j - gap];
                move(stats, j);
                j -= gap;
            }
            A[j] = temp; //set A[i]'s spot in array
            move(stats, j);
        }
        k += 1; //iterate to generate next gap
    }
}
