/*
    A Linux Process's memory layout (userspace)

     |_____________________|
     |  command line args  |
     |        and          |
     |environment variables|
     |_____________________| 
     |                     | Call Stack
     |                     |  
     |                     |  
     |.....................| 
     |                     |
     |                     |  
     |                     |   
     |                     |
     |                     |  Heap memory area (malloc() and free())
     |                     |
     |                     |  
     |                     |   
     |_____________________| 
     |                     |
     |                     |  Global memory area
     |                     |  
     |_____________________|  
     |                     |
     |                     |  Code Area
     |_____________________| 
     |                     |


                                    COMP9024 24T2
 */
#include <stdio.h>    // for printf()
#include <stdlib.h>   // for malloc() and free()


// global variable, visible in all *.c files
long year = 2024;

// The default value of a global/static variable is 0
// The static variable 'number' is visible in the current C file
static int number;

// "CSE@UNSW" is a constant string.
// cptr is a pointer which points to the first character of "CSE@UNSW"
char *cptr ="CSE@UNSW";

// Define a function pointer type, which points to a function whose type is 'int (void)'.
typedef int (*FuncPtrTy)(void);

// visible in all *.c files
int test1(void) {
    printf("test1()\n");
    return 2024;
}

// visible in the current *.c file containing this function
static int test2(void) {
    // 'count' is only visible in f()
    static int count = 0;            
    // How many times test2() has been called
    count++;

    printf("\ntest2() has been called %d times\n", count);
    return 2025;
}

// Parameters and Local variables in C
static void g(int n){
    int x;
    int *ptr = &n;
    // ...
    x = *ptr;
    printf("x = %d\n", x);
 }

void TestMemoryLayout(void){
    /*
                    --------
        pLong ---->   2024 
                    --------
                    heap-allocated variable (unnamed)
     */
    long *pLong = (long *) malloc(sizeof(long));
    *pLong = 2024;
    free(pLong);
    // 
    FuncPtrTy fptr = &test2;
    fptr();
    // Both '&test2' and 'test2' represent the function address of test2
    fptr = test2;
    fptr();

    g(2024);
}

