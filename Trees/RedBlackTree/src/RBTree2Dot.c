#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // system()
#include "RedBlackTree.h"
#include "BTree.h"
#include "Queue.h"

#define FILE_NAME_LEN  255

void RBTreeGenOneImage(RBTreeNodePtr root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    RedBlackTree2Dot(root, dotFileName, graphName, 1);

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
                    "\"%s_H=%d_B=%d\" [color=orangered] [style=filled] %s\n", 
                    root->value.name, 
                    root->blackHeight,
                    RBTreeBalanceFactor(root), 
                    visitedShape);
        } else {
            fprintf(dotFile, 
                    "\"%s_H=%d_B=%d\" [color=grey] [style=filled] %s\n", 
                    root->value.name, 
                    root->blackHeight,
                    RBTreeBalanceFactor(root), 
                    visitedShape);                
        }

        DisplayVisited(dotFile, root->leftChild);
        DisplayVisited(dotFile, root->rightChild);
    }   
}

/*
    Convert a reg-black tree to a dot file
 */
void RedBlackTree2Dot(RBTreeNodePtr root, 
                      char *filePath,
                      char *graphName,
                      int displayVisited) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        
        struct Queue *pQueue = CreateQueue();
        long hiddenNodeCnt = 0;
        char *hiddenNodePrefix = "HD";        
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                RBTreeNodePtr curNode = (RBTreeNodePtr) QueueDequeue(pQueue);
                if (!curNode->leftChild && !curNode->rightChild) {
                    continue;
                }
                // edges
                if (curNode->leftChild) {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s_H=%d_B=%d\"} [label=\"L\"]\n",
                            curNode->value.name,
                            curNode->blackHeight,
                            RBTreeBalanceFactor(curNode),
                            edgeConnectorStr,                         
                            curNode->leftChild->value.name,
                            curNode->leftChild->blackHeight,
                            RBTreeBalanceFactor(curNode->leftChild));
                    QueueEnqueue(pQueue, curNode->leftChild);
                } else {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s%ld\"} [label=\"L\"] [style=invis]\n",
                            curNode->value.name,
                            curNode->blackHeight,
                            RBTreeBalanceFactor(curNode),
                            edgeConnectorStr,                         
                            hiddenNodePrefix,
                            hiddenNodeCnt);
                    fprintf(dotFile, "\"%s%ld\" [style=invis]\n", hiddenNodePrefix, hiddenNodeCnt);
                    hiddenNodeCnt++;
                }

                if (curNode->rightChild) {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s_H=%d_B=%d\"} [label=\"R\"]\n",                        
                            curNode->value.name,
                            curNode->blackHeight, 
                            RBTreeBalanceFactor(curNode),
                            edgeConnectorStr,
                            curNode->rightChild->value.name,
                            curNode->rightChild->blackHeight,
                            RBTreeBalanceFactor(curNode->rightChild));
                    QueueEnqueue(pQueue, curNode->rightChild);
                } else {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s%ld\"} [label=\"L\"] [style=invis]\n",
                            curNode->value.name,
                            curNode->blackHeight,
                            RBTreeBalanceFactor(curNode),
                            edgeConnectorStr,                         
                            hiddenNodePrefix,
                            hiddenNodeCnt);
                    fprintf(dotFile, "\"%s%ld\" [style=invis]\n", hiddenNodePrefix, hiddenNodeCnt);
                    hiddenNodeCnt++;                   
                }            
                
            }
        }
        ReleaseQueue(pQueue);
        // nodes
        DisplayVisited(dotFile, root); 
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


