#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
//Graph Struct from assignment doc
struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

//Graph Constructor from assignment doc
//uses the Graph ADT, inputs vertices and undirected
//outputs a pointer to the constructed graph
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

//Graph Destructor from assignment doc
//inputs a double pointer to Graph G
//frees memory associated with pointer G
//sets pointer G to null
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

//returns # of vertices in graph
uint32_t graph_vertices(Graph *G) {
    uint32_t counter = 0;
    for (uint32_t i = 0; i < G->vertices; i += 1) {
        counter += 1;
    }
    return counter;
}

//adds an edge of weight k from vertex i to vertex j
//an edge exsists if it has a non zero, pos weight
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    bool edge_sucess = false;
    //return true if both vertices are within bounds and the edges are sucessfully added
    if (j < G->vertices && i < G->vertices) {
        //graph is always assumed to first be directed
        //add an edge, with weight k from i to j
        if (k > 0) {
            G->matrix[j][i] = k;
            edge_sucess = true;
        }
        //if graph is undirected reflect the edge on the diagonal
        if (G->undirected == true && k > 0) {
            G->matrix[i][j] = k;
            edge_sucess = true;
        }
        if (edge_sucess == true) {
            return true;
        }
    }
    return false;
}

//returns true if vertex i, j are inbounds and an edge exsists
//from i to j. returns false otherwise
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (j < G->vertices && i < G->vertices && G->matrix[j][i] > 0) {
        return true;
    }
    return false;
}

//returns the weight of the edge from vertex i, j
//if i or j is not in bounds or
//if an edge doesnt exsist return 0
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (j < G->vertices && i < G->vertices && G->matrix[j][i] > 0) {
        uint32_t edge_weight = G->matrix[j][i];
        return edge_weight;
    }
    return 0;
}

//return true if vertex v has been visited
//else return false
bool graph_visited(Graph *G, uint32_t v) {
    if (G->visited[v] == true) {
        return true;
    }
    return false;
}

//if vertex v is within bounds, mark visited
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < VERTICES) {
        G->visited[v] = true;
    }
}
//if vertex v is within bounds, mark unvisited
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < VERTICES) {
        G->visited[v] = false;
    }
}
//prints 2d matrix
void graph_print(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; i += 1) {
        for (uint32_t x = 0; x < G->vertices; x += 1) {
            printf("%u ", G->matrix[i][x]);
        }
        printf("\n");
    }
}
