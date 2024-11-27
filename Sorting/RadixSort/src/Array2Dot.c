#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Array2Dot.h"
#include "RadixSort.h"


////////////////////////////// Array2Dot (for visualizing the algorithm) ///////////////////////////////////////

static void PrintHtmlTable(FILE *dotFile, struct RadixInfo *pRadixInfo) {
    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");
    for (long r = 0; r < DEFAULT_RADIX; r++) {      
        fprintf(dotFile, "<tr> \n");
        // radix
        fprintf(dotFile, "<td width=\"50\" height=\"50\" bgcolor=\"green\">r=%ld</td> \n", r);
        // put elements with the same radix in the same row
        for (long c = 0; c < pRadixInfo->arr[r].count; c++) {
            fprintf(dotFile, "<td width=\"50\" height=\"50\" bgcolor=\"white\">%ld</td> \n",
                    (long) pRadixInfo->arr[r].elements[c]);
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
               struct RadixInfo *radixArr) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);
        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");

        PrintHtmlTable(dotFile, radixArr);

        fprintf(dotFile, "  >]; \n");
        fprintf(dotFile, "} \n");

        fclose(dotFile);
    }
}


#define FILE_NAME_LEN  255

void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, struct RadixInfo *radixArr) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Array2Dot(dotFileName, graphName, radixArr);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


