#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define OPTIONS "uvhi:o:"

//dfs algoritm
static int calls = 0;
static bool found = false;
//verbose initalized up here so dfs can see it
static bool verbose = false;
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {
    //start at origin
    //mark v as visited
    graph_mark_visited(G, v);

    //whenever a vertex is marked as visited
    //it should be pushed onto the path
    path_push_vertex(curr, v, G);

    //find all edges that connect to v
    for (uint32_t w = 0; w < graph_vertices(G); w += 1) {

        if (graph_has_edge(G, v, w) == true && w != START_VERTEX) {
            //it has an edge from i to j and it is not the origin
            if (graph_visited(G, w) != true) {
                //w has not been visited
                //so we recursivly call dfs
                //so that v is now w
                if (path_length(curr) < path_length(shortest) || found == false) {
                    calls += 1;
                    dfs(G, w, curr, shortest, cities, outfile);
                }
            }
        }
    }

    //if all vertices have been visited
    //and and edge exsists between the origin and the top vertex
    //push the start vertex onto the stack
    if (graph_has_edge(G, v, START_VERTEX) && path_vertices(curr) == graph_vertices(G)) {
        path_push_vertex(curr, START_VERTEX, G);
        if (path_length(curr) < path_length(shortest) || found == false) {
            path_copy(shortest, curr);
            found = true;
            //if verbose printing is enabled
            if (verbose == true) {
                path_print(shortest, outfile, cities);
            }
        }
        //we want to remove the origin from the top of stack
        graph_mark_unvisited(G, v);
        path_pop_vertex(curr, &v, G);
    }
    //if a vertex is marked as unvisited
    //it should be popped off the path
    graph_mark_unvisited(G, v);
    path_pop_vertex(curr, &v, G);
}

int main(int argc, char **argv) {
    //initalize nessisary variables
    FILE *infile = stdin;
    FILE *outfile = stdout;

    int opt = 0;
    bool do_display = false;
    bool undirected = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'u': undirected = true; break;
        case 'v': verbose = true; break;
        case 'h': do_display = true; break;
        //credit to Eugene for io example
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        default: fprintf(stderr, ""); do_display = true;
        }
    }

    if (do_display) { //display program help and usage, true if -h, incorrect input, or no input.
        printf("SYNOPSIS\n   Traveling Salesman Problem using DFS.\n\n");
        printf("USEAGE\n   %s [-u] [-v] [-h] [-i infile] [-o outfile]\n\n", argv[0]);
        printf("OPTIONS\n");
        printf("   -u             Use undirected graph.\n");
        printf("   -v             Enable verbose printing.\n");
        printf("   -h             Program help and usage.\n");
        printf("   -i infile      Input containing graph (default: stdin)\n");
        printf("   -o outfile     Output of computed path (default: stdout)\n");
    }
    //if the display asked for do the rest of the program
    if (!do_display) {

        //gets number of vertices from file
        uint32_t numVertices;
        fscanf(infile, "%d", &numVertices);

        char buf[1024];
        char *cities_ptr = NULL;
        char *cities[numVertices];
        //initialize size of chars in each element of city array
        for (uint32_t i = 0; i < numVertices; i += 1) {
            cities[i] = calloc(sizeof(cities), sizeof(char));
        }

        //puts city names into cities[] array
        for (uint32_t i = 0; i < numVertices; i += 1) {
            fgets(buf, sizeof(buf), infile); //get line
            buf[strlen(buf) - 1] = '\0'; //gets rid of new line

            if (*buf != 0) {
                cities_ptr = strdup(buf);
                //appends string copyied to city index
                int n = 32; //max size of char
                strncat(cities[i], cities_ptr, n);
                //frees memory
                free(cities_ptr);
            } else {
                //buffer is empty and we want i = 0
                //for next iteration
                i -= 1;
            }
        }

        //we want to get the vertices now and construct our graph
        Graph *G = graph_create(numVertices, undirected);
        uint32_t vtex_i, vtex_j, weight = 0;
        while (fscanf(infile, "%d %d %d", &vtex_i, &vtex_j, &weight) != EOF) {
            if (vtex_i == 0 && vtex_j == 0) {
                //an origin vertex connecting itself would be useless
                //and it adds bugs for the solarsystem graph
                weight = 0;
            }
            graph_add_edge(G, vtex_i, vtex_j, weight);
        }

        //create two paths, one for current path traveled, one for shortest path
        Path *curr = path_create();
        Path *shortest = path_create();

        //call dfs
        calls += 1;
        dfs(G, START_VERTEX, curr, shortest, cities, outfile);

        //print our shortest path!
        path_print(shortest, outfile, cities);
        printf("Total recursive calls: %d\n", calls);

        //free all memory used
        path_delete(&curr);
        path_delete(&shortest);
        graph_delete(&G);
        fclose(infile);
        fclose(outfile);
        for (uint32_t i = 0; i < numVertices; i += 1) {
            free(cities[i]);
        }
    }
    return 0;
}
