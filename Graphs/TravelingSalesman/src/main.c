#include <stdio.h>    // printf()
#include <stdlib.h>   // system()
#include "Graph.h"

#define NUM_OF_NODES  6

int TestTravelingSalesman(int isDirected) {
    // Create a directed graph with 8 nodes
    struct Graph *pGraph = CreateGraph(NUM_OF_NODES, isDirected);
    char *nodeNames[NUM_OF_NODES] = {"City0", "City1", "City2", "City3", "City4", "City5"};

    
    // Add nodes
    for (long u = 0; u < NUM_OF_NODES; u++) {
        GraphAddNode(pGraph, u, nodeNames[u]);
    }
    // edges: source node id, target node id, value of the edge
    long edges[][3] = {
        {0, 1, 2},
        {0, 2, 2},
        {0, 3, 5}, 
        {0, 4, 9},
        {0, 5, 3},        
        
        {1, 2, 4},
        {1, 3, 6},
        {1, 4, 7},
        {1, 5, 8},
        {2, 3, 8},

        {2, 4, 6}, 
        {2, 5, 3},
        {3, 4, 4},
        {3, 5, 9},
        {4, 5, 10},       
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

    TspPermute(pGraph);

    TspNearestNeighbour(pGraph, 0);

    //TspNearestNeighbour(pGraph, 1);

    ReleaseGraph(pGraph);

    return 0;
}

int main(void) {
    printf("\n\n\n\n\n########################### TestTravelingSalesman(undirected) ######################\n\n\n");
    // undirected graph
    TestTravelingSalesman(0);
    return 0;    
}



