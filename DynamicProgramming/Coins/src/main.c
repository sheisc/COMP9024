#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Knapsack.h"
#include "Knapsack2Dot.h"



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

    struct KnapsackInfo *pKnapsack = CreateKnapsackInfo(values, values, n, capacity);
    printf("\nKnapsack() = %ld \n\n", SolveKnapsackTabulation(pKnapsack, n, capacity));           
    PrintKnapsack(pKnapsack, n, capacity);

    KnapsackGenOneImage("Coins", "images/Coins", 0, pKnapsack, n, capacity);
    
    ReleaseKnapsackInfo(pKnapsack);

    return 0;
}


