#ifndef ARRAY2DOT_H
#define ARRAY2DOT_H

typedef int ArrayElementTy;

void Array2Dot(ArrayElementTy *arr, 
               long n,
               long index,
               char *filePath,
               char *graphName,
               int displayVisited);

void ArrayGenOneImage(ArrayElementTy *arr, long n, long index, char *graphName, char *fileName, long seqNo);

#endif

