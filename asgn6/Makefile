CC = clang 
CFLAGS = -g -Wall -Wpedantic -Werror -Wextra $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)
OBJ_K = keygen.o rsa.o numtheory.o randstate.o
OBJ_E = encrypt.o rsa.o numtheory.o randstate.o
OBJ_D = decrypt.o rsa.o numtheory.o randstate.o
all: keygen encrypt decrypt

keygen: $(OBJ_K)
	$(CC) -o keygen $(OBJ_K) $(LFLAGS)

encrypt: $(OBJ_E)
	$(CC) -o encrypt $(OBJ_E) $(LFLAGS)	

decrypt: $(OBJ_D)
	$(CC) -o decrypt $(OBJ_D) $(LFLAGS)

clean:
	rm -f keygen *.o
	rm -f encrypt *.o
	rm -f decrypt *.o

format:
	clang-format -i -style=file *.[ch]
