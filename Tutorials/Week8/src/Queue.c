#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Queue.h"

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


void PrintQueue(struct Queue *pQueue) {
    QueueNode *cur = pQueue->head;
    printf("Queue: ");
    // free the heap space of all nodes in the queue
    while (cur != NULL) {
        printf("%p", cur->item);
        cur = cur->next;
        if(cur) {
            printf(" --> ");
        }
    }
    printf("\n");
}


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

int QueueIsEmpty(struct Queue *pQueue) {
    return pQueue->n == 0;
}

QUEUE_ITEM_T QueuePeek(struct Queue *pQueue) {
    assert(!QueueIsEmpty(pQueue));
    QUEUE_ITEM_T item = pQueue->head->item;
    return item;
}
