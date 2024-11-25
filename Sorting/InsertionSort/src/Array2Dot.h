#ifndef ARRAY2DOT_H
#define ARRAY2DOT_H

typedef long ArrayElementTy;

void Array2Dot(char *filePath,
               char *graphName,
               ArrayElementTy *arr, long n, long left, long right, long index);
               
void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, 
                      ArrayElementTy *arr, long n, long left, long right, long index);               

#endif

