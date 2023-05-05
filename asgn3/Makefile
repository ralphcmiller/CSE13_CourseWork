CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: sorting

sorting: sorting.o insert.o heap.o quick.o shell.o stats.o
	$(CC) -o sorting sorting.o insert.o heap.o quick.o shell.o stats.o -lm

sorting.o: sorting.c 
	$(CC) $(CFLAGS) -c sorting.c

clean:
	rm -f sorting *.o

format:
	clang-format -i -style=file *.[ch]
