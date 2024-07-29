#include <stdio.h>    // printf()
#include <stdlib.h>   // system()
#include <time.h>
#include "Graph.h"


#define NUM_OF_NODES  9


int TestTopologicalSort(long (*edges)[3], long n) {
    // directed graph
    printf("########################### TestTopologicalSort() ######################\n\n\n");
        
    struct Graph *pGraph = CreateGraph(NUM_OF_NODES, 1);

    char *nodeNames[NUM_OF_NODES] = {"Course 0", "Course 1", "Course 2", "Course 3", 
                        "Course 4", "Course 5", "Course 6", "Course 7", "Course 8"};
    //char *nodeNames[NUM_OF_NODES] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};

    // Add nodes
    for (long u = 0; u < NUM_OF_NODES; u++) {
        GraphAddNode(pGraph, u, nodeNames[u]);
    }

    // Add edges
    for (long i = 0; i < n; i++) {
        GraphAddEdge(pGraph, edges[i][0], edges[i][1], edges[i][2]);
    }

    PrintGraph(pGraph);

    TopologicalSort(pGraph);

    ReleaseGraph(pGraph);

    return 0;
}

int main(void) {
    srandom(time(NULL));

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");


    // edges: source node id, target node id, value of the edge
    long edges[][3] = {
        {0, 2, CONNECTED},
        {0, 3, CONNECTED},
        {0, 4, CONNECTED},
        {1, 3, CONNECTED},
        {1, 4, CONNECTED},
        {2, 5, CONNECTED},
        {2, 6, CONNECTED},
        {3, 2, CONNECTED},
        {3, 6, CONNECTED},
        {3, 7, CONNECTED},
        {4, 7, CONNECTED},
        {4, 8, CONNECTED},
        {5, 8, CONNECTED},
        {6, 8, CONNECTED},
        {7, 8, CONNECTED},
    };    
    TestTopologicalSort(edges, sizeof(edges)/sizeof(edges[0]));

    // with cycles
    long edges2[][3] = {
        {0, 2, CONNECTED},
        {0, 3, CONNECTED},
        {0, 4, CONNECTED},
        {1, 3, CONNECTED},
        {1, 4, CONNECTED},
        {2, 5, CONNECTED},
        {2, 6, CONNECTED},
        {3, 2, CONNECTED},
        {3, 6, CONNECTED},
        {3, 7, CONNECTED},
        {4, 8, CONNECTED},
        {5, 8, CONNECTED},
        {6, 4, CONNECTED},
        {7, 4, CONNECTED},
        {8, 7, CONNECTED},
        {8, 6, CONNECTED},
    };
    TestTopologicalSort(edges2, sizeof(edges2)/sizeof(edges2[0]));   
    return 0;
}


