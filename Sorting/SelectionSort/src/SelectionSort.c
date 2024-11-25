#include <stdio.h>
#include <stdlib.h>
#include "SelectionSort.h"


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


void SelectionSort(ArrayElementTy *ptr, long left, long right, long n) {
    for (long i = 0; i <= n - 2; i++) {
        // Elements: ptr[i], ptr[i+1], ...,  ptr[n-1]
        long minIndex = i;
        ArrayGenOneImage("SelectionSort", "images/SelectionSort", imgCount, ptr, n, i, n-1, i);
        imgCount++;
        for (long j = i + 1; j <= n - 1; j++) {
            if (ptr[j] < ptr[minIndex]) {
                minIndex = j;
            }
        }

        ArrayGenOneImage("SelectionSort", "images/SelectionSort", imgCount, ptr, n, i, n-1, minIndex);
        imgCount++; 
        Swap(ptr + i, ptr + minIndex);
        ArrayGenOneImage("SelectionSort", "images/SelectionSort", imgCount, ptr, n, i, n-1, i);
        imgCount++;        
    }
}


void TestSelectionSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    ArrayGenOneImage("SelectionSort", "images/SelectionSort", imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;   

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    SelectionSort(arr, 0, len - 1, len);

    ArrayGenOneImage("SelectionSort", "images/SelectionSort", imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;

    printf("After sorting:\n");
    PrintArray(arr, len);

}

