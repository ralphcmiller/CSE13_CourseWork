#include "path.h"

#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
struct Path {
    Stack *vertices;
    uint32_t length;
};
//Path Constructor
//set vertices as a freshly created stack of capacity VERTICES
//initilizes length to 0
//length tracks the sum of the edge weights between vertices in the vertices stack
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->vertices = stack_create(VERTICES);
    p->length = 0;
    return p;
}
//Path Destructor
//we need to call stack_delete for p->vertices
//then we need to free p->length
//and then set pointer p to null
void path_delete(Path **p) {
    stack_delete(&(*p)->vertices);
    free(*p);
    *p = NULL;
    return;
}
//pushes vertex v onto path p
//length of path is increased by edge weight
//connecting vertex at the top of the stack and v
//return ture if vertex successful
//return false otherwise
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    //get top of vertices stack
    uint32_t x = 0;
    uint32_t *ptr = &x;
    stack_peek(p->vertices, ptr);
    //pushes vertex v onto path p
    if (stack_push(p->vertices, v) == true) {
        //increase length by edge weight of
        //vertices stack top and v
        p->length += graph_edge_weight(G, x, v);
        return true;
    }
    return false;
}
//pops vertices stack
//passes popped vertex to pointer v
//length of path is decreased by edge weight that
//connects top of stack and popped vertex
//returns ture if vertex sucessfully popped
//returns false otherwise
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_pop(p->vertices, v) == true) {
        //get top of vertices stack
        uint32_t x = 0;
        uint32_t *ptr = &x;
        stack_peek(p->vertices, ptr);
        //decreases length by edge weight of
        //vertices stack top and v
        p->length -= graph_edge_weight(G, x, *v);
        return true;
    }
    return false;
}
//returns number of vertices in the path
//uses stack_size function as vertices is a stack
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}
//returns length of path
uint32_t path_length(Path *p) {
    return p->length;
}
//copys the entire stack of src to dst
//including the value of top
//makes a copy of vertices stack and length
void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
}
//prints out the vertices stack and path length
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %u\n", p->length);
    printf("Path: ");
    stack_print(p->vertices, outfile, cities);
}
