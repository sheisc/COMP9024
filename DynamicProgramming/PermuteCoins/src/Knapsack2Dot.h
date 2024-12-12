#ifndef KNAPSACK2DOT_H
#define KNAPSACK2DOT_H

#include "Knapsack.h"

void Knapsack2Dot(char *filePath, char *graphName, struct KnapsackInfo *pKnapsack, long row, long col);
               
void KnapsackGenOneImage(char *graphName, char *fileName, long seqNo, struct KnapsackInfo *pKnapsack, long row, long col);

#endif

