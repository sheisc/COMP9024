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

1.  Directed graph with labels

    digraph ShortestGraph {    
    "3" -> {"0"} [label="4"]
    "3" -> {"4"} [label="2"]
    "0" -> {"2"} [label="3"]
    "0" -> {"4"} [label="4"]
    "4" -> {"2"} [label="4"]
    "4" -> {"6"} [label="5"]
    "2" -> {"5"} [label="5"]
    "1" -> {"2"} [label="2"]
    "1" -> {"5"} [label="2"]
    "6" -> {"5"} [label="5"]
    "6" -> {"7"} [label="3"]
    }

2. Undirected graph without labels

    graph DfsGraph {    
    "3" -- {"0"}
    "0" -- {"2"}
    "0" -- {"4"}
    "4" -- {"2"}
    "2" -- {"5"}
    "2" -- {"1"}
    "2" -- {"6"}
    "1" -- {"5"}
    "6" -- {"7"}
    }

3.  Once we get a dot file, we can convert it into a png file.

$ dot -T png images/RecursiveDFS_0000.dot -o images/RecursiveDFS_0000.png   

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
        // if (displayVisited && visited) {
        //     for (long i = 0; i < pGraph->n; i++) {
        //         if (visited[i]) {
        //             char iDistance[MAX_ID_LEN+1] = {0};
        //             GenerateDistanceStr(pGraph, i, iDistance);
                   
        //             fprintf(dotFile, "\"Node %s:%s\" [color=red]\n", pGraph->pNodes[i].name, iDistance);
        //         }
        //     }
        // }
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

static long getNodeIdWithMinDistance(AdjMatrixElementTy *distances, int *visited, long n) {
    AdjMatrixElementTy min = INFINITY_VALUE;
    long minIndex = -1;
    for (long u = 0; u < n; u++) {
        if (!visited[u] && distances[u] <= min) {
            min = distances[u];
            minIndex = u;
        }
    }
    assert(minIndex != -1);
    return minIndex;
}

static void PrintTableLine(struct Graph *pGraph) {
    printf("\t-----------");
    for (long i = 0; i < pGraph->n; i++) {
        printf("---------");
    }
    printf("\n");
}

static void PrintDistancesAndVisited(struct Graph *pGraph, AdjMatrixElementTy *distances, int *visited, long *preNodeIds) {
    PrintTableLine(pGraph);

    printf("\tnode id:   ");
    for (long i = 0; i < pGraph->n; i++) {
        printf("%9ld", i);
    }
    printf("\n");

    PrintTableLine(pGraph);

    printf("\tdistances: ");
    for (long i = 0; i < pGraph->n; i++) {
        if (distances[i] == INFINITY_VALUE) {
            printf("%9s", "INF");
        } else {
            printf("%9ld", (long) distances[i]);            
        }      
    }
    printf("\n");

    printf("\tvisited:   ");
    for (long i = 0; i < pGraph->n; i++) {
        printf("%9d", visited[i]);                    
    }
    printf("\n");

    if (preNodeIds) {
        printf("\tpreNodeIds:");
        for (long i = 0; i < pGraph->n; i++) {
            printf("%9ld", preNodeIds[i]);                    
        }    
    }

    printf("\n");
    
    PrintTableLine(pGraph);
 
    printf("\n");
}

static long imgCnt = 0;

void Dijkstra(struct Graph *pGraph, long startNodeId) {
    assert(IsLegalNodeNum(pGraph, startNodeId));

    AdjMatrixElementTy *distances = pGraph->distances;
    int *visited = (int *) malloc(pGraph->n * sizeof(int));

    assert(visited);

    imgCnt = 0;     

    // 
    for (long i = 0; i < pGraph->n; i++) {
        distances[i] = INFINITY_VALUE;
        visited[i] = 0;
    }

    if (pGraph->isDirected) {
        GenOneImage(pGraph, "DijkstraDirected", "images/DijkstraDirected", imgCnt, visited);
    } else {
        GenOneImage(pGraph, "DijkstraUndirected", "images/DijkstraUndirected", imgCnt, visited);
    }
    //
    distances[startNodeId] = 0;
    // Find the shortest distances
    for (long i = 0; i < pGraph->n; i++) {
        printf("============================================== Step %ld ==============================================\n\n", i+1);
        long u = getNodeIdWithMinDistance(distances, visited, pGraph->n);
        PrintDistancesAndVisited(pGraph, distances, visited, NULL);

        visited[u] = 1;
        printf("Node %ld is selected\n\n", u);               
            
        imgCnt++;

        if (pGraph->isDirected) {
            GenOneImage(pGraph, "DijkstraDirected", "images/DijkstraDirected", imgCnt, visited);
        } else {
            GenOneImage(pGraph, "DijkstraUndirected", "images/DijkstraUndirected", imgCnt, visited);
        }
        int changed = 0;
        for (long v = 0; v < pGraph->n; v++) {
            if (!visited[v] && MatrixElement(pGraph, u, v) != 0 && distances[u] != INFINITY_VALUE) {
                if (distances[u] + MatrixElement(pGraph, u, v) < distances[v]) {
                    printf("Updating distances[%ld]: %ld --> ... --> %ld --> %ld; distance from %ld to %ld is %ld\n\n", v, 
                            startNodeId, u, v, u, v,(long) MatrixElement(pGraph, u, v));
                    distances[v] = distances[u] + MatrixElement(pGraph, u, v);
                    changed = 1;
                }
            }
        }
        if (changed) {
            imgCnt++;

            if (pGraph->isDirected) {
                GenOneImage(pGraph, "DijkstraDirected", "images/DijkstraDirected", imgCnt, visited);
            } else {
                GenOneImage(pGraph, "DijkstraUndirected", "images/DijkstraUndirected", imgCnt, visited);
            }            
        }
        PrintDistancesAndVisited(pGraph, distances, visited, NULL);
    }
    // Output the shortest distances
    for (long v = 0; v < pGraph->n; v++) {
        if (distances[v] == INFINITY_VALUE) {
            printf("Shortest path from node %s to node %s: INF \n", 
                    pGraph->pNodes[startNodeId].name, 
                    pGraph->pNodes[v].name);
        } else {
            printf("Shortest path from node %s to node %s: %ld\n", 
                    pGraph->pNodes[startNodeId].name, 
                    pGraph->pNodes[v].name, 
                    (long) distances[v]);
        }
    }

    free(visited); 
}
