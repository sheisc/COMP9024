#include <stdio.h>    // printf(), scanf()
#include <stdlib.h>   // malloc()
#include <string.h>   // memset()
#include <assert.h>   // assert()
#include <limits.h>   // LONG_MAX, INT_MAX
#include "Graph.h"
#include "Queue.h"

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

    AdjMatrixElementTy *pOrigMatrix;
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
#define  OrigElement(pGraph, u, v)    (pGraph)->pOrigMatrix[(pGraph)->n * (u) + (v)]
/*
    Create a graph which can contain n nodes
 */
struct Graph *CreateGraph(long n, int isDirected) {
    assert(n > 0);
    struct Graph *pGraph = (struct Graph *) malloc(sizeof(struct Graph));
    assert(pGraph);
    pGraph->pAdjMatrix = (AdjMatrixElementTy *) malloc(sizeof(AdjMatrixElementTy) * n * n);
    pGraph->pOrigMatrix = (AdjMatrixElementTy *) malloc(sizeof(AdjMatrixElementTy) * n * n);
    pGraph->pNodes = (struct GraphNode *) malloc(sizeof(struct GraphNode) * n);
    
    pGraph->distances = (AdjMatrixElementTy *) malloc(sizeof(AdjMatrixElementTy) * n);
    assert(pGraph->pAdjMatrix && pGraph->pNodes && pGraph->distances);

    memset(pGraph->pAdjMatrix, 0, sizeof(AdjMatrixElementTy) * n * n);
    memset(pGraph->pOrigMatrix, 0, sizeof(AdjMatrixElementTy) * n * n);
    memset(pGraph->pNodes, 0, sizeof(struct GraphNode) * n);
    pGraph->n = n;
    pGraph->isDirected = isDirected;
    
    return pGraph;
}

void ReleaseGraph(struct Graph *pGraph) {
    assert(pGraph);
    free(pGraph->pNodes);
    free(pGraph->pAdjMatrix);
    free(pGraph->pOrigMatrix);
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

        OrigElement(pGraph, u, v) = val;
        OrigElement(pGraph, v, u) = val;        
    }
}

/*
    Add a directed edge from u to v
 */
void GraphAddDirectedEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val) {
    if (IsLegalNodeNum(pGraph, u) && IsLegalNodeNum(pGraph, v)) {
        MatrixElement(pGraph, u, v) = val;

        OrigElement(pGraph, u, v) = val;
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


static void PrintPath(struct Graph *pGraph,  
                      long source, 
                      long sink, 
                      long *parents, 
                      AdjMatrixElementTy pathFlow) {
    // Print the path from source to sink
    printf("Path: ");
    long *path = (long *) malloc(sizeof(long) * pGraph->n);
    long len = 0;
    for (long v = sink; v != source; v = parents[v]) {
        path[len] = v;
        len++;
    }
    path[len] = source;
    len++;

    for (int i = len - 1; i >= 0; i--) {
        printf("%s", pGraph->pNodes[path[i]].name);
        if (i > 0) {
            printf(" -> ");
        }
    }
    printf(", Flow: %ld\n", (long) pathFlow);
    free(path);
}

int GetPathInBfs(struct Graph *pGraph, long source, long sink, long *parents, int *visited) {  
    memset(visited, 0, sizeof(int) * pGraph->n);

    struct Queue *pQueue = CreateQueue();
    assert(pQueue);

    QueueEnqueue(pQueue, source);
    visited[source] = 1;
    parents[source] = -1;

    while(!QueueIsEmpty(pQueue)) {
        long u = QueueDequeue(pQueue);
        for (long v = 0; v < pGraph->n; v++) {
            if (!visited[v] && MatrixElement(pGraph, u, v) > 0) {
                QueueEnqueue(pQueue, v);                
                visited[v] = 1;
                parents[v] = u;
                if (v == sink) {
                    ReleaseQueue(pQueue);                  
                    return 1;
                }   
            }                        
        }
    }
    printf("\n");
    ReleaseQueue(pQueue);
    return 0;
}

static long imgCount = 0;

AdjMatrixElementTy EdmondsKarpMF(struct Graph *pGraph, long source, long sink) {

    long *parents = (long *) malloc(sizeof(long) * pGraph->n);
    int *visited = (int *) malloc(pGraph->n * sizeof(int));
    long *path = (long *) malloc(sizeof(long) * pGraph->n);
    
    for (long i = 0; i < pGraph->n; i++) {
        parents[i] = -1;
        path[i] = -1;
        visited[i] = 0;
    }
    AdjMatrixElementTy maxFlow = 0;

    long len = 0;

    GenOneImage(pGraph, "EdmondsKarpMF", "images/EdmondsKarpMF", imgCount, visited, path, 0, 0);
    imgCount++;

    while (GetPathInBfs(pGraph, source, sink, parents, visited)) {
        AdjMatrixElementTy pathFlow = INFINITY_VALUE;

        len = 0;
        path[len] = sink;
        len++;

        // Get the minimum capacity in the found path
        for (long v = sink; v != source; v = parents[v]) {
            long u = parents[v];

            path[len] = u;
            len++;

            if (MatrixElement(pGraph, u, v) < pathFlow) {
                pathFlow = MatrixElement(pGraph, u, v);
            }
        }

        GenOneImage(pGraph, "EdmondsKarpMF", "images/EdmondsKarpMF", imgCount, visited, path, len, 1);
        imgCount++;

        for (long v = sink; v != source; v = parents[v]) {
            long u = parents[v];
            MatrixElement(pGraph, u, v) -= pathFlow;
            MatrixElement(pGraph, v, u) += pathFlow;
        }

        maxFlow += pathFlow;
        //
        PrintPath(pGraph, source, sink, parents, pathFlow);

        GenOneImage(pGraph, "EdmondsKarpMF", "images/EdmondsKarpMF", imgCount, visited, path, 0, 1);
        imgCount++;

        GenOneImage(pGraph, "EdmondsKarpMF", "images/EdmondsKarpMF", imgCount, visited, path, 0, 0);
        imgCount++;        
    }

    free(parents);
    free(visited);
    free(path);
    return maxFlow;   
}

////////////////////// Graph2Dot ////////////////////////////////////////////////

#define FILE_NAME_LEN  255

void GenOneImage(struct Graph *pGraph, 
                 char *graphName, 
                 char *fileName, 
                 long seqNo, 
                 int *visited,
                 long *path,
                 long len,
                 int displayReversedEdges) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Graph2Dot(pGraph, dotFileName, graphName, pGraph->isDirected, 1, visited, 1, path, len, displayReversedEdges);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

static int IsInPath(long u, long *path, long len) {
    for (long i = 0; i < len; i++) {
        if (path[i] == u) {
            return 1;
        }
    }
    return 0;
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
               int displayVisited,
               long *path,
               long len,
               int displayReversedEdges) {
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
                long origVal = OrigElement(pGraph, u, v);

                if (val || origVal) {
                    if (origVal == 0 && !displayReversedEdges) {
                        continue;
                    }                    
                    fprintf(dotFile, "\"%s\" %s {\"%s\"}", 
                            pGraph->pNodes[u].name, 
                            edgeConnectorStr, 
                            pGraph->pNodes[v].name);
                    if (origVal == 0) {
                        fprintf(dotFile, " [label=\"0/%ld\"]", val);
                        fprintf(dotFile, " [color=red] ");
                    } else {
                        fprintf(dotFile, " [label=\"%ld/%ld\"]", origVal - val, origVal);
                        if (val == 0) {
                            fprintf(dotFile, " [color=gray] ");
                        }
                    }                    
                    fprintf(dotFile, "\n"); 
                }
            }
        }

        /*
        "0" [color=red]
         */
        for (long i = 0; i < pGraph->n; i++) {
            // if (displayVisited && visited && visited[i]) {                
            //     fprintf(dotFile, "\"%s\" [color=red]", pGraph->pNodes[i].name);
            // } else {
            //     fprintf(dotFile, "\"%s\"", pGraph->pNodes[i].name);
            // }
            fprintf(dotFile, "\"%s\"", pGraph->pNodes[i].name);
            if (IsInPath(i, path, len)) {
                fprintf(dotFile, " [shape=box]\n");
            } else {
                fprintf(dotFile, "\n");
            }
        }        
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


