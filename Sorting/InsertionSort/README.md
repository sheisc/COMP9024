# InsertionSort

### make view 

```sh
InsertionSort$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```

### The process of InsertionSort(arr, 6)

|Initial | 
|:-------------:|
| <img src="images/InsertionSort_0000.png" width="50%" height="50%"> |

|Pass 1: i=1 | Update the insertion point | 
|:-------------:|:-------------:|
| <img src="images/InsertionSort_0001.png" width="100%" height="100%"> |<img src="images/InsertionSort_0002.png" width="100%" height="100%"> |

|Pass 2: i=2 |Update the insertion point | 
|:-------------:|:-------------:|
| <img src="images/InsertionSort_0003.png" width="100%" height="100%"> |<img src="images/InsertionSort_0004.png" width="100%" height="100%"> |


|Pass 3: i=3 |Update the insertion point | 
|:-------------:|:-------------:|
| <img src="images/InsertionSort_0005.png" width="100%" height="100%"> |<img src="images/InsertionSort_0006.png" width="100%" height="100%"> |

|Pass 4: i=4 | Update the insertion point| 
|:-------------:|:-------------:|
| <img src="images/InsertionSort_0007.png" width="100%" height="100%"> |<img src="images/InsertionSort_0008.png" width="100%" height="100%"> |

|Pass 5: i=5 | Update the insertion point| 
|:-------------:|:-------------:|
| <img src="images/InsertionSort_0009.png" width="100%" height="100%"> |<img src="images/InsertionSort_0010.png" width="100%" height="100%"> |

|Final | 
|:-------------:|
| <img src="images/InsertionSort_0011.png" width="50%" height="50%"> |

### Output
```sh
InsertionSort$ make
InsertionSort$ ./main

Before sorting:
30 50 20 10 60 40 
After sorting:
10 20 30 40 50 60 

```