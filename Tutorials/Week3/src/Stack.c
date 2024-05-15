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
            Please complete the following code.

            Q1. call malloc() to allocate pStack->size * 2 * sizeof(STACK_ITEM_T) bytes of heap memory,
                and saved the return value in a pointer variable named as newItems
            Q2. call assert() to make sure newItems is not NULL
            Q3. call memcpy() to copy the items pointed to by pStack->pItems 
                to the heap memory pointed to by newItems

                For help, please type 'man memcpy' in the command line.

                $ man memcpy
             
            Q4. call free() to release the heap memory pointed to by pStack->pItems
            Q5. double the capacity size of the stack
            Q6. let pStack->pItems point to the heap space allocated in Q1

            You need to echo these questions in our weekly Quiz.
            Our tutors will NOT answer these questions in the tutorial.

            To test the code you have completed, you can set INITIAL_STACK_SIZE to be 2 (line 8 in Stack.c)
         */
    
        // Double the capacity of the stack        

        
        // Q1. ___________________
        // Q2. ___________________
        // Q3. ___________________
        // Q4. ___________________
        // Q5. ___________________
        // Q6. ___________________
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

