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
void GetLengthOfLPPS(char *pattern, long *lppsArr, long m) {
    /*
        len stores the length of the longest proper prefix (also a suffix) 
        of the previous index (i - 1).
     */
    long len = 0;
    /*
        There is only one proper prefix (i.e., "") of the string pattern[0].
        "" is also a suffix of pattern[0].
        The length of "" is 0.
     */
    lppsArr[0] = 0;

    long i = 1;

    while (i < m) {          
        if (pattern[i] == pattern[len]) {
            // If characters match, increment the length of lpps
            len++;
            lppsArr[i] = len;
            i++;
        } else {
            // If there is a mismatch
            if (len != 0) {
                len = lppsArr[len - 1];
            } else {
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
void GetLengthOfLPPSV2(char *pattern, long *lppsArr, long m) {
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

    It returns

    (1) the position of the pattern in the text if there is a match
    (2) -1 if there is no match
 */
long KMPSearch(char *pattern, char *text) {
    long n = strlen(text);
    long m = strlen(pattern);
    long pos = -1;
    // index i for traversing the text
    long i = 0;
    // index j for traversing the pattern
    long j = 0;

    long *lppsArr = (long *) malloc(sizeof(long) * m);
    for (long k = 0; k < m; k++) {
        lppsArr[k] = -1;
    }    

    GetLengthOfLPPS(pattern, lppsArr, m);
    //GetLengthOfLPPSV2(pattern, lppsArr, m);
    for (long k = 0; k < m; k++) {
        printf("%ld ", lppsArr[k]);
    }
    printf("\n");
   
    imgCount = 0;

    while (i < n) {
        ArrayGenOneImage("KMP", "images/KMP", imgCount, 
                         lppsArr, pattern, m, text, n, 
                         j, i);
        imgCount++;

        if (text[i] == pattern[j]) {
            i++;
            j++;
            if (j == m) {
                pos = i - m;                
                break;

                // printf("Found at index %ld\n", pos);
                // j = lppsArr[j-1];
            }
        } else { // there is a mismatch        
            if (j != 0) {
                j = lppsArr[j-1];
            } else {
                i++;
                // assert(j == 0);
            }
        }            
    }    
    ArrayGenOneImage("KMP", "images/KMP", imgCount, 
                     lppsArr, pattern, m, text, n, 
                     j, i);
    imgCount++;    

    free(lppsArr);
    return pos;
}


