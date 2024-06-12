# Bubble Sort

``` sh
/*******************************************************************
                Tutorial 1    Bubble Sort

    1.  How to use VS Code to debug a program

    2.  Array in C (used as data structures for bubble sort)

    3.  The algorithm of bubble sort

    4.  Pointer arithmetic in *(ptr + i)

    5.  Why Swap2(int a, int b) doesn't work?

                                             COMP9024 24T2

 *******************************************************************/
``` 

## 1 How to download Tutorials in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Tutorials/Week2

Week2$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

Week2$ code

```

Two configuration files (Week2/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and Week2/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Tutorials/Week2", then click the "Open" button.


### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/BubbleSort.c, and click to add a breakpoint (say, line 63).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
├── src                  containing *.c and *.h
|   |
│   └── BubbleSort.c
|
└── .vscode              containing configuration files for Visual Studio Code
    |
    ├── launch.json      specifying which program to debug and with which debugger,
    |                    used when you click "Run -> Start Debugging"
    |
    └── tasks.json       specifying which task to run (e.g., 'make' or 'make clean')
                         used when you click "Terminal -> Run Build Task" or "Terminal -> Run Task"
```

Makefile is discussed in [COMP9024/C/HowToMake](../../C/HowToMake/README.md).

## 3 The process of bubble sort

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

``` sh

Week2$ make

Week2$ ./main

Before sorting:
30 50 20 10 60 40 
............... iMax = 4 ...............

30 50 20 10 60 40 


After swapping ptr[1] and ptr[2]:
30 20 50 10 60 40 

After swapping ptr[2] and ptr[3]:
30 20 10 50 60 40 

After swapping ptr[4] and ptr[5]:
30 20 10 50 40 60 

............... iMax = 3 ...............

30 20 10 50 40 


After swapping ptr[0] and ptr[1]:
20 30 10 50 40 

After swapping ptr[1] and ptr[2]:
20 10 30 50 40 

After swapping ptr[3] and ptr[4]:
20 10 30 40 50 

............... iMax = 2 ...............

20 10 30 40 


After swapping ptr[0] and ptr[1]:
10 20 30 40 


............... iMax = 1 ...............

10 20 30 


............... iMax = 0 ...............

10 20 


After sorting:
10 20 30 40 50 60 


Why Swap2(i, j) doesn't work?

i = 20, j = 24


Swap2(i, j)
a = 20, b = 24
a = 24, b = 20
i = 20, j = 24


```


## 4 Access an array via its name (e.g., arr)
``` C
    int arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    int len = sizeof(arr) / sizeof(arr[0]);
    // only &arr[0] is passed  
    PrintArray(arr, len);

    4.1  arr[0] in C

    In C, arr[0] is the syntax used to access the first element of an array arr.

    Here's what it means:
    
    (1)
        arr is the name of the array.
    (2)
        [0] indicates the index of the element you want to access.
    (3)
        array indices start from 0, 
        so arr[0] refers to the first element of the array arr.
    
    4.2  arr passed as an argument in a function call

       When an array arr is passed as an argument in a function call (say, PrintArray(arr, len)), 
       
       only &arr[0] is passed (i.e., the address of arr[0]), rather than the value of the whole array arr.  

       By contrast, the value of len is passed (e.g., PrintArray(arr, len)).

    4.3  arr[-1] in C

    Attempting to access an array element with a negative index, 
    such as arr[-1], results in undefined behavior. 
    It's considered as an "out of bounds" access, 
    meaning you're trying to access memory outside the allocated space for the array. 

    This can lead to unpredictable program behavior, crashes, or even security vulnerabilities. 

    Always ensure that you're accessing array elements within the bounds of the array.    

    4.4 arr[i], *(arr+i)

        // access array elements via arr[i] 
        for (int i = 0; i < len; i++) {
            printf("%d ", arr[i]);
        }

        // access array elements via *(arr+i)
        // *(arr+i) is equivalent to arr[i], where arr is an array
        for (int i = 0; i < len; i++) {
            printf("%d ", *(arr+i));
        }
            

    4.5  li[-1] in Python (COMP9021)

        In Python, accessing li[-1] returns the last element of the list. 

        $ python3

        >>> li = [10, 20, 30]

        >>> li[-1]
        30
```


## 5 Access an array via a pointer variable (e.g., ptr)

```C
    void PrintArray(int *ptr, int n) {
        for (int i = 0; i < n; i++) {
            printf("%d ", ptr[i]);
        }
        printf("\n");
    }

    5.1  ptr[i]

    ptr[i] accesses the value at the memory location i positions away 
    from the base address pointed to by ptr.
    
    It's equivalent to *(ptr + i).       

    5.2 The address of ptr[i] can be expressed as:

    (1)  &ptr[i]
    (2)  (ptr + i)

    5.3 ptr + i

    In C, ptr + i is an expression used in pointer arithmetic. 
    Here, ptr is a pointer variable, and i is an integer. 
    When you add an integer to a pointer, 
    the result is a new pointer that points to a memory location i elements away from the original location, 
    where the size of each element (sizeof(*ptr) or sizeof(T)) is determined by the type of the pointer (T *).
    In this case, T is int.    

    5.4  *(ptr + i)

    In C, *(ptr + i) is an expression that is used to access the value 
    stored at a memory location that is i elements away 
    from the memory location pointed to by the pointer ptr.

    Let us break it down:

    (1)
        ptr is a pointer variable, pointing to some memory location.
    (2)
        i is an integer.
    (3)
        ptr + i performs pointer arithmetic, 
        resulting in a pointer that points to a memory location i elements away from the original location.
    (4)
        *(ptr + i) dereferences this pointer, 
        meaning it retrieves the value stored at that memory location.

```

## 6 MyCOMP9024/Tutorials/MyWeek2

After fully grasping the program in Week2, you can create a directory MyCOMP9024/Tutorials/MyWeek2, 
and write the same program from scratch by yourselves in your own way.

**In order to understand one's knowledge blind spots in programming, one must write code oneself.**


## 7 Practical exercise

**Our tutors will NOT answer the following questions in tutorials.**

**Please complete the following code in Q1-Q5 and then answer the questions in Quiz 1 (Week 2) in [Moodle](https://moodle.telt.unsw.edu.au/my/courses.php).**

```C
#include <stdio.h>

void Swap(int *pa, int *pb) {
    int tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

void PrintArray(int *ptr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

int IsLess(int a, int b) {
    return a < b;
}

int IsLarger(int a, int b) {
    return a > b;
}

// Define a function pointer type, which points to a function
typedef int (*ComparatorFuncPtr)(int, int);


void BubbleSort(int *ptr, int n, ComparatorFuncPtr compare) {
    for (int iMax = n - 2; iMax >= 0; iMax--) {
        for (int i = 0; ____Q1_____; _____Q2____) {
            if (_____Q3_____) {
                ____Q4____;
            }
        }       
    }
}

int main(void) {    
    int arr[] = {30, 50, 20, 10, 60, 40};
    int len = sizeof(arr) / sizeof(arr[0]);
    
    // a function pointer variable which points to the function IsLarger
    ComparatorFuncPtr fptr = &IsLarger;    
    printf("Before sorting:\n");
    PrintArray(arr, len);
    BubbleSort(arr, len, fptr);
    // in an ascending order
    printf("After sorting:\n");
    PrintArray(arr, len);

    // a function pointer variable which points to the function IsLess
    fptr = ____Q5____;    
    printf("\nBefore sorting:\n");
    PrintArray(arr, len);
    BubbleSort(arr, len, fptr);
    // in a descending order
    printf("After sorting:\n");
    PrintArray(arr, len);
    return 0;
}

```


