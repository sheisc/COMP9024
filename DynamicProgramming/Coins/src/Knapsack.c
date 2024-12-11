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
    n represents the number of items.
    row is the row number of the dynamic programming table, from 0 to n
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
    VisitChoiceDag(pKnapsack, &ChoiceNodeElement(pKnapsack, row, col), choices, 0);
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
            InitChoiceDagNode(& ChoiceNodeElement(pKnapsack, i, j), i, j, NULL, NULL);
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

    The optimal value when the first n items are considered under the capacity cap.

    Memorize the solutions for the solved sub-problems.
 */
long SolveKnapsackMem(struct KnapsackInfo *pKnapsack, long n, long cap) {
    // FIXME: sanity check
    if (DpTableElement(pKnapsack, n, cap) != KNAPSACK_INVALID_VALUE) {
        return DpTableElement(pKnapsack, n, cap);
    }
    
    if (n == 0 || cap == 0) {
        DpTableElement(pKnapsack, n, cap) = 0;
    } else if (ItemWeight(pKnapsack, n) > cap) {
        DpTableElement(pKnapsack, n, cap) = SolveKnapsackMem(pKnapsack, n - 1, cap);
        // set the tree node to remember the choices
        ChoiceNodeElement(pKnapsack, n, cap).excluded = &ChoiceNodeElement(pKnapsack, n - 1, cap);
    } else {
        long k = cap - ItemWeight(pKnapsack, n);
        long included = ItemValue(pKnapsack, n) + SolveKnapsackMem(pKnapsack, n - 1, k);
        long excluded = SolveKnapsackMem(pKnapsack, n - 1, cap);
        long max;
        // set the tree node to remember the choices
        if (included > excluded) {
            ChoiceNodeElement(pKnapsack, n, cap).included = &ChoiceNodeElement(pKnapsack, n - 1, k);
            max = included;
        } else if (included < excluded) {
            ChoiceNodeElement(pKnapsack, n, cap).excluded = &ChoiceNodeElement(pKnapsack, n - 1, cap);
            max = excluded;
        } else {
            ChoiceNodeElement(pKnapsack, n, cap).included = &ChoiceNodeElement(pKnapsack, n - 1, k);
            ChoiceNodeElement(pKnapsack, n, cap).excluded = &ChoiceNodeElement(pKnapsack, n - 1, cap);
            max = included;
        }
        DpTableElement(pKnapsack, n, cap) = max;
    }
    return DpTableElement(pKnapsack, n, cap);
}

/*
    Bottom-up.
 */
long SolveKnapsackTabulation(struct KnapsackInfo *pKnapsack, long n, long cap) {
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
                    ChoiceNodeElement(pKnapsack, row, col).excluded = &ChoiceNodeElement(pKnapsack, row - 1, col);
                }
            } else {
                long k = col - ItemWeight(pKnapsack, row);
                long included = DpTableElement(pKnapsack, row, k);
                long excluded = DpTableElement(pKnapsack, row - 1, col);
                // Create the binary tree to remember the choices
                if (included > 0) {
                    ChoiceNodeElement(pKnapsack, row, col).included = &ChoiceNodeElement(pKnapsack, row, k);
                } 
                if (excluded > 0) {
                    ChoiceNodeElement(pKnapsack, row, col).excluded = &ChoiceNodeElement(pKnapsack, row - 1, col);
                } 
                DpTableElement(pKnapsack, row, col) = included + excluded;
            }
        }
    }
    return DpTableElement(pKnapsack, n, cap);
}


