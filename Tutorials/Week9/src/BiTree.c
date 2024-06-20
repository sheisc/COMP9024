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
            fprintf(dotFile, "\"%s\" [color=red]\n", root->value.name);            
        }
        DisplayVisited(dotFile, root->leftChild);
        DisplayVisited(dotFile, root->rightChild);
    }
}

/*
    Dot Files

    We simple assume each node has a distinct key value.

   
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

/////////////////////////////////// Binary Search Tree ///////////////////////////////////////////

void BiTreeInsert(BiTreeNodePtr *pNodePtr, long numVal, char *nodeName) {  
    BiTreeNodePtr pNode = *pNodePtr;
    if (pNode == NULL) {
        BiTreeNodePtr tmp = CreateBinaryTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;
    } else {
        if (numVal < pNode->value.numVal) {
            BiTreeInsert(&pNode->leftChild, numVal, nodeName);
        } else if (numVal > pNode->value.numVal) {
            BiTreeInsert(&pNode->rightChild, numVal, nodeName);
        } else {
            // If numVal is already in the binary search tree, do nothing.
        }
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

void BiTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal) {
    static long cnt = 0;

    BiTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        if (numVal < pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->leftChild), numVal);
        } else if (numVal > pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->rightChild), numVal);
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

                cnt++;
                GenOneImage(*pRoot, "BiTreeDelete", "images/BiTreeDelete", cnt);
            } else if (pNode->rightChild == NULL) { // case 10
                BiTreeNodePtr tmp = pNode->leftChild;
                printf("deleting %ld\n", pNode->value.numVal);      
                free(pNode);
                *pNodePtr = tmp;

                cnt++;
                GenOneImage(*pRoot, "BiTreeDelete", "images/BiTreeDelete", cnt);                
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
                cnt++;
                GenOneImage(*pRoot, "BiTreeDelete", "images/BiTreeDelete", cnt);
                // Now, numVal is in right sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                BiTreeDelete(pRoot, &pNode->rightChild, pSuccessor->value.numVal);
            }
        }
    }
}

