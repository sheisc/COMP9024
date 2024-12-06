#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "KMP.h"


static long imgCount = 0;


/* 
    LPPS: the Longest Proper Prefix (also a Suffix).

    lppsArr[i] is the length of the longest proper prefix of pattern[0..i]
    which is also a suffix of pattern[0..i].
 */
static void InitLPPSArray(char *pattern, long *lppsArr, long m) {
    /*
        Base case:

            The empty string "" (with a length of 0) is the LPPS of pattern[0 .. 0].
     */
    lppsArr[0] = 0;

    long i = 1;
    long len = 0;
    while (i < m) {
        /*
            We have
                lppsArr[i-1] == len
            
            pattern[0 .. len-1] == pattern[i-len .. i-1]
         */
        if (pattern[len] == pattern[i]) {
            /*
                pattern[0 .. len] == pattern[i-len .. i]
             */
            lppsArr[i] = len + 1;
            len++;
            i++;
        } else {
            if (len > 0) {
                /*
                    Since lppsArr[i-1] == len,
                    pattern[0 .. len-1] is the LPPS of pattern[0 .. i-1].
                    
                    Try a shorter proper prefix of pattern[0 .. i-1].
                 */
                len = lppsArr[len-1];
            } else {
                // no shorter one, only "" left.
                assert(len == 0);
                lppsArr[i] = 0;
                i++;
            }
        }
    }
}


/*
    Test whether pattern[0..j] is a LPPS of pattern[0..i].

    LPPS: the Longest Proper Prefix (also a Suffix)
 */
static int IsLPPS(char *pattern, long i, long j) {
    assert(i > j);
    for (long k = j; k >= 0; k--, i--) {
        if (pattern[i] != pattern[k]) {
            return 0;
        }
    }
    return 1;
}

/*
    Naive method in calculating the length of the longest proper prefix (also a suffix).
 */
void InitLPPSArrayV2(char *pattern, long *lppsArr, long m) {
    lppsArr[0] = 0;

    for (long i = 1; i < m; i++) {
        lppsArr[i] = 0;
        // Test each possible prefix:  
        for (long j = i - 1; j >= 0; j--) {
            if (IsLPPS(pattern, i, j)) {
                // j+1 is the length of patter[0..j]
                lppsArr[i] = j + 1;
                break;
            }
        } 
    }
}


/* 
    Search the pattern in the text.
 */
void KMPSearch(char *pattern, char *text) {
    long m = strlen(pattern);
    long n = strlen(text);
    assert(m > 0 && n > 0);

    long *lppsArr = (long *) malloc(sizeof(long) * m);

    InitLPPSArray(pattern, lppsArr, m);
    
    long i = 0, j = 0;

    while (i < n) {
        ArrayGenOneImage("KMP", "images/KMP", imgCount, 
                         lppsArr, pattern, m, text, n, 
                         j, i);
        imgCount++;

        if (pattern[j] == text[i]) {
            i++;
            j++;
            if (j == m) {
                ArrayGenOneImage("KMP", "images/KMP", imgCount, 
                                lppsArr, pattern, m, text, n, 
                                j, i);
                imgCount++;

                printf("%s found in %s at index %ld\n", pattern, text, i-m);
                j = lppsArr[m-1];
            }
        } else {
            if (j > 0) {
                // try the second longest proper prefix (also a suffix)
                j = lppsArr[j-1];
            } else {
                assert(j == 0);
                i++;                
            }
        }
    }
    free(lppsArr);
}



