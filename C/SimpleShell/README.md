# A Simple Shell

``` sh
/****************************************************************************************
                A Simple Shell

    1.  How to implement a simple shell program based on the C library function system()

    2.  How to implement a simple shell program by calling fork() and execlp() directly

                                                                    COMP9024 24T2

 ******************************************************************************************/

``` 
**A Unix/Linux shell is a command-line interpreter that offers a command line interface for Unix-like operating systems.**


## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/C/SimpleShell

SimpleShell$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

SimpleShell$ code

```

Two configuration files (SimpleShell/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and SimpleShell/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/C/SimpleShell", then click the "Open" button.


### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 40).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
|
├── src                  containing *.c and *.h
|   |
│   |── main.c           A simple shell
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

## 3 Build and run a project in command line

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

``` sh

SimpleShell$ make

SimpleShell$ ./main

#############################   Simple Shell  ############################
Press ctrl+c to exit.

% ls
build  main  Makefile  README.md  src


```


### 4 A simple shell based on the C library function system()

```C

#include <unistd.h>    // fork(), execl()
#include <stdio.h>     // fprintf()
#include <stdlib.h>    // system()
#include <string.h>    // strlen()
#include <sys/wait.h>  // waitpid()

#define MAX_LINE_LENGTH 1024

char commandBuf[MAX_LINE_LENGTH];

int main(void) {
    printf("#############################   Simple Shell  ############################\n");
    printf("Press ctrl+c to exit.\n\n");
    //print prompt
    printf("%% ");
    while (fgets(commandBuf, MAX_LINE_LENGTH, stdin) != NULL) {
        // replace newline with null
        if (commandBuf[strlen(commandBuf) - 1] == '\n') {
            commandBuf[strlen(commandBuf) - 1] = 0; 
        }
        // On Linux: fork() + execl()
        // Create a child process and execute the command
        system(commandBuf);
        printf("%% ");
    }
    return 0;
}

```
#### fgets() and system()

```sh

    In the command line:

    /////////////////////////////////////////////////////////////////////////
 
    $ man fgets


    char *fgets(char *s, int size, FILE *stream);

    DESCRIPTION

    fgets()  reads in at most one less than size characters from stream and
    stores them into the buffer pointed to by s.  Reading  stops  after  an
    EOF  or a newline.  If a newline is read, it is stored into the buffer.
    A terminating null byte ('\0') is stored after the  last  character  in
    the buffer.

    /////////////////////////////////////////////////////////////////////////

    $ man system

    int system(const char *command);

    The  system()  library  function uses fork() to create a child process
    that executes the shell command specified in command using execl()  as
    follows:
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);

    system() returns after the command has been completed.
 
```

We can also implement a simple shell by calling **fork()** and **execlp()** directly.

Please refer to [COMP9024/C/SimpleShell](./src/main.c).

