#ifndef ARRAY2DOT_H
#define ARRAY2DOT_H

typedef char ArrayElementTy;

void Array2Dot(char *filePath, char *graphName,
               long *lppsArr, char *pattern, long m, char *text, long n,
               long indexPat, long indexTxt);
               
void ArrayGenOneImage(char *graphName, char *fileName, long seqNo, 
                      long *lppsArr, char *pattern, long m, char *text, long n, 
                      long indexPat, long indexTxt);               

#endif

