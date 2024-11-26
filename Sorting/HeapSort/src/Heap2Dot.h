#ifndef HEAP2DOT_H
#define HEAP2DOT_H

typedef long ArrayElementTy;

void Heap2Dot(ArrayElementTy *ptr,
              long n, 
              long root,
              char *filePath,
              char *graphName);
               

void HeapGenOneImage(char *graphName, char *fileName, long seqNo, ArrayElementTy *arr, long n, long root);               

#endif

