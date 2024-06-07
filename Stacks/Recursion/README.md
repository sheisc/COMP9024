# Recursion

``` sh
/****************************************************************************************
                   Recursion and Call Stack

    1.  Observe the stack frames when calling a recursive function

    2.  Different versions/instances of the local variable 'n' in Factorial(n)

    3.  A Linux Process's memory layout (userspace)

                                                                    COMP9024 24T2

 ******************************************************************************************/

``` 

## Recursive functions



Recursive functions are functions that call themselves directly or indirectly within their definitions. 

They are a powerful tool in programming, particularly for tasks that exhibit repetitive or self-similar structures. 

However, when using recursion, it's important to understand how it interacts with the call stack.


## Call Stack

Note that a call stack is a stack data structure (Last-In-First-Out) that stores information about the active functions of a running program, 

which is supported by the operating system and CPU (or by an [interpreter](https://github.com/rocky/x-python/)).

Interestingly, recursive functions depend on the call stack.

The call stack also operates on a Last-In-First-Out principle. 

**This implies that the function most recently invoked will be the first one to be removed from the call stack.**


## Stack Frame

Each time a function is called, a new stack frame (containing the return address, local variables, parameters, and temporary variables) 

is created and pushed onto the call stack. 


## A Linux Process's memory layout (userspace)


A process can be thought of as an instance of a running program. 

When you start a program on your computer, the operating system creates a process to manage that program's execution. 

This process contains various resources, including memory, CPU time, and other system resources, 

which are allocated to the program as it runs.

**Due to dynamic linking, the actual layout might be more complex than the following figure.**


``` sh
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

    A Linux Process's memory layout (userspace)

```

An executing program often partitions memory into:

```sh
Code area
    contains the machine code instructions for the program

    In C:
    after compiling, the machine code of functions will be put in the code area.
    
    // visible in all *.c files
    int test1(void) {
        return 2024;
    }

    // visible in the current *.c file containing this function
    static int test2(void) {
        return 2025;
    }

Data area

    Global memory area
        contains global/static variables and constant strings.        
        Its size is often determined by the compiler and linker when building the program, not at run time.

        global/static variables in C:

        Their lifetime extends throughout the entire execution of the program.

        // global variable, visible in all *.c files
        long year = 2024;

        // The default value of a global/static variable is 0
        // The static variable 'number' is visible in the current C file
        static int number;

        // "CSE@UNSW" is a constant string.
        // cptr is a pointer which points to the first character of "CSE@UNSW"
        char *cptr ="CSE@UNSW";

        void f(void){
            // 'count' is only visible in f()
            static int count = 0;            
            // How many times f() has been called
            count++;
        }


    Heap memory area 
        dynamically allocated via malloc()/free(), 
        C programers are responsible for calling malloc() and free().

        Python has automatic garbage collection for managing its heap memory.
        The main garbage collection mechanism in CPython is through reference counts.

        Variables allocated on the heap have a dynamic lifetime, controlled by the programmer.
        A heap-allocated variable itself doesn't have any name,
        so it should be accessed via a pointer variable.

        In C:

        long *pLong = (long *) malloc(sizeof(long));
        *pLong = 2024;

        Memory Layout:
                       --------
        pLong ---->      2024 
                       --------
                     heap-allocated variable (unnamed)

    Call stack 
        dynamically-allocated due to function calls/returns.
        The push operation (creating a new frame) is caused by a function call (generated by the compiler),
        while its pop operation (removing a frame) is caused by a function return (generated by the compiler).
        It consists of stack frames (First-In-Last-Out), one for each called function,
        Each frame might contain the return address, local variables, parameters, and temporary variables.

        Since different functions can have different sets of local variables, their stack frames vary in size.

        Local variables are declared within a function and are only accessible within that function's scope.
        Their lifetime is limited to the duration of the function call in which they are declared.

        Parameters and local variables in C:

        static void g(int n){
            int x;
            int *ptr = &n;
            // ...
        }


```


**Call stack** is studied in [Stacks/CallStack](../CallStack/README.md) and [Stacks/Recursion](./README.md), 
and maintained in [lines 553-566 in LargeAssignment/src/stmt.c](../../LargeAssignment/src/stmt.c), [lines 498-581 in LargeAssignment/src/expr.c](../../LargeAssignment/src/expr.c), and [lines 142-161 and 284-288 in LargeAssignment/src/emit.c](../../LargeAssignment/src/emit.c).

**Heap allocator** is customized in  [OurMalloc() and OurFree() of our large assignment](../../LargeAssignment/libs/SccHeap.c).

**Global variables** are generated in [EmitGlobalDeclarationNode() of our large assignment](../../LargeAssignment/src/decl.c).

**Code** is generated in [EmitFuncDefNode() of our large assignment](../../LargeAssignment/src/func.c).

**The command line arguments and environment variables** are accessed in [LargeAssignment/tests/EnvVars.scc](../../LargeAssignment/tests/EnvVars.scc) and [C/EnvVars](../../C/EnvVars/README.md).



## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Stacks/Recursion

Recursion$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

Recursion$ code

```

Two configuration files (Recursion/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and Recursion/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Stacks/Recursion", then click the "Open" button.


### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 32).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
├── src                  containing *.c and *.h
|   |
│   |── main.c           containing a recursive function, long Factorial(long n)
|
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


## 3 Build and run the project in command line

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

The address (e.g., 0x7fff73cf8a28) might differ due to [Address Space Layout Randomization (ASLR)](https://pax.grsecurity.net/docs/aslr.txt).

``` sh

Recursion$ make

Recursion$ ./main

main():      n = 4, &n = 0x7fff73cf8a28
Factorial(): n = 4, &n = 0x7fff73cf89f8
Factorial(): n = 3, &n = 0x7fff73cf89c8
Factorial(): n = 2, &n = 0x7fff73cf8998
Factorial(): n = 1, &n = 0x7fff73cf8968

Factorial(4) = 24


```

## 4 The Call Stack Memory Layout (ignoring return addresses and other information)
```C
long Factorial(long n) {
    printf("Factorial(): n = %ld, &n = %p\n", n, &n);
    if (n <= 1) {
        // the base case for the recursive function
        return 1;
    } else {
        long result = Factorial(n - 1);
        result = result * n;
        return result;
    }
}

int main(void) {
    long n = 4;
    printf("main(): n = %ld, &n = %p\n", n, &n);
    long x = Factorial(n);
    printf("\nFactorial(%ld) = %ld\n", n, x);    
    return 0;
}
```
```sh
 High Address
              |             | 
              |_____________|
              |             |
          n   |     4       |  
              |             |  main()'s stack frame  
          x   |             |
              |_____________| 
              |             |
          n   |     4       |  
              |             |  Factorial(4)'s stack frame  
       result |             |
              |_____________| 
              |             |
          n   |     3       |  
              |             |  Factorial(3)'s stack frame  
       result |             |
              |_____________| 
              |             |
          n   |     2       |  
              |             |  Factorial(2)'s stack frame  
       result |             |
              |_____________| 
              |             |
          n   |     1       |  
              |             |  Factorial(1)'s stack frame  
              |             |              
              |_____________| 
              |             |
              |             |  

                Call Stack
Low Address
```

## 5 A similar test case in [our large assignment](../../LargeAssignment/tests/Factorial.scc)

The following test case is not written in C, but with a simple language defined in our large assignment.

```C

// COMP9024/LargeAssignment/tests/Factorial.scc

long result;

// f(n) = 1 * 2 * ... * (n-1) * n
f(n) {
  if (SccLessEqual(n, 1)) { // n <= 1
    return 1;
  }
  else {
    return f(n-1) * n;
  }
}


main(argc, argv, env) {
  long i;
  
  i = 4;
  result = f(i);
  output(result);

  return 0;
}
```

### How to build and run the test case
```sh

LargeAssignment$ make

LargeAssignment$ make compileOne SCC_SRC_FILE=tests/Factorial.scc

./scc   < tests/Factorial.scc   > tests/Factorial.scc.s

    x86_64 assembly code generated in tests/Factorial.scc.s


LargeAssignment$ make runOne SCC_SRC_FILE=tests/Factorial.scc

24

```

[Factorial.scc.s](../Factorial.scc.s) is assembly file generated by [our large assignment](../../LargeAssignment/), 
**SCC**, a simple C-like compiler.

You can take a look at [Factorial.scc.s](../Factorial.scc.s).

The assembly code is clearly commented line by line.

If you want to know "The Stack Layout in SCC When A Function Is Called", please refer to [LargeAssignment/src/func.c](../../LargeAssignment/src/func.c).

The comments in [EmitAstExprNode() in LargeAssignment/src/expr.c](../../LargeAssignment/src/expr.c), and [EmitPrologue() and EmitEpilogue() in LargeAssignment/src/emit.c](../../LargeAssignment/src/emit.c) 

could help you understand the assembly code in [Factorial.scc.s](../Factorial.scc.s).

You can also refer to [Guide to x86-64](https://web.stanford.edu/class/cs107/guide/x86-64.html).

We will discuss the details in the coming weeks.


