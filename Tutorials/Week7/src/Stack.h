#ifndef STACK_H
#define STACK_H

// Used to save the node id of a graph in Week7
typedef long STACK_ITEM_T;



struct Stack;

struct Stack *CreateStack(void);

void ReleaseStack(struct Stack *pStack);

void StackPush(struct Stack *pStack, STACK_ITEM_T item);

STACK_ITEM_T StackPop(struct Stack *pStack);

STACK_ITEM_T StackPeek(struct Stack *pStack);

int StackIsEmpty(struct Stack *pStack);

// For debugging
void PrintStack(struct Stack *pStack);

#endif


