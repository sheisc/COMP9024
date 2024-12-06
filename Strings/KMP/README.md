# KMP

KMP stands for Knuth-Morris-Pratt, which is an efficient algorithm used for string matching or substring searching. 

LPPS: The longest proper prefix (also a suffix). 

```sh
String: "ababab"
LPPS: "abab"
Explanation:  The longest proper prefix ("abab") is also a suffix.
              "ab" is another proper prefix (also a suffix) of "ababab", but not the longest.
```


It uses preprocessing to build an auxiliary array called the longest proper prefix (also a suffix) array, lppsArr[]. 

This array helps in skipping unnecessary comparisons when a mismatch occurs.

### When a mismatch occurs (text[i] != pattern[j])

```sh

(1) j == 0

text
                                    i
            .   .   .   .   .   .   y   .   .   

pattern
                                    x   .   .

                                    j

Shift:
        i++;
```

```sh
(2) j > 0            


text
                        i-j         i
            .   .   .   .   .   .   y   .   .   

pattern
                        .   .   .   x   .   .

                        0           j


We want to reuse the information (i.e., text[i-j .. i-1] == pattern[0 .. j-1])

Shift:
        j = lppsArr[j-1],  
        where lppsArr[j-1] is the length of the longest proper 
        prefix (also a suffix) of pattern[0 .. j-1].
```

### The key point of KMP is how to generate the lppsArr[] array

- Scan the pattern from left to right.
- The base case is lppsArr[0] = 0, as the empty string "" (with a length of 0) is the LPPS of pattern[0 .. 0].
- The notation pattern[0 .. -1] is a special notation, representing ""

### The substring pattern[0 .. lppsArr[len - 1]-1] is the second longest proper prefix (also a suffix) of pattern[0 .. i-1], where lppsArr[i-1] == len && i >= 1 && len > 0

For a length k, where lppsArr[len - 1] < k < len, pattern[0 .. k-1] can't be a proper prefix (also a proper suffix) of pattern[0 .. i-1].

```sh
Proof by contradiction:

Suppose pattern[0 .. k-1] is a proper prefix (also a proper suffix) of pattern[0 .. i-1].

    pattern[0 .. k-1] == pattern[i-k .. i-1]

then it is also a proper prefix (also a proper suffix) of pattern[0 .. len-1].

Note that pattern[0 .. len-1] is the LPPS of pattern[0 .. i-1]

    pattern[0 .. len-1] == pattern[i-len .. i-1]

Since k < len, we have

    i-len < i-k

So pattern[0 .. k-1] is also a proper prefix and a proper suffix of pattern[0 .. len-1].

But pattern[0 .. k-1] is longer than pattern[0 .. lppsArr[len - 1]-1].

This contradicts the definition of lppsArr[len - 1]:

    pattern[0 .. lppsArr[len - 1]-1] (with a length of lppsArr[len - 1]) is the LPPS of pattern[0 .. len-1].
```

### InitLPPSArray()

```C
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
```


### make view 

```sh
KMP$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```



### Output
```sh
KMP$ make
KMP$ ./main

aaba found in acaadaaaababaaba at index 7
aaba found in acaadaaaababaaba at index 12

aaa found in aaaabaaaa at index 0
aaa found in aaaabaaaa at index 1
aaa found in aaaabaaaa at index 5
aaa found in aaaabaaaa at index 6

aabaabaab found in aaaabaaaabaabaabaa at index 7

caba found in ddcabacc at index 2


```

### The process of KMP

```C
/* 
    Search the pattern in the text.
 */
void KMPSearch(char *pattern, char *text);
 
```
#### When a mismatch occurs (text[i] != pattern[j])

```
    if j == 0
        i++
    else
        j = lpps[j-1]
```


### Example 1: KMPSearch("aaba", "acaadaaaababaaba")

|  | 
|:-------------:|
| <img src="images/KMP_0000.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0001.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0002.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0003.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0004.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0005.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0006.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0007.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0008.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0009.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0010.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0011.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0012.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0013.png" width="100%" height="100%"> |

|  | 
|:-------------:|
| <img src="images/KMP_0014.png" width="100%" height="100%"> |


|  | 
|:-------------:|
| <img src="images/KMP_0015.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0016.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0017.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0018.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0019.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0020.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0021.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0022.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0023.png" width="100%" height="100%"> |


    

### Example 2: KMPSearch("aaa", "aaaabaaaa")

| | 
|:-------------:|
| <img src="images/KMP_0024.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0025.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0026.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0027.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0028.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0029.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0030.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0031.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0032.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0033.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0034.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0035.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0036.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0037.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0038.png" width="100%" height="100%"> |

### Example 3: KMPSearch("aabaabaab", "aaaabaaaabaabaabaa")

| | 
|:-------------:|
| <img src="images/KMP_0039.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0040.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0041.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0042.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0043.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0044.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0045.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0046.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0047.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0048.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0049.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0050.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0051.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0052.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0053.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0054.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0055.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0056.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0057.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0058.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0059.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0060.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0061.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0062.png" width="100%" height="100%"> |

### Example 4: KMPSearch("caba", "ddcabacc")

| | 
|:-------------:|
| <img src="images/KMP_0063.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0064.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0065.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0066.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0067.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0068.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0069.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0070.png" width="100%" height="100%"> |

| | 
|:-------------:|
| <img src="images/KMP_0071.png" width="100%" height="100%"> |


| | 
|:-------------:|
| <img src="images/KMP_0072.png" width="100%" height="100%"> |

