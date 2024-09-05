#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Stack.h"

// number of items, not in bytes
#define INITIAL_STACK_SIZE   64

// the top position when a stack is empty
#define TOP_POS_FOR_EMPTY_STACK  -1

// The type defintion of our Stack
struct Stack {
    // the current capacity size of a stack, in the number of items, not in bytes
    long size;
    // the stack top position
    long top;
    // pItems points to an array dynamically allocated in heap
    STACK_ITEM_T *pItems;
};

// Test whether the stack is full
static int StackIsFull(struct Stack *pStack) {
    return pStack->top == pStack->size - 1;    
}

// Test whether the stack is empty
int StackIsEmpty(struct Stack *pStack) {
    return pStack->top == TOP_POS_FOR_EMPTY_STACK;
}

// Create a stack
struct Stack *CreateStack(void) {
    struct Stack *pStack;
    STACK_ITEM_T *pItems;

    //printf("sizeof(STACK_ITEM_T) == %ld\n", sizeof(STACK_ITEM_T));
    pStack = (struct Stack *) malloc(sizeof(struct Stack));
    pItems = (STACK_ITEM_T *) malloc(sizeof(STACK_ITEM_T) * INITIAL_STACK_SIZE);

    assert(pStack && pItems);

    pStack->size = INITIAL_STACK_SIZE;    
    pStack->top = TOP_POS_FOR_EMPTY_STACK;
    pStack->pItems = pItems;

    return pStack;
}

// Release the heap space
void ReleaseStack(struct Stack *pStack) {
    if (pStack) {
        free(pStack->pItems);
        free(pStack);
    }
}

// Push an item onto/at the top of the stack 
void StackPush(struct Stack *pStack, STACK_ITEM_T item) {
    if (StackIsFull(pStack)) {
        /*
            Please complete the following code in Q1-Q5.

            Q1. call malloc() to allocate enough bytes of heap memory,
                and save the return value in a pointer variable named as newItems           
          
            Q2. call memcpy() to copy the items pointed to by pStack->pItems 
                to the heap memory pointed to by newItems

                For help, please type 'man memcpy' in the command line.

                $ man memcpy
             
            Q3. call free() to release the unused heap memory
            Q4. double the capacity size of the stack
            Q5. let pStack->pItems point to the heap space allocated in Q1

            You need to echo these questions in our weekly Quiz.
            Our tutors will NOT answer these questions in the tutorial.

            To test the code you have completed, you can set INITIAL_STACK_SIZE to be 2 (line 8 in Stack.c)
         */
    
        // Double the capacity of the stack        
        
        // Q1. ___________________
        // assert(newItems);
        // Q2. ___________________
        // Q3. ___________________
        // Q4. ___________________
        // Q5. ___________________
    }

    // Now the stack is not full. Let us do the push operation.
    pStack->top++;
    pStack->pItems[pStack->top] = item;
}

// Pop the top item from the stack
STACK_ITEM_T StackPop(struct Stack *pStack) {
    assert(!StackIsEmpty(pStack));

    STACK_ITEM_T item = pStack->pItems[pStack->top];
    pStack->top--;

    return item;
}

// Peek the top item without popping
STACK_ITEM_T StackPeek(struct Stack *pStack) {
    assert(!StackIsEmpty(pStack));

    STACK_ITEM_T item = pStack->pItems[pStack->top];
    return item;
}


////////////////////////////// Stack2Dot (for visualizing the algorithm) ///////////////////////////////////////

/*
    Dot Files
 */
void Stack2Dot(struct Stack *pStack, 
               char *filePath,
               char *graphName,
               int displayVisited) {
    (void) displayVisited;

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);
        fprintf(dotFile, "rankdir=\"LR\";\n");

        STACK_ITEM_T *pseduoStackNode = (STACK_ITEM_T *) (-8);
        STACK_ITEM_T *pseduoTopNode = (STACK_ITEM_T *) (-16);       

        long cur = 0; 

        if (StackIsEmpty(pStack)) {
            fprintf(dotFile, "\"%p\" [label=\"size=%ld, n=0\"] \n", pseduoStackNode, pStack->size);            
        } else {
            fprintf(dotFile, "\"%p\" -> \"%p\" [style=invis] \n", pseduoStackNode, pStack->pItems);
            fprintf(dotFile, "\"%p\" [label=\"size=%ld, n=%ld\"] \n", pseduoStackNode, pStack->size, pStack->top + 1);
        }

        while (cur <= pStack->top) {
            // test whether there is a successor
            if (cur + 1 <= pStack->top) {
                fprintf(dotFile, "\"%p\" -> \"%p\" [style=invis] \n", pStack->pItems + cur, pStack->pItems + cur + 1);
            }
            // draw the current node
            fprintf(dotFile, "\"%p\" [label=\"%ld\"] [shape=box] \n", pStack->pItems + cur, pStack->pItems[cur]);          
            // draw the top index
            if (cur == pStack->top) {
                fprintf(dotFile, "\"%p\" -> \"%p\" [color=orange] \n", pseduoTopNode, pStack->pItems + cur); 
                fprintf(dotFile, "\"%p\" [label=\"top=%ld\"] [shape=hexagon]\n", pseduoTopNode, cur);                
            }
            cur++;
        }      
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


#define FILE_NAME_LEN  255

void StackGenOneImage(struct Stack *pStack, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Stack2Dot(pStack, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 
}

