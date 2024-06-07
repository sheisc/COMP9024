# Command Line Arguments and Environment Variables

``` sh
/****************************************************************************************
                Command Line Arguments and Environment Variables

    How to access the command line arguments and environment variables in C.


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

                                                                    COMP9024 24T2

 ******************************************************************************************/

``` 



## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/C/EnvVars

EnvVars$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

EnvVars$ code

```

Two configuration files (EnvVars/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and EnvVars/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/C/EnvVars", then click the "Open" button.


### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 58).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
├── src                  containing *.c and *.h
|   |
│   |── main.c           how to access command line arguments and environment variables
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

``` sh

EnvVars$ make

EnvVars$ ./main aa bb cc dd

*************** COMP9024 TestEnvironment() ***************

argc = 5

argv[0] = ./main
argv[1] = aa
argv[2] = bb
argv[3] = cc
argv[4] = dd

./main
aa
bb
cc
dd

SHELL=/bin/bash
LANGUAGE=en_AU:en
GNOME_SHELL_SESSION_MODE=ubuntu
...


```

## 4 Access command line arguments and environ variables



### 4.1 Memory Layout of command line arguments

```sh

    EnvVars$ make
    EnvVars$ ./main aa bb cc dd

                 -------------
                     NULL
                  argv[argc-1]    ---------->  "dd"

                    ....

                    argv[1]       ---------->  "aa"
    argv  ------>   argv[0]       ---------->  "./main"
                  -------------     
    char **       array of (char *)            
    
```

### 4.2 C Source Code

```C
int main(int argc, char **argv, char **env) {    
    printf("\n\n*************** COMP9024 TestEnvironment() ***************\n\n");

    // print the command-line arguments
    printf("\n");
    printf("argc = %d\n\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);        
    }

    // print the command-line arguments in another way
    char **pStr = argv;
    printf("\n");
    while (*pStr != NULL) {
        printf("%s\n", *pStr);
        pStr++;
    }
   
    // print all the environment variables (passed to this program by the shell)
    pStr = env;
    printf("\n\n");
    while (*pStr != NULL) {
        printf("%s\n", *pStr);
        pStr++;
    }

    //
    printf("\n");
    char *name = getenv("HOME");
    char *shell = getenv("SHELL");
    if ((name != NULL) && shell) {
        printf("Your HOME directory is %s.\nYour SHELL is %s.\n", name, shell);    
    }  
    return 0;
}
```


### 4.3 Memory Layout of environment variables

```sh
                 -------------
                     NULL
                     ...         ---------->  "...=....."

                     ...

                    env[1]       ---------->  "...=....."
    env  ------>    env[0]       ---------->  "SHELL=/bin/bash"
                  -------------     
    char **       array of (char *) 
```


## 5 A similar test case in [our large assignment](../../LargeAssignment/tests/EnvVars.scc)

The following test case is not written in C, but with a simple language defined in our large assignment.

You can see how low-level pointer arithmetic (e.g., SccRead64()) is used in [accessing memory](../../C/AccessMemory/README.md).

```C

// COMP9024/LargeAssignment/tests/EnvVars.scc

printStrs(strs){
  long i;
  long s;
  i = 0;
  s = SccRead64(strs, i*8);
  while(s){
    puts(s);
    i = i + 1;
    s = SccRead64(strs, i*8);
  }
}

main(argc, argv, env){
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