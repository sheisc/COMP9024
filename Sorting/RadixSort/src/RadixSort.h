#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include "Array2Dot.h"

#define  SORT_ALG_NAME  "RadixSort"

// DEFAULT_RADIX can be 2, 8, 10, ...
#define  DEFAULT_RADIX        10

struct DigitBucket {
    // for storing n elements at most, where n is specified in RadixInfo
    ArrayElementTy *elements;
    // the actual number of elements stored
    long count;
};

struct RadixInfo {
    struct DigitBucket arr[DEFAULT_RADIX];
    // for sorting n elements
    long n;
};

struct RadixInfo * CreateRadixInfo(long n);

void ReleaseRadixInfo(struct RadixInfo *pRadixInfo);

void RadixSort(ArrayElementTy *ptr, long n);

void TestRadixSort(void);

#endif

