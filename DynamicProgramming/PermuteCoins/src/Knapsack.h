#ifndef KNAPSACK_H
#define KNAPSACK_H

#define  DpTableElement(pKnapsack, i, j)  (pKnapsack)->dpTable[(i) * ((pKnapsack)->capacity + 1) + (j)]

#define  DpTableElementVisited(pKnapsack, i, j)  (pKnapsack)->visited[(i) * ((pKnapsack)->capacity + 1) + (j)]

// r is the row number in the dpTable, from 0 to n
#define  ItemValue(pKnapsack, r)    (pKnapsack)->values[(r) - 1]

// r is the row number in the dpTable, from 0 to n
#define  ItemWeight(pKnapsack, r)   (pKnapsack)->weights[(r) - 1]

#define  KNAPSACK_INVALID_VALUE     -1

struct KnapsackInfo {
    long *values;
    long *weights;
    long numOfItems;
    long capacity;
    long *dpTable;
    // for visualizing the algorithm
    int *visited;        
};

struct KnapsackInfo *CreateKnapsackInfo(long *values, long *weights, long numbOfItems, long capacity);

void ReleaseKnapsackInfo(struct KnapsackInfo *pKnapsack);

void PrintKnapsack(struct KnapsackInfo *pKnapsack, long row, long col);

long SolveKnapsackNoMem(struct KnapsackInfo *pKnapsack, long n, long cap);

long SolveKnapsackMem(struct KnapsackInfo *pKnapsack, long n, long cap);

long SolveKnapsackTabulation(struct KnapsackInfo *pKnapsack, long n, long cap);


#endif

