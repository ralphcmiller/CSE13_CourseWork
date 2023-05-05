# Traveling Salesman Problem

This program uses custom ADTs and Depth-first search to read in an input graph and output the shortest Hamiltonian path.

## How to build the program

With the included Makefile you will be able to build the program through the terminal.
To do this, open the terminal, and navigate to the /asgn4 directory. Then in the terminal type
```
make
```
This will run the compiler and create the executable to run the program.

## How to run the program
To run, open the terminal, navigate to the /asgn4 directory and type 
```
./tsp -h
```
The terminal will then print the usage statements on how to run each test.
The usage statements are also provided below:
```
-h         : Help message. Prints the usage of the program
-v         : Verbose Printing. Prints each new optimal path as it is found
-u         : Uses an undirected graph
-i infile  : specifies the input file
-o outfile : specifies the output file
```

An example of running the program is shown below.
```
./tsp -i diego.graph -o graph.txt
```
## How to clean up files after

After running the program, to clean up excess output files, type in the terminal 
```
make clean
```
## Discrepancies

Due to extra optimizations in DFS, it is not possible to match the verbose output
of the example binary. This is due to the fact that everytime a recursive call is made
I am checking if the length is shorter than the current shortest path. I am also doing 
this when I am setting the shortest path. Therefore, the verbose output will only display
paths that are shorter than the previous shortest path.
