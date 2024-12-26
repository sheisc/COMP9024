#include <stdio.h>
#include <stdlib.h>
#include "SplayTree.h"

/////////////////////////////// SplayTree2Dot ///////////////////////////////////////////


#define FILE_NAME_LEN  4096

static long hiddenNodeId;

static void PrintSplayTreeInDot(FILE *dotFile, struct SplayTreeNode *root) {
    
    if (root) {
        // node
        fprintf(dotFile, 
                "\"%p\"  [label=\"%ld\"] \n", 
                root, 
                root->k);

        if (!root->left && !root->right) {
            return;
        }        
        // left edge
        if (root->left) {
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"]\n", root, root->left);
        } else {
            hiddenNodeId++;
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"] [style=invis]\n", root, (void *) hiddenNodeId);            
            fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);            
        }
        // right edge
        if (root->right) {
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"R\"]\n", root, root->right);
        } else {
            hiddenNodeId++;
            fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"R\"] [style=invis]\n", root, (void *) hiddenNodeId);
            fprintf(dotFile, "\"%p\" [style=invis]\n", (void *) hiddenNodeId);            
        }

        PrintSplayTreeInDot(dotFile, root->left);
        PrintSplayTreeInDot(dotFile, root->right);
    }   
}

/*
    Convert a reg-black tree to a dot file
 */
void SplayTree2Dot(struct SplayTreeNode *root, char *filePath, char *graphName, int displayVisited, char *comment) {
    hiddenNodeId = 0;
    FILE *dotFile = fopen(filePath, "w");
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);        
        PrintSplayTreeInDot(dotFile, root); 
        fprintf(dotFile, "\nlabel=\"\n\n%s\"\n", comment);
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


void SplayTreeGenOneImage(struct SplayTreeNode *root, char *graphName, char *fileName, long seqNo, char *comment) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    SplayTree2Dot(root, dotFileName, graphName, 1, comment);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}
