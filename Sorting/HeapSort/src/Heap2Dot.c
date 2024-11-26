#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "HeapSort.h"
#include "Heap2Dot.h"


static void PrintOneNode(FILE *dotFile, ArrayElementTy *ptr, long i, long root) {
    if (i == root) {
        fprintf(dotFile, "\"%p\" [label=\"[%ld]=%ld\"] [color=red] \n", ptr + i, i, (long) ptr[i]);   
    } else {
        fprintf(dotFile, "\"%p\" [label=\"[%ld]=%ld\"] \n", ptr + i, i, (long) ptr[i]);
    }
}

void Heap2Dot(ArrayElementTy *ptr,
              long n, 
              long root,
              char *filePath,
              char *graphName) {

    FILE *dotFile = fopen(filePath, "w");
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);

        //for (long i = 0; i <= n / 2 - 1; i++) {
        for (long i = 0; i < n; i++) {    
            long leftIndex = 2 * i + 1;
            long rightIndex = 2 * i + 2;

            PrintOneNode(dotFile, ptr, i, root);
            
            if (leftIndex < n) {
                fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"L\"]\n", ptr + i, ptr + leftIndex);
            }

            if (rightIndex < n) {
                fprintf(dotFile, "\"%p\" -> {\"%p\"} [label=\"R\"]\n", ptr + i, ptr + rightIndex);
            }
        }
    
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

#define FILE_NAME_LEN  255

void HeapGenOneImage(char *graphName, char *fileName, long seqNo, ArrayElementTy *arr, long n, long root) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Heap2Dot(arr, n, root, dotFileName, graphName);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


