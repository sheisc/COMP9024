#include <stdio.h>    // printf(), scanf()
#include <stdlib.h>   // malloc()
#include <string.h>   // memset()
#include <assert.h>   // assert()
#include <limits.h>   // LONG_MAX, INT_MAX
#include "Graph.h"

// Max length of an identifier (e.g., a graph node's name) 
#define MAX_ID_LEN 127


// Storing information of a graph node
struct GraphNode {
    char name[MAX_ID_LEN + 1];
};

struct Graph{
    /*
       Memory Layout:
                          -----------------------------------------------------------
        pAdjMatrix ---->  Element(0, 0),   Element(0, 1),    ...,       Element(0, n-1),     // each row has n elements
                          Element(1, 0),   Element(1, 1),    ...,       Element(1, n-1),
      
                          .....                            Element(u, v)     ...             // (n * u + v) elements away from Element(0, 0)
      
                          Element(n-1, 0), Element(n-1, 1),  ...,       Element(n-1, n-1)
                          ----------------------------------------------------------- 
                                      Adjacency Matrix on Heap

     */
    AdjMatrixElementTy *pAdjMatrix;
    /*
       Memory Layout
                        ---------------------------
                        pNodes[n-1]
       
       
                        pNodes[1]
       pNodes ----->    pNodes[0]
                       ----------------------------
                        struct GraphNode[n] on Heap
     */
    struct GraphNode *pNodes;
    // number of nodes
    long n;
    // whether it is a directed graph
    int isDirected;
    // Added for Dijkstra
    AdjMatrixElementTy *distances;
};

// 0 <= u < n,  0 <= v < n
// ELement(u, v) is (n * u + v) elements away from Element(0, 0)
#define  MatrixElement(pGraph, u, v)  (pGraph)->pAdjMatrix[(pGraph)->n * (u) + (v)]
/*
    Create a graph which can contain n nodes
 */
struct Graph *CreateGraph(long n, int isDirected) {
    assert(n > 0);
    struct Graph *pGraph = (struct Graph *) malloc(sizeof(struct Graph));
    assert(pGraph);
    pGraph->pAdjMatrix = (AdjMatrixElementTy *) malloc(sizeof(AdjMatrixElementTy) * n * n);
    pGraph->pNodes = (struct GraphNode *) malloc(sizeof(struct GraphNode) * n);
    
    pGraph->distances = (AdjMatrixElementTy *) malloc(sizeof(AdjMatrixElementTy) * n);
    assert(pGraph->pAdjMatrix && pGraph->pNodes && pGraph->distances);

    memset(pGraph->pAdjMatrix, 0, sizeof(AdjMatrixElementTy) * n * n);
    memset(pGraph->pNodes, 0, sizeof(struct GraphNode) * n);
    pGraph->n = n;
    pGraph->isDirected = isDirected;
    
    return pGraph;
}

void ReleaseGraph(struct Graph *pGraph) {
    assert(pGraph);
    free(pGraph->pNodes);
    free(pGraph->pAdjMatrix);
    free(pGraph->distances);
    free(pGraph);
}

static int IsLegalNodeNum(struct Graph *pGraph, long u) {
    return (u >= 0) && (u < pGraph->n); 
}

/*
    Add an undirected edge between u and v
 */

void GraphAddUndirectedEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val) {
    if (IsLegalNodeNum(pGraph, u) && IsLegalNodeNum(pGraph, v)) {
        MatrixElement(pGraph, u, v) = val;
        MatrixElement(pGraph, v, u) = val;
    }
}

/*
    Add a directed edge from u to v
 */
void GraphAddDirectedEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val) {
    if (IsLegalNodeNum(pGraph, u) && IsLegalNodeNum(pGraph, v)) {
        MatrixElement(pGraph, u, v) = val;
    }
}

/*
    Add a directed edge from u to v, or an undirected edge between u and v
 */
void GraphAddEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val) {
    if (pGraph->isDirected) {
        GraphAddDirectedEdge(pGraph, u, v, val);
    } else {
        GraphAddUndirectedEdge(pGraph, u, v, val);
    }

}

void GraphAddNode(struct Graph *pGraph, long u, char *name) {
    if (IsLegalNodeNum(pGraph, u)) {        
        snprintf(pGraph->pNodes[u].name, MAX_ID_LEN, "%s", name);
    }
}

// for debugging
void PrintGraph(struct Graph *pGraph) {
    printf("**********  The Adjacency Matrix ************* \n");
    for (long u = 0; u < pGraph->n; u++) {
        for (long v = 0; v < pGraph->n; v++) {
            printf("%ld ", (long) (MatrixElement(pGraph, u, v)));
        }
        printf("\n");
    }
    printf("\n****** Graph Nodes ********\n");
    for (long u = 0; u < pGraph->n; u++) {
        printf("Graph Node %ld: %s\n", u, pGraph->pNodes[u].name);
    }
    printf("\n");
}

#define FILE_NAME_LEN  255

void GenOneImage(struct Graph *pGraph, char *graphName, char *fileName, long seqNo, int *visited) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Graph2Dot(pGraph, dotFileName, graphName, pGraph->isDirected, 1, visited, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

static void GenerateDistanceStr(struct Graph *pGraph, long u, char *distanceBuf) {
    if (pGraph->distances[u] == INFINITY_VALUE) {
        snprintf(distanceBuf, MAX_ID_LEN, "INF");
    } else {
        snprintf(distanceBuf, MAX_ID_LEN, "%ld", (long) pGraph->distances[u]);
    }
}

/*
    Dot Files
 */
void Graph2Dot(struct Graph *pGraph, 
               char *filePath,
               char *graphName,
               int isDirectedGraph,
               int displayLabel,
               int *visited,
               int displayVisited) {
    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "";
        if (isDirectedGraph) {
            edgeConnectorStr = "->";
            fprintf(dotFile, "digraph %s {\n", graphName);
        } else {
            edgeConnectorStr = "--";
            fprintf(dotFile, "graph %s {\n", graphName);
        }

        for (long u = 0; u < pGraph->n; u++) {
            long vStart = u;
            if (isDirectedGraph) {
                vStart = 0;
            }
            for (long v = vStart; v < pGraph->n; v++) {
                long val = MatrixElement(pGraph, u, v);

                char uDistance[MAX_ID_LEN+1] = {0};
                char vDistance[MAX_ID_LEN+1] = {0};
                GenerateDistanceStr(pGraph, u, uDistance);
                GenerateDistanceStr(pGraph, v, vDistance);

                if (val) {
                    fprintf(dotFile, "\"Node %s:%s\" %s {\"Node %s:%s\"}", 
                            pGraph->pNodes[u].name, uDistance,
                            edgeConnectorStr, 
                            pGraph->pNodes[v].name, vDistance);
                    if (displayLabel) {
                        fprintf(dotFile, " [label=\"%ld\"]", val);
                    }
                    fprintf(dotFile, "\n"); 
                }
            }
        }
        /*
        "0" [color=red]
         */
        for (long i = 0; i < pGraph->n; i++) {
            char iDistance[MAX_ID_LEN+1] = {0};
            GenerateDistanceStr(pGraph, i, iDistance);
            if (displayVisited && visited && visited[i]) {                
                fprintf(dotFile, "\"Node %s:%s\" [color=red]\n", pGraph->pNodes[i].name, iDistance);
            } else {
                fprintf(dotFile, "\"Node %s:%s\"\n", pGraph->pNodes[i].name, iDistance);
            }
        }        
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

static void DisplayPath(struct Graph *pGraph, long *preds, long v) {
    long *path = (long *) malloc(sizeof(long) * pGraph->n);
    long last = pGraph->n;
    do {
        last--;
        path[last] = v;
        v = preds[v];
    } while (v != -1);

    printf("\t");
    for (long i = last; i < pGraph->n; i++) {
        printf("%s",  pGraph->pNodes[path[i]].name);   
        if (i != pGraph->n - 1) {
            printf(" --> ");
        }        
    }
    printf("\n\n");
    free(path);
} 


static long imgCount = 0;

/*
    If negative cycles are found,
        return 1
    else 
        return 0
 */
int BellmanFord(struct Graph *pGraph, long startNodeId) {
    AdjMatrixElementTy *distances = pGraph->distances;
    long *preds = (long *) malloc(sizeof(long) * pGraph->n);
    assert(preds);

    for (long i =  0; i < pGraph->n; i++) {
        distances[i] = INFINITY_VALUE;
        preds[i] = -1;
    }

    distances[startNodeId] = 0;

    GenOneImage(pGraph, "BellmanFord", "images/BellmanFord", imgCount, NULL);
    imgCount++;

    // checking all edges (n - 1) times
    for (long i = 0; i < pGraph->n - 1; i++) {
        for (long u = 0; u < pGraph->n; u++) {
            for (long v = 0; v < pGraph->n; v++) {
                if (MatrixElement(pGraph, u, v)) {
                    if (distances[u] != INFINITY_VALUE && distances[u] + MatrixElement(pGraph, u, v) < distances[v]) {
                        distances[v] = distances[u] + MatrixElement(pGraph, u, v);
                        preds[v] = u;
                    }
                }
            }
        }
        GenOneImage(pGraph, "BellmanFord", "images/BellmanFord", imgCount, NULL);
        imgCount++;          
    }

    // checking all edges one more time to detect negative cycles
    for (long u = 0; u < pGraph->n; u++) {
        for (long v = 0; v < pGraph->n; v++) {
            if (MatrixElement(pGraph, u, v)) {
                if (distances[u] != INFINITY_VALUE && distances[u] + MatrixElement(pGraph, u, v) < distances[v]) {
                    // for visualizing the algorithm
                    distances[v] = distances[u] + MatrixElement(pGraph, u, v);
                    GenOneImage(pGraph, "BellmanFord", "images/BellmanFord", imgCount, NULL);
                    imgCount++;

                    free(preds);
                    return 1;  // negative cycles found
                }
            }
        }
    }    

    // Output the shortest distances
    for (long v = 0; v < pGraph->n; v++) {
        if (distances[v] == INFINITY_VALUE) {
            printf("The shortest path from node %s to node %s: INF \n", 
                    pGraph->pNodes[startNodeId].name, 
                    pGraph->pNodes[v].name);
        } else {
            printf("The shortest path from node %s to node %s: %ld\n", 
                    pGraph->pNodes[startNodeId].name, 
                    pGraph->pNodes[v].name, 
                    (long) distances[v]);
            DisplayPath(pGraph, preds, v);
        }
    }

    free(preds);
    return 0;
}

