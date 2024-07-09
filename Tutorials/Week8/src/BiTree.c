#include <assert.h>   // assert()
#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // malloc() and free()
#include <string.h>   // memset()
#include "BiTree.h"
#include "Stack.h"
#include "Queue.h"

/*
  Create an Ast node for an expression.
 */
BiTreeNodePtr CreateBinaryTreeNode(long numVal, char *nodeName, BiTreeNodePtr left, BiTreeNodePtr right) {
    BiTreeNodePtr pNode = (BiTreeNodePtr) malloc(sizeof(struct BiTreeNode));
    assert(pNode != NULL);

    memset(pNode, 0, sizeof(*pNode));
    pNode->value.numVal = numVal;
    // If nodeName is not specified, we use the string representation of numVal as its node name.
    if (nodeName == NULL) {
        snprintf(pNode->value.name, MAX_ID_LEN, "%ld", numVal);
    } else {
        snprintf(pNode->value.name, MAX_ID_LEN, "%s", nodeName);
    }
    pNode->leftChild = left;
    pNode->rightChild = right;
    pNode->state = NS_FROM_UP;
    pNode->visited = 0;   
    return pNode;
}

BiTreeNodePtr CreateBinaryTree(void) {
    /*********************************  

           100
          /   \
        98    101
       /  \
     97    99

     *********************************/
#if 1
    // To be simple, let's manually create the binary tree. 
    BiTreeNodePtr leftLeft = CreateBinaryTreeNode(97, NULL, NULL, NULL);
    BiTreeNodePtr leftRight = CreateBinaryTreeNode(99, NULL, NULL, NULL);
    BiTreeNodePtr left = CreateBinaryTreeNode(98, NULL, leftLeft, leftRight);
    BiTreeNodePtr right = CreateBinaryTreeNode(101, NULL, NULL, NULL);
    BiTreeNodePtr root = CreateBinaryTreeNode(100, NULL, left, right);
#else
    // To be simple, let's manually create the binary tree. 
    BiTreeNodePtr leftLeft = CreateBinaryTreeNode(97, "Node A", NULL, NULL);
    BiTreeNodePtr leftRight = CreateBinaryTreeNode(99, "Node C", NULL, NULL);
    BiTreeNodePtr left = CreateBinaryTreeNode(98, "Node B", leftLeft, leftRight);
    BiTreeNodePtr right = CreateBinaryTreeNode(101, "Node E", NULL, NULL);
    BiTreeNodePtr root = CreateBinaryTreeNode(100, "Node D", left, right);
#endif
    return root;
}

void ReleaseBinaryTree(BiTreeNodePtr root) {
    if (root) {
        ReleaseBinaryTree(root->leftChild);
        ReleaseBinaryTree(root->rightChild);
        free(root);
    }
}

void ResetNodeState(BiTreeNodePtr root) {
    if (root) {
        ResetNodeState(root->leftChild);
        ResetNodeState(root->rightChild);
        root->visited = 0;
    }
}


void PreOrderTraversal(BiTreeNodePtr root, NodeVisitor visit) {
    if (root) {
        visit(root);
        PreOrderTraversal(root->leftChild, visit);    
        PreOrderTraversal(root->rightChild, visit);
    }
}


void InOrderTraversal(BiTreeNodePtr root, NodeVisitor visit) {
    if (root) {
        InOrderTraversal(root->leftChild, visit);
        visit(root);
        InOrderTraversal(root->rightChild, visit);
    }
}

void PostOrderTraversal(BiTreeNodePtr root, NodeVisitor visit) {
    if (root) {
        PostOrderTraversal(root->leftChild, visit);    
        PostOrderTraversal(root->rightChild, visit);
        visit(root);
    }
}

void PreOrderTraversal2(BiTreeNodePtr root, NodeVisitor visit) {
    static long cnt = 0;    

    if (root) {
        struct Stack *pStack = CreateStack();
        root->state = NS_FROM_UP;

        GenOneImage(root, "PreOrderTraversal2", "images/PreOrderTraversal2", cnt);

        StackPush(pStack, root);
        while (!StackIsEmpty(pStack)) {
            BiTreeNodePtr curNode = StackPeek(pStack);
            switch (curNode->state) {
                case NS_FROM_UP:
                    visit(curNode);
                    cnt++;
                    GenOneImage(root, "PreOrderTraversal2", "images/PreOrderTraversal2", cnt);        
                    // When curNode becomes the top node again, it means we have visited its left sub-tree.
                    curNode->state = NS_FROM_LEFT;
                    // Push its left child (if existing)
                    if (curNode->leftChild) {
                        curNode->leftChild->state = NS_FROM_UP;
                        StackPush(pStack, curNode->leftChild);
                    }
                    break;
                case NS_FROM_LEFT:
                    // When curNode becomes the top node again, it means we have visited its right sub-tree.
                    curNode->state = NS_FROM_RIGHT;
                    if (curNode->rightChild) {
                        curNode->rightChild->state = NS_FROM_UP;
                        StackPush(pStack, curNode->rightChild);
                    }
                    break;
                case NS_FROM_RIGHT:
                    // We can pop the node now
                    StackPop(pStack);
                    break;
                default:
                    break;
            }
        }
        ReleaseStack(pStack);
    }
} 

#if 0

/*
    Please complete the code in Q1-Q5.

    Non-recursive in-order traversal.
 */
void InOrderTraversal2(BiTreeNodePtr root, NodeVisitor visit) {
    if (root) {
        struct Stack *pStack = CreateStack();
        root->state = NS_FROM_UP;

        StackPush(pStack, root);
        while (!StackIsEmpty(pStack)) {
            BiTreeNodePtr curNode = StackPeek(pStack);
            
            switch (curNode->state) {
                case NS_FROM_UP:
                    ______Q1______;
                    if (curNode->leftChild) {
                        ______Q2______;
                        StackPush(pStack, curNode->leftChild);
                    }
                    break;
                case NS_FROM_LEFT:
                    ______Q3______;
                    ______Q4______;
                    if (curNode->rightChild) {
                        curNode->rightChild->state = NS_FROM_UP;
                        StackPush(pStack, curNode->rightChild);
                    }
                    break;
                case NS_FROM_RIGHT:
                    ______Q5______;
                    break;
                default:
                    break;
            }
        }
        ReleaseStack(pStack);
    }
}
#endif

void PostOrderTraversal2(BiTreeNodePtr root, NodeVisitor visit) {
    static long cnt = 0;

    if (root) {
        struct Stack *pStack = CreateStack();
        root->state = NS_FROM_UP;

        GenOneImage(root, "PostOrderTraversal2", "images/PostOrderTraversal2", cnt);

        StackPush(pStack, root);
        while (!StackIsEmpty(pStack)) {
            BiTreeNodePtr curNode = StackPeek(pStack);
            switch (curNode->state) {
                case NS_FROM_UP:
                    // When curNode becomes the top node again, it means we have visited its left sub-tree.          
                    curNode->state = NS_FROM_LEFT;
                    // Push its left child (if existing)
                    if (curNode->leftChild) {
                        curNode->leftChild->state = NS_FROM_UP;
                        StackPush(pStack, curNode->leftChild);
                    }
                    break;
                case NS_FROM_LEFT:
                    // when curNode becomes the top node again, it means we have visited its right sub-tree.
                    curNode->state = NS_FROM_RIGHT;
                    // Push its right child (if existing)
                    if (curNode->rightChild) {
                        curNode->rightChild->state = NS_FROM_UP;
                        StackPush(pStack, curNode->rightChild);
                    }
                    break;
                case NS_FROM_RIGHT:
                    visit(curNode);
                    cnt++;
                    GenOneImage(root, "PostOrderTraversal2", "images/PostOrderTraversal2", cnt);
                    // We can pop the node now
                    StackPop(pStack);
                    break;
                default:
                    break;
            }
        }
        ReleaseStack(pStack);
    }
}


#define FILE_NAME_LEN  255

void GenOneImage(BiTreeNodePtr root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    BiTree2Dot(root, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 

}

static void DisplayVisited(FILE *dotFile, BiTreeNodePtr root) {
    if (root) {
        if (root->visited) {
            fprintf(dotFile, "\"%s\" [color=red]\n", root->value.name);            
        }
        DisplayVisited(dotFile, root->leftChild);
        DisplayVisited(dotFile, root->rightChild);
    }
}


/*
    Dot Files

    We assume each node has a distinct key value.

   
          100
         /   \
       98    101
      /  \
    97    99
    

    digraph OutBiTree {    
    "100" -> {"98"} [label="L"]
    "100" -> {"101"} [label="R"]
    "98" -> {"97"} [label="L"]
    "98" -> {"99"} [label="R"]
    "97" [color=red]
    }  

    Force the left to right order of nodes.

    https://stackoverflow.com/questions/29864726/force-the-left-to-right-order-of-nodes-in-graphviz
 */

void BiTree2Dot(BiTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        
        struct Queue *pQueue = CreateQueue();
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                BiTreeNodePtr curNode = QueueDequeue(pQueue);
                if (curNode->leftChild) {
                    fprintf(dotFile, "\"%s\" %s {\"%s\"} [label=\"L\"]\n",
                            curNode->value.name,
                            edgeConnectorStr,                         
                            curNode->leftChild->value.name);
                    QueueEnqueue(pQueue, curNode->leftChild);
                }
                if (curNode->rightChild) {
                    fprintf(dotFile, "\"%s\" %s {\"%s\"} [label=\"R\"]\n",                        
                            curNode->value.name, 
                            edgeConnectorStr,
                            curNode->rightChild->value.name);
                    QueueEnqueue(pQueue, curNode->rightChild);
                }            
                
            }
        }
        ReleaseQueue(pQueue);
        /*
        "0" [color=red]
         */
        if (displayVisited) {
            DisplayVisited(dotFile, root);
        }        
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}
