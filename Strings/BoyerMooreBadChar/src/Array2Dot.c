#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Array2Dot.h"
#include "BoyerMoore.h"


////////////////////////////// Array2Dot (for visualizing the algorithm) ///////////////////////////////////////
static void PrintHtmlTable(FILE *dotFile, 
                           long *lastOccurrences, char *pattern, long m, char *text, long n,
                           long indexPat, long startTxt) {
    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");


    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">i=%ld, j=%ld</td> \n", startTxt, indexPat);
    fprintf(dotFile, "</tr> \n");

     // text
    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">index i</td> \n");
    for (long i = 0; i < n; i++) {
        fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", i);
    }
    fprintf(dotFile, "</tr> \n");

    fprintf(dotFile, "<tr> \n");
    //fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">text[%ld]</td> \n", startTxt);
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">text[i+j]</td> \n");
    for (long i = 0; i < n; i++) {
        if (indexPat < 0) {
            if (i < startTxt || i >= startTxt + m) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%c</td> \n", text[i]);
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"yellow\">%c</td> \n", text[i]);
            }
        } else {
            if (i < startTxt + indexPat || i >= startTxt + m) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%c</td> \n", text[i]);
            } else if (i == startTxt + indexPat) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%c</td> \n", text[i]);            
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"yellow\">%c</td> \n", text[i]); 
            }
        }
    }
    fprintf(dotFile, "</tr> \n");   

    // pattern
    fprintf(dotFile, "<tr> \n");   
    //fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">pattern[%ld]</td> \n", indexPat);
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">pattern[j]</td> \n");
    for (long i = 0; i < n;) {
        if (i < startTxt) {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
            i++;
        } else {
            for (long j = 0; j < m; j++) {
                if (j < indexPat) {
                    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%c</td> \n", pattern[j]);
                } else if (j == indexPat) {
                    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%c</td> \n", pattern[j]);
                } else {
                    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"yellow\">%c</td> \n", pattern[j]);
                }
            }
            i += m;
            break;
        }  
    }
    fprintf(dotFile, "</tr> \n");

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">index j</td> \n");
    for (long i = 0; i < n; i++) {
        if (i < startTxt || i >= startTxt + m) {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");            
        } else {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", i-startTxt);
        }
    }
    fprintf(dotFile, "</tr> \n");    

    // lastOccurrences
    for (long i = 0; i < NUM_OF_CHARS; i++) {
        if (lastOccurrences[i] != -1) {            
            fprintf(dotFile, "<tr> \n");
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\">lastOccurrences[%ld]</td> \n", i); 
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\"> %c </td> \n", (char) i);
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\"> %ld </td> \n", lastOccurrences[i]);
            fprintf(dotFile, "</tr> \n");  
        }
    }
    fprintf(dotFile, "</table> \n");
}

/*
    Dot Files
 */
void Array2Dot(char *filePath, char *graphName,
               long *lastOccurrences, char *pattern, long m, char *text, long n,
               long indexPat, long startTxt) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);
        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");

        PrintHtmlTable(dotFile, 
                       lastOccurrences, pattern, m, text, n,
                       indexPat, startTxt);

        fprintf(dotFile, "  >]; \n");
        fprintf(dotFile, "} \n");

        fclose(dotFile);
    }
}


#define FILE_NAME_LEN  255

void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, 
                      long *lastOccurrences, char *pattern, long m, char *text, long n, 
                      long indexPat, long startTxt) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Array2Dot(dotFileName, graphName, 
              lastOccurrences, pattern, m, text, n,
              indexPat, startTxt);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


