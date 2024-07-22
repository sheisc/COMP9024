#ifndef QUEUE_H
#define QUEUE_H
#include "Trie.h"

typedef Trie QUEUE_ITEM_T;

struct Queue;

struct Queue *CreateQueue(void);

void ReleaseQueue(struct Queue *pQueue);

void QueueEnqueue(struct Queue *pQueue, QUEUE_ITEM_T item);

QUEUE_ITEM_T QueueDequeue(struct Queue *pQueue);

int QueueIsEmpty(struct Queue *pQueue);

QUEUE_ITEM_T QueuePeek(struct Queue *pQueue);

// for debugging
void PrintQueue(struct Queue *pQueue);

#endif


