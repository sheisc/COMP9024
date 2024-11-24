#include <stdio.h>    // printf()
#include <stdlib.h>   // system()
#include "Graph.h"


#define NUM_OF_NODES  6


int TestEdmondsKarpMF(void) {
    // Create a directed graph with 8 nodes
    struct Graph *pGraph = CreateGraph(NUM_OF_NODES, 1);

    char *nodeNames[NUM_OF_NODES] = {"Source", "Node1", "Node2", "Node3", "Node4", "Sink"};
    //char *nodeNames[NUM_OF_NODES] = {"0", "1", "2", "3", "4", "5"};
    
    // Add nodes
    for (long u = 0; u < NUM_OF_NODES; u++) {
        GraphAddNode(pGraph, u, nodeNames[u]);
    }

    // edges: source node id, target node id, value of the edge  
    long edges[][3] = {
        {0, 1, 3},
        {0, 2, 7},
        {1, 3, 3},
        {1, 4, 4},
        {2, 1, 5},
        
        {2, 4, 3},
        {3, 4, 3},
        {3, 5, 2}, 
        {4, 5, 6} 
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

    AdjMatrixElementTy maxFlow = EdmondsKarpMF(pGraph, 0, 5);

    printf("Max flow is %ld \n\n", (long) maxFlow);

    ReleaseGraph(pGraph);

    return 0;
}

int main(void) {
    printf("\n########################### TestEdmondsKarpMF() ######################\n\n");
    TestEdmondsKarpMF();

    return 0;    
}

