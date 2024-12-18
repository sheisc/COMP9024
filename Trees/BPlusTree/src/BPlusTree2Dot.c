#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // system()
#include <string.h>
#include "BPlusTree.h"
#include "BPlusTree2Dot.h"


#define FILE_NAME_LEN  255

#define BPlusTree_NODE_NAME_LEN     2048

void BPlusTreeGenOneImage(struct BPlusTree *pBPlusTree, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    BPlusTree2Dot(pBPlusTree, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}


static char *GetBPlusTreeNodeName(struct BPlusTreeNode *pNode, char *nodeName, long n) {
    char *buf = nodeName;
    memset(nodeName, 0, n);
    long availableLen = n - 1;
    /*
        These functions return the number of characters printed  
        (excluding  the  null  byte used to end output to strings).

        The functions snprintf() and vsnprintf() write at most size  bytes  
        (including  the  terminating  null  byte).   
     */
    long lastIndex = (pNode->isLeaf) ? (pNode->nk) : (pNode->nk - 1);
    for (long i = 0; i <= lastIndex; i++) {
        long len;
        if (pNode->keys[i] == UNUSED_SLOT_KEY_VALUE) {
            continue;
        }
        if (i != lastIndex) {
            len = snprintf(buf, availableLen, "%ld_", (long) pNode->keys[i]);
        } else {
            len = snprintf(buf, availableLen, "%ld", (long) pNode->keys[i]);
        }
        buf += len;
        availableLen -= len;
        if (availableLen <= 0) {
            break;
        }
    }
    return nodeName;
}

static void PrintLeafNode(FILE *dotFile, struct BPlusTreeNode *pNode) {
    fprintf(dotFile, "\"%p\" [shape=box] [label=< ", pNode);

    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\"> key </td> \n");
    for (long i = 0; i <= pNode->nk; i++) {
        if (i == pNode->nk) {
            if (pNode->next) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\"> %ld </td> \n", (long) pNode->keys[i]);
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"orange\"> %ld </td> \n", (long) pNode->keys[i]);
            }
        } else {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\"> %ld </td> \n", (long) pNode->keys[i]);
        }
    }
    fprintf(dotFile, "</tr> \n");

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\"> value </td> \n");
    for (long i = 0; i <= pNode->nk; i++) {
        if (i == pNode->nk) {
            if (pNode->next) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\"> %ld </td> \n", (long) pNode->pChildren[i]);
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"orange\"> %ld </td> \n", (long) pNode->pChildren[i]);
            }
        } else {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\"> %ld </td> \n", (long) pNode->pChildren[i]);
        }
    }
    fprintf(dotFile, "</tr> \n");    
    fprintf(dotFile, "</table> \n");  
    fprintf(dotFile, ">]\n");
}

static void PrintBPlusTreeNodeInDot(FILE *dotFile, struct BPlusTreeNode *pNode, int displayVisited) {
    (void) displayVisited;
    if (pNode) {

        // edges
        for (long i = 0; i <= pNode->nk; i++) {
            if (!pNode->isLeaf && pNode->pChildren[i]) {               
                fprintf(dotFile, "\"%p\" -> {\"%p\"} \n", pNode, pNode->pChildren[i]);
                PrintBPlusTreeNodeInDot(dotFile, pNode->pChildren[i], displayVisited);
            }
        }
#if 0        
        // linked list of leaf nodes
        if (pNode->isLeaf && pNode->next) {
            fprintf(dotFile, "\"%p\" -> {\"%p\"} \n", pNode, pNode->next);
        }
#endif        
        // node
        if (pNode->isLeaf) {
            PrintLeafNode(dotFile, pNode);
        } else {
            char nodeName[BPlusTree_NODE_NAME_LEN];
            GetBPlusTreeNodeName(pNode, nodeName, BPlusTree_NODE_NAME_LEN);            
            fprintf(dotFile, "\"%p\" [label=\"%s\"]\n", pNode, nodeName);
        }                
    }
}

/*
   Convert a BPlusTree to a dot file
 */
void BPlusTree2Dot(struct BPlusTree *pBPlusTree, char *filePath, char *graphName, int displayVisited) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        
        fprintf(dotFile, "digraph %s {\n", graphName);

        PrintBPlusTreeNodeInDot(dotFile, pBPlusTree->root, displayVisited);
     
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

