# Array2Dot

``` sh
/*******************************************************************
                Array2Dot

    1.  Array in C (used as data structures for bubble sort)

    2.  How to visualize the Bubble Sort algorithm

                                             COMP9024

 *******************************************************************/
``` 

## Introduction

More details of *.dot files will be discussed in [COMP9024/Graphs/Dot2Png](../../Graphs/Dot2Png/README.md).

## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Sorting/Array2Dot

Array2Dot$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

Array2Dot$ code

```

Two configuration files (Array2Dot/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and Array2Dot/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Sorting/Array2Dot", then click the "Open" button.


### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


### 2.3 Debug the project in VS Code

Open src/BubbleSort.c, and click to add a breakpoint (say, line 63).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
├── src                  containing *.c and *.h
|   ├── Array2Dot.c
|   ├── Array2Dot.h
|   ├── BubbleSort.c
|   ├── BubbleSort.h
|   ├── main.c
|   ├── Util.c
|   └── Util.h
|
├── images               *.png and *.dot files
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

## 3 How to visualize the Bubble Sort algorithm

### 3.1 make && ./main

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

``` sh

Array2Dot$ make

Array2Dot$ ./main

```
### 3.2 make view

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.

```sh
Week2$ make view
```

Here, **feh** is an image viewer available in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/).

