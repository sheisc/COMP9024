#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

#define  NUM_OF_ITERATIONS   3

int main(void) {
    long len = 7;
    struct Queue *pQueue = CreateQueue();
    long iterationCnt = 0;
    long imgCount = 0;

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    QueueGenOneImage(pQueue, "CircularQueue", "images/CircularQueue", imgCount);
    imgCount++;

    while(iterationCnt < NUM_OF_ITERATIONS) {
        //len *= 2;
        printf("\n\n******************** count == %ld *********************\n\n", iterationCnt);
        for (long i = 1; i <= len; i++) { 
            QUEUE_ITEM_T item = i*10;
            printf("\nQueueEnqueue(pQueue, %ld)\n", (long) (item));       
            QueueEnqueue(pQueue, item);
            PrintQueue(pQueue);
            QueueGenOneImage(pQueue, "CircularQueue", "images/CircularQueue", imgCount);
            imgCount++;
        }
        printf("\nQueuePeek(pQueue): %ld\n\n", QueuePeek(pQueue));
        PrintQueue(pQueue);  

        while (!QueueIsEmpty(pQueue)) {
            QUEUE_ITEM_T item = QueueDequeue(pQueue);
            printf("\nQueueDequeue(pQueue) returns %ld\n", item);
            PrintQueue(pQueue);
            QueueGenOneImage(pQueue, "CircularQueue", "images/CircularQueue", imgCount);
            imgCount++;            
        }        
        iterationCnt++;
    }
    ReleaseQueue(pQueue);
    return 0;
}
