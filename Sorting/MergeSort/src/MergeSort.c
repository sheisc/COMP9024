#include <stdio.h>
#include <stdlib.h>

#include "MergeSort.h"


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


static void DoMerge(ArrayElementTy *ptr, ArrayElementTy *tmp, long left, long middle, long right, long n) {
    long i = left;
    long j = middle + 1;
    long k = left;

    // copy dst[] to tmp[]
    for (long t = left; t <= right; t++) {
        tmp[t] = ptr[t];
    }
    // merging
    while (i <= middle && j <= right) {
        if (tmp[i] <= tmp[j]) {
            ptr[k] = tmp[i];
            i++;
            k++;
        } else {
            ptr[k] = tmp[j];
            j++;
            k++;            
        }
    }
    //
    while (i <= middle) {
        ptr[k] = tmp[i];
        i++;
        k++;
    }
    // 
    while (j <= right) {
        ptr[k] = tmp[j];
        j++;
        k++;
    }
}


static void RecurMergeSort(ArrayElementTy *ptr, ArrayElementTy *tmp, long left, long right, long n) {
    if (left < right) {
        long middle = (left + right) / 2;
        RecurMergeSort(ptr, tmp, left, middle, n);

        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, left, middle, -1);
        imgCount++;

        RecurMergeSort(ptr, tmp, middle + 1, right, n);

        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, middle + 1, right, -1);
        imgCount++;

        //Post-order traversal
        DoMerge(ptr, tmp, left, middle, right, n);        
       
    }
}

void MergeSort(ArrayElementTy *ptr, long left, long right, long n) {
    if (left < right) {
        ArrayElementTy *tmp = (ArrayElementTy *) malloc(sizeof(ArrayElementTy) * n);
        RecurMergeSort(ptr, tmp, left, right, n);

        ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, ptr, n, left, right, -1);
        imgCount++;
        
        free(tmp);
    }
}


void TestMergeSort(void) {
    // Let the C compiler determine the number of array elements for us.
    ArrayElementTy arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    long len = sizeof(arr) / sizeof(arr[0]);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;   

    printf("Before sorting:\n");
    PrintArray(arr, len);
    
    MergeSort(arr, 0, len - 1, len);

    ArrayGenOneImage(SORT_ALG_NAME, "images/" SORT_ALG_NAME, imgCount, 
                      arr, len, -1, -1, -1);
    imgCount++;

    printf("After sorting:\n");
    PrintArray(arr, len);

}

