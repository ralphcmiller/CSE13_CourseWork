# Sorting

This program implements four different sorting algorithms. Insertion, Shell, Heap, and Quick Sort. It takes command line arguments that will run the different sorts, with the given number of elements, random seed, and prints the sorted array as well as statistical data collected.

## How to build the program

With the included Makefile you will be able to build the program through the terminal.
To do this, open the terminal, and navigate to the /asgn3 directory. Then in the terminal type
```
make
```
This will run the compiler and create the executable to run the program.

## How to run the program
To run, open the terminal, navigate to the /asgn3 directory and type 
```
./sorting -h
```
The terminal will then print the usage statements on how to run each test.
The usage statements are also provided below:
```
-a : Employs all sorting algorithms.
-e : Enables Heap Sort
-i : Enables Insertion Sort
-s : Enables Shell Sort.
-q : Enables Quicksort
-r seed : Set the random seed to seed. The default seed should be 13371453.
-n size : Set the array size to size. The default size should be 100.
-p elements : Print out elements number of elements from the array. 
-h : Prints out program usage.
```

An example of this for running Insertion Sort with 10 elements, a random seed of 1337, and printing out 0 elements would be.
```
./sorting -i -n 10 -r 1337 -p 0
```
## How to clean up files after

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```

