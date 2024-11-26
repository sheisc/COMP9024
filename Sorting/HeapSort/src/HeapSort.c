#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "HeapSort.h"


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


/*
    Heapify a subtree at index i.
 */
static void Heapify(ArrayElementTy *ptr, long n, long i) {
    long maxIndex = i;  
  
    long leftIndex = 2 * i + 1;
    long rightIndex = 2 * i + 2;

    // left child
    if (leftIndex < n && ptr[leftIndex] > ptr[maxIndex]) {
        maxIndex = leftIndex;
    }

    // right child
    if (rightIndex < n && ptr[rightIndex] > ptr[maxIndex]) {
        maxIndex = rightIndex;
    }

    if (i != maxIndex) {
        // Now the max value is at index i
        Swap(ptr + i, ptr + maxIndex);    
        // Recursively heapify the sub-subtree at maxIndex
        Heapify(ptr, n, maxIndex);
    }
}

void HeapSort(ArrayElementTy *ptr, long n) {
    /*
        build a max heap

        (n / 2 - 1) * 2 + 1  = n - 1,  the index of the last element
     */
    for (long i = n / 2 - 1; i >= 0; i--) {
        Heapify(ptr, n, i);
        HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, i);
        imgCount++;
    }

    for (long i = n - 1; i > 0; i--) {
        // move the max value to the end
        Swap(ptr, ptr + i);
        HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, i+1, 0);
        imgCount++;

        HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, i, 0);
        imgCount++;
        // heapify the tree at index 0, with i elements
        Heapify(ptr, i, 0);
        HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, i, 0);
        imgCount++;
    }       
}


void TestHeapSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, arr, len, -1);
    imgCount++;   

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    HeapSort(arr, len);

    HeapGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, arr, len, -1);
    imgCount++; 

    printf("After sorting:\n");
    PrintArray(arr, len);

}

