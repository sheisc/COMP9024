/********************************************************************
 Include Guard
   sometimes also called a macro guard, header guard
   https://en.wikipedia.org/wiki/Include_guard
   
   (1) It is used to avoid the problem of double inclusion.

   For example, in main.c

    #include "myadd.h"  
    #include "mysub.h"
    #include "main.h"
   
   In main.h
    #include "myadd.h"

   Therefore, "myadd.h" will be included in main.c twice.

   (2) What will happen if we delete this header guard (i.e., MYADD_H)?

   Without the include guard in myadd.h (i.e., MYADD_H), we will get an error as follows:

    HowToMake$ make

    In file included from src/main.h:4,
                    from src/main.c:4:
    src/myadd.h:45:8: error: redefinition of ‘struct BiTree’
    45 | struct BiTree {
        |        ^~~~~~
    In file included from src/main.c:2:
    src/myadd.h:45:8: note: originally defined here
    45 | struct BiTree {
        |        ^~~~~~

   (3)
    Also see the header guard in the following header file (Python.h)
    https://github.com/python/cpython/blob/main/Include/Python.h
 
 ********************************************************************/
#ifndef MYADD_H
#define MYADD_H

int add(int a, int b);
// definitions of a type can be put in the header file
struct BiTree {
    struct BiTree *left;
    struct BiTree *right;
    long val;
};

#define PUT_DEFINITIONS_HERE  0

//  Don't put definitions of global variables and functions in a header file.
//  Put these definitions in a *.c file.
//  Their declarations can be put in a header file.
//
//  extern int term;      // declaration of a global variable
//  int GetYear(void);    // declaration of a global function
//
#if PUT_DEFINITIONS_HERE
int term = 2;

/*
    In C++, 'int GetYear() {return 2024;}' and 'int GetYear(void) {return 2024;}' are same.

    In C, they are different.
    (1)
    int GetYear1() {return 2024;}
        Any number of arguments can be passed while calling the GetYear1() function.
    (2)
    int GetYear2(void) {return 2024;}
        No argument can be passed when calling GetYear2().

    Advice:
        int GetYear2(void) {return 2024;}

    Refer to: Py_GetRecursionLimit(void) in CPython
    https://github.com/python/cpython/blob/main/Python/ceval.c
 */
int GetYear(void) {
    return 2024;
}
#endif

#endif
