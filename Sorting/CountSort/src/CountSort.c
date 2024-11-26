#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CountSort.h"


static long imgCount = 0;


void PrintArray(ArrayElementTy *ptr, long n) {
    for (long i = 0; i < n; i++) {
        printf("%ld ", (long) ptr[i]);
    }
    printf("\n");
}


void CountSort(ArrayElementTy *ptr, long left, long right, long n) {
    ArrayElementTy maxVal = ptr[0];
    assert(maxVal >= 0);
    for (long i = 1; i < n; i++) {
        assert(ptr[i] >= 0);
        if (ptr[i] > maxVal) {
            maxVal = ptr[i];
        }
    }

    ArrayElementTy *counters = (ArrayElementTy *) malloc(sizeof(ArrayElementTy) * (maxVal + 1));
    
    // init counters
    for (long i = 0; i <= maxVal; i++) {
        counters[i] = 0;
    }

    // counting
    for (long i = 0; i < n; i++) {
        long val = ptr[i];
        counters[val]++;
    }

    // save the sorted elements in the array pointed to by ptr
    long k = 0;
    for (long i = 0; i <= maxVal; i++) {
        long count = counters[i];
        if (count) {
            ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, counters, maxVal+1, 0, maxVal, i);
            imgCount++;
        }

        while (counters[i] > 0) {
            ptr[k] = i;
            k++;
            counters[i]--;                        
        }

        if (count) {
            ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, k, k-count, k-1, -1);
            imgCount++;            
        }
    }   
    free(counters);

       
}


void TestCountSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {3, 5, 2, 1, 6, 4, 3, 5, 2};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;   

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    CountSort(arr, 0, len - 1, len);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;

    printf("After sorting:\n");
    PrintArray(arr, len);

}

