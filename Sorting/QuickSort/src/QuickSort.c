#include <stdio.h>
#include <stdlib.h>
#include "QuickSort.h"


static long imgCount = 0;


void Swap(ArrayElementTy *pa, ArrayElementTy *pb) {
    ArrayElementTy tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

void PrintArray(ArrayElementTy *ptr, int n) {
    for (long i = 0; i < n; i++) {
        printf("%ld ", (long) ptr[i]);
    }
    printf("\n");
}

/*
    Given an array, one element is chosen and others are partitioned 
    into two subsets:
    (1) those less than the partition element
    (2) those greater than or equal to it

    The same process is then applied recursively to the two subsets.

    When a subset has fewer than two elements, 
    it doesn't need any sorting; this stops the recursion.

 */
void QuickSort(ArrayElementTy *ptr, long left, long right, long n) {
    if (left >= right) {
        return;
    }
    long pivot = (left + right) / 2;
    
    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, ptr, n, left, right, pivot);
    imgCount++;
    
    // move partition element to ptr[left]
    Swap(ptr + left, ptr + pivot);
    
    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, ptr, n, left, right, left);
    imgCount++;
    
    long last = left;
    
    // partitionElement, [smaller elements],  [elements larger or equal to]
    for (long i = left + 1; i <= right; i++) {    
        if (ptr[i] < ptr[left]) {
            last++;
            Swap(ptr + last, ptr + i);
        }
    }
    
    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, ptr, n, left, right, left);
    imgCount++;
    
    // [smaller elements],  partitionElement, [elements larger or equal to]
    Swap(ptr + left, ptr + last);
    
    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, ptr, n, left, right, last);
    imgCount++;    
    //
    QuickSort(ptr, left, last - 1, n);
    QuickSort(ptr, last + 1, right, n);
}


void TestQuickSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);
    
    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, arr, len, -1, -1, -1);
    imgCount++;

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    QuickSort(arr, 0, len - 1, len);

    printf("After sorting:\n");
    PrintArray(arr, len);

    ArrayGenOneImage("QuickSort", "images/QuickSort", imgCount, arr, len, -1, -1, -1);
    imgCount++;    

}

