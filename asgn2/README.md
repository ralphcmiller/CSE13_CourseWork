# A Little Slice of Pi

This program implements mathmatical functions that minic the <math.h> library. It approximates e, pi, and sqrt(x),
then compares the output to the <math.h> libaray printing the difference between the two.

## How to build the program

With the included Makefile you will be able to build the program through the terminal.
To do this, open the terminal, and navigate to the /asgn2 directory. Then in the terminal type
```
make
```
This will run the compiler and create the executable to run the program.

## How to run the program

To run, open the terminal, navigate to the /asgn2 directory and type 
```
./mathlib-test -h
```
The terminal will then print the usage statements on how to run each test.
The usage statements are also provided below:
```
 -a: run all tests
 -e: runs e approximation test
 -b: runs Bailey-Borwein-Plouffe pi approximation test
 -m: runs Madhava pi approximation test
 -r: runs Euler sequence pi approximation test
 -v: runs Viete pi approximation test
 -n: runs Newton-Raphson square root approximation tests
 -s: enable printing of statistics to see computed terms/factors for each tested function
 -h: display a help message detailing program usage
```

## How to clean up files after

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```
