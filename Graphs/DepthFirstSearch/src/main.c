#include <stdio.h>    // printf()
#include <stdlib.h>   // system()
#include "Graph.h"



#define NUM_OF_NODES  8


int TestDFS(int isDirected) {
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
        {0, 2, CONNECTED},
        {1, 5, CONNECTED},
        {2, 1, CONNECTED},
        {2, 4, CONNECTED},
        {2, 5, CONNECTED},        
        {3, 0, CONNECTED},
        {4, 0, CONNECTED},
        {4, 7, CONNECTED},        
        {6, 2, CONNECTED},        
        {7, 6, CONNECTED},        
    };

    // Add edges
    for (long i = 0; i < sizeof(edges)/sizeof(edges[0]); i++) {
        GraphAddEdge(pGraph, edges[i][0], edges[i][1], edges[i][2]);
    }

    PrintGraph(pGraph);

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    //system("rm -f images/*.dot images/*.png");

    RecursiveDFS(pGraph);

    ReleaseGraph(pGraph);

    return 0;
}


int main(void) {
    // directed graph
    printf("########################### TestDFS(directed) ######################\n\n\n");
    TestDFS(1);
    printf("\n\n\n\n\n########################### TestDFS(undirected) ######################\n\n\n");
    // undirected graph
    TestDFS(0);
    return 0;
}



