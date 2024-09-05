#include <stdio.h>
#include <stdlib.h>
#include "Array2Dot.h"
#include "Util.h"

static long imgCount = 1;

/*
    Bubble sort the n numbers pointed to by ptr.
    The elements are sorted in an ascending order (from the least to the greatest).
 */
void BubbleSort(int *ptr, int n) {
    /*
        1. iMax represents the max value of i in a pass
              (in the following "if (ptr[i] > ptr[i+1])")

        2. (n-1) passes needed in Bubble Sort
     */
    for (int iMax = n - 2; iMax >= 0; iMax--) { // (n-1) passes
        
        // printf() is quite useful in observing the behavior of a program
        printf("............... iMax = %d ...............\n\n", iMax);
        PrintArray(ptr, iMax + 2);
        printf("\n\n");

        // i is in [0, iMax] in the current pass
        for (int i = 0; i <= iMax; i++) {
            ArrayGenOneImage(ptr, iMax + 2, i, "BubbleSort", "images/BubbleSort", imgCount);
            imgCount++;            
            if (ptr[i] > ptr[i+1]) {
                Swap(ptr + i, ptr + i + 1);             
                printf("After swapping ptr[%d] and ptr[%d]:\n", i, i+1);
                PrintArray(ptr, iMax + 2);
                printf("\n");
                ArrayGenOneImage(ptr, iMax + 2, i, "BubbleSort", "images/BubbleSort", imgCount);
                imgCount++;                              
            }
        }       
    }
}

void TestBubbleSort(void) {
    // Let the C compiler determine the number of array elements for us.
    int arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    int len = sizeof(arr) / sizeof(arr[0]);
    
    printf("Before sorting:\n");
    PrintArray(arr, len);
    BubbleSort(arr, len);
    printf("After sorting:\n");
    PrintArray(arr, len);

    ArrayGenOneImage(arr, len, 0, "BubbleSort", "images/BubbleSort", imgCount);
    imgCount++;

}

