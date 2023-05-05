CC = clang 
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra
E_OBJ = encode.o node.o pq.o code.o io.o huffman.o stack.o
D_OBJ = decode.o io.o code.o node.o huffman.o stack.o pq.o
all: encode decode

encode: $(E_OBJ)
	$(CC) -o encode $(E_OBJ)
	
decode: $(D_OBJ)
	$(CC) -o decode $(D_OBJ)

clean:
	rm -f encode *.o
	rm -f decode *.o

format:
	clang-format -i -style=file *.[ch]
