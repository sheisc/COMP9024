#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Array2Dot.h"

#define	ROWS 1


////////////////////////////// Array2Dot (for visualizing the algorithm) ///////////////////////////////////////
static void PrintHtmlTable(FILE *dotFile, 
                           long *lppsArr, char *pattern, long m, char *text, long n,
                           long indexPat, long indexTxt) {
    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");

    long startTxt = indexTxt - indexPat;    

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">m=%ld, n=%ld</td> \n", m, n);
    fprintf(dotFile, "</tr> \n");     

    // text
    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">index i</td> \n");
    for (long i = 0; i < n; i++) {
        fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", i);
    }
    fprintf(dotFile, "</tr> \n");     
    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">text[%ld]</td> \n", indexTxt);
    for (long i = 0; i < n; i++) {
        if (i >= startTxt && i < indexTxt) {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"yellow\">%c</td> \n", text[i]);
        } else if (i == indexTxt && indexPat != m) {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%c</td> \n", text[i]);            
        } else {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%c</td> \n", text[i]); 
        }
    }
    fprintf(dotFile, "</tr> \n");   

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">i=%ld, j=%ld</td> \n", indexTxt, indexPat);
    fprintf(dotFile, "</tr> \n");

    // pattern
    fprintf(dotFile, "<tr> \n");   
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">pattern[%ld]</td> \n", indexPat); 

    long len = n;
    if (startTxt + m  > n) {
        len = startTxt + m;
    }

    for (long i = 0; i < len; i++) {
        if (i < startTxt) {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
        } else if (i - startTxt < m) {
            if (i >= startTxt && i < indexTxt) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"yellow\">%c</td> \n", pattern[i-startTxt]);
            } else if (i == indexTxt) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%c</td> \n", pattern[i-startTxt]);            
            } else if (i - startTxt < m) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%c</td> \n", pattern[i-startTxt]); 
            } 
        } else {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
        }    
    }
    fprintf(dotFile, "</tr> \n");

    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">index j</td> \n");
    for (long i = 0; i < len; i++) {
        if (i < startTxt || i >= startTxt + m) {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");            
        } else {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", i-startTxt);
        }
    }
    fprintf(dotFile, "</tr> \n");    

    // lppsArr
    fprintf(dotFile, "<tr> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\"> lppsArr </td> \n");
    for (long i = 0; i < len; i++) {
        if (i < startTxt) {
            fprintf(dotFile, "<td border=\"0\" width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
        } else if (i - startTxt < m) {
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\">%ld</td> \n", lppsArr[i-startTxt]);
        } else {
            fprintf(dotFile, "<td  border=\"0\"  width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
        }        
    }

    fprintf(dotFile, "</tr> \n");    

    fprintf(dotFile, "</table> \n");
}

/*
    Dot Files
 */
void Array2Dot(char *filePath, char *graphName,
               long *lppsArr, char *pattern, long m, char *text, long n,
               long indexPat, long indexTxt) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);
        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");

        PrintHtmlTable(dotFile, 
                       lppsArr, pattern, m, text, n,
                       indexPat, indexTxt);

        fprintf(dotFile, "  >]; \n");
        fprintf(dotFile, "} \n");

        fclose(dotFile);
    }
}


#define FILE_NAME_LEN  255

void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, 
                      long *lppsArr, char *pattern, long m, char *text, long n, 
                      long indexPat, long indexTxt) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Array2Dot(dotFileName, graphName, 
              lppsArr, pattern, m, text, n,
              indexPat, indexTxt);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


