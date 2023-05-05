# Pass the Pigs

This game is based on a simplified version of David Moffat's game, Pass the Pigs, in which players roll a 'die' and each dice roll corresponds to a pigs side. Depending on what the player rolls, they collect different amounts of points. First to 100 wins! A players turn is ended if the player rolls one of the two rolls which corresponds to the pigs side. Then the next players turn begins.

The program takes user input for the amount of players, 2-10, and a seed for the random number generator. Then will run and output each players rolls until a winner is declared. Then the program terminates

## How to build the program

With the included make file all you will need to be in the /asgn1 directory and type 'make' into the terminal.
This will run the Makefile included which is as follows:

```
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: pig

pig: pig.o
	$(CC) -o pig pig.o

pig.o: pig.c names.h
	$(CC) $(CFLAGS) -c pig.c

clean:
	rm -f pig pig.o

format:
	clang-format -i -style=file *.[ch]

```

## How to run the program

To run Pass the Pigs you will need to be in the /asgn1 directory and type ./pig into the terminal.
