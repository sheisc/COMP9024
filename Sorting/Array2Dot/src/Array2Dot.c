#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Array2Dot.h"
////////////////////////////// Array2Dot (for visualizing the algorithm) ///////////////////////////////////////

static void PrintOneEdge(FILE *dotFile, ArrayElementTy *arr, long src, long dst) {
    fprintf(dotFile, "\"%p\" -> \"%p\" [style=invis] \n", arr + src, arr + dst);
}

static void PrintOneNode(FILE *dotFile, ArrayElementTy *arr, long i) {
    fprintf(dotFile, "\"%p\" [label=\"%ld\"] \n", arr + i, (long) arr[i]);
}


/*
    Dot Files
 */
void Array2Dot(ArrayElementTy *arr,
               long n,
               long index,
               char *filePath,
               char *graphName,
               int displayVisited) {
    (void) displayVisited;

    FILE *dotFile = fopen(filePath, "w");
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);
        fprintf(dotFile, "rankdir=\"LR\";\n");

        long i = 0;
        // hidden node with an illegal address in C as its node id: arr[-1]
        fprintf(dotFile, "\"%p\" [label=\"n=%ld\"] [shape=box] \n", arr - 1, n);
        while (i < n) {
            // Use out-of-bound addresses, but not the value inside.
            PrintOneEdge(dotFile, arr, i-1, i);
            PrintOneNode(dotFile, arr, i);
            if (i == index) {
                // use arr-1 (illegal address) as a special node
                ArrayElementTy *indexNode = arr - 2;
                fprintf(dotFile, "\"%p\" -> \"%p\" [color=red]\n", indexNode, arr + index);
                fprintf(dotFile, "\"%p\" [label=\"i=%ld\"] [shape=hexagon] \n", indexNode, index);
            }
            i++;
        }      
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

#define FILE_NAME_LEN  255

void ArrayGenOneImage(ArrayElementTy *arr, long n, long index, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Array2Dot(arr, n, index, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 
}

