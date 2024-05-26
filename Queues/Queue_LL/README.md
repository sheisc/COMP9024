# Queue

``` sh
/*******************************************************************
                Queue (based on a linked list)

    1.  How to create a queue

    2.  struct in C (for describing the data structure of Queue)

    3.  QueueEnqueue() and QueueDequeue()

                                             COMP9024 24T2

 *******************************************************************/
``` 

A queue is a first-in-first-out (FIFO) data structure, 

where the first element added to the queue will be the first one to be removed. 

Two fundamental operations:


Enqueue

    adds a new element at the tail of the queue

Dequeue

    removes and returns the first (front) element from the queue.



A queue can be implemented with a linked list for storing queue items / elements.



## 1 How to download Tutorials in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Queues/Queue_LL

Queue_LL$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

Queue_LL$ code

```

Two configuration files (Queue_LL/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and Queue_LL/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



#### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Queues/Queue_LL", then click the "Open" button.


#### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


#### 2.3 Debug the project in VS Code

Open src/Queue.c, and click to add a breakpoint (say, line 112).

Then, click **Run -> Start Debugging**


### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this tutorial
|
├── src                  containing *.c and *.h
|   |
│   ├── main.c           
│   ├── Queue.h
│   └── Queue.c          
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

## 3 Data structure and memory layout

```C
typedef long QUEUE_ITEM_T;

// the definition of a node in a linked list
typedef struct node {    
    struct node *next;
    QUEUE_ITEM_T item;
} QueueNode;

// The type defintion of Queue (based on a linked list)
struct Queue {
    // (1) point to the first node in the queue (2) NULL when the queue is empty
    QueueNode *head;
    // (1) point to the last node in the queue  (2) NULL when the queue is empty
    QueueNode *tail;    
    // number of elements in the queue
    long n;
};


                ----------            ----------         ----------         ----------      
  pQueue -----> head         ------>     next      ------>  next      .....   next: NULL                           
                tail                     item               item              item          
                n                     ----------         ----------         ----------  
                -----------
               struct Queue           QueueNode           QueueNode           QueueNode   
                                                                              ^
                                                                              ^
                                                                              ^
                                                                              pQueue->tail

```

## 4 The Enqueue/Dequeue operations

```sh
Queue_LL$ make

Queue_LL$ ./main

./main

QueueEnqueue(pQueue, 30)
Queue: 30

QueueEnqueue(pQueue, 20)
Queue: 30 --> 20

QueueEnqueue(pQueue, 50)
Queue: 30 --> 20 --> 50

QueueEnqueue(pQueue, 60)
Queue: 30 --> 20 --> 50 --> 60

QueueEnqueue(pQueue, 70)
Queue: 30 --> 20 --> 50 --> 60 --> 70

QueuePeek(pQueue): 30

Queue: 30 --> 20 --> 50 --> 60 --> 70

QueueDequeue(pQueue) returns 30
Queue: 20 --> 50 --> 60 --> 70

QueueDequeue(pQueue) returns 20
Queue: 50 --> 60 --> 70

QueueDequeue(pQueue) returns 50
Queue: 60 --> 70

QueueDequeue(pQueue) returns 60
Queue: 70

QueueDequeue(pQueue) returns 70
Queue: 
```

## 5 Algorithms

### 5.1 CreateQueue

```C
/*
    Create an empty queue.

                ----------       
  pQueue -----> head: NULL                                    
                tail: NULL             
                n   :  0         
                -----------
               struct Queue    

 */ 
struct Queue *CreateQueue(void) {
    struct Queue *pQueue = (struct Queue *) malloc(sizeof(struct Queue));
    assert(pQueue);
    //memset(pQueue, 0, sizeof(*pQueue));
    pQueue->n = 0;
    pQueue->head = NULL;
    pQueue->tail = NULL;
    return pQueue;
}
```

### 5.2  Enqueue
```C
/*
    Add one node at the tail of the queue.

                ----------            ----------         ----------         ----------      
  pQueue -----> head         ------>     next      ------>  next      .....   next: NULL                           
                tail                     item               item              item          
                n                     ----------         ----------         ----------  
                -----------
               struct Queue           QueueNode           QueueNode           QueueNode   
                                                                              ^
                                                                              ^
                                                                              ^
                                                                              pQueue->tail


                                                                ----------
                                                pNode ---->     next  NULL
                                                                item     
                                                                ----------
                                                                QueueNode

 */
void QueueEnqueue(struct Queue *pQueue, QUEUE_ITEM_T item) {
    QueueNode *pNode = (QueueNode *) malloc(sizeof(QueueNode));
    assert(pNode);
    // The node pointed to by pNode is inserted as the last node in the queue
    pNode->item = item;
    pNode->next = NULL;
    // Test whether the queue is empty
    if (QueueIsEmpty(pQueue)) {        
        pQueue->head = pNode;
        pQueue->tail = pNode; 
    } else {
        pQueue->tail->next = pNode;
        pQueue->tail = pNode;
    }
    // increase the number of nodes by 1
    pQueue->n++;
}
```

### 5.3  Dequeue
```C
/*
    Remove the first node from the queue.

                ----------            ----------         ----------         ----------      
  pQueue -----> head         ------>     next      ------>  next      .....   next: NULL                           
                tail                     item               item              item          
                n                     ----------         ----------         ----------  
                -----------
               struct Queue           QueueNode           QueueNode           QueueNode   
                                       ^                                        ^
                                       ^                                        ^
                                       ^                                        ^
                                       ^                                       pQueue->tail
                                       pNode 

 */
QUEUE_ITEM_T QueueDequeue(struct Queue *pQueue) {
    assert(!QueueIsEmpty(pQueue));
    // let pNode point to the first node
    QueueNode *pNode = pQueue->head;
    // remove the first node from the queue
    pQueue->head = pNode->next;
    // If the queue becomes empty after QueueDequeue(), 
    // we also need to set pQueue->tail to be NULL
    if (pQueue->head == NULL) {
        pQueue->tail = NULL;
    }
    // decrease the number of nodes by 1
    pQueue->n--;
    // save the data in a local variable on call stack before calling free(pNode)
    QUEUE_ITEM_T item = pNode->item;
    free(pNode);
    return item;
}
```

### 5.4  ReleaseQueue
```C
/*
    Release the heap space occupied by the queue.

                ----------            ----------         ----------         ----------      
  pQueue -----> head         ------>     next      ------>  next      .....   next: NULL                           
                tail                     item               item              item          
                n                     ----------         ----------         ----------  
                -----------
               struct Queue           QueueNode           QueueNode           QueueNode   
                                       ^                  ^  
                                       ^                  ^   
                                       ^                  ^  
                                       cur                tmp                

 */                                                                              
void ReleaseQueue(struct Queue *pQueue) {
    
    QueueNode *cur = pQueue->head;
    // free the heap space of all nodes in the queue
    while (cur != NULL) {
        QueueNode *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    // free the 'struct Queue' object itself
    free(pQueue);
}
```