#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "BoyerMoore.h"


static long imgCount = 0;

static long lastOccurrences[NUM_OF_CHARS];

/*
    A bad char is a character in the text which has caused a mismatch
    at the current position.
 */

static void InitLastOccurrences(char *pattern, long m, long *lastOccurrences) {
    for (long i = 0; i < NUM_OF_CHARS; i++) {
        lastOccurrences[i] = -1;
    }

    for (long i = 0; i < m; i++) {
        long val = (unsigned char) pattern[i];
        // save the last occurrence of a character in the pattern
        lastOccurrences[val] = i;
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

    InitLastOccurrences(pattern, m, lastOccurrences);

    while ( i <= (n-m)) {
        // index j for traversing the pattern from right to left
        long j = m - 1;

        ArrayGenOneImage("BoyerMooreBadChar", "images/BoyerMooreBadChar", imgCount, 
                        lastOccurrences, pattern, m, text, n, 
                        j, i);
        imgCount++;

        while (j >= 0 && pattern[j] == text[i+j]) {
            j--;
            ArrayGenOneImage("BoyerMooreBadChar", "images/BoyerMooreBadChar", imgCount, 
                            lastOccurrences, pattern, m, text, n, 
                            j, i);
            imgCount++;            
        }

        if (j < 0) {
            // Found one
            printf("%s found at index %ld in %s\n", pattern, i, text);
            if (i + m < n) {
                // make sure text[i+m] is a legal memory access, not out-of-bound.
                badChar = text[i + m];
                long val = (unsigned char) badChar;
                shiftVal = m - lastOccurrences[val];
                i += shiftVal;
            } else {
                i += 1;
            }
        } else {
            badChar = text[i + j];
            long val = (unsigned char) badChar;
            long lastIndex = lastOccurrences[val];
            if (j > lastIndex) {
                /*
                (1)
                    The last occurrence of the badChar b is on the left side of pattern[j]
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
                    The last occurrence of the badChar b is NOT ot on the left side of pattern[j]
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


