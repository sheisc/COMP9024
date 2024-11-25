# SelectionSort

``` sh
/*******************************************************************
                SelectionSort

    1.  The algorithm of selection sort


                                             COMP9024

 *******************************************************************/
``` 

### make view 

```sh
SelectionSort$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```

### The process of SelectionSort(arr, 0, 5, 6)

|Initial | 
|:-------------:|
| <img src="images/SelectionSort_0000.png" width="50%" height="50%"> |

|Pass 1: minIndex=0 | Update minIndex| After Swap(arr+0, arr+minIndex) |
|:-------------:|:-------------:|:-------------:|
| <img src="images/SelectionSort_0001.png" width="100%" height="100%"> |<img src="images/SelectionSort_0002.png" width="100%" height="100%"> |<img src="images/SelectionSort_0003.png" width="100%" height="100%"> |

|Pass 2: minIndex=1 | Update minIndex|  After Swap(arr+1, arr+minIndex) |
|:-------------:|:-------------:|:-------------:|
| <img src="images/SelectionSort_0004.png" width="100%" height="100%"> |<img src="images/SelectionSort_0005.png" width="100%" height="100%"> |<img src="images/SelectionSort_0006.png" width="100%" height="100%"> |


|Pass 3: minIndex=2 | Update minIndex|  After Swap(arr+2, arr+minIndex) |
|:-------------:|:-------------:|:-------------:|
| <img src="images/SelectionSort_0007.png" width="100%" height="100%"> |<img src="images/SelectionSort_0008.png" width="100%" height="100%"> |<img src="images/SelectionSort_0009.png" width="100%" height="100%"> |

|Pass 4: minIndex=3 | Update minIndex|  After Swap(arr+3, arr+minIndex) |
|:-------------:|:-------------:|:-------------:|
| <img src="images/SelectionSort_0010.png" width="100%" height="100%"> |<img src="images/SelectionSort_0011.png" width="100%" height="100%"> |<img src="images/SelectionSort_0012.png" width="100%" height="100%"> |

|Pass 5: minIndex=4 | Update minIndex|  After Swap(arr+4, arr+minIndex) |
|:-------------:|:-------------:|:-------------:|
| <img src="images/SelectionSort_0013.png" width="100%" height="100%"> |<img src="images/SelectionSort_0014.png" width="100%" height="100%"> |<img src="images/SelectionSort_0015.png" width="100%" height="100%"> |

|Final | 
|:-------------:|
| <img src="images/SelectionSort_0016.png" width="50%" height="50%"> |

### Output
```sh
SelectionSort$ ./main

Before sorting:
30 50 20 10 60 40 
After sorting:
10 20 30 40 50 60 

```