#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Array2Dot.h"

#define	ROWS 1


////////////////////////////// Array2Dot (for visualizing the algorithm) ///////////////////////////////////////
/*
    For the n elements arr[0],  ...,  arr[n-1]:

    Highlight the elements arr[left], arr[left+1], ..., arr[right]

    Highlight a special element arr[index]
 */
static void PrintHtmlTable(FILE *dotFile, ArrayElementTy *arr, long n, long left, long right, long index) {
    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");
    for (long r = 0; r < ROWS; r++) {
        fprintf(dotFile, "<tr> \n");
        for (long c = 0; c < n; c++) {
            if (c == index) {
                fprintf(dotFile, "<td width=\"80\" height=\"80\" bgcolor=\"green\">[%ld]=%ld</td> \n", c, (long) arr[c]);
            } else if (c >= left && c <= right) {
                fprintf(dotFile, "<td width=\"80\" height=\"80\" bgcolor=\"yellow\">[%ld]=%ld</td> \n", c, (long) arr[c]);
            } else {
                fprintf(dotFile, "<td width=\"80\" height=\"80\" bgcolor=\"white\">[%ld]=%ld</td> \n", c, (long) arr[c]);
            }
        }
        fprintf(dotFile, "</tr> \n");
    }
    fprintf(dotFile, "</table> \n");
}

/*
    Dot Files
 */
void Array2Dot(char *filePath,
               char *graphName,
               ArrayElementTy *arr, long n, long left, long right, long index) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);
        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");

        PrintHtmlTable(dotFile, arr, n, left, right, index);

        fprintf(dotFile, "  >]; \n");
        fprintf(dotFile, "} \n");

        fclose(dotFile);
    }
}


#define FILE_NAME_LEN  255

void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, ArrayElementTy *arr, long n, long left, long right, long index) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Array2Dot(dotFileName, graphName, arr, n, left, right, index);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


