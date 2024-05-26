#include <stdio.h>
#include "Queue.h"

int main(void) {
    long arr[] = {30, 20, 50, 60, 70};
    int len = sizeof(arr) / sizeof(arr[0]);
    struct Queue *pQueue = CreateQueue();

    for (int i = 0; i < len; i++) { 
        printf("\nQueueEnqueue(pQueue, %ld)\n", arr[i]);       
        QueueEnqueue(pQueue, arr[i]);
        PrintQueue(pQueue);
    }
    printf("\nQueuePeek(pQueue): %ld\n\n", QueuePeek(pQueue));
    PrintQueue(pQueue);

    while (!QueueIsEmpty(pQueue)) {
        QUEUE_ITEM_T item = QueueDequeue(pQueue);
        printf("\nQueueDequeue(pQueue) returns %ld\n", item);
        PrintQueue(pQueue);
    }
    ReleaseQueue(pQueue);
    return 0;
}
