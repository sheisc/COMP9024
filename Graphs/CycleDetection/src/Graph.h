#ifndef GRAPH_H
#define GRAPH_H

typedef long AdjMatrixElementTy;

#define CONNECTED   1

struct Graph;
struct Graph *CreateGraph(long n, int isDirected);

void ReleaseGraph(struct Graph *pGraph);

void GraphAddUndirectedEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val);

void GraphAddDirectedEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val);

void GraphAddEdge(struct Graph *pGraph, long u, long v, AdjMatrixElementTy val);

void GraphAddNode(struct Graph *pGraph, long u, char *name);

void PrintGraph(struct Graph *pGraph);


void RecursiveDFS(struct Graph *pGraph, long u);


void Graph2Dot(struct Graph *pGraph, 
               char *filePath,
               char *graphName,
               int isDirectedGraph,
               int displayLabel,
               int *visited,
               int displayVisited);

// Based on Graph2Dot()
void GenOneImage(struct Graph *pGraph, char *graphName, char *fileName, long seqNo, int *visited);

int HasCycle(struct Graph *pGraph);
#endif


