#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // system()
#include "RedBlackTree.h"
#include "BTree.h"
#include "BTree2Dot.h"
#include "Queue.h"

#define FILE_NAME_LEN  255

void BTreeGenOneImage(struct BTreeNode *root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    BTree2Dot(root, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}



/*
    Convert a B-tree to a dot file
 */
void BTree2Dot(struct BTreeNode *root, 
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
                struct BTreeNode *curNode = (struct BTreeNode *) QueueDequeue(pQueue);

                long nc = GetNumberOfChildren(curNode);
                char nodeName[BTREE_NODE_NAME_LEN];
                char childName[BTREE_NODE_NAME_LEN];
                GetBTreeNodeName(curNode, nodeName, BTREE_NODE_NAME_LEN);
                for (long i = 0; i < nc; i++) {
                    if (curNode->children[i]) {
                        GetBTreeNodeName(curNode->children[i], childName, BTREE_NODE_NAME_LEN);                    
                        fprintf(dotFile, "\"%s\" %s {\"%s\"} \n",
                                nodeName,
                                edgeConnectorStr,                         
                                childName
                                );
                        QueueEnqueue(pQueue, curNode->children[i]);
                    }
                }
                char *visitedShape = "";
                if (root->visited) {
                    visitedShape = "[shape=box]";
                }
                fprintf(dotFile, 
                        "\"%s\" %s\n", 
                        nodeName, 
                        visitedShape);                                  
            }
        }
        ReleaseQueue(pQueue);
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

