# SCC:  How data structures and algorithms can be used to create a "Simple C Compiler"

```sh
# ***********************************************************************************#
#                                                                                    #
# 1. For better understanding of C, recursion, call stack, tree and linked list.     #
#                                                                                    #
# 2. No code optimization.                                                           #
#                                                                                    #
#                                                                                    #
#                                                           COMP9024 24T2            #
# ***********************************************************************************#
```

## 1 How to download SCC in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/LargeAssignment

LargeAssignment$ 

```


## 2 How to build/run a test case in the directory *tests*


```sh

LargeAssignment$ make

LargeAssignment$ make compileOne SCC_SRC_FILE=tests/Factorial.scc


// If your computer has an Intel/AMD x86_64 CPU
// (it is available in CSE VLAB)

LargeAssignment$ make runOne SCC_SRC_FILE=tests/Factorial.scc

LargeAssignment$ cat tests/Factorial.scc.s

```

By reading the small source file (tests/Factorial.scc) and its assembly file (generated in tests/Factorial.scc.s),

we can get a brief overview of SCC.

More details will be discussed in classes.


## 3 How to build/run all test cases in the directory *tests*



```sh

LargeAssignment$ make

LargeAssignment$ make compileAll

// If your computer has an Intel/AMD x86_64 CPU
// (it is available in CSE VLAB)

LargeAssignment$ make runAll


```


## 4 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug our LargeAssignment.


```sh

$ code

```

Two configuration files (LargeAssignment/.vscode/launch.json and LargeAssignment/.vscode/tasks.json) have been preset.

In launch.json, "tests/Factorial.scc" is used as the default input file for SCC.


#### 4.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/LargeAssignment", then click the "Open" button.


#### 4.2 Build the project in VS Code

click **Terminal -> Run Build Task**

#### 4.3 Debug the project in VS Code

First, please open src/main.c, and click to add a breakpoint in line 150.

Then, click **Run -> Start Debugging**




## 5 Directory

```C

LargeAssignment
|
├── libs                                   // The library of SCC
│   ├── SccHeap.c                          // Our heap allocator
│   ├── SccHeap.h
│   ├── SccLib.c                           // Library functions such as SccLess(), SccLogicAnd(), SccRead64() and SccWrite64()
│   ├── SccLib.h
│   └── TestSccHeap.c                      // For testing our heap allocator
├── Makefile                               // Makefile for our large assignment
├── README.md
├── src                                    // The source code of SCC
│   ├── decl.c                             // Declaration, only supporting 'long'
│   ├── decl.h                             
│   ├── emit.c                             // For emitting output after visiting the tree
│   ├── emit.h
│   ├── error.c                            // Generate some diagnostic messages when compiling a test case in 'tests'
│   ├── error.h
│   ├── expr.c                             // Expression in SCC:  +, -, *, / and function calls.
│   ├── expr.h                             // Other expressions (>, >=, !, ...) are implemented as library functions in libs/Scclib.c 
│   ├── func.c                             // Function in SCC
│   ├── func.h
│   ├── lex.c                              // Lexical analysis for recognizing tokens (words) in the input file.
│   ├── lex.h
│   ├── main.c                             // main()
│   ├── stmt.c                             // Statement in SCC:  if, while, output(id), input(id), return, ...
│   ├── stmt.h
│   └── tokens.txt                         // Tokens' kind and name
└── tests                                  // Test cases for SCC
    ├── Associativity.scc                  // For testing left/right associativity
    ├── BinaryTree.scc                     // A binary tree implemented in SCC
    ├── DoWhile.scc                        // For testing do-while 
    ├── EnvVars.scc                        // For outputting command-line arguments and environment variables.
    ├── Factorial.scc                      // The default test case 
    ├── Makefile                           // For cleaning the *.s and *.exe generated
    ├── MultipleArguments.scc              // For testing multiple arguments
    ├── Stack.scc                          // A data stack implemented in SCC
    └── SystemPython3.scc                  // Call system("python3") in SCC

```

**Please complete the code in Q1-Q10 (libs/SccHeap.c) and Q11-Q20 (src/stmt.c)**.

Smaller projects such as [COMP9024/Trees/BiTree](../Trees/BiTree/README.md), [COMP9024/Tutorials/Week4](../Tutorials/Week4/README.md),
and [COMP9024/HeapAllocator](../HeapAllocator/README.md) are introduced to help you comprehend the large assignment step by step.

## 6 How to test our heap allocator (based on a Singly Linked List) 

**After completing the code (Q1-Q10) in libs/SccHeap.c**, we can run the following commands in VLAB

```sh

LargeAssignment$ make testHeap

```
													
If everything goes well, we will see the output (the status of the linked list) as follows.

A node (i.e., a free heap memory block) in the linked list is represented as **[blockAddress, blockSize]**.

For example, in [0x564728956040,4194296], 0x564728956040 is the address of the memory block, 
and 4194296 is the usable size of the memory block.

See **struct HeapMemBlock** in libs/SccHeap.c for more details.

The address (e.g., 0x564728956040) might differ due to [Address Space Layout Randomization (ASLR)](https://pax.grsecurity.net/docs/aslr.txt).


					
```C

    [0x564728956040,4194296]

    ************** SccMalloc() *****************

    void * ptr16 = SccMalloc(16)
    [0x564728956040,4194272]

    void * ptr32 = SccMalloc(32)
    [0x564728956040,4194232]

    void * ptr48 = SccMalloc(48)
    [0x564728956040,4194176]

    void * ptr64 = SccMalloc(64)
    [0x564728956040,4194104]

    void * ptr80 = SccMalloc(80)
    [0x564728956040,4194016]

    ************** SccFree() *****************

    SccFree(ptr32)
    [0x564728956040,4194016]-->[0x564728d56000,32]

    SccFree(ptr64)
    [0x564728956040,4194016]-->[0x564728d55f80,64]-->[0x564728d56000,32]

    SccFree(ptr16)
    [0x564728956040,4194016]-->[0x564728d55f80,64]-->[0x564728d56000,56]

    SccFree(ptr48)
    [0x564728956040,4194016]-->[0x564728d55f80,184]

    SccFree(ptr80)
    [0x564728956040,4194296]

```


## 7 How to test our do-while statement (based on a Tree) 

**After completing the code (Q11-Q20) in the function DoWhileStatement() in src/stmt.c** , 

we can run the following commands in VLAB.

```sh

LargeAssignment$ make compileOne SCC_SRC_FILE=tests/DoWhile.scc

LargeAssignment$ make runOne SCC_SRC_FILE=tests/DoWhile.scc

```

If everything goes well, we will see the output (i.e., command-line arguments and environment variables) as follows.

See tests/DoWhile.scc and tests/EnvVars.scc for more details.


```C
1
tests/DoWhile.scc.exe

GNOME_SHELL_SESSION_MODE=ubuntu
SSH_AGENT_LAUNCHER=gnome-keyring
QT_ACCESSIBILITY=1
...
XDG_MENU_PREFIX=gnome-
GDMSESSION=ubuntu
MAKELEVEL=1

```

## 8 Assessed online via Moodle

Please complete the code in Q1-Q20 and then answer questions (more than 20) in Large Assignment (Week 10) on [Moodle](https://moodle.telt.unsw.edu.au/my/courses.php).
