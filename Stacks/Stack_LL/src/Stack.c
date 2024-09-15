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


////////////////////////////// Stack2Dot (for visualizing the algorithm) ///////////////////////////////////////

static void PrintOneEdge(FILE *dotFile, struct Stack *pStack, StackNode *src, StackNode *dst) {
    fprintf(dotFile, "\"%p\" -> \"%p\" \n", src, dst);
}

static void PrintOneNode(FILE *dotFile, struct Stack *pStack, StackNode *cur) {
    fprintf(dotFile, "\"%p\" [label=\"%ld\"] [shape=box] \n", cur, (long) cur->item);
}

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
                
        StackNode *cur = pStack->top;
        StackNode *pseudoStackNode = (StackNode *) (-8);
        fprintf(dotFile, "\"%p\" [label=\"n=%ld\"]  \n", pseudoStackNode, pStack->n);
        if (cur) {
            fprintf(dotFile, "\"%p\" -> \"%p\" [style=invis] \n", pseudoStackNode, cur);
        }
        // 
        while (cur) {
            // test whether there is a successor
            if (cur->next) {
                PrintOneEdge(dotFile, pStack, cur, cur->next);
            }
            // draw the current node
            PrintOneNode(dotFile, pStack, cur);         
            // draw the top index
            if (cur == pStack->top) {
                StackNode *pseudoTopNode = (StackNode *) (-16);
                fprintf(dotFile, "\"%p\" -> \"%p\" [color=orange] \n", pseudoTopNode, cur);
                fprintf(dotFile, "\"%p\" [label=\"top\"] [shape=hexagon] \n", pseudoTopNode);                
            }
            cur = cur->next;
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
