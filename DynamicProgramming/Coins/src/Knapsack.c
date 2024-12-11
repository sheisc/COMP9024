#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Knapsack.h"


// static long imgCount = 0;

/*
    In most cases, it is a binary tree.    
 */
static void InitChoiceDagNode(struct ChoiceDagNode *pNode, long i, long j, 
                                 struct ChoiceDagNode *included,
                                 struct ChoiceDagNode *excluded) {
    pNode->i = i;
    pNode->j = j;
    pNode->included = included;
    pNode->excluded = excluded;
    pNode->visited = 0;
}

/*
    Item numbers are saved in choices[].
    The parameter count represents the current index for appending an item number in choices[].
 */
static void VisitChoiceDag(struct KnapsackInfo *pKnapsack, struct ChoiceDagNode *pNode, long choices[], long count) {
    if (pNode) {
        if (pNode->j == 0) {
            // leaf node 
            printf("Choices: ");
            for (long i = 0; i < count; i++) {
                printf("%ld ", choices[i]);
            }
            printf("\n");
            return;
        } else {        
            choices[count] = ItemWeight(pKnapsack, pNode->i);
            VisitChoiceDag(pKnapsack, pNode->included, choices, count + 1);
            //
            VisitChoiceDag(pKnapsack, pNode->excluded, choices, count);
        }
    }
}

static void PrintAllChoices(struct KnapsackInfo *pKnapsack, long row, long col) {       
    long n = pKnapsack->numOfItems;
    long capacity = pKnapsack->capacity;
    // FIXME:  
    long *choices = (long *) malloc(sizeof(long) * (n + 1) * (capacity + 1));    
    VisitChoiceDag(pKnapsack, &DpDagNode(pKnapsack, row, col), choices, 0);
    free(choices);
}

struct KnapsackInfo *CreateKnapsackInfo(long *values, long *weights, long numbOfItems, long capacity) {
    struct KnapsackInfo *pKnapsack = (struct KnapsackInfo *) malloc(sizeof(struct KnapsackInfo));
    memset(pKnapsack, 0, sizeof(struct KnapsackInfo));
    long *dpTable = (long *) malloc(sizeof(long) * (numbOfItems + 1) * (capacity + 1));
    struct ChoiceDagNode *nodes = (struct ChoiceDagNode *) malloc(
                sizeof(struct ChoiceDagNode) * (numbOfItems + 1) * (capacity + 1));
    
    pKnapsack->values = (long *) malloc(sizeof(long) * numbOfItems);
    memcpy(pKnapsack->values, values, sizeof(long) * numbOfItems);

    pKnapsack->weights = (long *) malloc(sizeof(long) * numbOfItems);
    memcpy(pKnapsack->weights, weights, sizeof(long) * numbOfItems);

    pKnapsack->numOfItems = numbOfItems;
    pKnapsack->capacity = capacity;
    pKnapsack->dpTable = dpTable;
    pKnapsack->nodes = nodes;

    for (long i = 0; i <= numbOfItems; i++) {
        for (long j = 0; j <= capacity; j++) {
            DpTableElement(pKnapsack, i, j) = KNAPSACK_INVALID_VALUE;
            InitChoiceDagNode(& DpDagNode(pKnapsack, i, j), i, j, NULL, NULL);
        }
    }
    return pKnapsack;    
}

void PrintKnapsack(struct KnapsackInfo *pKnapsack, long row, long col) {
    printf("%9s%9s%9s", "weights", "values", "item");
    for (long j = 0; j <= pKnapsack->capacity; j++) {
         printf("%9ld", j);
    }
    printf("\n");

    for (long i = 0; i <= pKnapsack->numOfItems; i++) {
        if (i != 0) {
            printf("%9ld%9ld%9ld", ItemWeight(pKnapsack, i), ItemValue(pKnapsack, i), i);
        } else {
            printf("%9s%9s%9s", " ", " ", " ");
        }
        for (long j = 0; j <= pKnapsack->capacity; j++) {
            printf("%9ld", DpTableElement(pKnapsack, i, j));
        }
        printf("\n");
    }
    printf("\n");

    PrintAllChoices(pKnapsack, row, col);    
}

void ReleaseKnapsackInfo(struct KnapsackInfo *pKnapsack) {
    if (pKnapsack) {
        free(pKnapsack->dpTable);
        free(pKnapsack->nodes);
        free(pKnapsack->weights);
        free(pKnapsack->values);
        free(pKnapsack);
    }
}



/*
    Top-down.

    Memorize the solutions for the solved sub-problems.
 */
long SolveKnapsackMem(struct KnapsackInfo *pKnapsack, long n, long cap) {
    assert(n >= 0 && n <= pKnapsack->numOfItems && cap >= 0 && cap <= pKnapsack->capacity);

    if (DpTableElement(pKnapsack, n, cap) != KNAPSACK_INVALID_VALUE) {
        return DpTableElement(pKnapsack, n, cap);
    }    

    if (cap == 0) {
        DpTableElement(pKnapsack, n, cap) = 1;
    } else if (n == 0){
        DpTableElement(pKnapsack, n, cap) = 0;
    } else if (ItemWeight(pKnapsack, n) > cap) {
        DpTableElement(pKnapsack, n, cap) = SolveKnapsackMem(pKnapsack, n - 1, cap);
        // set the dag node to remember the choices
        if (DpTableElement(pKnapsack, n, cap) > 0) {
            DpDagNode(pKnapsack, n, cap).excluded = &DpDagNode(pKnapsack, n - 1, cap);
        }
    } else {
        long k = cap - ItemWeight(pKnapsack, n);
        long included = SolveKnapsackMem(pKnapsack, n, k);
        long excluded = SolveKnapsackMem(pKnapsack, n - 1, cap);
        // set the dag node to remember the choices
        if (included > 0) {
            DpDagNode(pKnapsack, n, cap).included = &DpDagNode(pKnapsack, n, k);
        }         
        if (excluded > 0) {
            DpDagNode(pKnapsack, n, cap).excluded = &DpDagNode(pKnapsack, n - 1, cap);
        } 
        DpTableElement(pKnapsack, n, cap) = included + excluded;
    }
    return DpTableElement(pKnapsack, n, cap);
}

/*
    Bottom-up.
 */
long SolveKnapsackTabulation(struct KnapsackInfo *pKnapsack, long n, long cap) {
    assert(n >= 0 && n <= pKnapsack->numOfItems && cap >= 0 && cap <= pKnapsack->capacity); 

    if (DpTableElement(pKnapsack, n, cap) != KNAPSACK_INVALID_VALUE) {
        return DpTableElement(pKnapsack, n, cap);
    }
         
    // row 0    
    for (long col = 1; col <= pKnapsack->capacity; col++) {
        DpTableElement(pKnapsack, 0, col) = 0;
    }
    // col 0
    for (long row = 0; row <= pKnapsack->numOfItems; row++) {
        DpTableElement(pKnapsack, row, 0) = 1;
    }
    // other rows
    for (long row = 1; row <= pKnapsack->numOfItems; row++) {
        for (long col = 0; col <= pKnapsack->capacity; col++) {
            if (col < ItemWeight(pKnapsack, row)) {
                DpTableElement(pKnapsack, row, col) = DpTableElement(pKnapsack, row - 1, col);
                if (DpTableElement(pKnapsack, row, col) > 0 && col != 0) {
                    DpDagNode(pKnapsack, row, col).excluded = &DpDagNode(pKnapsack, row - 1, col);
                }
            } else {
                long k = col - ItemWeight(pKnapsack, row);
                long included = DpTableElement(pKnapsack, row, k);
                long excluded = DpTableElement(pKnapsack, row - 1, col);
                // set the dag node to remember the choices
                if (included > 0) {
                    DpDagNode(pKnapsack, row, col).included = &DpDagNode(pKnapsack, row, k);
                } 
                if (excluded > 0) {
                    DpDagNode(pKnapsack, row, col).excluded = &DpDagNode(pKnapsack, row - 1, col);
                } 
                DpTableElement(pKnapsack, row, col) = included + excluded;
            }
        }
    }
    return DpTableElement(pKnapsack, n, cap);
}


