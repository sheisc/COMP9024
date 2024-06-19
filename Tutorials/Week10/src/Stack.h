#ifndef STACK_H
#define STACK_H
#include "Maze.h"

typedef struct PositionInfo *STACK_ITEM_T;



struct Stack;

struct Stack *CreateStack(void);

void ReleaseStack(struct Stack *pStack);

void StackPush(struct Stack *pStack, STACK_ITEM_T item);

STACK_ITEM_T StackPop(struct Stack *pStack);

STACK_ITEM_T StackPeek(struct Stack *pStack);

int StackIsEmpty(struct Stack *pStack);

#endif


