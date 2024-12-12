#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Knapsack.h"
#include "Knapsack2Dot.h"

typedef long (*KnapsackSolveFptrTy)(struct KnapsackInfo *pKnapsack, long n, long cap);

static void TestKnapsackSolver(long values[], long weights[], long n, long capacity, 
                              KnapsackSolveFptrTy knapsackSolve, long row, long col) {
    static long imgCount = 0;
    struct KnapsackInfo *pKnapsack = CreateKnapsackInfo(values, weights, n, capacity);
    // query
    printf("\nKnapsack() = %ld \n\n", knapsackSolve(pKnapsack, row, col));           
    PrintKnapsack(pKnapsack, row, col);
    
    KnapsackGenOneImage("Coins", "images/Coins", imgCount, pKnapsack, row, col);
    imgCount++;
    
    ReleaseKnapsackInfo(pKnapsack);
}

int main(void) {
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    long values[] = {2, 3, 5};
    //long weights[] = {2, 3, 5}; 

    // Subset sum
    // long values[] = {5, 7, 8, 9};
    // long weights[] = {5, 7, 8, 9};  

    long n = sizeof(values) / sizeof(values[0]);
    long capacity = 10;
    // 0 <= row <= n, 0 <= col <= capacity
    long row = n;
    long col = capacity;

    // Bottom up
    TestKnapsackSolver(values, values, n, capacity, SolveKnapsackTabulation, row, col);
    // Top down
    TestKnapsackSolver(values, values, n, capacity, SolveKnapsackMem, row, col);

    return 0;
}



