CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: tsp

tsp: tsp.o graph.o path.o stack.o
	$(CC) -o tsp tsp.o graph.o path.o stack.o -lm

tsp.o: tsp.c 
	$(CC) $(CFLAGS) -c tsp.c

clean:
	rm -f tsp *.o

format:
	clang-format -i -style=file *.[ch]
