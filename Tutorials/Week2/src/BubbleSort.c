#include <stdio.h>

/*
    Swap the values of the two integer variables pointed to by pa and pb, respectively.  
 */
void Swap(int *pa, int *pb) {
    int tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

/*
  Why Swap2(int a, int b) doesn't work?

    Only the values of the two parameters a and b are swapped.

 */
void Swap2(int a, int b) {
    printf("a = %d, b = %d\n", a, b);
    int tmp = a;
    a = b;
    b = tmp;
    printf("a = %d, b = %d\n", a, b);
}

/*
    void PrintArray(int *ptr, int n);

        Print the values of the n integer variables pointed to by an pointer ptr:

            ptr[0],  ptr[1],    ptr[2],    ...,  ptr[n-1]

        or 
            *ptr,   *(ptr+1),  *(ptr+2),   ...,  *(ptr + n -1)

 */
void PrintArray(int *ptr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

/*
    Bubble sort the n numbers pointed to by ptr.
    The elements are sorted in ascending order (from the least to the greatest).
 */
void BubbleSort(int *ptr, int n) {
    /*
        1. iMax represents the max value of i in a pass
              (in the following "if (ptr[i] > ptr[i+1])")

        2. (n-1) passes needed in Bubble Sort
     */
    for (int iMax = n - 2; iMax >= 0; iMax--) { // (n-1) passes
        
        // printf() is quite useful in observing the behavior of a program
        printf("............... iMax = %d ...............\n\n", iMax);
        PrintArray(ptr, iMax + 2);
        printf("\n\n");

        // i is in [0, iMax] in the current pass
        for (int i = 0; i <= iMax; i++) {
            if (ptr[i] > ptr[i+1]) {
                Swap(ptr + i, ptr + i + 1);

                printf("After swapping ptr[%d] and ptr[%d]:\n", i, i+1);
                PrintArray(ptr, iMax + 2);
                printf("\n");
            }
        }       
    }
}

#if 0 
int IsLess(int a, int b) {
    return a < b;
}

int IsLarger(int a, int b) {
    return a > b;
}

// Define a function pointer type, which points to a function
typedef int (*ComparatorFuncPtr)(int, int);


void BubbleSort2(int *ptr, int n, ComparatorFuncPtr compare) {
    for (int iMax = n - 2; iMax >= 0; iMax--) {
        for (int i = 0; ____Q1_____; _____Q2____) {
            if (_____Q3_____) {
                ____Q4____;
            }
        }       
    }
}

int TestBubbleSort2(void) {    
    int arr[] = {30, 50, 20, 10, 60, 40};
    int len = sizeof(arr) / sizeof(arr[0]);
    
    // a function pointer variable which points to the function IsLarger()
    ComparatorFuncPtr fptr = &IsLarger;    
    printf("Before sorting:\n");
    PrintArray(arr, len);
    BubbleSort2(arr, len, fptr);
    // in ascending order
    printf("After sorting:\n");
    PrintArray(arr, len);

    // a function pointer variable which points to the function IsLess()
    fptr = ____Q5____;    
    printf("\nBefore sorting:\n");
    PrintArray(arr, len);
    BubbleSort2(arr, len, fptr);
    // in descending order
    printf("After sorting:\n");
    PrintArray(arr, len);
    return 0;
}
#endif

int main(void) {    
    // Let the C compiler determine the number of array elements for us.
    int arr[] = {30, 50, 20, 10, 60, 40};
    // calculate the number of elements
    int len = sizeof(arr) / sizeof(arr[0]);
    
    printf("Before sorting:\n");
    PrintArray(arr, len);
    BubbleSort(arr, len);
    printf("After sorting:\n");
    PrintArray(arr, len);

    printf("\n\nWhy Swap2(i, j) doesn't work?\n\n");
    int i = 20, j = 24;
    printf("i = %d, j = %d\n\n", i, j);
    printf("\nSwap2(i, j)\n");
    // The values of i and j are passed.
    Swap2(i, j);
    printf("i = %d, j = %d\n\n", i, j);

    printf("After completing the code in Q1-Q5 (BubbleSort.c), please also uncomment line %d in %s:\n\n", (__LINE__ + 1), __FILE__);
    //TestBubbleSort2();
    return 0;
}

