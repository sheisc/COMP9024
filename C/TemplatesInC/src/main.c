#include <stdio.h>
#include <assert.h>
#include "Vector.h"

#define PrintVector(type)   Print##type##Vector


void PrintVector(long)(struct Vector(long) *pVec) { 
    for (long i = 0; i < VectorSize(long)(pVec); i++) { 
        printf("%ld ", VectorGet(long)(pVec, i)); 
    } 
    printf("\n"); 
}

void PrintVector(double)(struct Vector(double) *pVec) { 
    for (long i = 0; i < VectorSize(double)(pVec); i++) { 
        printf("%f ", VectorGet(double)(pVec, i)); 
    } 
    printf("\n"); 
}


int main(void) {
    // a vector of integers
    struct Vector(long) *pVec1 = CreateVector(long)();
    VectorAppend(long)(pVec1, 10);
    VectorAppend(long)(pVec1, 20);
    PrintVector(long)(pVec1);
    ReleaseVector(long)(pVec1);

    // a vector of doubles
    struct Vector(double) *pVec2 = CreateVector(double)();
    VectorAppend(double)(pVec2, 300.0);
    VectorAppend(double)(pVec2, 3.14);
    PrintVector(double)(pVec2);
    ReleaseVector(double)(pVec2);
    return 0;  
}

