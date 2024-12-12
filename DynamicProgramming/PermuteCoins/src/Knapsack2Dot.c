#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Knapsack2Dot.h"
#include "Knapsack.h"

////////////////////////////// Knapsack2Dot (for visualizing the algorithm) ///////////////////////////////////////
static int IsInChoiceDag(struct KnapsackInfo *pKnapsack, long row, long col) {
    return DpTableElementVisited(pKnapsack, row, col);    
}

static void PrintHtmlTable(FILE *dotFile, struct KnapsackInfo *pKnapsack) {
    fprintf(dotFile, "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\" cellpadding=\"0\"> \n");

    fprintf(dotFile, "<tr> \n");    
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">weights</td> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">values</td> \n");
    fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">items/capacity</td> \n");

    for (long j = 0; j <= pKnapsack->capacity; j++) {
         fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\">%ld</td> \n", j);
    }
    fprintf(dotFile, "</tr> \n");

    for (long i = 0; i <= pKnapsack->numOfItems; i++) {
        fprintf(dotFile, "<tr> \n");
        if (i != 0) {            
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", ItemWeight(pKnapsack, i));
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", ItemValue(pKnapsack, i));
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\">%ld</td> \n", i);
        } else {            
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\"> </td> \n");
            fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"gray\"> 0 </td> \n");
        }
        for (long j = 0; j <= pKnapsack->capacity; j++) {
            if (IsInChoiceDag(pKnapsack, i, j)) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%ld</td> \n", DpTableElement(pKnapsack, i, j));
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", DpTableElement(pKnapsack, i, j));
            }            
        }
        fprintf(dotFile, "</tr> \n");
    }

    fprintf(dotFile, "</table> \n");
}


static void PrintChoiceDagInDot(FILE *dotFile, struct KnapsackInfo *pKnapsack, long row, long col) {
    if (DpTableElementVisited(pKnapsack, row, col)) {
        return;
    }
    DpTableElementVisited(pKnapsack, row, col) = 1;
    if (col == 0) {
        // leaf node 
        return;
    } else {
        /*
            Given coins = {2, 3, 5} and the target sum m,
            the number of permutations can be defined as

                f(m) = f(m-2) + f(m-3) + f(m-5)
            
            Edges added:
                     2
                f(m) --> f(m-2)

                     3
                f(m) --> f(m-3)

                     5 
                f(m) --> f(m-5)
         */
        for (long item = 1; item <= row; item++) {
            long k = col - ItemWeight(pKnapsack, item);
            if (k >= 0 && DpTableElement(pKnapsack, row, k) > 0) {
                fprintf(dotFile, "\"(%ld,%ld)\" -> {\"(%ld,%ld)\"} [label=\"%ld\"]\n",
                        row, col, row, k,
                        ItemWeight(pKnapsack, item));
                PrintChoiceDagInDot(dotFile, pKnapsack, row, k);        
            }
        }
    }
}

/*
    Dot Files
 */
void Knapsack2Dot(char *filePath, char *graphName, struct KnapsackInfo *pKnapsack, long row, long col) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);

        // dag
        fprintf(dotFile, "rankdir=\"LR\"\n");
        for (long i = 0; i <= pKnapsack->numOfItems; i++) {
            for (long j = 0; j <= pKnapsack->capacity; j++) {
                DpTableElementVisited(pKnapsack, i, j) = 0;
            }
        }         
        PrintChoiceDagInDot(dotFile, pKnapsack, row, col);

        // table
        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");        
        PrintHtmlTable(dotFile, pKnapsack);
        fprintf(dotFile, "  >]; \n");

        fprintf(dotFile, "} \n");

        fclose(dotFile);
    }
}

#define FILE_NAME_LEN  255

void KnapsackGenOneImage(char *graphName, char *fileName, long seqNo, struct KnapsackInfo *pKnapsack, long n, long capacity) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Knapsack2Dot(dotFileName, graphName, pKnapsack, n, capacity);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);

    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}


