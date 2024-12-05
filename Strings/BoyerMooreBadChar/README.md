# Boyer-Moore algorithm (using the bad character rule)

- Scan the pattern from right to left , rather than from left to right.
- Focus on the bad character (i.e., text[i+j]) when a mismatch occurs (text[i+j] != pattern[j]).

### The "last occurrences array" in the Boyer-Moore algorithm

It is typically indexed by the bad character (or its corresponding integer value) and holds the index of its last occurrence in the pattern. 

If a bad character is not found in the pattern, the array stores a default value, often set to -1.

| Pattern |   |  |  |  | 
|:-------:|:-------:|:-------:|:-------:|:-------:|
| index| 0 | 1 | 2 | 3 |
|pattern[] | a | a | b | a| 



| Last Occurrences |   |
|:-------:|:-------:|
|lastOccurrences['a'] | 3 |
|lastOccurrences['b'] | 2 |
| other               | -1|

### Increasing the index i in the text means 'shifting the pattern to the right' in Boyer-Moore.

After increasing i, the pattern will be realigned with the text in the next iteration (with j reset to m-1).

In other words, after re-alignment, pattern[0] will always correspond to text[i].

### When a mismatch occurs (text[i+j] != pattern[j])

```C
    long lastIndex = lastOccurrences[(long)badChar];
    if (j > lastIndex) {
        /*
        (1)
            The last occurrence of the badChar b is on the left side of pattern[j]
            in the pattern.


                          0       j
            pattern:      . . b . a . . .
            
               text:  . . . . . . b . . .  
                          i      i+j

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

            Regress to the most conservative method (i.e., increasing i by one)
         */                
        i += 1;
    }
```

### When a match occurs (text[i+j] == pattern[j])

```C
    j--;
    if (j < 0) {
        /*
            Pattern found

                       -1 0             m-1 m
            pattern:      . . . . . . . .   
            
               text:  . . . . . . . . . .   b 
                          i                 m

            if ( (i+m) < n ) {
                // Treat text[i+m] as the bad char
                last = lastOccurrences[text[i+m]];
                i += m - last;
            } else { // text[i+m] is out-of-bound
                i += 1;
            }
         */
    }
```
### make view 

```sh
BoyerMooreBadChar$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```



### Output
```sh
BoyerMooreBadChar$ make
BoyerMooreBadChar$ ./main

aaba found at index 7 in acaadaaaababaaba

aaba found at index 12 in acaadaaaababaaba

```

### The process of BoyerMooreBadChar

```C

void BoyerMooreBadChar(char *pattern, char *text);
 
```
### BoyerMooreBadChar("aaba", "acaadaaaababaaba")

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0000.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0001.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0002.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0003.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0004.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0005.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0006.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0007.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0008.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0009.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0010.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0011.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0012.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0013.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0014.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0015.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0016.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0017.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0018.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0019.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0020.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0021.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0022.png" width="100%" height="100%"> |

