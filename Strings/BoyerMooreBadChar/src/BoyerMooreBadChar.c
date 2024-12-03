#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "BoyerMoore.h"


static long imgCount = 0;

static long lastOccurrances[NUM_OF_CHARS];

/*
    A bad char is a character in the text which has caused a mismatch
    at the current position.
 */

static void InitLastOccurrances(char *pattern, long m, long *lastOccurrances) {
    for (long i = 0; i < NUM_OF_CHARS; i++) {
        lastOccurrances[i] = -1;
    }

    for (long i = 0; i < m; i++) {
        long val = (long) pattern[i];
        // save the last occurrence of a character in the pattern
        lastOccurrances[val] = i;
    }
}

void BoyerMooreBadChar(char *pattern, char *text) {
    long n = strlen(text);
    long m = strlen(pattern);
    // index i for traversing the text
    long i = 0;
    char badChar = 0;
    long shiftVal = 0;

    assert(m > 0 && n >0);

    InitLastOccurrances(pattern, m, lastOccurrances);

    while ( i <= (n-m)) {
        // index j for traversing the pattern from right to left
        long j = m - 1;

        ArrayGenOneImage("BoyerMooreBadChar", "images/BoyerMooreBadChar", imgCount, 
                        lastOccurrances, pattern, m, text, n, 
                        j, i);
        imgCount++;

        while (j >= 0 && pattern[j] == text[i+j]) {
            j--;
            ArrayGenOneImage("BoyerMooreBadChar", "images/BoyerMooreBadChar", imgCount, 
                            lastOccurrances, pattern, m, text, n, 
                            j, i);
            imgCount++;            
        }

        if (j < 0) {
            // Found one
            printf("%s found at index %ld in %s\n", pattern, i, text);
            if (i + m < n) {
                // make sure text[i+m] is a legal memory access, not out-of-bound.
                badChar = text[i + m];
                shiftVal = m - lastOccurrances[(long) badChar];
                i += shiftVal;
            } else {
                i += 1;
            }
        } else {
            badChar = text[i + j];
            long lastIndex = lastOccurrances[(long)badChar];
            if (j > lastIndex) {
                /*
                (1)
                    The last occurrance of the badChar b is on the left side of pattern[j]
                    in the pattern.


                                        j
                    pattern:      . b . a . . .
                    
                       text:  . . . . . b . . .  
                                  i

                (2) lastIndex == -1
                    
                    the badChar b does not exist in the pattern.
                 */
                shiftVal = j - lastIndex;
                i += shiftVal;
            } else {
                /*
                    The last occurrance of the badChar b is NOT ot on the left side of pattern[j]
                    in the pattern.

                                        j
                    pattern:      . . . a . . b . .
                    
                       text:  . . . . . b . . b . .  
                                  i
                 */                
                i += 1;
            }
        }
    }
}


