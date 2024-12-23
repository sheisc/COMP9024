#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // system()
#include "RedBlackTree.h"


#define FILE_NAME_LEN  255

static long hiddenNodeId;

static void PrintRBTreeInDot(FILE *dotFile, struct RBTreeNode *root) {
    
    if (root) {
        if (root->color == RED) { 
            fprintf(dotFile, 
                    "\"%p\" [color=orangered] [style=filled] [label=\"%ld_H=%ld\"] \n", 
                    root, 
                    root->k,
                    RBTreeBlackHeight(root));
        } else {
            fprintf(dotFile, 
                    "\"%p\" [color=grey] [style=filled] [label=\"%ld_H=%ld\"] \n", 
                    root, 
                    root->k,
                    RBTreeBlackHeight(root));              
        }
        if (!root->left && !root->right) {
            return;
        }        
        if (root->left) {
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"]\n", root, root->left);
        } else {
            hiddenNodeId++;
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"] [style=invis]\n", root, (void *) hiddenNodeId);            
            fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);            
        }
        if (root->right) {
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"R\"]\n", root, root->right);
        } else {
            hiddenNodeId++;
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"R\"] [style=invis]\n", root, (void *) hiddenNodeId);
            fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);            
        }

        PrintRBTreeInDot(dotFile, root->left);
        PrintRBTreeInDot(dotFile, root->right);
    }   
}

/*
    Convert a reg-black tree to a dot file
 */
void RedBlackTree2Dot(struct RBTree *pRBTree, char *filePath, char *graphName, int displayVisited) {
    struct RBTreeNode *root = RBTreeGetRoot(pRBTree);
    hiddenNodeId = 0;
    FILE *dotFile = fopen(filePath, "w");
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);        
        PrintRBTreeInDot(dotFile, root); 
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


void RBTreeGenOneImage(struct RBTree *pRBTree, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    RedBlackTree2Dot(pRBTree, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}
