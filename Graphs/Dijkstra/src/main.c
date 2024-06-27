#include <stdio.h>    // printf()
#include <stdlib.h>   // system()
#include "Graph.h"


#define NUM_OF_NODES  8


int TestDijkstra(int isDirected) {
    // Create a directed graph with 8 nodes
    struct Graph *pGraph = CreateGraph(NUM_OF_NODES, isDirected);

    //char *nodeNames[NUM_OF_NODES] = {"A", "B", "C", "D", "E", "F", "G", "H"};
    char *nodeNames[NUM_OF_NODES] = {"0", "1", "2", "3", "4", "5", "6", "7"};
    
    // Add nodes
    for (long u = 0; u < NUM_OF_NODES; u++) {
        GraphAddNode(pGraph, u, nodeNames[u]);
    }
    // edges: source node id, target node id, value of the edge
    long edges[][3] = {
        {3, 0, 4},
        {3, 4, 2},
        {0, 2, 3},
        {0, 4, 4},
        {4, 2, 4},

        {4, 6, 5},
        {2, 5, 5},
        {1, 2, 2},
        {1, 5, 2},
        {6, 5, 5},
        {6, 7, 3},  
    }; 

    // Add edges
    for (long i = 0; i < sizeof(edges)/sizeof(edges[0]); i++) {
        GraphAddEdge(pGraph, edges[i][0], edges[i][1], edges[i][2]);
    }

    PrintGraph(pGraph);

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    long u = 3;

    printf("Dijkstra() starting from node %ld:\n\n", u);
    Dijkstra(pGraph, u);

    ReleaseGraph(pGraph);

    return 0;
}

int main(void) {
    // directed graph
    printf("########################### TestDijkstra(directed) ######################\n\n\n");
    TestDijkstra(1);
    printf("\n\n\n\n\n########################### TestDijkstra(undirected) ######################\n\n\n");
    // undirected graph
    //TestDijkstra(0);
    return 0;    
}



