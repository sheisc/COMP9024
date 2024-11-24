#include <stdio.h>    // printf(), scanf()
#include <stdlib.h>   // malloc()
#include <string.h>   // memset()
#include <assert.h>   // assert()
#include <limits.h>   // LONG_MAX, INT_MAX
#include "Graph.h"
#include "DisjointSets.h"


// Structure to represent an edge
struct GraphEdge{
    long u;
    long v;
    AdjMatrixElementTy weight;
};

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

void GenOneImage(struct Graph *pGraph, 
                 char *graphName, 
                 char *fileName, 
                 long seqNo, 
                 int *visited, 
                 struct GraphEdge *pMstEdges,
                 long mstCount) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Graph2Dot(pGraph, dotFileName, graphName, pGraph->isDirected, 
              1, visited, pMstEdges, mstCount, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

// FIXME:
static int isEdgeInMst(long u, long v, struct GraphEdge *pMstEdges, long mstCount) {
    for (long i = 0; i < mstCount; i++) {
        if (u == pMstEdges[i].u && v == pMstEdges[i].v) {
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
               struct GraphEdge *pMstEdges,
               long mstCount,
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
                    fprintf(dotFile, "\"Node %s\" %s {\"Node %s\"}", 
                            pGraph->pNodes[u].name, 
                            edgeConnectorStr, 
                            pGraph->pNodes[v].name);
                    if (displayLabel) {
                        fprintf(dotFile, " [label=\"%ld\"]", val);
                    }
                    // FIXME:
                    if (isEdgeInMst(u, v, pMstEdges, mstCount)) {
                        fprintf(dotFile, "[color=red]");
                    }
                    fprintf(dotFile, "\n"); 
                }
            }
        }

        /*
        "0" [color=red]
         */
        for (long i = 0; i < pGraph->n; i++) {
            if (displayVisited && visited && visited[i]) {                
                fprintf(dotFile, "\"Node %s\" [color=red]\n", pGraph->pNodes[i].name);
            } else {
                fprintf(dotFile, "\"Node %s\"\n", pGraph->pNodes[i].name);
            }
        }        
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

static int CompareTwoEdges(const void *edgeA, const void *edgeB) {
    return ((struct GraphEdge *) edgeA)->weight - ((struct GraphEdge *) edgeB)->weight;
}


static long imgCount = 0;

void KruskalMST(struct Graph *pGraph) {
    assert(!pGraph->isDirected);
    long edgeCount = 0;
    // calculate the number of edges
    for (long u = 0; u < pGraph->n; u++) {
        for (long v = u; v < pGraph->n; v++) {
            // FIXME:
            if (MatrixElement(pGraph, u, v) != 0) {
                edgeCount++;
            }
        }
    }
    if (edgeCount == 0) {
        return;
    }

    struct GraphEdge *pAllEdges = (struct GraphEdge *) malloc(sizeof(struct GraphEdge) * edgeCount);
    struct GraphEdge *pMstEdges = (struct GraphEdge *) malloc(sizeof(struct GraphEdge) * pGraph->n);
    assert(pAllEdges && pMstEdges);

    struct DisjointSets *djs = CreateDisjointSets(pGraph->n);

    long i = 0;
    long mstCount = 0;
    for (long u = 0; u < pGraph->n; u++) {
        for (long v = u; v < pGraph->n; v++) {
            // FIXME:
            if (MatrixElement(pGraph, u, v) != 0) {
                pAllEdges[i].u = u;
                pAllEdges[i].v = v;
                pAllEdges[i].weight = MatrixElement(pGraph, u, v);
                i++;
            }
        }
    }

    qsort(pAllEdges, edgeCount, sizeof(pAllEdges[0]), CompareTwoEdges);
    GenOneImage(pGraph, "KruskalMST", "images/KruskalMST", 
                        imgCount, NULL, pMstEdges, mstCount);
    imgCount++;
    for (i = 0; i < edgeCount; i++) {
        long uRep = DisjointSetsFind(djs, pAllEdges[i].u);
        long vRep = DisjointSetsFind(djs, pAllEdges[i].v);
        if (uRep != vRep) {
            pMstEdges[mstCount] = pAllEdges[i];
            mstCount++;
            DisjointSetsUnion(djs, uRep, vRep);

            printf("============================================== Step %ld ==============================================\n\n", mstCount);
            printf("\tAdd: %s -- %s\n\n", pGraph->pNodes[pAllEdges[i].u].name, pGraph->pNodes[pAllEdges[i].v].name);
            //           
            GenOneImage(pGraph, "KruskalMST", "images/KruskalMST", 
                        imgCount, NULL, pMstEdges, mstCount);
            imgCount++;
        }
    }

    ReleaseDisjointSets(djs);
    free(pAllEdges);
    free(pMstEdges);
}


