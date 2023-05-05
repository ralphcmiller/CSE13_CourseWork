CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: mathlib-test

mathlib-test: mathlib-test.o e.o newton.o euler.o madhava.o viete.o bbp.o
	$(CC) -o mathlib-test mathlib-test.o e.o newton.o euler.o madhava.o viete.o bbp.o -lm

mathlib-test.o: mathlib-test.c 
	$(CC) $(CFLAGS) -c mathlib-test.c

clean:
	rm -f mathlib-test *.o

format:
	clang-format -i -style=file *.[ch]
