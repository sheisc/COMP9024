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
        pNodes ----->   pNodes[0]
                       ----------------------------
                        struct GraphNode[n] on Heap
     */
    struct GraphNode *pNodes;
    // number of nodes
    long n;
    // whether it is a directed graph
    int isDirected;
    /*
        Added for Dijkstra       

        Memory Layout
                        ---------------------------
                            distances[n-1]
       
       
                            distances[1]
        distances ----->    distances[0]
                       ----------------------------
                              on Heap
     */
    AdjMatrixElementTy *distances;
};

// 0 <= u < n,  0 <= v < n
// ELement(u, v) is (n * u + v) elements away from Element(0, 0)
#define  MatrixElement(pGraph, u, v)  (pGraph)->pAdjMatrix[(pGraph)->n * (u) + (v)]

//static long imgCount = 0;

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

/////////////////////////////////////  TravelingSalesman() ///////////////////////////////////////////
static long imgCount = 0;

static void Swap(long *pa, long *pb) {
    long t = *pa;
    *pa = *pb;
    *pb = t;
}

static AdjMatrixElementTy GetRouteLength(struct Graph *pGraph, long route[], long n) {
    AdjMatrixElementTy length = 0;

    for (long i = 0; i < n; i++) {
        long u = route[i];
        long v = route[(i+1) % n];
        long val = MatrixElement(pGraph, u, v);
        if (val == 0) { // not connected
            return INFINITY_VALUE;
        } else {
            length += val;
        }
    }
    return length;
}

static void PrintCurrentRoute(struct Graph *pGraph, long route[], long n, AdjMatrixElementTy minVal) {
    printf("The length of the route is %ld: ", (long) minVal);
    for (long i = 0; i < n; i++) {
        long u = route[i];
        printf("%s -> ", pGraph->pNodes[u].name);
        if (i == n -1) {
            long v = route[(i+1) % n];
            printf("%s\n\n", pGraph->pNodes[v].name);
        }
    }    
}

/*
    [left, right]
 */
static void PermuteAllRoutes(struct Graph *pGraph, long route[], long left, long right, 
                 long minRoute[], long n, AdjMatrixElementTy *pMinVal) {
    if (left >= right) {
        AdjMatrixElementTy length = GetRouteLength(pGraph, route, n);
        // if (length != INFINITY_VALUE) {
        //     PrintCurrentRoute(pGraph, route, n, length);
        // }
        if (length < *pMinVal) {
            *pMinVal = length;
            memcpy(minRoute, route, n * sizeof(long));
            PrintCurrentRoute(pGraph, route, n, length);
            GenOneImage(pGraph, "TravelingSalesman", "images/TravelingSalesman", imgCount, NULL, route);
            imgCount++;            
        }
    } else {
        for (long i = left; i <= right; i++) {
            Swap(route + left, route + i);
            PermuteAllRoutes(pGraph, route, left + 1, right, minRoute, n, pMinVal);
            Swap(route + left, route + i);
        }
    }
}

void TspPermute(struct Graph *pGraph) {
    long n = pGraph->n;
    long *route = (long *) malloc(sizeof(long) * n);
    long *minRoute = (long *) malloc(sizeof(long) * n);
    AdjMatrixElementTy minVal = INFINITY_VALUE;

    for (long u = 0; u < n; u++) {
        route[u] = u;
    }

    PermuteAllRoutes(pGraph, route, 0, n - 1, minRoute, n, &minVal);

    if (minVal != INFINITY_VALUE) {
        printf("\n\n******** The shortest route found in TspPermute()  ************ \n\n");
        PrintCurrentRoute(pGraph, minRoute, n, minVal);      
    } else {
        printf("\n\nNo route found in TspPermute().\n\n");
    }

    GenOneImage(pGraph, "TravelingSalesman", "images/TravelingSalesman", imgCount, NULL, minRoute);
    imgCount++;  

    free(route);
    free(minRoute);    
}

/*
    Starting from node u
 */
void TspNearestNeighbour(struct Graph *pGraph, long u) {
    assert(IsLegalNodeNum(pGraph, u));
    long n = pGraph->n;
    long *visited = (long *) malloc(sizeof(long) * n);
    // (n+1) slots allocated
    long *route = (long *) malloc(sizeof(long) * (n+1));
    for (long i = 0; i < n; i++) {
        visited[i] = 0;
    }
    route[0] = u;
    visited[u] = 1;
    long step = 1;
    for ( ; step < n; step++) {
        AdjMatrixElementTy minDistance = INFINITY_VALUE;
        long minNodeId = -1;
        // find the nearest unvisited neighbour        
        for (long v = 0; v < n; v++) {
            if (!visited[v]) {
                AdjMatrixElementTy distance = MatrixElement(pGraph, u, v);
                if (distance != 0 && distance < minDistance) {
                    minNodeId = v;
                    minDistance = distance;                 
                }
            }
        }
        u = minNodeId;
        if (minNodeId != -1) {            
            route[step] = minNodeId;
            visited[minNodeId] = 1;
        } else {
            break;
        }
    }
    if (step < n) {
        printf("\n\nNo route found in TspNearestNeighbour().\n\n");
    } else {
        AdjMatrixElementTy length = GetRouteLength(pGraph, route, n);
        printf("\n\n******** The shortest route in TspNearestNeighbour(%ld) ************ \n\n", route[0]);
        PrintCurrentRoute(pGraph, route, n, length);
        GenOneImage(pGraph, "TravelingSalesman", "images/TravelingSalesman", imgCount, NULL, route);
        imgCount++;                 
    }
    free(visited);
    free(route);
}

///////////////////////// Graph2Dot() ////////////////////////////////////////////////////////////////

#define FILE_NAME_LEN  255

void GenOneImage(struct Graph *pGraph, char *graphName, char *fileName, long seqNo, int *visited, long route[]) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Graph2Dot(pGraph, dotFileName, graphName, pGraph->isDirected, 1, visited, 1, route);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

// Test whether edge u-->v or v-->u is in the route[]
static int IsInRoute(long u, long v, long route[], long n, int isDirected) {
    for (long i = 0; i < n; i++) {
        int u2v = (route[i] == u && route[(i+1) % n] == v);
        int v2u = (route[i] == v && route[(i+1) % n] == u);
        if (isDirected && u2v) {
            return 1;
        }
        if (!isDirected && (u2v || v2u)) {
            return 1;
        }
    }
    return 0;
}

void Graph2Dot(struct Graph *pGraph, 
               char *filePath,
               char *graphName,
               int isDirectedGraph,
               int displayLabel,
               int *visited,
               int displayVisited,
               long route[]) {
    FILE *dotFile = fopen(filePath, "w");
    long n = pGraph->n;
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
                if (val) {
                    fprintf(dotFile, "\"%p\" %s \"%p\"", 
                            &(pGraph->pNodes[u]),
                            edgeConnectorStr, 
                            &(pGraph->pNodes[v]));
                    if (displayLabel) {
                        fprintf(dotFile, " [label=\"%ld\"]", val);
                    }
                    if (route && IsInRoute(u, v, route, n, pGraph->isDirected)) {
                        fprintf(dotFile, " [color=red]");
                    }
                    fprintf(dotFile, "\n"); 
                }
            }
        }
        for (long i = 0; i < pGraph->n; i++) {
            if (displayVisited && visited && visited[i]) {                
                fprintf(dotFile, "\"%p\" [label=\"%s\"] [color=red]\n", &(pGraph->pNodes[i]), pGraph->pNodes[i].name);
            } else {
                fprintf(dotFile, "\"%p\" [label=\"%s\"] \n", &(pGraph->pNodes[i]), pGraph->pNodes[i].name);
            }
        }        
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

