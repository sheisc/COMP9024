#include <assert.h>   // assert()
#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // malloc() and free()
#include <string.h>   // memset()
#include "BiTree.h"
#include "Huffman.h"
#include "Queue.h"


#define FILE_NAME_LEN  255

void GenOneImage(BiTreeNodePtr root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    BiTree2Dot(root, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}


void BiTree2Dot(BiTreeNodePtr root, 
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
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                BiTreeNodePtr curNode = QueueDequeue(pQueue);
                // node
                if (curNode->visited && displayVisited) {
                    fprintf(dotFile, "\"%p\" [label=\"%s\"] [color=red] \n", curNode, curNode->value.name);
                } else {
                    fprintf(dotFile, "\"%p\" [label=\"%s\"] \n", curNode, curNode->value.name);
                }
                
                // two edges
                if (curNode->leftChild) {
                    fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"0\"]\n",
                            curNode,
                            edgeConnectorStr,                         
                            curNode->leftChild);
                    QueueEnqueue(pQueue, curNode->leftChild);
                }
                if (curNode->rightChild) {
                    fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"1\"]\n",                        
                            curNode, 
                            edgeConnectorStr,
                            curNode->rightChild);
                    QueueEnqueue(pQueue, curNode->rightChild);
                }            
                
            }
        }
        ReleaseQueue(pQueue);     
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

