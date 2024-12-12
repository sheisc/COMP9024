#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "Knapsack.h"


// static long imgCount = 0;

/*
    The parameter 'count' represents the current index for appending an item in choices[].
 */
static void VisitChoiceDag(struct KnapsackInfo *pKnapsack, long row, long col, long choices[], long count) {

    if (col == 0) {
        // leaf node 
        printf("Choices: ");
        for (long i = 0; i < count; i++) {
            printf("%ld ", choices[i]);
        }
        printf("\n");
        return;
    } else {        
        for (long item = 1; item <= row; item++) {
            long k = col - ItemWeight(pKnapsack, item);
            if (k >= 0 && DpTableElement(pKnapsack, row, k) > 0) {
                choices[count] = ItemWeight(pKnapsack, item);
                VisitChoiceDag(pKnapsack, row, k, choices, count + 1);
            }
        }
    }
}

static void PrintAllChoices(struct KnapsackInfo *pKnapsack, long row, long col) {       
    long capacity = pKnapsack->capacity;
    // FIXME:  
    long *choices = (long *) malloc(sizeof(long) * (capacity + 1));    
    VisitChoiceDag(pKnapsack, row, col, choices, 0);
    free(choices);
}

struct KnapsackInfo *CreateKnapsackInfo(long *values, long *weights, long numbOfItems, long capacity) {
    struct KnapsackInfo *pKnapsack = (struct KnapsackInfo *) malloc(sizeof(struct KnapsackInfo));
    memset(pKnapsack, 0, sizeof(struct KnapsackInfo));
    long *dpTable = (long *) malloc(sizeof(long) * (numbOfItems + 1) * (capacity + 1));
    
    pKnapsack->values = (long *) malloc(sizeof(long) * numbOfItems);
    memcpy(pKnapsack->values, values, sizeof(long) * numbOfItems);

    pKnapsack->weights = (long *) malloc(sizeof(long) * numbOfItems);
    memcpy(pKnapsack->weights, weights, sizeof(long) * numbOfItems);

    pKnapsack->visited = (int *) malloc(sizeof(int) * (numbOfItems + 1) * (capacity + 1));

    pKnapsack->numOfItems = numbOfItems;
    pKnapsack->capacity = capacity;
    pKnapsack->dpTable = dpTable;

    for (long i = 0; i <= numbOfItems; i++) {
        for (long j = 0; j <= capacity; j++) {
            DpTableElement(pKnapsack, i, j) = KNAPSACK_INVALID_VALUE;
            DpTableElementVisited(pKnapsack, i, j) = 0;
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
        free(pKnapsack->weights);
        free(pKnapsack->values);
        free(pKnapsack->visited);
        free(pKnapsack);
    }
}

/*
    Top-down.

    Memorize the solutions for the solved sub-problems.
    
    When cap < 0:
        return 0;
 */
long SolveKnapsackMem(struct KnapsackInfo *pKnapsack, long n, long cap) {
    // 
    assert(n >= 0 && n <= pKnapsack->numOfItems && cap <= pKnapsack->capacity);

    if (cap < 0) {
        return 0;
    }
    
    if (DpTableElement(pKnapsack, n, cap) != KNAPSACK_INVALID_VALUE) {
        return DpTableElement(pKnapsack, n, cap);
    }    

    if (cap == 0) {
        DpTableElement(pKnapsack, n, cap) = 1;
    } else if (n == 0){
        DpTableElement(pKnapsack, n, cap) = 0;
    } else {
        long sum = 0;
        for (long row = 1; row <= n; row++) {
            sum += SolveKnapsackMem(pKnapsack, n, cap - ItemWeight(pKnapsack, row));
        }
        DpTableElement(pKnapsack, n, cap) = sum;
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
        for (long col = 1; col <= pKnapsack->capacity; col++) {
            /*
                (1) row == 1, {item1} used
                    
                    DpTableElement(row, col) = DpTableElement(row, col - ItemWeight(item1))

                (2) row == 2, {item1, item2} used
                  
                    DpTableElement(row, col) = DpTableElement(row, col - ItemWeight(item1)) 
                                               +
                                               DpTableElement(row, col - ItemWeight(item2))
                                               
                (3) row == 3, {item1, item2, item3} used

                    DpTableElement(row, col) = DpTableElement(row, col - ItemWeight(item1)) 
                                               +
                                               DpTableElement(row, col - ItemWeight(item2))
                                               +
                                               DpTableElement(row, col - ItemWeight(item3))                           
             */
            long sum = 0;
            for (long item = 1; item <= row; item++) {
                long k = col - ItemWeight(pKnapsack, item);
                if (k >= 0) {
                    sum += DpTableElement(pKnapsack, row, k);
                }
            }
            DpTableElement(pKnapsack, row, col) = sum;       
        }
    }
    return DpTableElement(pKnapsack, n, cap);
}


