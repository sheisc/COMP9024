#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Stack.h"

// the definition of a node in a linked list
typedef struct node {    
    struct node *next;
    STACK_ITEM_T item;
} StackNode;

// The type definition of our Stack (based on a linked list)
struct Stack {
    // the top element on stack; 
    // i.e., the first element in the linked list
    StackNode *top;    
    // number of elements on stack
    long n;
};

// Test whether the stack is empty
int StackIsEmpty(struct Stack *pStack) {
    return pStack->n == 0;
}

// Create a stack
struct Stack *CreateStack(void) {
    struct Stack *pStack;

    pStack = (struct Stack *) malloc(sizeof(struct Stack));
    assert(pStack != NULL);
    // empty stack
    pStack->n = 0;
    pStack->top = NULL;
    return pStack;
}


/*
    Release the heap space
 
                ----------            ----------         ----------         ----------      
  pStack -----> top         ------>     next      ------>  next      .....   next: NULL                           
                n                       item               item              item          
                ----------            ----------         ----------         ----------  
               struct Stack           StackNode          StackNode           StackNode   
                                        ^                 ^
                                        ^                 ^
                                        ^                 ^
                                        cur               tmp                        
 */
void ReleaseStack(struct Stack *pStack) {
    StackNode *cur = pStack->top;
    while(cur != NULL) {
        StackNode *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(pStack);
}

/*
    Push an item onto/at the top of the stack.
 
                ----------            ----------         ----------         ----------      
  pStack -----> top         ------>     next      ------>  next      .....   next: NULL                           
                n                       item               item              item          
                ----------            ----------         ----------         ----------  
               struct Stack           StackNode          StackNode           StackNode   
                      

                         ----------
          pNode ---->     next    
                          item     
                         ----------
                        StackNode

 */
void StackPush(struct Stack *pStack, STACK_ITEM_T item) {
    StackNode *pNode = (StackNode *) malloc(sizeof(StackNode));
    assert(pNode);
    // save the item in the StackNode
    pNode->item = item;
    // Let the node pointed to by pNode be the first node in the linked list        
    pNode->next = pStack->top;    
    pStack->top = pNode;
    // increase the number of elements by 1
    pStack->n++;
}

/*
   Pop the top item from the stack.
               
   (1)  Before popping
                ----------            ----------         ----------         ----------      
  pStack -----> top         ------>     next      ------>  next      .....   next: NULL                           
                n                       item               item              item          
                ----------            ----------         ----------         ----------  
               struct Stack           StackNode          StackNode           StackNode  
                                       ^
                                       ^
                                       ^
                                StackNode *top  

   (2)  After popping
                ----------                               ----------          ----------      
  pStack ----->   top        ----------------------->     next        .....   next: NULL              
                   n                                      item                item    
                ----------                               ----------          ----------  
               struct Stack                              StackNode            StackNode   
                                    ----------
                                     next     
                                     item     
                                    ----------
                                    StackNode 
                                       ^
                                       ^
                                       ^
                                StackNode *top        
 */
STACK_ITEM_T StackPop(struct Stack *pStack) {
    assert(!StackIsEmpty(pStack));
    // let 'top' point to the first node in the linked list
    StackNode *top = pStack->top;
    // remove the first node from the linked list
    pStack->top = top->next;
    // decrease the number of elements by 1
    pStack->n--;
    // save the data in a local variable on call stack before calling free(pNode)
    STACK_ITEM_T item = top->item;
    // free the heap space
    free(top);  
    return item;
}

// Peek the top item without popping
STACK_ITEM_T StackPeek(struct Stack *pStack) {
    assert(!StackIsEmpty(pStack));

    STACK_ITEM_T item = pStack->top->item;
    return item;
}

void PrintStack(struct Stack *pStack) {
    StackNode *cur = pStack->top;
    printf("Stack: ");
    // free the heap space of all nodes in the queue
    while (cur != NULL) {
        printf("%ld", cur->item);
        cur = cur->next;
        if(cur) {
            printf(" --> ");
        }
    }
    printf("\n");
}
