# Format String in C

``` sh
/*******************************************************************
                Format String in C

    1.  How does a variadic function in C work
        (see OurPrintf_V3(const char *fmt, ...) in OurPrintf32.c)

    2.  The call stack memory layout when calling a variadic function on a 32-bit system

    3.  How to use va_start() and va_arg() to access the unnamed parameters of a variadic function
        (see OurPrintf_V1(const char *fmt, ...) in OurPrintf32.c)

                                             COMP9024 24T2

 *******************************************************************/
``` 

## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Strings/FormatString

FormatString$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

FormatString$ code

```

Two configuration files (FormatString/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and FormatString/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Strings/FormatString", then click the "Open" button.


### 2.2 Build the project in VS Code

**We assume the 32-bit system is available.**

Please click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/OurPrintf32.c, and click to add a breakpoint (say, line 73).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this project
|
├── src                  containing *.c and *.h
|   |
│   └── OurPrintf32.c
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

## 3 Introduction

If necessary, please install gcc-multilib and g++-multilib as follows in your own computers.

```sh
$ sudo apt-get install gcc-multilib g++-multilib
```


**CSE has installed gcc-multilib in [VLAB](https://vlabgateway.cse.unsw.edu.au/).**

**The 32-bit sub-system in [VLAB](https://vlabgateway.cse.unsw.edu.au/) works, although there is a linking error (ignored), due to broken dependencies.**


### 3.1 on a 64-bit system

How to build 
```sh
FormatString$ make CFLAGS="-g"
```

How to run
```sh
FormatString$ ./main

a = 30, b = 40, c = 50, d = 3.140000 
OurPrintf_V1(): a = 30, b = 40, c = 50, d = 3.140000 
OurPrintf_V2(): a = 30, b = 40, c = 50, d = 3.140000 

```

How to clean
```sh
FormatString$ make clean
```

### 3.2 on a 32-bit system

How to build 
```sh
FormatString$ make CFLAGS="-g -D ON_A_32_BIT_SYSTEM -m32"
```

How to run
```sh
FormatString$ ./main

a = 30, b = 40, c = 50, d = 3.140000 
OurPrintf_V1(): a = 30, b = 40, c = 50, d = 3.140000 
OurPrintf_V2(): a = 30, b = 40, c = 50, d = 3.140000 
OurPrintf_V3(): a = 30, b = 40, c = 50, d = 3.140000 

```

How to clean
```sh
FormatString$ make clean
```

## 4 Variadic functions on a 32-bit system

### 4.1 OurPrintf32_V3()

```C

/*
    In OurPrintf32_V3(), we do the pointer arithmetic by ourselves on a 32-bit system.
    It is not portable.
    But it can help us get a big picture of what is going on under the hood.   
 */
void OurPrintf32_V3(char *fmt, ...) {
    char *ap;                                   // similar with but not necessarily equal to 'va_list ap';
    ap = ((char *) &fmt) + sizeof(char *);      // similar with va_start(ap, fmt);
    printf("OurPrintf_V3(): ");
    // Now ap points to the unnamed parameters.               
    while (*fmt) {
        if (*fmt == '%') { // %: This is the leading sign that denotes the beginning of the format specifier
            fmt++;
            if (*fmt == 'd') {
                int ival = *((int *) ap);        // similar with va_arg(ap, int)
                ap += sizeof(int);

                printf("%d", ival);
                // flush the buffered data, for debugging
                fflush(stdout);
                fmt++;
            }
            else if (*fmt == 'f') {  
                double fval = *((double *) ap);  // similar with va_arg(ap, double)
                ap += sizeof(double);
                printf("%f", fval);
                fflush(stdout);
                fmt++;
            }
            else {
                //...
            }
        }
        else {  // regular character
            printf("%c", *fmt);
            fflush(stdout);
            fmt++;
        }
    }
    ap = NULL;                                  // similar with va_end(ap)
}

int main(void) {
    int a = 30;
    int b = 40;
    int c = 50;
    double d = 3.14;
    ....
    OurPrintf32_V3("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
    return 0;
}

################################################################################


      High Address
                      __|_________________________|__  
                        |                         |    
                        |                         | 
                    a   |      30                 |  
                    b   |      40                 |  
                    c   |      50                 |   
                    d   |      3.14               |           
                        |                         |
         main()         |                         |         
                        |                         |  
                        |                         |
                        |         3.14            |                  "a = %d, b = %d, c = %d, d = %f \n"
                        |          50             |                   ^
                        |          40             |                   +
         +------------->|          30             |                   +         
         +              |          fmt            |-------------------+  
         +            __|_______return address____|__   
         +              |                         |                       Global Data Area
         ---------------|          ap             |  
                        |                         |        
      OurPrintf32_V3()  |                         | 
                        |_________________________|__
                        |                         | 

                              Call Stack

    
      Low Address
                                                       

```


### 4.2 OurPrintf_V1() and OurPrintf_V2()

```C

/*
    Use va_start() and va_arg() to access the unnamed parameters of a variadic function in C.

    int printf(const char *format, ...);
 */
void OurPrintf_V1(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("OurPrintf_V1(): ");
    while (*fmt) {
        if (*fmt == '%') { // %: This is the leading sign that denotes the beginning of the format specifier
            fmt++;
            if (*fmt == 'd') {
                int val = va_arg(ap, int);
                printf("%d", val);
                // flush the buffered data, for debugging
                fflush(stdout);
                fmt++;
            }
            else if (*fmt == 'f') {
                double val = va_arg(ap, double);
                printf("%f", val);
                fflush(stdout);
                fmt++;
            }
            else {
                //...
            }
        }
        else {  // regular character
            printf("%c", *fmt);
            fflush(stdout);
            fmt++;
        }
    }
    va_end(ap);
}


/*
    Pass a va_list to vprintf()

        int vprintf(const char *format, va_list ap);

 */
void OurPrintf_V2(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    printf("OurPrintf_V2(): ");
    vprintf(fmt, ap);
    va_end(ap);
}
```

## 5 Variadic functions on a 64-bit system 

Since some function arguments are passed via physical registers 
([System V AMD64 ABI](https://en.wikipedia.org/wiki/X86_calling_conventions))
in AMD64 or Intel's CPU x86_64,

the memory layout of a variadic function on a 64-bit system is much more complex.

**OurPrintf32_V3()** is only used to show what's going on under the hood on a 32-bit system. 
It is NOT portable.

The high-level usage of **va_list**, **va_start**, **va_arg**, and **va_end** remains consistent across both 32-bit and 64-bit systems.

If we understand **OurPrintf32_V3()** and the **Call Stack**, a variadic function can be much easier to us.

The two functions **OurPrintf_V1()** and **OurPrintf_V2()** above work on both 32-bit and 64-bit systems.




