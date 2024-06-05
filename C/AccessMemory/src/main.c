/****************************************************************************************
                How to access memory in C

    1.  Types in C

            Primary Types:  
                        char, short, int, long, double, float, ...

            Aggregate Types:  
                        array, struct
                        
            Pointer Types:
                        T *,  where T is a primary/aggregate/pointer type

                        Examples:
                            char *, int *, struct Date *, char **, 


        We don't discuss function pointer in this program.

    2.   Access memory via variable names (for accessing global/local variables)

         For simplicity, we only take global variables as an example in this program.
         But global/local memory areas can be accessed similarly via variable names.
         

    3.   Access memory via pointer variables (for accessing global/local/heap variables)
         
         The object allocated on heap (via malloc()) does not have any name.
         But the pointer variable 'ptr', which points to the heap object, has a name.
         
         void test(void) {
            int *ptr = (int *) malloc(sizeof(int));
            ...
         }

         
         Memory Layout:

                        --------------
         ptr ------>     heap object
                        --------------
                           int

        Again, we only access the global memory area via pointer variables in this program.
        But global/local/heap memory areas can be accessed similarly via pointer variables.

    4.   Low-level pointer arithmetic via MemoryRead64() and MemoryWrite64()

         Low-level pointer arithmetic is essentially integer arithmetic,  
         with the challenge lying in the type of the pointer (T *)    
         and the size of T, determined by sizeof(T). 
                           
    5.  Tested on a 64-bit system (Intel x86_64).


                                                                    COMP9024 24T2

 ******************************************************************************************/

#include <stdio.h>     // printf(), scanf()
#include <string.h>    // strlen()
#include <stddef.h>    // offsetof()
#include "MemoryRW.h"

#define N    4

/*************************************************************
    Primary types: 
        char, short, int, long, double, float, ...
    Var: 
        variable
 *************************************************************/
char cVar = 'A';
short sVar = 28;
int iVar = 5;
long lVar = 2024; 
double dfVar = 3.14;

/*
    Pointer variables which point to primary types

    1. Pointer operator:

        In C, the asterisk operator, *, is used to declare and manipulate pointers. 

        It is also used as a multiplication operator in "30 * 40"

    2. Address operator:
    
        &:  the address operator in C.  
        It is denoted as the ampersand symbol, &.

        It is also used as a Bitwise AND operator in "2 & 4"

Memory Layout:
                    ---------
    pCharVar ---->   'A'
                    ---------
                    char cVar

                    ----------
    pShortVar --->    28
                    ----------
                    short sVar

                    ---------
    pIntVar ----->    5
                    ---------
                    int iVar

                    ---------
    pLongVar ---->    2024
                    ---------
                    long lVar

                    ------------
    pDoubleVar -->    3.14
                    ------------
                    double dfVar
 */ 
char *pCharVar = &cVar;
short *pShortVar = &sVar;
int *pIntVar = &iVar;
long *pLongVar = &lVar;
double *pDoubleVar = &dfVar;

/*************************************************************
Aggregate types: 
    struct, array
 *************************************************************/
/*
                    array 

    Let us start with one-dimensional arrays.

    #define N    4

    Memory Layout:
            --------------------------
             Access Name     Value
            --------------------------
             arr[3]           40
             arr[2]           30
             arr[1]           20
             arr[0]           10
            ------------------------
                  int arr[4]



 */
int arr[N] = {10, 20, 30, 40};

/*
                struct

    We assume: sizeof(int) == 4,  sizeof(long) == 8

     ------------------------
     FieldName  FieldOffset  
     ------------------------
     day         0           
     month       4           
     year        8           
     ------------------------
       struct Date
     
 */
struct Date {
    int day;
    int month;
    long year;
};
struct Date date = {28, 5, 2024};


/*************************************************************
   We assume: sizeof(int) == 4,  sizeof(long) == 8
   
   Memory Layout:
                    -------------
                      arr[3]   
                      arr[2]
                      arr[1]
    pArr -------->    arr[0]  
                    -------------
                     int arr[4]


                    --------------------------------
                    FieldName  FieldOffset   Value
                    --------------------------------
   pDate ----->     day         0            28
                    month       4            5
                    year        8            2024
                    --------------------------------
                        struct Date date
 *************************************************************/
// pointer variables
int *pArr = &arr[0];
struct Date *pDate = &date;



static void AccessMemoryByVariableNames(void) {
    printf("\n\n*************** AccessMemoryByVariableNames() ***************\n\n\n");
    // access primary types
    printf("cVar == %c \n", cVar);
    printf("sVar == %hd \n", sVar);
    printf("iVar == %d \n", iVar);
    printf("lVar == %ld \n", lVar);  
    printf("dfVar == %f \n\n", dfVar);

    // access an array
    int len = (sizeof(arr) / sizeof(arr[0]));
    for (int i = 0; i < len; i++) {
        printf("arr[%d] == %d\n", i, arr[i]);
        // *(arr+i) is equivalent to arr[i], where arr is an array.
        // arr in "*(arr + i)" is treated as &arr[0]
        printf("*(arr + %d) == %d\n\n", i, *(arr + i));
    }
    printf("\n");

    // access each field of the struct
    printf("date.day == %d \n", date.day);
    printf("date.month == %d \n", date.month);
    printf("date.year == %ld \n\n", date.year);

#if 0
    printf("pCharVar == %p, &cVar == %p\n", pCharVar, &cVar);
    printf("pShortVar == %p, &sVar == %p\n", pShortVar, &sVar);
    printf("pIntVar == %p,  &iVar == %p\n", pIntVar, &iVar);
    printf("pLongVar == %p, &lVar == %p\n", pLongVar, &lVar);
    printf("pDoubleVar == %p, &lVar == %p\n", pDoubleVar, &dfVar);
    printf("pArr == %p, &arr[0] == %p\n", pArr, &arr[0]);
    printf("pDate == %p, &date == %p\n\n", pDate, &date);
#endif    

    printf("\n\n***** Low-level pointer arithmetic in accessing arr[i] and *(arr + i), where arr is an array *******\n\n\n");
    /*
      -----------
        arr[3]   
        arr[2]
        arr[1]
        arr[0]
      -----------
       int arr[4]    
     */
    MM_INT_64 addrOfArr = (MM_INT_64) &arr[0];
    // arr[0]
    MM_INT_32 value = MemoryRead32(addrOfArr);
    printf("MM_INT_64 addrOfArr = (MM_INT_64) &arr[0];\n");
    printf("MM_INT_32 value = MemoryRead32(addrOfArr);\n");
    printf("value == arr[0] == %d\n", value);  

    //
    printf("\n\n***** Low-level pointer arithmetic in accessing date.day, date.month, and date.year *******\n\n\n");

    MM_INT_64 addrOfDate = (MM_INT_64)(&date);
    printf("MM_INT_64 addrOfDate = (MM_INT_64)(&date);\n\n");

    MM_INT_64 offset;
    // date.day is at offset 0
    offset = offsetof(struct Date, day);
    // addrOfDate + 0
    int day = MemoryRead32(addrOfDate + offset);  
    printf("date.day: MemoryRead32(addrOfDate + %ld) == %d \n", offset, day);
    // addrOfDate + 4
    offset = offsetof(struct Date, month);
    int month = MemoryRead32(addrOfDate + offset);
    printf("date.month: MemoryRead32(addrOfDate + %ld) == %d \n", offset, month);
    // addrOfDate + 8
    offset = offsetof(struct Date, year);
    long year = MemoryRead64(addrOfDate + offset);
    printf("date.year: MemoryRead64(addrOfDate + %ld) == %ld \n", offset, year);
}

static void AccessMemoryByPointerVariables(void) {
    printf("\n\n*************** AccessMemoryByPointerVariables() ***************\n\n\n");
    // access primary types
    printf("cVar == *pCharVar == %c \n", *pCharVar);
    printf("sVar == *pShortVar == %hd \n", *pShortVar);
    printf("iVar == *pIntVar == %d \n", *pIntVar);
    printf("lVar == *pLongVar == %ld \n", *pLongVar);  
    printf("dfVar == *pDoubleVar == %f \n\n", *pDoubleVar);

    // access an array
    for (int i = 0; i < N; i++) {
        // pArr[i] is equivalent to *(pArr + i), where pArr is a pointer variable.
        printf("pArr[%d] == %d\n", i, pArr[i]);
        printf("*(pArr + %d) == %d\n\n", i, *(pArr + i));
    }
    printf("\n");

    // access each field of the struct
    printf("pDate->day == %d \n", pDate->day);
    printf("pDate->month == %d \n", pDate->month);
    printf("pDate->year == %ld \n\n", pDate->year); 

    printf("\n\n***** Low-level pointer arithmetic in accessing *(pArr + i) and pArr[i], where pArr is a pointer variable *******\n\n\n");
    /*
                    -----------
                      arr[3]   
                      arr[2]
                      arr[1]
    pArr -------->    arr[0]
                    -----------
                     int arr[4]    
     */
    MM_INT_64 addrOfpArr = (MM_INT_64) (&pArr);
    // Read the value of 'pArr', i.e., the address of 'arr'
    MM_INT_64 addrOfArr = MemoryRead64(addrOfpArr);
    // arr[0]
    MM_INT_32 value = MemoryRead32(addrOfArr);
    printf("MM_INT_64 addrOfpArr = (MM_INT_64) (&pArr);\n");
    printf("MM_INT_64 addrOfArr = MemoryRead64(addrOfpArr);\n");
    printf("MM_INT_32 value = MemoryRead32(addrOfArr);\n");
    printf("value == arr[0] == %d\n", value);


    printf("\n\n***** Low-level pointer arithmetic in accessing pDate->day, pDate->month and pDate->year *******\n\n\n");
    /*
                         --------
            pDate ---->   day
                          month
                          year
                         --------
                     struct Date date;
     */
    MM_INT_64 addrOfpDate = (MM_INT_64) (&pDate);
    // Read the value of pDate from memory, i.e., the address of 'struct Date date'    
    MM_INT_64 addrOfDate = MemoryRead64(addrOfpDate);
    printf("MM_INT_64 addrOfpDate = (MM_INT_64) (&pDate);\n");
    printf("MM_INT_64 addrOfDate = MemoryRead64(addrOfpDate);\n\n");

    MM_INT_64 offset;
    // date.day is at offset 0
    offset = offsetof(struct Date, day);
    // addrOfDate + 0
    int day = MemoryRead32(addrOfDate + offset);  
    printf("pDate->day: MemoryRead32(addrOfDate + %ld) == %d \n", offset, day);
    // addrOfDate + 4
    offset = offsetof(struct Date, month);
    int month = MemoryRead32(addrOfDate + offset);
    printf("pDate->month: MemoryRead32(addrOfDate + %ld) == %d \n", offset, month);
    // addrOfDate + 8
    offset = offsetof(struct Date, year);
    long year = MemoryRead64(addrOfDate + offset);
    printf("pDate->year: MemoryRead64(addrOfDate + %ld) == %ld \n", offset, year);
}


static void TestCString(void) {
    printf("\n\n*************** TestCString() ***************\n\n\n");
    /*
    In C, strings (character arrays) are terminated by null character '\0' character with value zero. 

    https://www.asciitable.com/
    In ASCII, the NUL control code has value 0 (0x00) 
    */    
    // str is a pointer, which points to the first character of "Hello 9024"
    char *str = "Hello 9024";
    // visit each character in "Hello 9024"
    char *cptr = str;
    while (*cptr != 0) {
        printf("%c", *cptr);
        cptr++;
    }
    printf("\n");
    printf("\"%s\", sizeof(str) == %ld, strlen(str) == %ld \n", 
            str, sizeof(str), strlen(str));

    // character arrays
    char charBuf1[] = "CSE@UNSW"; // 
    char charBuf2[] = {'C', 'S', 'E', '@', 'U', 'N', 'S', 'W', '\0'};
    printf("%s, sizeof(charBuf1) == %ld, strlen(charBuf1) == %ld \n",
            charBuf1, sizeof(charBuf1), strlen(charBuf1));
    printf("%s, sizeof(charBuf2) == %ld, strlen(charBuf2) == %ld  \n\n",
            charBuf2, sizeof(charBuf2), strlen(charBuf2));
}

static void TestMemoryWrite(void) {
    printf("\n\n*************** TestMemoryWrite(): only for explaining low-level pointer arithmetic ***************\n\n\n");
    MM_INT_64 addr;

    addr = (MM_INT_64) &cVar;
    MemoryWrite8(addr, 'B');
    printf("cVar = %c\n", cVar);

    addr = (MM_INT_64) &sVar;
    MemoryWrite16(addr, 29);
    printf("sVar = %hd\n", sVar);

    addr = (MM_INT_64) &iVar;
    MemoryWrite32(addr, 6);
    printf("iVar = %d\n", iVar);       

    addr = (MM_INT_64) &lVar;
    MemoryWrite64(addr, 2025);
    printf("lVar = %ld\n", lVar); 
}

int main(void) {
    printf("Hello World\n");
    
    AccessMemoryByVariableNames();
    AccessMemoryByPointerVariables();
    TestMemoryWrite();
    TestCString();
    return 0;
}

