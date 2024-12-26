#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // system()
#include "RBTree2Dot.h"
#include "RedBlackTree.h"
#include "BTree.h"
#include "Queue.h"

#define FILE_NAME_LEN  255

static long hiddenNodeId;

void RBTreeGenOneImage(RBTreeNodePtr root, char *graphName, char *fileName, long seqNo, char *comment, long nodeKey) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    hiddenNodeId = 0;
    RedBlackTree2Dot(root, dotFileName, graphName, 1, comment, nodeKey);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

static void DisplayVisited(FILE *dotFile, RBTreeNodePtr root) {
    if (root) {
        char *visitedShape = "";
        if (root->visited) {
            visitedShape = "[shape=box]";
        }

        if (root->color == RED) { 
            fprintf(dotFile, 
                    "\"%p\" [label=\"%ld\"] [color=orangered] [style=filled] %s\n",
                    root, 
                    (long) root->value.numVal, 
                    visitedShape);
        } else {
            fprintf(dotFile, 
                    "\"%p\" [label=\"%ld\"] [color=grey] [style=filled] %s\n", 
                    root,
                    (long) root->value.numVal, 
                    visitedShape);                
        }

        DisplayVisited(dotFile, root->left);
        DisplayVisited(dotFile, root->right);
    }   
}

/*
    Convert a reg-black tree to a dot file
 */
void RedBlackTree2Dot(RBTreeNodePtr root, 
                      char *filePath,
                      char *graphName,
                      int displayVisited,
                      char *comment,
                      long nodeKey) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);
        
        struct Queue *pQueue = CreateQueue();
   
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                RBTreeNodePtr curNode = (RBTreeNodePtr) QueueDequeue(pQueue);
                if (!curNode->left && !curNode->right) {
                    continue;
                }
                // edges
                if (curNode->left) {
                    fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"]\n",
                            curNode,                      
                            curNode->left);
                    QueueEnqueue(pQueue, curNode->left);
                } else {
                    hiddenNodeId++;
                    fprintf(dotFile, "\"%p\" -> {\"%p\"}  [label=\"L\"] [style=invis]\n",
                            curNode,
                            (void *) hiddenNodeId);
                    fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);

                }

                if (curNode->right) {
                    fprintf(dotFile, "\"%p\" -> {\"%p\"}  [label=\"R\"]\n",                        
                            curNode,
                            curNode->right);
                    QueueEnqueue(pQueue, curNode->right);
                } else {
                    hiddenNodeId++;
                    fprintf(dotFile, "\"%p\" -> {\"%p\"}  [label=\"R\"] [style=invis]\n",
                            curNode,
                            (void *) hiddenNodeId);
                    fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);                  
                }            
                
            }
        }
        ReleaseQueue(pQueue);
        // nodes
        DisplayVisited(dotFile, root);
        fprintf(dotFile, "\nlabel=\"\n\n%s: node %ld\"\n", comment, nodeKey);
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


