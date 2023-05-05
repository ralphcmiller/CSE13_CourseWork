#include "names.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

int main(void) {

    uint64_t input_seed;
    int num_players;
    int roll = 0;
    int first_roll = 0;
    int check_err;
    int position = 0; //position of pointArray
    int gameover = 0;
    //Query user for # of players, create dynamically sized array to store player points

    printf("How many players? ");
    check_err = scanf("%d", &num_players);
    if (check_err == 0) {
        fprintf(stderr, "Invalid number of players. Using 2 instead\n");
        num_players = 2;
    }
    if (num_players > 10 || num_players < 2) {
        fprintf(stderr, "Invalid number of players. Using 2 instead\n");
        num_players = 2;
    }

    //Initialize Player Point Array
    int pointArray[num_players - 1];
    for (int i = 0; i < num_players; i++) {
        pointArray[i] = 0;
    }

    //Query user for seed, validate user input, set random num generator.

    printf("Random seed: ");
    check_err = scanf("%lu", &input_seed);
    srand(input_seed);

    if (check_err == 0) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead\n");
        srand(2021);
    }

    //Roll Cases

    //while players turn is true, first roll, then add points to array, if its side increment player rolling, if points =100 end game.

    while (gameover == 0) {

        roll = pig[(rand() % 7)];
        //printf("%d", roll);
        if (first_roll == 0) {
            printf("%s rolls the pig...", names[position]);
            first_roll = 1;
        }
        if (roll == SIDE) {
            printf(" pig lands on side\n");
            first_roll = 0;
            if (position == num_players - 1) {
                position = 0; //reset to first player in array
            } else {
                position += 1; //increment player turn
            }
        }
        if (roll == RAZORBACK) {
            printf(" pig lands on back");
            pointArray[position] += 10; //+10 points
        }
        if (roll == TROTTER) {
            printf(" pig lands upright");
            pointArray[position] += 10; //+10 points
        }
        if (roll == SNOUTER) {
            printf(" pig lands on snout");
            pointArray[position] += 15; //+15 points
        }
        if (roll == JOWLER) {
            printf(" pig lands on ear");
            pointArray[position] += 5; //+5 points
        }

        //printf("%d ", pointArray[position]);
        if (pointArray[position] >= 100) {
            gameover = 1;
        }
    }
    printf("\n%s wins with %d points!\n", names[position], pointArray[position]);
}
