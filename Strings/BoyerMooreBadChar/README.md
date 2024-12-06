# Boyer-Moore algorithm (using the bad character rule)

- Scan the pattern from right to left , rather than from left to right.
- Focus on the bad character (i.e., text[i+j]) when a mismatch occurs (text[i+j] != pattern[j]).

### The "last occurrences array" in the Boyer-Moore algorithm

It is typically indexed by the bad character (or its corresponding integer value) and holds the index of its last occurrence in the pattern. 

If a bad character is not found in the pattern, the array stores a default value, often set to -1.

Given a pattern "aaba".

|  |   |  |  |  | 
|:-------:|:-------:|:-------:|:-------:|:-------:|
| index| 0 | 1 | 2 | 3 |
|pattern[] | a | a | b | a| 

Its lastOccurrences[] table is as follows.

| |   |
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

aaa found at index 0 in aaaabaaaa
aaa found at index 1 in aaaabaaaa
aaa found at index 5 in aaaabaaaa
aaa found at index 6 in aaaabaaaa

aabaabaab found at index 7 in aaaabaaaabaabaabaa

caba found at index 2 in ddcabacc


```

### The process of BoyerMooreBadChar

```C

void BoyerMooreBadChar(char *pattern, char *text);
 
```
### Example 1: BoyerMooreBadChar("aaba", "acaadaaaababaaba")

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0000.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0001.png" width="60%" height="60%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0002.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0003.png" width="60%" height="60%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0004.png" width="60%" height="60%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0005.png" width="60%" height="60%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0006.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0007.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0008.png" width="60%" height="60%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0009.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0010.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0011.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0012.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0013.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0014.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0015.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0016.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0017.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0018.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0019.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0020.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0021.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0022.png" width="60%" height="60%"> |


### Example 2: BoyerMooreBadChar("aaa", "aaaabaaaa")

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0023.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0024.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0025.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0026.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0027.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0028.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0029.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0030.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0031.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0032.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0033.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0034.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0035.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0036.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0037.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0038.png" width="60%" height="60%"> |

### Example 3: BoyerMooreBadChar("aabaabaab", "aaaabaaaabaabaabaa");

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0039.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0040.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0041.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0042.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0043.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0044.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0045.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0046.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0047.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0048.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0049.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0050.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0051.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0052.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0053.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0054.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0055.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0056.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0057.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0058.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0059.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0060.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0061.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0062.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0063.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0064.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0065.png" width="60%" height="60%"> |

### Example 4: BoyerMooreBadChar("caba", "ddcabacc");   

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0066.png" width="60%" height="60%"> |

    

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0067.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0068.png" width="60%" height="60%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0069.png" width="60%" height="60%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0070.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0071.png" width="60%" height="60%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreBadChar_0072.png" width="60%" height="60%"> |


