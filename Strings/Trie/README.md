# Trie

``` sh
/*******************************************************************
                            Trie

    1.  The data structure used for storing and retrieving strings

    2.  How to insert a key-value pair into a Trie

    3.  How to search for a key in a Trie


                                             COMP9024 24T2

 *******************************************************************/
``` 
A Trie, also known as a prefix tree, is a specialized tree-based data structure used for efficiently storing and retrieving strings in a way that supports prefix matching.

The term "Trie" (pronounced "try") is derived from the word "retrieval," which underscores the structure's suitability for efficient retrieval operations, particularly for words or strings based on prefixes.

The root of the Trie usually represents an empty string or null prefix.

Each node in a Trie typically consists of a set of child pointers (usually implemented as an array, representing possible next characters), and a flag indicating whether the node marks the end of a valid word.

Tries are frequently employed in spell checkers and dictionary implementations.

| Trie |
|:-------------:|
| <img src="diagrams/OurTrie.png" width="100%" height="100%"> |


## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Strings/Trie

Trie$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

Trie$ code

```

Two configuration files (Trie/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and Trie/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



#### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Strings/Trie", then click the "Open" button.


#### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


#### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 35).

Then, click **Run -> Start Debugging**

### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this project
|
├── src                  containing *.c and *.h
|   |
|   |
│   ├── Trie.c           The data structure used for storing and retrieving strings
│   ├── Trie.h
|   |
│   ├── Queue.c          used in a breadth-first tree traversal when generating *.dot files
│   ├── Queue.h
|   |
│   └── main.c           main()
|
|── images               containing *.dot and *.png files
|
|── diagrams             containing *.png files
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

## 3 The main procedure

### 3.1 make and ./main

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

``` sh

Trie$ make

Trie$ ./main

Our dictionary contains the following items:

ear
	the sense organ for hearing
apply
	put into service
ape
	a large primate that lacks a tail
apes
	the plural noun of the word ape
earth
	the planet on which we live
east
	the eastern part of the world
app
	an application, especially as downloaded by a user to a mobile device
ace
	a playing card with a single spot on it
early
	happening or done before the usual or expected time
earl
	a British nobleman 
aces
	the plural noun of the word ace

Our dictionary does not contain the word "nullpointer"


```

### 3.2 make view

**Ensure that you have executed 'make' and './main' before 'make view'.**


```sh
Trie$ make view
```

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.

Here, **feh** is an image viewer available in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/).

```C
static char *words[] = { "ear", "apply", "ape", "apes", "earth", "east",
		  "app", "ace", "early", "earl", "aces" };
```

#### 3.2.1 TrieInsert()

| Insert "ear" | 
|:-------------:|
| <img src="images/TrieInsert_0001.png" width="100%" height="100%"> |

| Insert "apply" |
|:-------------:|
| <img src="images/TrieInsert_0002.png" width="100%" height="100%"> |

| Insert "ape" |
|:-------------:|
| <img src="images/TrieInsert_0003.png" width="100%" height="100%">  |

| Insert "apes" |
|:-------------:|
| <img src="images/TrieInsert_0004.png" width="100%" height="100%"> |

| Insert "earth"|
|:-------------:|
| <img src="images/TrieInsert_0005.png" width="100%" height="100%"> |

| Insert "east" |
|:-------------:|
| <img src="images/TrieInsert_0006.png" width="100%" height="100%"> |

| Insert "app" |
|:-------------:|
| <img src="images/TrieInsert_0007.png" width="100%" height="100%"> |

| Insert "ace" | 
|:-------------:|
|  <img src="images/TrieInsert_0008.png" width="100%" height="100%"> |

| Insert "early" | 
|:-------------:|
| <img src="images/TrieInsert_0009.png" width="100%" height="100%"> |

| Insert "earl" |
|:-------------:|
| <img src="images/TrieInsert_0010.png" width="100%" height="100%"> |

| Insert "aces" |
|:-------------:|
| <img src="images/TrieInsert_0011.png" width="100%" height="100%"> |


## 4 Data structures

```C

// To be simple, support lowercases only
static char *words[] = { "ear", "apply", "ape", "apes", "earth", "east",
		  "app", "ace", "early", "earl", "aces" };

// The meaning of each word
static char *meanings[] = { 
    "the sense organ for hearing", 
    "put into service", 
    "a large primate that lacks a tail", 
    "the plural noun of the word ape", 
    "the planet on which we live", 
    "the eastern part of the world",
	"an application, especially as downloaded by a user to a mobile device", 
    "a playing card with a single spot on it", 
    "happening or done before the usual or expected time", 
    "a British nobleman ", 
    "the plural noun of the word ace"
};

#define ALPHABET_SIZE 26
#define FIRST_CHAR 'a'

typedef char *ValueType;

struct TrieNode;
typedef struct TrieNode *Trie;

struct TrieNode {
    // Test whether it represents the last char in a key? 
    int wordEnd;
    // value for a key when wordEnd == 1  
    ValueType value;
    // Only support lowercase letters         
    Trie kids[ALPHABET_SIZE];
};

```


## 5 Algorithms

### 5.1 main()

```C

// To be simple, support lowercases only
static char *words[] = { "ear", "apply", "ape", "apes", "earth", "east",
		  "app", "ace", "early", "earl", "aces" };

// The meaning of each word
static char *meanings[] = { 
    "the sense organ for hearing", 
    "put into service", 
    "a large primate that lacks a tail", 
    "the plural noun of the word ape", 
    "the planet on which we live", 
    "the eastern part of the world",
	"an application, especially as downloaded by a user to a mobile device", 
    "a playing card with a single spot on it", 
    "happening or done before the usual or expected time", 
    "a British nobleman",
    "the plural noun of the word ace"
};    

int main(void) {
    long cnt = 1;
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    Trie t = CreateTrie();

    int n = sizeof(words)/sizeof(words[0]);   
    for (int i = 0; i < n; i++) {
        t = TrieInsert(t, words[i], meanings[i]);
        GenOneImage(t, "TrieInsert", "images/TrieInsert", cnt);
        cnt++;
    }
    
    printf("Our dictionary contains the following items:\n\n");
    ValueType value = NULL;
    for (int i = 0; i < n; i++) {
        if (SearchKey(t, words[i], &value)) {
            printf("%s\n\t%s\n", words[i], value);
        }
    }   
    printf("\n");

    char *newWord = "nullpointer";
    if (SearchKey(t, newWord, &value) == 0) {
        printf("Our dictionary does not contain the word \"%s\"\n\n", newWord);
    } else {
        printf("%s\n\t%s\n", newWord, value);
    }     
    ReleaseTrie(t);
    return 0;
}

```

### 5.2 TrieInsert()

```C

Trie CreateTrie() {
    return NULL;
}

void ReleaseTrie(Trie t) {
    if (t != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
	         ReleaseTrie(t->kids[i]);
        }
        free(t);
    }
}

/*
    Create a Trie node
 */
static Trie CreateTrieNode(void) {
    Trie pNode = malloc(sizeof(struct TrieNode));
    assert(pNode != NULL);
    pNode->wordEnd = 0;
    pNode->value = 0;   
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->kids[i] = NULL;
    }
    return pNode;
}

/*
    Insert 
 */
Trie TrieInsert(Trie t, char *key, ValueType value) {
    if (t == NULL) {
        t = CreateTrieNode();
    }
    if (key[0] == '\0') {
        t->wordEnd = 1;
        t->value = value;
    } else {
        int i = key[0] - FIRST_CHAR;
        assert((i >= 0) && (i <ALPHABET_SIZE));
        t->kids[i] = TrieInsert(t->kids[i], key+1, value);
    }
    return t;
}

```

### 5.3 SearchKey()

```C

/*
   SearchKey() returns 1 when it finds the key in the Trie.
           It will also stores the value in *pVal.
           
   SearchKey() returns 0 when the key doesn't exist.           
 */
int SearchKey(Trie t, char *key, ValueType *pVal) {
    Trie curNode = t;
    char *ptr = key;
    //
    while(*ptr) {
        int i = *ptr - FIRST_CHAR;
        assert((i >= 0) && (i <ALPHABET_SIZE));
        if (curNode->kids[i] == NULL) {
            return 0;
        }
        curNode = curNode->kids[i];
        ptr++;
    }
    //
    if (curNode->wordEnd) {
        if (pVal) {
            *pVal = curNode->value;
        }
        return 1;
    } else {
        return 0;
    }
}

```
### 5.4 Trie2Dot()

```C

/*
    digraph TrieInsert {
    "0x587a961a42a0" -> {"0x587a961a4390"} [label="e"]
    "0x587a961a4390" -> {"0x587a961a4480"} [label="a"]
    "0x587a961a4480" -> {"0x587a961a4570"} [label="r"]
    "0x587a961a4570" [color=red]
    }
 */
void Trie2Dot(Trie root, char *filePath, char *graphName) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        
        struct Queue *pQueue = CreateQueue();
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                Trie curNode = QueueDequeue(pQueue);
                // output current node
                if (curNode->wordEnd) {
                    fprintf(dotFile, 
                            "\"%p\" [color=red]\n",
                            curNode);
                }
                // output the directed edge
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (curNode->kids[i]) {
                        fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"%c\"]\n",
                                curNode,
                                edgeConnectorStr,                         
                                curNode->kids[i],
                                FIRST_CHAR + i);
                        QueueEnqueue(pQueue, curNode->kids[i]);                        
                    }
                }                
            }
        }
        ReleaseQueue(pQueue);     
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

#define FILE_NAME_LEN  255

void GenOneImage(Trie root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Trie2Dot(root, dotFileName, graphName);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}

```