#include <stdio.h>    // printf(), scanf()
#include <stdlib.h>   // malloc()
#include <string.h>   // memset()
#include <assert.h>   // assert()
#include "Queue.h"
#include "Graph.h"


// Max length of an identifier (e.g., a graph node's name) 
#define MAX_ID_LEN 127


// Storing information of a graph node
struct GraphNode {
    char name[MAX_ID_LEN + 1];
    // for topological sorting
    long inDegree;
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
    assert(pGraph->pAdjMatrix && pGraph->pNodes);
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

    Graph2Dot(pGraph, dotFileName, graphName, pGraph->isDirected, 0, visited, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

/*

digraph topoSort {
"Job 0_In=0" -> {"Job 2_In=0"} [style=invis]
"Job 0_In=0" -> {"Job 3_In=0"} [style=invis]
"Job 0_In=0" -> {"Job 4_In=0"} [style=invis]
"Job 1_In=0" -> {"Job 3_In=0"} [style=invis]
"Job 1_In=0" -> {"Job 4_In=0"} [style=invis]
"Job 2_In=0" -> {"Job 5_In=0"} [style=invis]
"Job 2_In=0" -> {"Job 6_In=0"} [style=invis]
"Job 3_In=0" -> {"Job 2_In=0"} [style=invis]
"Job 3_In=0" -> {"Job 6_In=0"} [style=invis]
"Job 3_In=0" -> {"Job 7_In=0"} [style=invis]
"Job 4_In=0" -> {"Job 7_In=0"} [style=invis]
"Job 4_In=0" -> {"Job 8_In=3"} [style=invis]
"Job 5_In=0" -> {"Job 8_In=3"} 
"Job 6_In=0" -> {"Job 8_In=3"} 
"Job 7_In=0" -> {"Job 8_In=3"} 
"Job 0_In=0" [color=red]
"Job 1_In=0" [color=red]
"Job 2_In=0" [color=red]
"Job 3_In=0" [color=red]
"Job 4_In=0" [color=red]
"Job 5_In=0"
"Job 6_In=0"
"Job 7_In=0"
"Job 8_In=3"
}
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
                if (val) {
                    char *edgeStyleStr = "";
                    if (visited && visited[u]) {
                        edgeStyleStr = "[style=invis]";
                    }
                    fprintf(dotFile, "\"%s_In=%ld\" %s {\"%s_In=%ld\"} %s", 
                            pGraph->pNodes[u].name, pGraph->pNodes[u].inDegree,
                            edgeConnectorStr, 
                            pGraph->pNodes[v].name, pGraph->pNodes[v].inDegree,
                            edgeStyleStr);
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
        //             fprintf(dotFile, "\"%s\" [color=red]\n", pGraph->pNodes[i].name);
        //         }
        //     }
        // }
        for (long i = 0; i < pGraph->n; i++) {
            if (displayVisited && visited && visited[i]) {
                fprintf(dotFile, 
                        "\"%s_In=%ld\" [color=red]\n", 
                        pGraph->pNodes[i].name,
                        pGraph->pNodes[i].inDegree);
            } else {
                fprintf(dotFile, 
                        "\"%s_In=%ld\"\n", 
                        pGraph->pNodes[i].name,
                        pGraph->pNodes[i].inDegree);
            }
        }                
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

#define ILLEGAL_NODE_ID     -1

static void EnqueueNodeIds(long *nodeIds, long count, struct Queue *pQueue, long randomly) {
    if (randomly) {
        long x;
        long n = 0;
        while (n < count) {
            x = random();
            x %= count;
            // If nodeIds[x] has not been processed yet
            if (nodeIds[x] != ILLEGAL_NODE_ID) {
                QueueEnqueue(pQueue, nodeIds[x]);
                nodeIds[x] = ILLEGAL_NODE_ID;
                n++;
            }
        }
    } else {
        for (long i = 0; i < count; i++) {
            QueueEnqueue(pQueue, nodeIds[i]);
        }
    }
}

long GetInDegree(struct Graph *pGraph, long v) {
    assert(IsLegalNodeNum(pGraph, v));
    long inDegree = 0;
    for (long u = 0; u < pGraph->n; u++) {
        if (MatrixElement(pGraph, u, v)) {
            inDegree++;            
        }
    }
    return inDegree;
}

static long topoImageCnt = 0;

void TopologicalSort(struct Graph *pGraph) {
    int *visited = (int *) malloc(pGraph->n * sizeof(int));
    long *nodeIds = (long *) malloc(pGraph->n * sizeof(long));
    assert(visited && nodeIds);

    for (long v = 0; v < pGraph->n; v++) {
        pGraph->pNodes[v].inDegree = GetInDegree(pGraph, v);
        visited[v] = 0;
    }

    topoImageCnt++;
    GenOneImage(pGraph, "TopologicalSort", "images/TopologicalSort", topoImageCnt, visited);   

    struct Queue *pQueue = CreateQueue();

    // number of nodes waiting to be enqueued
    long waitingCount = 0;    
    for (long v = 0; v < pGraph->n; v++) {
        if (pGraph->pNodes[v].inDegree == 0) {
            // Start from the nodes that don't have any predecessor                       
            nodeIds[waitingCount] = v;
            waitingCount++;          
        }
    } 
    // enqueue node ids randomly
    EnqueueNodeIds(nodeIds, waitingCount, pQueue, 1);

    // the number of nodes in the topological order list
    long sortedNodesCount = 0;
    while (!QueueIsEmpty(pQueue)) {
        long u = QueueDequeue(pQueue);
        printf("%s\n", pGraph->pNodes[u].name);
        
        waitingCount = 0;
        for (long v = 0; v < pGraph->n; v++) {
            if (MatrixElement(pGraph, u, v)) {
                pGraph->pNodes[v].inDegree--;
                // When inDegrees[v] is 0, it means all its predecessors have been visited
                if (pGraph->pNodes[v].inDegree == 0) {                                      
                    nodeIds[waitingCount] = v;
                    waitingCount++;                 
                }
            }
        }
        // enqueue node ids randomly
        EnqueueNodeIds(nodeIds, waitingCount, pQueue, 1);
        sortedNodesCount++;
        // Generate the image
        visited[u] = 1;
        topoImageCnt++;
        GenOneImage(pGraph, "TopologicalSort", "images/TopologicalSort", topoImageCnt, visited);        
    }
    
    if (sortedNodesCount != pGraph->n) {
        printf("\n\nThe graph is cyclic.\n\n");
    } 

    free(nodeIds);
    free(visited);
    ReleaseQueue(pQueue);
}

