# SCC:  How data structures and algorithms can be used to create a "Simple C Compiler"

```sh
# ***********************************************************************************#
#                                                                                    #
# 1. For better understanding of C, recursion, call stack, tree and linked list.     #
#                                                                                    #
# 2. No code optimization.                                                           #
#                                                                                    #
#                                                                                    #
#                                                           COMP9024                 #
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


## 7 From Expression to Statement and Function 

We have discussed how to create an abstract syntax tree for an expression in [COMP9024/Trees/Str2Ast](../Trees/Str2Ast/).

```sh
An expression (i.e., a string) from a user:

   "9000 + (6 * 4)" 

It is represented as an abstract syntax tree after parsing.

        + 
      /   \ 
    9000   * 
          /  \ 
         6    4 
```

### 7.1 Statement

```sh
Statement:
        IfStatement
        WhileStatement
        DoWhileStatement
        CompoundStatement
        ReturnStatement
        ...


IfStatement:
        if (Expression) Statement
        if (Expression) Statement else Statement        


ReturnStatement:
        return Expresion;

...
```

### 7.2 How to create an abstract syntax tree for a return statement

Example

```C
    return 0;
```





```sh
ReturnStatement:
    return Expresion;
```

Parser
```C
/* 
    ReturnStatement:
        return Expresion;
 */
AstStmtNodePtr ReturnStatement() {
    AstStmtNodePtr returnStmt = NULL;
    returnStmt = CreateStmtNode(TK_RETURN);
    Expect(TK_RETURN);
    returnStmt->expr = Expression();
    Expect(TK_SEMICOLON);
    return returnStmt;
}
```

### struct astStmtNode
```C
/*
    AST node for different statements.
    We use the field op to distinguish different statements.  
 */
struct astStmtNode {
    // The kind of a statement.
    // To keep it simple, we reuse the TokenKind defined in tokens.txt.
    TokenKind op;
    // The value got from lex.c
    //      name of id,  integer values, ...
    Value value;
    /////////////////////////////
    /*
    1. Used in IfStatement and WhileStatement for labels
        See the comments for IfStatement() in stmt.c.  
    2. Other statements might use these fields for other purposes.
        see ExpressionStatement()
    */
    struct astExprNode *kids[2];
    /*
    1. See IfStatement() in stmt.c
          if (expr)
            thenStmt
          else
            elseStmt
          
          nextStmt
     2. Other statements might reuse part of these fields.
        For example, WhileStatement only use expr, thenStmt, and next.
    */
    struct astExprNode *expr;
    struct astStmtNode *thenStmt;
    struct astStmtNode *elseStmt;
    struct astStmtNode *next;
};

typedef struct astStmtNode *AstStmtNodePtr;
```

### 7.3 How to create an abstract syntax tree for a while statement

Example

```C
// COMP9024/LargeAssignment/tests/EnvVars.scc

printStrs(strs) {
    long i;
    long s;
    i = 0;
    s = SccRead64(strs, i * 8);
    while (s) {
        puts(s);
        i = i + 1;
        s = SccRead64(strs, i * 8);
    }
}

main(argc, argv, env) {
    output(argc);
    // Display command-line arguments
    printStrs(argv);
    // '\n'
    putchar(10);
    // Display environment variables
    printStrs(env);
    return 0;
}
```

Parser
```C
/**********************************************
    WhileStatement:
        while (Expression) Statement
  
    Semantics:
  
          label_begin:         // saved in WhileStatement.kids[0]
  
                if(!Expression)  
                    goto	label_next
                
                Statement
                
                goto label_begin
  
          label_next:         //  saved in WhileStatement.kids[1]
                ...
 **********************************************/
static AstStmtNodePtr WhileStatement(void) {
    AstStmtNodePtr whileStmt = NULL;
    whileStmt = CreateStmtNode(TK_WHILE);

    whileStmt->kids[0] = CreateLabelNode();
    Expect(TK_WHILE);
    Expect(TK_LPAREN);
    whileStmt->expr = Expression();
    Expect(TK_RPAREN);
    whileStmt->thenStmt = Statement();
    // label for the statement after while
    whileStmt->kids[1] = CreateLabelNode();
    return whileStmt;
}
```

### 7.4 How to create an abstract syntax tree for a do-while statement

Example
```C

// COMP9024/LargeAssignment/tests/DoWhile.scc

printStrs(strs) {
    long i;
    long s;
    i = 0;
    // assume the first one is not NULL
    do {
        s = SccRead64(strs, i * 8);
        if (s) {
            puts(s);
        }
        i = i + 1;
    } while (s);
}

main(argc, argv, env) {
    output(argc);
    // Display command-line arguments
    printStrs(argv);
    // '\n'
    putchar(10);
    // Display environment variables
    printStrs(env);
    return 0;
}
```

Parser
```C
/**********************************************
    DoWhileStatement:
          do Statement while (Expression);
 
    Semantics:   
         label_begin:     // saved in DoWhileStatement.kids[0]
 
             Statement
 
             if(Expression)
                 goto label_begin
 
         label_next:     //  saved in DoWhileStatement.kids[1]
             ....
 **********************************************/
static AstStmtNodePtr DoWhileStatement(void) {
    AstStmtNodePtr doWhileStmt = NULL;

    /*
      Create a sub-tree for the do-while statement.
      It is similar with the code in WhileStatement().
  
      Q11. call CreateStmtNode(TK_DO) to create an AST node and save the return value in doWhileStmt
      Q12. call Expect() to match TK_DO in the input file
      Q13. call CreateLabelNode() to create an AST node for label_begin, and save the return value in doWhileStmt->kids[0]
      Q14. call Statement() to create an AST node for the Statement, and save the return value in doWhileStmt->thenStmt
      Q15. call Expect() to match TK_WHILE in the input file
      Q16. call Expect() to match TK_LPAREN in the input file
      Q17. call Expression() to create an AST node for the Expression in do-while, and save the return value in doWhileStmt->expr.
      Q18. call Expect() to match TK_RPAREN in the input file
      Q19. call Expect() to match TK_SEMICOLON in the input file
      Q20. call CreateLabelNode() to create an AST node for label_next, and save the return value in doWhileStmt->kids[1]     
     */
    ////////////////////////////////////////////////////////////////////////////////////
    //
    //    Please read the above comments first and then complete the following code (Q11-Q20).
    //
    ////////////////////////////////////////////////////////////////////////////////////

    // Q11. ___________________
    // Q12. ___________________
    // Q13. ___________________
    // Q14. ___________________
    // Q15. ___________________
    // Q16. ___________________
    // Q17. ___________________
    // Q18. ___________________
    // Q19. ___________________
    // Q20. ___________________

    return doWhileStmt;
}

```

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

### 7.5 How to create an abstract syntax tree for a compound statement

Example
```C
main(argc, argv, env) 
{
    // ...
    i = 4;
    result = f(i);
    output(result);

    return 0;
}
```
Parser
```C
/*
    CompoundStatement:
        {Statement  Statement   ...  Statement}
  
    Semantics:
        The list of statements is saved in CompoundStatement.next.
 */
AstStmtNodePtr CompoundStatement(void) {
    AstStmtNodePtr comStmt;
    AstStmtNodePtr *pStmt;
    Value value;

    comStmt = CreateStmtNode(TK_COMPOUND);
    pStmt = &(comStmt->next);

    Expect(TK_LBRACE);
    while (isPrefixOfStatement(curToken.kind)) {
        *pStmt = Statement();
        pStmt = &((*pStmt)->next);
    }
    Expect(TK_RBRACE);
    return comStmt;
}

///////////////////////////////////////////////////////////////////////////////

// The first token of a statement
static TokenKind prefixOfStmt[] = {TK_ID,     TK_IF,     TK_WHILE,
                                   TK_LBRACE, TK_INT,    TK_INPUT,
                                   TK_OUTPUT, TK_RETURN, TK_DO};

// Test whether the token tk can be the first token of a statement in scc.
static int isPrefixOfStatement(TokenKind tk) {
    int i = 0;
    for (i = 0; i < sizeof(prefixOfStmt) / sizeof(prefixOfStmt[0]); i++) {
        if (tk == prefixOfStmt[i]) {
            return 1;
        }
    }
    return 0;
}
```

### 7.6 Function Definition

```C
FunctionDefinition:
    id(id, id, id, ..., id) CompoundStatement
```

Example
```C
// COMP9024/LargeAssignment/tests/Factorial.scc

long result;

// f(n) = 1 * 2 * ... * (n-1) * n
f(n) {
    if (SccLessEqual(n, 1)) { // n <= 1
        return 1;
    } else {
        return f(n - 1) * n;
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


#### struct astFuncDefNode 
```C 
#define MAX_PARAMETERS_CNT 10
#define MAX_LOCAL_VARS_CNT 64

/*
    Abstract syntax tree node for a function definition.
 */
struct astFuncDefNode {
    // TK_FUNC
    TokenKind op;
    // Function's name
    Value value;

    ///////////////////////////////////////////////
    // The local symbol table of the function:
    //
    //     paras and local_vars
    //
    ///////////////////////////////////////////////

    // for function parameters
    struct astExprNode paras[MAX_PARAMETERS_CNT];
    // number of parameters defined
    int para_cnt;
    // for local variables
    struct astExprNode local_vars[MAX_LOCAL_VARS_CNT];
    // number of local variables defined in the function body
    int local_vars_cnt;

    // number of temporary variables
    int tmpVarNum;
    // the frame size (including stack space needed for local and temporary variables)
    int frameSize;

    // funcBody
    AstStmtNodePtr funcBody;
    // next function
    struct astFuncDefNode *next;
};

typedef struct astFuncDefNode *AstFuncDefNodePtr;
```

### 7.7 The Memory Layout of a Linux Process

[Factorial.scc.s](../Stacks/Factorial.scc.s) contains the assembly code generated by SCC for [Factorial.scc](./tests/Factorial.scc).

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

    The Memory Layout of a Linux Process (userspace)

```



**Call stack** is studied in [Stacks/Recursion](../Stacks/Recursion/README.md), 
[lines 535-550 in LargeAssignment/src/stmt.c](./src/stmt.c), [lines 473-564 in LargeAssignment/src/expr.c](./src/expr.c), and [lines 139-160 and 283-287 in LargeAssignment/src/emit.c](./src/emit.c).

**Heap allocator** is customized in  [OurMalloc() and OurFree() of our large assignment](./libs/SccHeap.c).

**Global variables** are generated in [EmitGlobalDeclarationNode() of our large assignment](./src/decl.c).

**Code** is generated in [EmitFuncDefNode() of our large assignment](./src/func.c).

**The command line arguments and environment variables** are accessed in [LargeAssignment/tests/EnvVars.scc](./tests/EnvVars.scc) and [C/EnvVars](../C/EnvVars/README.md).

## 8 Assessed online via Moodle

Please complete the code in Q1-Q20 and then answer the 24 questions in Large Assignment (Week 10) on [Moodle](https://moodle.telt.unsw.edu.au/my/courses.php).

BTW, the other four questions (i.e., Q21-Q24) are as follows.

```sh
Q21.    
    Modifying one line in ______Q21______ can make the subtraction operator right associative in our large assignment.

Q22.    
    Modifying one line in ______Q22______ can make the division operator right associative in our large assignment.

Q23.    
    Suppose there are n free memory blocks in the singly linked list in our large assignment. 
    The time complexity of OurMalloc() is ______Q23______.

Q24.
    Suppose there are n free memory blocks in the singly linked list in our large assignment. 
    The time complexity of OurFree() is ______Q24______.
```

```sh
AdditiveExpression:
          MultiplicativeExpression aop MultiplicativeExpression  ...  aop MultiplicativeExpression 
```

```sh
// A stands for AdditiveExpression
// M is short for MultiplicativeExpression

A:
    M + M + ... + M
```

It represents a set containing the elements M, M+M, M+M+M, ...

```sh
A = {M, M+M, M+M+M, ...}
  = {M} U {M+M, M+M+M, ...}
  = {M} U M + {M, M+M, M+M+M, ...}
  = {M} U M + A
```

So we have the following rule for A when A is defined as right associative

```sh
A:
    M 
    M + A
```

That is, the right-associative AdditiveExpression can be defined as follows.

```sh
AdditiveExpression:
    MultiplicativeExpression
    MultiplicativeExpression + AdditiveExpression:

```
