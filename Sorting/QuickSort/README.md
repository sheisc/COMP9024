# QuickSort


### make view 

```sh
QuickSort$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```

### The process of QuickSort(arr, 0, 5, 6)

```C
void QuickSort(ArrayElementTy *ptr, long left, long right, long n);
```

|Initial | 
|:-------------:|
| <img src="images/QuickSort_0000.png" width="50%" height="50%"> |

|QuickSort(arr, 0, 5, 6): partitionElement in the middle | partitionElement, {other elements} | partitionElement, [ < ],  [ >= ] | [ < ],  partitionElement, [ >= ] | 
|:-------------:|:-------------:|:-------------:|:-------------:|
| <img src="images/QuickSort_0001.png" width="100%" height="100%"> |<img src="images/QuickSort_0002.png" width="100%" height="100%"> | <img src="images/QuickSort_0003.png" width="100%" height="100%"> | <img src="images/QuickSort_0004.png" width="100%" height="100%"> |


|QuickSort(arr, 2, 5, 6): partitionElement in the middle | partitionElement, {other elements} | partitionElement, [ < ],  [ >= ] | [ < ],  partitionElement, [ >= ] | 
|:-------------:|:-------------:|:-------------:|:-------------:|
| <img src="images/QuickSort_0005.png" width="100%" height="100%"> |<img src="images/QuickSort_0006.png" width="100%" height="100%"> | <img src="images/QuickSort_0007.png" width="100%" height="100%"> | <img src="images/QuickSort_0008.png" width="100%" height="100%"> |

|QuickSort(arr, 2, 3, 6): partitionElement in the middle | partitionElement, {other elements} | partitionElement, [ < ],  [ >= ] | [ < ],  partitionElement, [ >= ] | 
|:-------------:|:-------------:|:-------------:|:-------------:|
| <img src="images/QuickSort_0009.png" width="100%" height="100%"> |<img src="images/QuickSort_0010.png" width="100%" height="100%"> | <img src="images/QuickSort_0011.png" width="100%" height="100%"> | <img src="images/QuickSort_0012.png" width="100%" height="100%"> |

|Final | 
|:-------------:|
| <img src="images/QuickSort_0013.png" width="50%" height="50%"> |

### Output
```sh
QuickSort$ make
QuickSort$ ./main

Before sorting:
30 50 20 10 60 40 
After sorting:
10 20 30 40 50 60 
```