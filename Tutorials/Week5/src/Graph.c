#include <stdio.h>    // printf(), scanf()
#include <stdlib.h>   // malloc()
#include <string.h>   // memset()
#include <assert.h>   // assert()
#include "Stack.h"
#include "Graph.h"


// Max length of an identifier (e.g., a graph node's name) 
#define MAX_ID_LEN 127


// Storing information of a graph node
struct GraphNode {
    char name[MAX_ID_LEN + 1]; 
} GraphNode;

struct Graph{
    /*
       Memory Layout:
                          -----------------------------------------------------------
        pAdjMatrix ---->  Element(0, 0),   Element(0, 1),    ...,       Element(0, n-1),     // each row has n elements
                          Element(1, 0),   Element(1, 1),    ...,       Element(1, n-1),
      
                          .....                            Element(u, v)     ...             // (n * u + v) elements away from Element(0, 0)
      
                          Element(n-1, 0), Element(n-1, 1),  ...,       Element(n-1, n-1)
                          ----------------------------------------------------------- 
                                      Adjacent Matrix on Heap

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

#if 0
/*
    Please complete the code in Q1-Q5:

    Q1:  create a data stack
    Q2:  push u onto the data stack
    Q3:  test whether the data stack is empty
    Q4:  push v onto the data stack
    Q5:  free the heap space occupied by the data stack

  */
void NonRecursiveDFS(struct Graph *pGraph, long u) {
    assert(IsLegalNodeNum(pGraph, u));
    static long cnt = 0;

    int *visited = (int *) malloc(sizeof(int) * pGraph->n);
    struct Stack *pStack = ______Q1______;
    assert(visited && pStack);

    for (long i = 0; i < pGraph->n; i++) {
        visited[i] = 0;
    }
    GenOneImage(pGraph, "dfs", "images/NonRecursiveDFS", cnt, visited);
    printf("\n\t\t\t\tpush %ld\n", u);
    ______Q2______;

    while (______Q3______) {
        printf("\n");
        PrintStack(pStack);
        STACK_ITEM_T curNodeId = StackPop(pStack);
        printf("\t\t\t\tpop %ld\n", curNodeId);
        if (!visited[curNodeId]) {
            visited[curNodeId] = 1;
            printf("\t\t\t\t\t\tvisiting %s\n", pGraph->pNodes[curNodeId].name);
            
            cnt++;
            GenOneImage(pGraph, "dfs", "images/NonRecursiveDFS", cnt, visited);             

            //for (long v = 0; v < pGraph->n; v++) {
            for (long v = pGraph->n - 1; v >= 0; v--) {
                if (MatrixElement(pGraph, curNodeId, v) == CONNECTED && !visited[v]) {
                    ______Q4______;
                    printf("\t\t\t\tpush %ld\n", v);
                }
            }            
        }
    }
    printf("\n");
    ______Q5______;
    free(visited);    
}
#endif


static void DepthFirstSearch(struct Graph *pGraph, long u, int *visited) {
    visited[u] = 1;
    printf("visiting %s\n", pGraph->pNodes[u].name);
    
    static long i = 0;
    i++; 
    GenOneImage(pGraph, "dfs", "images/RecursiveDFS", i, visited);      

    // recursively visit the adjacent nodes of u, if they have not been visited yet
    for(long v = 0; v < pGraph->n; v++) {
        if (MatrixElement(pGraph, u, v) == CONNECTED && !visited[v]) {
            DepthFirstSearch(pGraph, v, visited);
        }
    }
}

void RecursiveDFS(struct Graph *pGraph, long u) {
    int *visited = (int *) malloc(pGraph->n * sizeof(int));
    //memset(visited, 0, sizeof(int) * pGraph->n);
    for (long v = 0; v < pGraph->n; v++) {
        visited[v] = 0;
    }
    GenOneImage(pGraph, "dfs", "images/RecursiveDFS", 0, visited);
    DepthFirstSearch(pGraph, u, visited);
    printf("\n");
    free(visited);
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

Week5$ dot -T png images/RecursiveDFS_0000.dot -o images/RecursiveDFS_0000.png   

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
                    fprintf(dotFile, "\"%s\" %s {\"%s\"}", 
                            pGraph->pNodes[u].name, 
                            edgeConnectorStr, 
                            pGraph->pNodes[v].name);
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
                fprintf(dotFile, "\"%s\" [color=red]\n", pGraph->pNodes[i].name);
            } else {
                fprintf(dotFile, "\"%s\"\n", pGraph->pNodes[i].name);
            }
        }                
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}
