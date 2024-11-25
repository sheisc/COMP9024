#include <stdio.h>
#include <stdlib.h>

#include "InsertionSort.h"


static long imgCount = 0;


void Swap(ArrayElementTy *pa, ArrayElementTy *pb) {
    ArrayElementTy tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}


void PrintArray(ArrayElementTy *ptr, long n) {
    for (long i = 0; i < n; i++) {
        printf("%ld ", (long) ptr[i]);
    }
    printf("\n");
}


void InsertionSort(ArrayElementTy *ptr, long n) {
    for (long i = 1; i < n; i++) {
        ArrayElementTy curValue = ptr[i];
        long insPos = i;
        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, 0, i, i);
        imgCount++;
        for (long j = i - 1; j >= 0 && ptr[j] > curValue; j--) {
            ptr[j+1] = ptr[j];
            insPos = j;
        }
        ptr[insPos] = curValue;
        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, 0, i, insPos);
        imgCount++;        
    }
}


void TestInsertionSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;   

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    InsertionSort(arr, len);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;

    printf("After sorting:\n");
    PrintArray(arr, len);

}

