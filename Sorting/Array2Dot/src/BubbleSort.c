#include <stdio.h>
#include <stdlib.h>

#include "BubbleSort.h"


static long imgCount = 0;

/*
    Swap the values of the two integer variables pointed to by pa and pb, respectively.  
 */
void Swap(ArrayElementTy *pa, ArrayElementTy *pb) {
    ArrayElementTy tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}


/*
    void PrintArray(int *ptr, int n);

        Print the values of the n integer variables pointed to by an pointer ptr:

            ptr[0],  ptr[1],    ptr[2],    ...,  ptr[n-1]

        or 
            *ptr,   *(ptr+1),  *(ptr+2),   ...,  *(ptr + n -1)

 */
void PrintArray(ArrayElementTy *ptr, long n) {
    for (long i = 0; i < n; i++) {
        printf("%ld ", (long) ptr[i]);
    }
    printf("\n");
}

/*
    Bubble sort the n numbers pointed to by ptr.
    The elements are sorted in an ascending order (from the least to the greatest).
 */
void BubbleSort(ArrayElementTy *ptr, long n) {
    /*
        1. iMax represents the max value of i in a pass
              (in the following "if (ptr[i] > ptr[i+1])")

        2. (n-1) passes needed in Bubble Sort
     */
    for (long iMax = n - 2; iMax >= 0; iMax--) { // (n-1) passes
        
        // printf() is quite useful in observing the behavior of a program
        printf("............... iMax = %ld ...............\n\n", iMax);
        PrintArray(ptr, iMax + 2);
        printf("\n\n");

        // i is in [0, iMax] in the current pass
        for (long i = 0; i <= iMax; i++) {
            ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                             ptr, n, 0, iMax+1, i);
            imgCount++;

            if (ptr[i] > ptr[i+1]) {
                Swap(ptr + i, ptr + i + 1);             
                printf("After swapping ptr[%ld] and ptr[%ld]:\n", i, i+1);
                PrintArray(ptr, iMax + 2);
                printf("\n");

                ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                                 ptr, n, 0, iMax+1, i);
                imgCount++;                                                        
            }
        }       
    }
}

void TestBubbleSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    ArrayElementTy len = sizeof(arr) / sizeof(arr[0]);
    
    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                     arr, len, -1, -1, -1);
    imgCount++;  

    printf("Before sorting:\n");
    PrintArray(arr, len);
    BubbleSort(arr, len);
    printf("After sorting:\n");
    PrintArray(arr, len);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                     arr, len, -1, -1, -1);
    imgCount++;

}

