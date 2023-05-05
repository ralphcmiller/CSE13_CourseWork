CC = clang 
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra
OBJ = banhammer.o bv.o bf.o speck.o node.o bst.o ht.o parser.o
all: banhammer format

banhammer: $(OBJ)
	$(CC) -o banhammer $(OBJ) -lm 

clean:
	rm -f banhammer *.o

format:
	clang-format -i -style=file *.[ch]
