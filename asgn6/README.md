# RSA Keygen/Encryption/Decryption

This program uses RSA key generation to encrypt and decrypt files given to the programs

## How to build the programs

With the included Makefile you will be able to build the programs through the terminal.
To do this, open the terminal, and navigate to the /asgn6 directory. Then in the terminal type
```
make all
```
This will run the compiler and create the executables to run the program.

## How to run the program
To get the help message of any program type
```
./program -h
```
First, generate your public and private key pairs. To do this, type in the terminal 
```
./keygen
```
Now to use these key pairs to encrypt a file, type in the terminal
```
./encrypt -i text.zzz -o encrypted.zzz
```
Finally to decrypt your encrypted file, type into the terminal
```
./decrypt -i encrypted.zzz -o text_decrypted.zzz
```

## How to clean up files after

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```
## Discrepancies

This program does not properly encrypt or decrypt files as well as leaks memory.
All of the numtheory functions work properly, as well as keygen.c
The issue has to do with the rsa encrypt and decrypt file functions. Everything else
works as indended.

