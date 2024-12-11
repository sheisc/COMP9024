#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Knapsack2Dot.h"
#include "Knapsack.h"
#include "Queue.h"


////////////////////////////// Knapsack2Dot (for visualizing the algorithm) ///////////////////////////////////////
static int IsInChoiceDag(struct ChoiceDagNode *root, long i, long j) {
    if (root) {
        if (root->i == i && root->j == j) {
            return 1;
        } else {
            int found1 = IsInChoiceDag(root->included, i, j);
            int found2 = IsInChoiceDag(root->excluded, i, j);
            return found1 || found2;
        }
    } else {
        return 0;
    }
}


static void PrintHtmlTable(FILE *dotFile, struct KnapsackInfo *pKnapsack, struct ChoiceDagNode *root) {
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
            if (IsInChoiceDag(root, i, j)) {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"green\">%ld</td> \n", DpTableElement(pKnapsack, i, j));
            } else {
                fprintf(dotFile, "<td width=\"30\" height=\"30\" bgcolor=\"white\">%ld</td> \n", DpTableElement(pKnapsack, i, j));
            }            
        }
        fprintf(dotFile, "</tr> \n");
    }

    fprintf(dotFile, "</table> \n");
}

static void PrintChoiceDag(FILE *dotFile, struct KnapsackInfo *pKnapsack, struct ChoiceDagNode *root) {
    char *edgeConnectorStr = "->";     
    fprintf(dotFile, "rankdir=\"BT\"\n");
    struct Queue *pQueue = CreateQueue();
    QueueEnqueue(pQueue, root);
    while (!QueueIsEmpty(pQueue)) {
        struct ChoiceDagNode *curNode = QueueDequeue(pQueue);
        // node
        fprintf(dotFile, "\"%p\" [label=\"(%ld, %ld)\"] \n", curNode, curNode->i, curNode->j);        
        // two edges
        if (curNode->included) {
            fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"1\"]\n",
                    curNode,
                    edgeConnectorStr,                         
                    curNode->included);
            QueueEnqueue(pQueue, curNode->included);
        }
        if (curNode->excluded) {
            fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"0\"]\n",                        
                    curNode, 
                    edgeConnectorStr,
                    curNode->excluded);
            QueueEnqueue(pQueue, curNode->excluded);
        }        
    }
    ReleaseQueue(pQueue); 
}

/*
    Dot Files
 */
void Knapsack2Dot(char *filePath, char *graphName, struct KnapsackInfo *pKnapsack, long row, long col) {
    FILE *dotFile = fopen(filePath, "w");

    if (dotFile) {
        fprintf(dotFile, "digraph %s { \n", graphName);

        fprintf(dotFile, "Array [shape=none, margin=0, label=< \n");
        struct ChoiceDagNode *root = &ChoiceNodeElement(pKnapsack, row, col);
        PrintHtmlTable(dotFile, pKnapsack, root);
        fprintf(dotFile, "  >]; \n");

        PrintChoiceDag(dotFile, pKnapsack, root);
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


