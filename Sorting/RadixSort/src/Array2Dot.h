#ifndef ARRAY2DOT_H
#define ARRAY2DOT_H

typedef long ArrayElementTy;

struct RadixInfo;

void Array2Dot(char *filePath,
               char *graphName,
               struct RadixInfo *radixArr);
               
void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, 
                      struct RadixInfo *radixArr);               

#endif

