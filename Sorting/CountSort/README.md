# CountSort

### make view 

```sh
CountSort$ make view

find ./images -name "*.png" | sort | xargs feh -g 1024x768  &
```

### The process of CountSort(arr, 6)



```C
void CountSort(ArrayElementTy *ptr, long n);
```

|Initial | 
|:-------------:|
| <img src="images/CountSort_0000.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0001.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0002.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0003.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0004.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0005.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0006.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0007.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0008.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0009.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0010.png" width="100%" height="100%"> |

|Counters | 
|:-------------:|
| <img src="images/CountSort_0011.png" width="100%" height="100%"> |

|Sorted | 
|:-------------:|
| <img src="images/CountSort_0012.png" width="100%" height="100%"> |

|Final | 
|:-------------:|
| <img src="images/CountSort_0013.png" width="100%" height="100%"> |


### Output
```sh
CountSort$ make
CountSort$ ./main

Before sorting:
3 5 2 1 6 4 3 5 2 
After sorting:
1 2 2 3 3 4 5 5 6

```