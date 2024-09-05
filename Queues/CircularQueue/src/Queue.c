#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Queue.h"

// number of items, not in bytes, INITIAL_QUEUE_SIZE  >= 2
#define INITIAL_QUEUE_SIZE   2

// The type definition of Circular Queue (based on an array)
struct Queue {
    // The index of the first element in the queue
    long head;
    // The index of the next available slot
    long tail;
    // The current capacity of the queue.
    // At most (size - 1) elements are used for simplifying QueueEnQueue(), QueueIsFull(), QueueIsEmpty(), ...
    long size;
    // An array dynamically allocated on heap
    QUEUE_ITEM_T *pItems;
};

// Get the next position in the circular queue
static long NextPosition(struct Queue *pQueue, long cur) {
    long nextPos = (cur + 1) % pQueue->size;
    return nextPos;
}

// Only (size - 1) slots are used
static int QueueIsFull(struct Queue *pQueue) {
    return pQueue->head == NextPosition(pQueue, pQueue->tail);
    // return pQueue->head == (pQueue->tail + 1) % pQueue->size;
}

int QueueIsEmpty(struct Queue *pQueue) {
    return pQueue->head == pQueue->tail;
}




void PrintQueue(struct Queue *pQueue) {
   
    long cur = pQueue->head;
    printf("Queue(head = %ld, tail = %ld, size = %ld, len = %ld): ", 
            pQueue->head, pQueue->tail, pQueue->size, QueueLength(pQueue));
    while (cur != pQueue->tail) {
        printf("%ld ", (long) pQueue->pItems[cur]);
        cur = NextPosition(pQueue, cur);
    }
    printf("\n");
}

/*
    Create an empty queue.

                  ----------                   ----------------             
    pQueue -----> head                          pItems[size-1]                                 
                  tail                                                                            
                  n                               ...                            
                  size                          pItems[1]                         
                  pItems      --------------->  pItems[0]                                     
                  -----------                  ----------------
                 struct Queue                   Array on the heap

 */ 
struct Queue *CreateQueue(void) {
    struct Queue *pQueue = (struct Queue *) malloc(sizeof(struct Queue));
    QUEUE_ITEM_T *pItems = (QUEUE_ITEM_T *) malloc(sizeof(QUEUE_ITEM_T) * INITIAL_QUEUE_SIZE);
    //memset(pItems, 0, sizeof(QUEUE_ITEM_T) * INITIAL_QUEUE_SIZE);
    assert(pQueue && pItems && (INITIAL_QUEUE_SIZE >= 2));

    pQueue->head = 0;
    pQueue->tail = 0;
    pQueue->size = INITIAL_QUEUE_SIZE;
    pQueue->pItems = pItems;
    return pQueue;
}


// This function is used to increase the capacity of the queue when it is full.
static void ResizeQueue(struct Queue *pQueue) {
    assert(QueueIsFull(pQueue));
    //printf("\t\t\t\t\t\t\t\t ................ ResizeQueue() ...............\n");

    long newSize = pQueue->size * 2;
    QUEUE_ITEM_T *pNewItems = (QUEUE_ITEM_T *) malloc(sizeof(QUEUE_ITEM_T) * newSize);    
    assert(pNewItems);
    //memset(pNewItems, 0, sizeof(QUEUE_ITEM_T) * newSize);

    long head = pQueue->head;
    long tail = pQueue->tail;
    if (tail > head) {
        /*
        Items in the queue pointed to by pQueue:
                                                                     (Not Used)
            pItems[0] pItems[1] pItems[2]  ...   pItems[size - 2]  pItems[size - 1]
             ^                                                          ^
             ^                                                          ^
             |                                                          |
             head==0                                                     tail==size -1

         */
        long n = tail - head;
        memcpy(pNewItems + head, pQueue->pItems + head, n * sizeof(QUEUE_ITEM_T));
    } else {
        assert(tail < head);
        /*
        Items in the queue pointed to by pQueue:
                          (Not Used)                                          
            pItems[0] ...  pItems[i] ...  pItems[k]  ...   pItems[size - 2]  pItems[size - 1]
                             ^               ^                                                         
                             ^               ^                                                         
                             |               |                                                         
                            tail==i          head==k

                                             | .............  n2  ....................|                                    
            |... n1  ...|

            There are n2 elements from pItems[head] to pItems[size-1].

            There are n1 elements from pItems[0] to pItems[tail - 1].

         */        
        long n2 = (pQueue->size - head);
        long n1 = tail - 0;
        memcpy(pNewItems + head, pQueue->pItems + head, n2 * sizeof(QUEUE_ITEM_T));
        // n1 might be 0 when tail is 0
        if (n1 > 0) {
            memcpy(pNewItems + head + n2, pQueue->pItems, n1 * sizeof(QUEUE_ITEM_T));
        }
        // adjust the position of tail
        pQueue->tail = head + n1 + n2;
    }
    free(pQueue->pItems);
    pQueue->size = newSize;
    pQueue->pItems = pNewItems;
}

/*
    Release the heap space occupied by the queue.

                  ----------                   ----------------            
    pQueue -----> head                          pItems[size-1]                                
                  tail                                                                           
                  n                               ...                           
                  size                          pItems[1]                        
                  pItems      --------------->  pItems[0]                                    
                  -----------                  ----------------
                 struct Queue                   Array on the heap          

 */                                                                              
void ReleaseQueue(struct Queue *pQueue) {
    if (pQueue) {
        free(pQueue->pItems);
        free(pQueue);
    }
}


void QueueEnqueue(struct Queue *pQueue, QUEUE_ITEM_T item) {
    if (QueueIsFull(pQueue)) {
        ResizeQueue(pQueue);
    }

    pQueue->pItems[pQueue->tail] = item;
    // let tail point to the next available slot
    pQueue->tail = NextPosition(pQueue, pQueue->tail);
}


QUEUE_ITEM_T QueueDequeue(struct Queue *pQueue) {
    assert(!QueueIsEmpty(pQueue));

    // Get the first element
    QUEUE_ITEM_T item = pQueue->pItems[pQueue->head];
    // Its successor now becomes the new first element
    pQueue->head = NextPosition(pQueue, pQueue->head);

    return item;
}


QUEUE_ITEM_T QueuePeek(struct Queue *pQueue) {
    assert(!QueueIsEmpty(pQueue));
    QUEUE_ITEM_T item = pQueue->pItems[pQueue->head];
    return item;
}

// Get the number of elements in the queue
long QueueLength(struct Queue *pQueue) {
    long n;

    if (pQueue->tail >= pQueue->head) {
        n = pQueue->tail - pQueue->head;
    } else {
        n = pQueue->tail + pQueue->size - pQueue->head;
    }
    return n;
}

////////////////////////////// Queue2Dot (for visualizing the algorithm) ///////////////////////////////////////

static int IsLegalIndex(struct Queue *pQueue, long i) {
    long head = pQueue->head;
    long tail = pQueue->tail;
    if (!QueueIsEmpty(pQueue)) {
        if (tail > head) {
            return (i >= head) && (i < tail);
        } else { // tail < head
            return (i >= head && i <= pQueue->size - 1) || (i >= 0 && i < tail);
        }
    } else {
        return 0;
    }
}

/*
    Dot Files
 */
void Queue2Dot(struct Queue *pQueue, 
               char *filePath,
               char *graphName,
               int displayVisited) {
    (void) displayVisited;

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        //char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        fprintf(dotFile, "rankdir=\"LR\";\n");
        
 
        long cur = 0;
        QUEUE_ITEM_T *pseudoQueueNode = (QUEUE_ITEM_T *) (-8);
        fprintf(dotFile, "\"%p\" [label=\"size=%ld, n=%ld\"]  \n", 
                pseudoQueueNode, pQueue->size, QueueLength(pQueue));

        fprintf(dotFile, "\"%p\" -> \"%p\" [style=invis] \n", pseudoQueueNode, pQueue->pItems + cur);

        while (cur < pQueue->size) {
            // test whether there is a successor
            if (cur + 1 < pQueue->size) {
                fprintf(dotFile, "\"%p\" -> \"%p\" \n", pQueue->pItems + cur, pQueue->pItems + cur + 1);
            }
            // draw the current node
            if (IsLegalIndex(pQueue, cur)) {
                fprintf(dotFile, "\"%p\" [label=\"%ld\"] [shape=box] \n", pQueue->pItems + cur, pQueue->pItems[cur]);
            } else {
                fprintf(dotFile, "\"%p\" [label=\" \"] [color=grey] [shape=box] [style=filled] \n", pQueue->pItems + cur);
            }            
            // draw the head index
            if (cur == pQueue->head) {
                QUEUE_ITEM_T *pseudoHeadNode = (QUEUE_ITEM_T *) (-16);
                fprintf(dotFile, "\"%p\" -> {\"%p\"}  [color=orange]\n", pseudoHeadNode, pQueue->pItems + cur);
                fprintf(dotFile, "\"%p\" [label=\"head=%ld\"] [shape=hexagon]\n", pseudoHeadNode, cur);                
            }
            // draw the tail index
            if (cur == pQueue->tail) {
                QUEUE_ITEM_T *pseudoTailNode = (QUEUE_ITEM_T *) (-24);
                fprintf(dotFile, "\"%p\" -> {\"%p\"}  [color=blue]\n", pseudoTailNode, pQueue->pItems + cur);
                fprintf(dotFile, "\"%p\" [label=\"tail=%ld\"] [shape=hexagon]\n", pseudoTailNode, cur); 
            }
            cur++;
        }      
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


#define FILE_NAME_LEN  255

void QueueGenOneImage(struct Queue *pQueue, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Queue2Dot(pQueue, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 
}





