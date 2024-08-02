#include <assert.h>   // assert()
#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // malloc() and free()
#include <string.h>   // memset()
#include "BiTree.h"

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
    pNode->height = 1;   
    return pNode;
}

#if 0
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
#endif

void ReleaseBinaryTree(BiTreeNodePtr root) {
    if (root) {
        ReleaseBinaryTree(root->leftChild);
        ReleaseBinaryTree(root->rightChild);
        free(root);
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
            fprintf(dotFile, 
                    "\"%s_H=%d_B=%d\" [color=red]\n", 
                    root->value.name, 
                    root->height,
                    BiTreeBalanceFactor(root));            
        } else {
            fprintf(dotFile, 
                    "\"%s_H=%d_B=%d\"\n", 
                    root->value.name, 
                    root->height,
                    BiTreeBalanceFactor(root));             
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
        long hiddenNodeCnt = 0;
        char *hiddenNodePrefix = "HD";
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                BiTreeNodePtr curNode = QueueDequeue(pQueue);
                if (!curNode->leftChild && !curNode->rightChild) {
                    continue;
                }

                if (curNode->leftChild) {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s_H=%d_B=%d\"} [label=\"L\"]\n",
                            curNode->value.name,
                            curNode->height,
                            BiTreeBalanceFactor(curNode),
                            edgeConnectorStr,
                            curNode->leftChild->value.name,
                            curNode->leftChild->height,
                            BiTreeBalanceFactor(curNode->leftChild));
                    QueueEnqueue(pQueue, curNode->leftChild);
                } else {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s%ld\"} [label=\"L\"] [style=invis]\n",
                            curNode->value.name,
                            curNode->height,
                            BiTreeBalanceFactor(curNode),
                            edgeConnectorStr,
                            hiddenNodePrefix,
                            hiddenNodeCnt);
                    fprintf(dotFile, "\"%s%ld\" [style=invis]\n", hiddenNodePrefix, hiddenNodeCnt);
                    hiddenNodeCnt++;
                }

                if (curNode->rightChild) {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s_H=%d_B=%d\"} [label=\"R\"]\n",                        
                            curNode->value.name,
                            curNode->height, 
                            BiTreeBalanceFactor(curNode),
                            edgeConnectorStr,
                            curNode->rightChild->value.name,
                            curNode->rightChild->height,
                            BiTreeBalanceFactor(curNode->rightChild));
                    QueueEnqueue(pQueue, curNode->rightChild);
                } else {
                    fprintf(dotFile, "\"%s_H=%d_B=%d\" %s {\"%s%ld\"} [label=\"R\"] [style=invis]\n",
                            curNode->value.name,
                            curNode->height,
                            BiTreeBalanceFactor(curNode),
                            edgeConnectorStr,
                            hiddenNodePrefix,
                            hiddenNodeCnt);
                    fprintf(dotFile, "\"%s%ld\" [style=invis]\n", hiddenNodePrefix, hiddenNodeCnt);
                    hiddenNodeCnt++;
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

/////////////////////////////////// Binary Search Tree ///////////////////////////////////////////

int GetMax(int h1, int h2) {
    return (h1 > h2) ? h1: h2;
}

int BiTreeHeight(BiTreeNodePtr root) {
    if (root) {
        return root->height;
    } else {
        return 0;
    }
}

static void UpdateHeight(BiTreeNodePtr root) {
    if (root) {
        int max = GetMax(BiTreeHeight(root->leftChild), BiTreeHeight(root->rightChild));
        root->height = max + 1;
    }
}

int BiTreeBalanceFactor(BiTreeNodePtr root) {
    if (root) {        
        return BiTreeHeight(root->leftChild) - BiTreeHeight(root->rightChild);
    } else {
        return 0;
    }
}

/*
------------------------------------------------------------------------
  Before rotation                           After right rotation
------------------------------------------------------------------------                                         
      *pNodePtr                                *pNodePtr
          .                                        .
          .                                        .
          .                                        .
          V                                        V
        NodeC                                    NodeA      
       /                                         /    \
    NodeA                                     Node0   NodeC    
    /   \                                             /
 Node0   NodeB (or NULL)                            NodeB (or NULL)      
------------------------------------------------------------------------        
 */
void BiTreeRightRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeC = *pNodePtr;
    BiTreeNodePtr pNodeA = pNodeC->leftChild;
    BiTreeNodePtr pNodeB = pNodeA->rightChild;

    pNodeA->rightChild = pNodeC;
    pNodeC->leftChild = pNodeB;
    // NodeC's height should be updated before NodeA's height
    UpdateHeight(pNodeC);
    UpdateHeight(pNodeA);
    // Let *pNodePtr point to NodeA
    *pNodePtr = pNodeA;
}

/*
-----------------------------------------------------------------------
  Before rotation                After left rotation
-----------------------------------------------------------------------                              
      *pNodePtr                     *pNodePtr
          .                             .
          .                             .
          .                             .
          V                             V
        NodeA                         NodeC      
            \                         /   \
           NodeC                  NodeA   Node0
           /   \                      \
       NodeB   Node0                  NodeB 
     (or NULL)                        (or NULL)  
-----------------------------------------------------------------------        
 */
void BiTreeLeftRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeA = *pNodePtr;
    BiTreeNodePtr pNodeC = pNodeA->rightChild;
    BiTreeNodePtr pNodeB = pNodeC->leftChild;

    pNodeC->leftChild = pNodeA;
    pNodeA->rightChild = pNodeB;
    // NodeA's height should be updated before NodeC's height
    UpdateHeight(pNodeA);
    UpdateHeight(pNodeC);
    // Let *pNodePtr point to NodeC
    *pNodePtr = pNodeC;
}

/*
    Rotate the subtree pointed to by *pNodePtr when necessary.

    pRoot and pCnt are used in generating images for visualizing the algorithms.

    We define:

    The balance factor of a tree node:

        BalanceFactor(pNode) = Height(pNode->left) - Height(pNode->right)

    The height of a empty tree (pNode == NULL) is 0.
    BTW, some people define the height of the node (pointed to by pNode) to be -1 when pNode is NULL.

    A tree node is left-heavy when its balance factor is larger than 0  (bf > 0).
    A tree node is right-heavy when its balance factor is smaller than 0 (bf < 0).
 */

static void BiTreeSelfBalance(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long *pCnt, char *graphName, char *fileName) {
//static void BiTreeSelfBalance(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pNode);

    // calculate its balance factor
    int bFactor = BiTreeBalanceFactor(pNode);

    if (bFactor > 1 && BiTreeBalanceFactor(pNode->leftChild) >= 0) {
        /*
            Left-Left Case:  the unbalanced node is left-heavy, and its left child is left-heavy or in-balance

                             Suppose NodeC has a right child, NodeD.
                             When NodeD is deleted from the AVL Tree, NodeC becomes unbalanced.
                             But NodeC's left child (NodeA) can be in-balance when NodeB exists.

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeC  (unbalanced node)
                           /    
                        NodeA  
                        /   \
                    Node0   NodeB (or NULL)  
         */
        printf("Left-Left Case: Right Rotate\n");           
        BiTreeRightRotate(pNodePtr);
    }
    else if (bFactor < -1 && BiTreeBalanceFactor(pNode->rightChild) <= 0) {
        /*
            Right-Right Case: the unbalanced node is right-heavy, and its right child is right-heavy or in-balance

                    *pNodePtr
                        .
                        .
                        .
                        V
                      NodeA  (unbalanced node)  
                         \
                        NodeC         
                        /   \
                    NodeB   Node0
                  (or NULL)        
         */
        printf("Right-Right Case: Left Rotate\n");  
        BiTreeLeftRotate(pNodePtr);
    }
    else if (bFactor > 1 && BiTreeBalanceFactor(pNode->leftChild) < 0) {
        /*
            Left-Right Case:  the unbalanced node is left-heavy, and its left child is right-heavy

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeC   (unbalanced) 
                           /    
                        NodeA     
                           \
                          NodeB 

         */
        printf("Left-Right Case: Left Rotate + Right Rotate\n");  
        BiTreeLeftRotate(&(pNode->leftChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);   
        BiTreeRightRotate(pNodePtr);
    }
    else if (bFactor < -1 && BiTreeBalanceFactor(pNode->rightChild) > 0) {
        /*
            Right-Left Case:  the unbalanced node is right-heavy, and its right child is left-heavy

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeA  (unbalanced)   
                             \
                            NodeC   
                            /   
                        NodeB                                     
         */
        printf("Right-Left Case: Right Rotate + Left Rotate\n");  
        BiTreeRightRotate(&(pNode->rightChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);          
        BiTreeLeftRotate(pNodePtr);
    }     
}

void BiTreeInsert(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {  
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pCnt);
    char *graphName = "BiTreeBiTreeInsert";
    char *fileName = "images/BiTreeBiTreeInsert";
    if (pNode == NULL) {
        BiTreeNodePtr tmp = CreateBinaryTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;
        printf("inserting %ld\n", numVal);
        return;
    } else {
        // on stack
        pNode->visited = 1;
        if (numVal < pNode->value.numVal) {
            BiTreeInsert(pRoot, &pNode->leftChild, numVal, nodeName, pCnt);
        } else if (numVal > pNode->value.numVal) {
            BiTreeInsert(pRoot, &pNode->rightChild, numVal, nodeName, pCnt);
        } else {
            // If numVal is already in the binary search tree, do nothing.
            // off stack
            pNode->visited = 0;
            return;
        }

        // recalculate and store its height
        pNode->height = 1 + GetMax(BiTreeHeight(pNode->leftChild), BiTreeHeight(pNode->rightChild));

        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);        

        BiTreeSelfBalance(pRoot, pNodePtr, pCnt, graphName, fileName);
        // off stack
        pNode->visited = 0;

#if 0        

        pNode->height = 1 + GetMax(BiTreeHeight(pNode->leftChild), BiTreeHeight(pNode->rightChild));

        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt); 

        int bFactor = BiTreeBalanceFactor(pNode);

        if (bFactor > 1 && numVal < pNode->leftChild->value.numVal) {
            printf("Left Left Case: Right Rotate\n");           
            BiTreeRightRotate(pNodePtr);
        }
        else if (bFactor < -1 && numVal > pNode->rightChild->value.numVal) {
            printf("Right Right Case: Left Rotate\n");
            BiTreeLeftRotate(pNodePtr);
        }
        else if (bFactor > 1 && numVal > pNode->leftChild->value.numVal) {
            printf("Left Right Case: Left Rotate + Right Rotate\n");  
            BiTreeLeftRotate(&(pNode->leftChild));
            BiTreeRightRotate(pNodePtr);

        }
        else if (bFactor < -1 && numVal < pNode->rightChild->value.numVal) {
            printf("Right Left Case: Right Rotate + Left Rotate\n"); 
            BiTreeRightRotate(&(pNode->rightChild));
            BiTreeLeftRotate(pNodePtr);
        }
#endif        
    }
}

#if 0
BiTreeNodePtr BiTreeSearch(BiTreeNodePtr root, long numVal) {
    if (______Q1______) {
        return ______Q2______;
    } else if (numVal == root->value.numVal) {
        return ______Q3______;
    } else if (numVal < root->value.numVal) {
        return ______Q4______;
    } else { // numVal > root->value.numVal
        return ______Q5______;
    }
}
#endif

BiTreeNodePtr BiTreeMinValueNode(BiTreeNodePtr root) {
    BiTreeNodePtr cur = root;
    // Get the left-most node
    while ((cur != NULL) && (cur->leftChild != NULL)) {
        cur = cur->leftChild;
    }
    return cur;
}




// The parameter pRoot is only used for generating the image of the binary search tree.
// In this recursive function, *pNodePtr might point to a sub-tree in the BST.
void BiTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    //static long cnt = 0;
    assert(pCnt);
    char *graphName = "BiTreeDelete";
    char *fileName = "images/BiTreeDelete";
    BiTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        // It is on stack
        pNode->visited = 1;
        if (numVal < pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->leftChild), numVal, pCnt);
        } else if (numVal > pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->rightChild), numVal, pCnt);
        } else {
            /************************************************************************
                If the node (to be deleted) has:

                    0 child:

                        leftChild == NULL && rightChild == NULL    // case 00

                    1 child:

                        leftChild == NULL && rightChild != NULL    // case 01

                        or 
                        leftChild != NULL && rightChild == NULL    // case 10
                 
                    2 children:

                        leftChild != NULL && rightChild != NULL    // case 11

             **************************************************************************/
            
            if (pNode->leftChild == NULL) {   // case 00 and case 01
                BiTreeNodePtr tmp = pNode->rightChild;
                printf("deleting %ld\n", pNode->value.numVal);
                free(pNode);
                *pNodePtr = tmp;                
            } else if (pNode->rightChild == NULL) { // case 10
                BiTreeNodePtr tmp = pNode->leftChild;
                printf("deleting %ld\n", pNode->value.numVal);      
                free(pNode);
                *pNodePtr = tmp;              
            } else {
                // case 11:  with two children
                // Get pNode's in-order successor, which is left-most node in its right sub-tree.
                BiTreeNodePtr pSuccessor = BiTreeMinValueNode(pNode->rightChild);

                // (Swapping is done for clearer debugging output)
                // Swap the values of the node pointed to by pNode and its in-order successor              
                NodeValue val = pNode->value;
                // Copy the successor's value (this copy is necessary)
                pNode->value = pSuccessor->value;
                pSuccessor->value = val;

                // Display the inconsistent state
                (*pCnt)++;
                GenOneImage(*pRoot, graphName, fileName, *pCnt);
                
                // Now, numVal is in right sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                BiTreeDelete(pRoot, &pNode->rightChild, pSuccessor->value.numVal, pCnt);
            }
        }
        // 
        pNode = *pNodePtr;
        // If it is NULL, just return.
        if (pNode == NULL) {            
            return;
        }

        // recalculate and store its height
        pNode->height = 1 + GetMax(BiTreeHeight(pNode->leftChild), BiTreeHeight(pNode->rightChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);        

        BiTreeSelfBalance(pRoot, pNodePtr, pCnt, graphName, fileName);
        // It is not on stack
        pNode->visited = 0;    
    }
}

