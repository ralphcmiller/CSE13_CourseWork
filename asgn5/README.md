# Huffman Encoding/Decoding

This program uses utilizes Huffman encoding and decoding to compress and decompress files.

## How to build the program

With the included Makefile you will be able to build the program through the terminal.
To do this, open the terminal, and navigate to the /asgn5 directory. Then in the terminal type
```
make all
```
This will run the compiler and create the executables to run the program.

## How to run the program
To compress a file, navigate to the /asgn5 directory and type 
```
./encode -h
```
To decompress a file, type
```
./decode -h
```
The terminal will then print the usage statements on how to use encode or decode
The usage statements are also provided below:
```
-h         : Help message. Prints the usage of the program
-i infile  : specifies the input file
-o outfile : specifies the output file
```

An example of running the program is shown below.
```
./encode -i yourtext.txt -o compressed.txt
./decode -i compressed.txt -o decoded.txt
```
## How to clean up files after

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```
## Discrepancies

This program leaks memory.
It also does not compress at the resource binaries efficiency, but will compress a file that the resource encoder can read, and vice versa. IE, it functions properly.

