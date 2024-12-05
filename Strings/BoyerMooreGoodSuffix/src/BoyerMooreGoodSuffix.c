#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "BoyerMoore.h"

static long imgCount = 0;

/*
    Only use longestBP[0 .. m-1].

    longestBP stands for the longest border position.
 */
static void PreprocessLongestBP(long *shiftTable, 
                                long *longestBP, 
                                char *pattern, 
                                long m) {

    long i = m - 1;
    long j = m;

    //longestBP[m-1] = m;
    longestBP[i] = j;

    while (i > 0) {
        assert(j > i);
        /*
            We already know longestBP[i] == j.
            Based on this, we want to calculate longestBP[i-1].
         */
        if (pattern[i-1] == pattern[j-1]) {
            // pattern[j-1 .. m-1] is the longest border of pattern[i-1 .. m-1]
            longestBP[i-1] = j-1;
            // 
            i--;
            j--;
        } else {
            // Case 1 found
            if (shiftTable[j] == 0) {
                /*
                    When a mismatch occurs at index (j-1),
                    shiftTable[j] will be used to skip characters in the Boyer-Moore algorithm.
                 */
                shiftTable[j] = j - i;
            }   
            //         
            if (j != m) {
                /*
                    Try the second longest border                    
                 */
                j = longestBP[j]; 
            } else {
                // no shorter border than the empty string ""
                longestBP[i-1] = m;
                i--;
            }      
        }
    }
}


static void PreprocessCase2AndCase3(long *shiftTable, 
                            long *longestBP, 
                            char *pattern, 
                            long m) {

    long j = longestBP[0];

    for (long k = 0; k <= m; k++) {
        if (shiftTable[k] == 0) {
            shiftTable[k] = j;
        }
        if (k < m && k == j) {
            // If we comment the following line, it will lead to unnecessary comparisons.
            j = longestBP[j];
        }
    }
}



void BoyerMooreGoodSuffix(char *pattern, char *text) {
    long i = 0;
    long j;
    long m = strlen(pattern);
    long n = strlen(text);

    assert(m > 0 && n > 0);
    /*
        A border is a substring that is both a proper prefix and a proper suffix.

        longestBP[i] 
        
            contains the index of the longest border of pattern[i .. m-1]. 
     */
    long *longestBP = (long *) malloc(sizeof(long) * (m + 1));
    /*
        shiftTable[i]

            contains the distance pattern will shift if mismatch occur at position i-1
     */
    long *shiftTable = (long *) malloc(sizeof(long) * (m + 1));

    for (j = 0; j < m + 1; j++) {
        shiftTable[j] = 0;
        longestBP[j] = 0;
    }
  
    PreprocessLongestBP(shiftTable, longestBP, pattern, m);

    ArrayGenOneImage("BoyerMooreGoodSuffix", "images/BoyerMooreGoodSuffix", imgCount, 
                     shiftTable, longestBP, pattern, m, text, n, 
                     j, i);
    imgCount++;

    PreprocessCase2AndCase3(shiftTable, longestBP, pattern, m);
  
    while (i <= n - m) {
        j = m - 1;

        ArrayGenOneImage("BoyerMooreGoodSuffix", "images/BoyerMooreGoodSuffix", imgCount, 
                         shiftTable, longestBP, pattern, m, text, n, 
                         j, i);
        imgCount++;

        while (j >= 0 && pattern[j] == text[i + j]) {
            j--;

            ArrayGenOneImage("BoyerMooreGoodSuffix", "images/BoyerMooreGoodSuffix", imgCount, 
                             shiftTable, longestBP, pattern, m, text, n, 
                             j, i);
            imgCount++;          
        }

        if (j < 0) {
            printf("%s found at index %ld in %s\n", pattern, i, text);
            i += shiftTable[0];
        } else {
            i += shiftTable[j+1];
        }
    }
    free(longestBP);
    free(shiftTable);
}


