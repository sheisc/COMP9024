#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "RadixSort.h"


static long imgCount = 0;


struct RadixInfo * CreateRadixInfo(long n) {
    struct RadixInfo *pRadixInfo = (struct RadixInfo *) malloc(sizeof(struct RadixInfo));
    assert(pRadixInfo);
    for (long r = 0; r < DEFAULT_RADIX; r++) {
        pRadixInfo->arr[r].count = 0;
        pRadixInfo->arr[r].elements = (ArrayElementTy *) malloc(sizeof(ArrayElementTy) * n);
        assert(pRadixInfo->arr[r].elements);
    }
    pRadixInfo->n = n;
    return pRadixInfo;
}

void ReleaseRadixInfo(struct RadixInfo *pRadixInfo) {
    if (pRadixInfo) {
        for (long r = 0; r < DEFAULT_RADIX; r++) {
            free(pRadixInfo->arr[r].elements);
        }
        free(pRadixInfo);
    }
}

void PrintArray(ArrayElementTy *ptr, long n) {
    for (long i = 0; i < n; i++) {
        printf("%ld", (long) ptr[i]);
        if (i < n -1) {
            printf(", ");
        } else {
            printf(" ");
        }
    }
    printf("\n");
}

static ArrayElementTy GetMaxValue(ArrayElementTy *ptr, long n) {
    ArrayElementTy maxVal = ptr[0];
    
    for (long i = 0; i < n; i++) {
        assert(ptr[i] >= 0);
        if (ptr[i] > maxVal) {
            maxVal = ptr[i];
        }
    }
    return maxVal;
}

void RadixSort(ArrayElementTy *ptr, long n) {
    ArrayElementTy maxVal = GetMaxValue(ptr, n);

    struct RadixInfo *pRadixInfo = CreateRadixInfo(n);

    long exp = 1;

    while ((maxVal / exp) > 0) {
        for (long i = 0; i < n; i++) {
            long r = (ptr[i] / exp) % DEFAULT_RADIX;
            long count = pRadixInfo->arr[r].count;          
            pRadixInfo->arr[r].elements[count] = ptr[i];
            pRadixInfo->arr[r].count++;
        }

        long k = 0;
        for (long r = 0; r < DEFAULT_RADIX; r++) {
            for (long j = 0; j < pRadixInfo->arr[r].count; j++) {
                ptr[k] = pRadixInfo->arr[r].elements[j];
                k++;
            }
        }

        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, pRadixInfo);
        imgCount++;

        PrintArray(ptr, n);

        // reset the counters
        for (long r = 0; r < DEFAULT_RADIX; r++) {
            pRadixInfo->arr[r].count = 0;
        }
        
        exp *= DEFAULT_RADIX;
    }
    ReleaseRadixInfo(pRadixInfo);
}


void TestRadixSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {22, 45, 308, 90, 9024, 24, 2, 64, 17, 20, 0};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    RadixSort(arr, len);

    printf("After sorting:\n");
    PrintArray(arr, len);

}

