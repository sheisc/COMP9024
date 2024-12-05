# Boyer-Moore algorithm (using the Good-Suffix rule) 

[The Bad-Character rule](../BoyerMooreBadChar/README.md) uses the mismatched character (text[i+j]) to determine the shift when a mismatch occurs (text[i+j] != pattern[j]).

The Good-Suffix rule uses the good suffix (pattern[j+1 .. m-1]) to guide the shift in the Boyer-Moore algorithm,
where m is the length of the pattern, and pattern[j+1 .. m-1] represents a substring from pattern[j+1] to pattern[m-1].

- Scan the pattern from right to left , rather than from left to right.
- Use the good suffix (i.e., pattern[j+1 .. m-1] == text[i+j+1 .. i+m-1]) to avoid unnecessary comparisons when a mismatch occurs.

```sh
text:
            . . .   text[i+j]  text[i+j+1 .. i+m-1] . . .               

pattern:
                    pattern[j] pattern[j+1 .. m-1] 
```



### Motivation for avoiding unnecessary comparisons when a mismatch occurs

Case 1:
    Try to find another occurrence of the substring pattern[j+1 .. m-1] in the pattern, but preceded by a character
    which is different from pattern[j].

    Since another occurrence of pattern[j] pattern[j+1 .. m-1] will cause another mismatch.

Case 2:
    When case 1 fails, find a prefix of the pattern (starting at index 0), which matches a suffix of pattern[j+1 .. m-1].

Case 3:
    When both case 1 and case 2 fail, skip text[i+j+1 .. i+m-1].


### longestBP[m]

A border is a substring that is both a proper prefix and a proper suffix.

LongestBP stands for the longest border position.

If longestBP[i] == j, the substring pattern[j .. m-1] is the longest border of pattern[i .. m-1].


### Imagine that pattern[m] contains an empty string (i.e., "").

Given a pattern "aaba".


|  |   |  |  |  |  |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
| index| 0 | 1 | 2 | 3 | 4|
|pattern[] | a | a | b | a| ""|
|longestBP[] | 3 | 3 | 4 | 4|  |

For example, longestBP[0] is 3.
It means that the substring "a" (starting from index 3) is the longest border of "aaba" (starting from index 0).

Similarly, longestBP[3] == 4 means "" is the longest border of the substring "a" (starting from index 3).

The notation pattern[m .. m-1] is a special case, representing the empty substring "".

### The substring pattern[longestBP[j] .. m-1] is the second longest border of pattern[i .. m-1], where longestBP[i] == j.

Suppose longestBP[i] == j, where i < j.

According to its definition, the substring pattern[j .. m-1] is the longest border of pattern[i .. m-1].

For an index k, where j < k < longestBP[j], pattern[k .. m-1] can't be a border of pattern[i .. m-1].

```sh
Proof by contradiction:

If pattern[k .. m-1] is a border of pattern[i .. m-1],
then it is also a border of pattern[j .. m-1].

Reason:
    1. The length of pattern[k .. m-1] is (m-k).

    2. The length of pattern[j .. m-1] is (m-j), where (m-j) > (m-k).

    3. Since pattern[j .. m-1] is the longest border of pattern[i .. m-1],    
    the first (m-j) characters of pattern[i .. m-1] and pattern[j .. m-1] are the same.

    4. As pattern[k .. m-1] is a border of pattern[i .. m-1],
    the first (m-k) characters of pattern[k .. m-1] and pattern[i .. m-1] are identical.
    
    So, the first (m-k) characters of pattern[k .. m-1] and pattern[j .. m-1] are the same. 
    That is, pattern[k .. m-1] is a proper prefix of pattern[j .. m-1].
    Obviously, pattern[k .. m-1] is a proper suffix of pattern[j .. m-1].
    Finally, pattern[k .. m-1] is a border of pattern[j .. m-1].

But pattern[k .. m-1], a border of pattern[j .. m-1], is longer than the longest border (pattern[longestBP[j] .. m-1]) 
of pattern[j .. m-1].

This contradicts the definition of longestBP[j].
```


### How to preprocess the longestBP[] table

- Scan the pattern from right to left.
- Imagine there is an empty string "" at pattern[m], where m is the length of the pattern.
- Starting from the base case: longestBP[m-1] = m, since the longest border of pattern[m-1 .. m-1] is "".

Note that a border is both a proper suffix and a proper prefix of a string.


```C
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
```

### shiftTable[]

Based on longestBP[m], 
the Boyer-Moore algorithm further constructs a shiftTable[m+1] to determine the shift value 
for a mismatch at text[i+j] != text[j].

#### Its implementation is very tricky.

- One part of shiftTable[] (i.e., Case 1) is preprocessed when constructing longestBP[].
- The other part is handled in PreprocessCase2AndCase3() as follows.


```C
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
```

Suppose the pattern is "aabaabaab".



After PreprocessLongestBP(), we have the following table.

|  |  |  |  |  |  |  |  |  |  |  |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
| index| 0 | 1 | 2 | 3 | 4|5 | 6 | 7 | 8| 9|
|pattern[] | a | a | b | a| a| b| a| a | b| "" |
|longestBP[] | 3 | 4 | 5 |6 | 7 | 8|  9|9 | 9 |
|shiftTable[] | 0 | 0 |0 | 0| 0 |0 | 0| 0 | 0| 1|

After PreprocessCase2AndCase3(), the shiftTable[] is updated as follows.

|  |  |  |  |  |  |  |  |  |  |  |
|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|:-------:|
| index| 0 | 1 | 2 | 3 | 4|5 | 6 | 7 | 8| 9|
|pattern[] | a | a | b | a| a| b| a| a | b| "" |
|longestBP[] | 3 | 4 | 5 |6 | 7 | 8|  9|9 | 9 |
|shiftTable[] | 3 | 3 |3 | 3| 6 |6 | 6| 9 | 9| 1|

If we consider how to match the prefix of the pattern (starting from pattern[0]) 
after shifting the pattern to the right, 
the possible shift values are listed as follows.



```C
{longestBP[0], longestBP[longestBP[0]],  longestBP[longestBP[longestBP[0]]], ...,  m}
```

In this example, they are 
```C
{3, 6, 9}
```

Shift values {3, 6} are for Case 2, while the shift value 9 (the value of m, the length of the pattern) is for Case 3.

Explanation:

When a mismatch occurs at pattern[4] != text[i+4],
the Boyer-Moore algorithm will use shiftTable[4+1] to shift the pattern to the right.

The shift value in the element shiftTable[5] should be 6, rather than 3 (resulting in unnecessary comparisons).

Reason:
```sh
If the shfit value 3 is used, there will be another mismatch at pattern[1] != text[i+1] after shifting (i.e., i += 3) and realignment .

Note that both pattern[1] and pattern[4] are 'a'.
```

### When a mismatch occurs (text[i+j] != text[j])

```C
    i += shiftTable[j+1];

    // j is reset to be m-1, where m is the length of the pattern    
```

### When a match occurs (text[i+j] == text[j])

```C
    j--;

    if (j < 0) {
        // Pattern found in the text
        i += shiftTable[0];

        // j will be reset to m-1        
    }
```




### Increasing the index i in the text means 'shifting the pattern to the right' in Boyer-Moore.

After increasing i, the pattern will be realigned with the text in the next iteration (with j reset to m-1).

In other words, after re-alignment, pattern[0] will always correspond to text[i].

### make view 

```sh
BoyerMooreGoodSuffix/$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```



### Output
```sh
BoyerMooreGoodSuffix$ make
BoyerMooreGoodSuffix$ ./main

aaba found at index 7 in acaadaaaababaaba
aaba found at index 12 in acaadaaaababaaba

aaa found at index 0 in aaaabaaaa
aaa found at index 1 in aaaabaaaa
aaa found at index 5 in aaaabaaaa
aaa found at index 6 in aaaabaaaa

aabaabaab found at index 7 in aaaabaaaabaabaabaa

```

### The process of BoyerMoore

```C

void BoyerMooreGoodSuffix(char *pattern, char *text);
 
```
### Example 1: BoyerMooreGoodSuffix("aaba", "acaadaaaababaaba")

| After PreprocessLongestBP() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0000.png" width="100%" height="100%"> |

| After PreprocessCase2AndCase3() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0001.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0002.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0003.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0004.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0005.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0006.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0007.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0008.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0009.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0010.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0011.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0012.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0013.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0014.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0015.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0016.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0017.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0018.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0019.png" width="100%" height="100%"> |

### Example 2: BoyerMooreGoodSuffix("aaa", "aaaabaaaa")



| After PreprocessLongestBP() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0020.png" width="100%" height="100%"> |


|  After PreprocessCase2AndCase3() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0021.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0022.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0023.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0024.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0025.png" width="100%" height="100%"> |



|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0026.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0027.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0028.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0029.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0030.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0031.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0032.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0033.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0034.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0035.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0036.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0037.png" width="100%" height="100%"> |

### Example 3: BoyerMooreGoodSuffix("aabaabaab", "aaaabaaaabaabaabaa")

| After PreprocessLongestBP() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0038.png" width="100%" height="100%"> |


|  After PreprocessCase2AndCase3() | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0039.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0040.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0041.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0042.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0043.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0044.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0045.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0046.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0047.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0048.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0049.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0050.png" width="100%" height="100%"> |



|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0051.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0052.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/BoyerMooreGoodSuffix_0053.png" width="100%" height="100%"> |

