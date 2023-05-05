# The Great Santa Cruz firewall

This program reads input from stdin, compares it to a list of badspeak.txt and newspeak.txt, and returns a message if any of the filtered words are input.

## How to build the program

With the included Makefile you will be able to build the program through the terminal.
To do this, open the terminal, and navigate to the /asgn7 directory. Then in the terminal type
```
make all
```
This will run the compiler and create the executables to run the program.

## How to run the program
In the /asgn7 directory, open the terminal and type
```
./banhammer
```
You will see that the command line is waiting for input. Type the word you want to check against the filter and press enter. When all words are input, press Ctrl + D to submit the words. The program will teminate and if the word is found in the filter, a message will appear. Below are some examples to try.
```
./banhammer
take
press (Ctrl + D)

"Response Message"

----------------------------

./banhammer
g
take
press (Ctrl + D)

"Response Message"
```

You can also type the command below to print the program help and usage statement.
```
./banhammer -h
```
The usage statements are also provided below:
```
./banhammer [-hs] [-t size] [-f size]

   -h             Program help and usage.
   -s             Print program statistics.
   -t             Specify hash table size (default: 2^16).
   -f             Specify Bloom filter size (default 2^20).

```

## How to clean up files

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```

