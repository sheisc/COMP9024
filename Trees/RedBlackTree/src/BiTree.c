#include <assert.h>   // assert()
#include <stdio.h>    // printf() and snprintf()
#include <stdlib.h>   // malloc() and free()
#include <string.h>   // memset()
#include "BiTree.h"
#include "Queue.h"

// whether to use the in-order successor or predecessor in red-black tree deletion
//#define  SWAP_IN_ORDER_SUCCESSOR  

// whether to generate intermediate images in inserting or deleting nodes
#define ENABLE_GEN_INTERMEDIATE_IMAGES 

#ifdef  ENABLE_GEN_INTERMEDIATE_IMAGES
    #define GEN_ONE_IMAGE()  do { \
        (*pCnt)++; \
        RBTreeGenOneImage(*pRoot, graphName, fileName, *pCnt); \
    } while (0)
#else
    #define GEN_ONE_IMAGE()  do { \
    } while (0)
#endif


/*
   RedParentRedUncle_RXXX

            Grandparent                
             /     \
            /       \
    Red Parent     Red Uncle         
        /
       /
   Red Child (inserted)
    
 */
typedef enum{
    // R + R 
    RedParentRedUncle_RXXX,         // red parent, red uncle, left red child    
    RedParentRedUncle_XRXX,         // red parent, red uncle, right red child
    RedUncleRedParent_XXRX,         // red uncle, red parent, left red child
    RedUncleRedParent_XXXR,         // red uncle, red parent, right red child
    // R + B
    RedParentBlackUncle_RXXX,       // red parent, black uncle, left red child
    RedParentBlackUncle_XRXX,       // red parent, black uncle, right red child
    BlackUncleRedParent_XXRX,       // black uncle, red parent, left red child
    BlackUncleRedParent_XXXR,       // black uncle, red parent, right red child   
    // other
    OtherRBState,
} RBTreeNodeState;

static void SetTreeNodeColor(BiTreeNodePtr pNode, NodeColor color) {
    pNode->color = color;
}

int hasRedLeft(BiTreeNodePtr pNode) {
    return pNode->leftChild && pNode->leftChild->color == RED;
}

int hasBlackLeft(BiTreeNodePtr pNode) {
    return (!pNode->leftChild) || (pNode->leftChild->color == BLACK);
}

int hasRedRight(BiTreeNodePtr pNode) {
    return pNode->rightChild && pNode->rightChild->color == RED;
}

int hasBlackRight(BiTreeNodePtr pNode) {
    return (!pNode->rightChild) || (pNode->rightChild->color == BLACK);
}

static RBTreeNodeState GetRBTreeNodeState(BiTreeNodePtr pNode) {
    assert(pNode);

    if (hasRedLeft(pNode) && hasRedRight(pNode)) {
        if (hasRedLeft(pNode->leftChild)) {
            return RedParentRedUncle_RXXX;
        } else if (hasRedRight(pNode->leftChild)) {
            return RedParentRedUncle_XRXX;
        } else if (hasRedLeft(pNode->rightChild)) {
            return RedUncleRedParent_XXRX;
        } else if (hasRedRight(pNode->rightChild)) {
            return RedUncleRedParent_XXXR;
        }
    } else if (hasRedLeft(pNode) && hasBlackRight(pNode)) {
        if (hasRedLeft(pNode->leftChild)) {
            return RedParentBlackUncle_RXXX;
        } else if (hasRedRight(pNode->leftChild)) {
            return RedParentBlackUncle_XRXX;
        }
    } else if (hasBlackLeft(pNode) && hasRedRight(pNode)) {
        if (hasRedLeft(pNode->rightChild)) {
            return BlackUncleRedParent_XXRX;
        } else if (hasRedRight(pNode->rightChild)) {
            return BlackUncleRedParent_XXXR;
        }
    }
    return OtherRBState;
}

/////////////////////////////////// Binary Search Tree ///////////////////////////////////////////

static int GetMax(int h1, int h2) {
    return (h1 > h2) ? h1: h2;
}

static int RBTreeBlackHeight(BiTreeNodePtr root) {
    if (root) {
        return root->blackHeight;
    } else {
        return 0;
    }
}

static void UpdateBlackHeight(BiTreeNodePtr root) {
    if (root) {
        int max = GetMax(RBTreeBlackHeight(root->leftChild), RBTreeBlackHeight(root->rightChild));
        if (root->color == BLACK) {
            root->blackHeight = max + 1;
        } else {
            root->blackHeight = max;
        }
    }
}

int BiTreeBalanceFactor(BiTreeNodePtr root) {
    if (root) {        
        return RBTreeBlackHeight(root->leftChild) - RBTreeBlackHeight(root->rightChild);
    } else {
        return 0;
    }
}

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
    pNode->blackHeight = 0;
    SetTreeNodeColor(pNode, RED);
    return pNode;
}

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
static void BiTreeRightRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeC = *pNodePtr;
    BiTreeNodePtr pNodeA = pNodeC->leftChild;
    BiTreeNodePtr pNodeB = pNodeA->rightChild;

    pNodeA->rightChild = pNodeC;
    pNodeC->leftChild = pNodeB;
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
static void BiTreeLeftRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeA = *pNodePtr;
    BiTreeNodePtr pNodeC = pNodeA->rightChild;
    BiTreeNodePtr pNodeB = pNodeC->leftChild;

    pNodeC->leftChild = pNodeA;
    pNodeA->rightChild = pNodeB;
    *pNodePtr = pNodeC;
}

/*
    The unbalance might be propagated upward.
 */
static void FixViolationsInInsertion(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long *pCnt, char *graphName, char *fileName) {
//static void BiTreeSelfBalance(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pNode);

    RBTreeNodeState state = GetRBTreeNodeState(pNode);
    switch (state) {
        case RedParentRedUncle_RXXX:
        case RedParentRedUncle_XRXX:
        case RedUncleRedParent_XXRX:
        case RedUncleRedParent_XXXR:
            /*
            RedParentRedUncle_RXXX:
            (Other three cases omitted)

            -----------------------------------------------------------------------
                    Initial                        After recoloring
            -----------------------------------------------------------------------
            
                 (*pNodePtr)                         (*pNodePtr)            
                     |                                   |                
                     |                                   |                
                     V                                   V                   
                    Node (B)                         Node (R)
                  /      \                            /      \                      
                 /        \                          /        \                      
             Red Parent    Red Uncle         Red Parent (B)    Red Uncle (B)              
               /                                   /                                  
              /                                   /                                      
            Red Child (inserted)            Red Child 
            
            -----------------------------------------------------------------------
             */
            SetTreeNodeColor(pNode, RED);
            SetTreeNodeColor(pNode->leftChild, BLACK);
            SetTreeNodeColor(pNode->rightChild, BLACK);

            UpdateBlackHeight(pNode->leftChild);
            UpdateBlackHeight(pNode->rightChild);
            UpdateBlackHeight(pNode);
            break;
        case RedParentBlackUncle_RXXX:
            /*
            -----------------------------------------------------------------------
                  Initial                   After right rotation and recoloring
            -----------------------------------------------------------------------

                 (*pNodePtr)                         (*pNodePtr)            
                     |                                   |                
                     |                                   |                
                     V                                   V                   
                   Node (B)                          Red Parent (B)
                  /      \                            /      \                      
                 /        \                          /        \                      
            Red Parent   Black Uncle         Red Child      Node (R)              
               /         (null)                                  \                     
              /                                                   \                      
            Red Child                                        Black Uncle (null)                        
            (inserted)
            -----------------------------------------------------------------------
             */ 
            BiTreeRightRotate(pNodePtr);

            SetTreeNodeColor(*pNodePtr, BLACK);
            SetTreeNodeColor((*pNodePtr)->rightChild, RED);

            UpdateBlackHeight((*pNodePtr)->rightChild);
            UpdateBlackHeight(*pNodePtr);
            break;                
        case RedParentBlackUncle_XRXX: 
            /*

            ------------------------------------------------------------------------------------------------------
                  Initial                      After left rotation         After right rotation and recoloring
            ------------------------------------------------------------------------------------------------------

                 (*pNodePtr)                        (*pNodePtr)                      (*pNodePtr)            
                     |                                  |                                |                
                     |                                  |                                |                
                     V                                  V                                V                   
                   Node (B)                           Node (B)                       Red Child (B)
                  /      \                           /      \                         /      \                      
                 /        \                         /        \                       /        \                      
            Red Parent   Black Uncle           Red Child   Black Uncle      Red Parent      Node (R)              
                 \       (null)                   /         (null)                               \                     
                  \                              /                                                \                      
                  Red Child                 Red Parent                                     Black Uncle (null)                        
                  (inserted)
            ------------------------------------------------------------------------------------------------------
            
             */
            BiTreeLeftRotate(&pNode->leftChild);
            GEN_ONE_IMAGE();

            BiTreeRightRotate(pNodePtr);

            SetTreeNodeColor(*pNodePtr, BLACK);
            SetTreeNodeColor((*pNodePtr)->rightChild, RED);

            UpdateBlackHeight((*pNodePtr)->rightChild);
            UpdateBlackHeight(*pNodePtr);                
            break;
        case BlackUncleRedParent_XXRX:
            /*
            ------------------------------------------------------------------------------------------------------
                  Initial                       After right rotation          After left rotation and recoloring
            ------------------------------------------------------------------------------------------------------

                 (*pNodePtr)                        (*pNodePtr)                      (*pNodePtr)            
                     |                                  |                                |                
                     |                                  |                                |                
                     V                                  V                                V                   
                    Node (B)                           Node (B)                        Red Child (B)
                  /      \                           /      \                          /      \                      
                 /        \                         /        \                        /        \                      
            Black Uncle   Red Parent           Black Uncle   Red Child             Node (R)    Red Parent             
             (null)       /                      (null)        \                    /                                
                         /                                      \                  /                                   
                     Red Child                                Red Parent      Black Uncle                               
                     (inserted)                                                  (null)            
            ------------------------------------------------------------------------------------------------------                                              
                           
             */ 
            BiTreeRightRotate(&pNode->rightChild);
            GEN_ONE_IMAGE();

            BiTreeLeftRotate(pNodePtr);

            SetTreeNodeColor(*pNodePtr, BLACK);
            SetTreeNodeColor((*pNodePtr)->leftChild, RED);

            UpdateBlackHeight((*pNodePtr)->leftChild);
            UpdateBlackHeight(*pNodePtr);                
            break;
        case BlackUncleRedParent_XXXR:
            /*
            -----------------------------------------------------------------------
                  Initial                    After left rotation and recoloring
            -----------------------------------------------------------------------

                 (*pNodePtr)                         (*pNodePtr)            
                     |                                   |                
                     |                                   |                
                     V                                   V                   
                   Node (B)                          Red Parent (B)
                  /      \                            /      \                      
                 /        \                          /        \                      
            Black Uncle   Red Parent           Node (R)      Red Child              
              (null)         \                      /                                
                              \                    /                                      
                           Red Child         Black Uncle (null)                        
                           (inserted)
            -----------------------------------------------------------------------
             */
            BiTreeLeftRotate(pNodePtr);

            SetTreeNodeColor(*pNodePtr, BLACK);
            SetTreeNodeColor((*pNodePtr)->leftChild, RED);

            UpdateBlackHeight((*pNodePtr)->leftChild);
            UpdateBlackHeight(*pNodePtr);                
            break;
        default:
            break;
    } 
}


static void BiTreeInsert(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {  
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pCnt);
#if 1    
    char *graphName = "RBTree";
    char *fileName = "images/RBTree1Insert";
#endif    
    if (pNode == NULL) {
        BiTreeNodePtr tmp = CreateBinaryTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;        
        //printf("inserting %ld\n", numVal);
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
        // The inserted node is RED node. We don't update its black height here.

        GEN_ONE_IMAGE();    
        
        FixViolationsInInsertion(pRoot, pNodePtr, pCnt, graphName, fileName);  

        GEN_ONE_IMAGE();

        // off stack
        pNode->visited = 0;        
    }
}

void RBTreeInsert(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {
    BiTreeInsert(pRoot, pNodePtr, numVal, nodeName, pCnt);
    BiTreeNodePtr pNode = *pNodePtr;
    pNode->color = BLACK;
    UpdateBlackHeight(pNode);
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

#ifdef SWAP_IN_ORDER_SUCCESSOR
static BiTreeNodePtr BiTreeMinValueNode(BiTreeNodePtr root) {
    BiTreeNodePtr cur = root;
    // Get the left-most node
    while ((cur != NULL) && (cur->leftChild != NULL)) {
        cur = cur->leftChild;
    }
    return cur;
}

#else

static BiTreeNodePtr BiTreeMaxValueNode(BiTreeNodePtr root) {
    BiTreeNodePtr cur = root;
    // Get the right-most node
    while ( (cur != NULL) && (cur->rightChild != NULL)) {
        cur = cur->rightChild;
    }
    return cur;
}
#endif

/*
    The unbalance can be fixed in this function.
 */
static void FixRedRightSiblingInDeletion(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, char *graphName, char *fileName, long *pCnt) {
    BiTreeNodePtr pNode = *pNodePtr;
    /*

    -----------------------------------------------------------------------
              Initial                            After left rotation 
    -----------------------------------------------------------------------

              (*pNodePtr)                              (*pNodePtr)            
                  |                                        |                
                  |                                        |                
                  V                                        V             
              Node (B)                                Sibling (R)          
              /      \                                 /        \          
             /        \                               /          \         
    Deleted (B)     Sibling (R)                  Node (B)      Black2      
                     /      \                    /     \                   
                    /        \                  /       \                  
                Black1       Black2       Deleted (B)    Black1     

    =======================================================================
    Black1 and Black2 may have 0, 1, or 2 red children.               

    The red-black tree needs to be rebalanced later.
    -----------------------------------------------------------------------
     */    
    BiTreeLeftRotate(pNodePtr);
    GEN_ONE_IMAGE();
    /*
                            WARNING 

        The structure of the red-black tree has been changed after a rotation.

        (*pNodePtr) and pNode might not point to the same node any more.

     */
    if (hasBlackLeft(pNode->rightChild) && hasBlackRight(pNode->rightChild)) {      
        /*
         
        Case 1:  After left rotation, black sibling (Black1) and two black nephews (e.g, two null nodes) exist 

        -----------------------------------------------------------------------||------------------------------------
                    Initial                        After left rotation (done)  ||           After recoloring
        -----------------------------------------------------------------------||------------------------------------
                                                                               ||
                  (*pNodePtr)                              (*pNodePtr)         ||                 (*pNodePtr)                                          
                      |                                        |               ||                     |                                              
                      |                                        |               ||                     |                                              
                      V                                        V               ||                     V                                           
                  Node (B)                                Sibling (R)          ||                Sibling (B)                                        
                  /      \                                 /        \          ||                 /        \                                        
                 /        \                               /          \         ||                /          \                                       
        Deleted (B)     Sibling (R)                  Node (B)      Black2      ||           Node (B)      Black2                                    
                         /      \                    /     \                   ||           /     \                                                 
                        /        \                  /       \                  ||          /       \                                                
                    Black1       Black2       Deleted (B)    Black1            ||    Deleted (B)   Black1 (R)                                       
                    /  \                                      /  \             ||                    /  \                                                        
                   /    \                                    /    \            ||                   /    \                                                       
                  null  null                                null  null         ||                  null  null                                    
        -----------------------------------------------------------------------||------------------------------------
         */
        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor(pNode->rightChild, RED);

        UpdateBlackHeight(pNode->rightChild);
        UpdateBlackHeight(pNode);        
        UpdateBlackHeight(*pNodePtr);
    } else if (hasRedRight(pNode->rightChild)) {
        /*
         Case 2: After left rotation, black sibling (Black1) and the red right nephew node (Red2) exist

        -------------------------------------------------------------------||----------------------------------------
                    Initial                     After left rotation (done) ||    After left rotation and recoloring
        -------------------------------------------------------------------||----------------------------------------
                                                                           ||
                  (*pNodePtr)                            (*pNodePtr)       ||                      (*pNodePtr)                                          
                      |                                      |             ||                          |                                              
                      |                                      |             ||                          |                                              
                      V                                      V             ||                          V                                           
                  Node (B)                              Sibling (R)        ||                     Sibling (B)                                        
                  /      \                               /        \        ||                      /        \                                        
                 /        \                             /          \       ||                     /          \                                       
        Deleted (B)     Sibling (R)                Node (B)      Black2    ||               Black1 (R)      Black2                                    
                         /      \                  /     \                 ||                /     \                                                 
                        /        \                /       \                ||               /       \                                                
                    Black1       Black2     Deleted (B)    Black1          ||         Node (B)      Red2 (B)                                      
                        \                                      \           ||             /                                                           
                         \                                      \          ||            /                                                            
                         Red2                                   Red2       ||       Deleted (B)                                               
        -------------------------------------------------------------------||----------------------------------------                                                                                                                         
         */
        BiTreeLeftRotate(&(*pNodePtr)->leftChild);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->leftChild, RED);
        SetTreeNodeColor((*pNodePtr)->leftChild->rightChild, BLACK);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight((*pNodePtr)->leftChild->rightChild);
        UpdateBlackHeight((*pNodePtr)->leftChild);
        UpdateBlackHeight(*pNodePtr);
    } else if (hasRedLeft(pNode->rightChild)) {
        /*
         Case 3: After left rotation, black sibling (Black1) and the left red nephew node (Red1) exist, 
                 but the right red nephew node (Red2) does not exist.

        ------------------------------------------------------------------||---------------------------------------------------------------------
                    Initial                   After left rotation (done)  ||     After right rotation         After left rotation and recoloring
        ------------------------------------------------------------------||---------------------------------------------------------------------
                                                                          ||                     
                  (*pNodePtr)                            (*pNodePtr)      ||              (*pNodePtr)                          (*pNodePtr)                                   
                      |                                      |            ||                  |                                    |                                       
                      |                                      |            ||                  |                                    |                                       
                      V                                      V            ||                  V                                    V                                    
                  Node (B)                              Sibling (R)       ||             Sibling (R)                          Sibling (B)                                 
                  /      \                               /        \       ||              /        \                           /        \                                 
                 /        \                             /          \      ||             /          \                         /          \                                
        Deleted (B)     Sibling (R)                Node (B)      Black2   ||        Node (B)      Black2                 Red1 (R)      Black2                              
                         /      \                  /     \                ||        /     \                              /     \                                          
                        /        \                /       \               ||       /       \                            /       \                                         
                    Black1       Black2     Deleted (B)    Black1         || Deleted (B)    Red1                  Node (B)      Black1                                
                    /                                      /              ||                 \                       /                                                  
                   /                                      /               ||                  \                     /                                                  
                Red1                                   Red1               ||                 Black1           Deleted (B)                              
        ------------------------------------------------------------------||---------------------------------------------------------------------                                                                                                                                         
         */
        BiTreeRightRotate(&pNode->rightChild);
        GEN_ONE_IMAGE();

        BiTreeLeftRotate(&(*pNodePtr)->leftChild);

        SetTreeNodeColor(*pNodePtr, BLACK);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight((*pNodePtr)->leftChild);
        UpdateBlackHeight(*pNodePtr);
    }
}

/*
    The unbalance can be fixed in this function.
 */
static void FixRedLeftSiblingInDeletion(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, char *graphName, char *fileName, long *pCnt) {
    BiTreeNodePtr pNode = *pNodePtr;
    /*
    -----------------------------------------------------------------------
                 Initial               After right rotation and recoloring
    -----------------------------------------------------------------------

                 (*pNodePtr)                         (*pNodePtr)            
                     |                                   |                
                     |                                   |                
                     V                                   V             
                  Node (B)                           Sibling (R)              
                  /      \                           /     \                     
                 /        \                         /       \                    
         Sibling (R)     Deleted (B)             Black1    Node (B)          
            /      \                                       /   \                  
           /        \                                     /     \                 
       Black1       Black2                            Black2     Deleted (B)

    =======================================================================
    Black1 and Black2 may have 0, 1, or 2 red children.               

    The red-black tree needs to be rebalanced later.
    -----------------------------------------------------------------------                            
     */
    BiTreeRightRotate(pNodePtr);
    GEN_ONE_IMAGE();
    /*
                            WARNING 

        The structure of the red-black tree has been changed after a rotation.

        (*pNodePtr) and pNode might not point to the same node any more.

     */
    if (hasBlackLeft(pNode->leftChild) && hasBlackRight(pNode->leftChild)) {   
        /*
         
        Case 1:  After right rotation, black sibling (Black2) and two black nephews (e.g., two null nodes) exist 

        -----------------------------------------------------------------||----------------------------------
                  Initial                   After right rotation (done)  ||         After recoloring
        -----------------------------------------------------------------||----------------------------------
                                                                         ||         
                   (*pNodePtr)                  (*pNodePtr)              ||         (*pNodePtr)                      
                       |                            |                    ||             |                          
                       |                            |                    ||             |                           
                       V                            V                    ||             V                                  
                    Node (B)                    Sibling (R)              ||         Sibling (B)                             
                    /      \                    /     \                  ||         /     \                    
                   /        \                  /       \                 ||        /       \                   
           Sibling (R)     Deleted (B)      Black1    Node (B)           ||     Black1    Node (B)         
              /      \                                /   \              ||               /   \                 
             /        \                              /     \             ||              /     \                
         Black1       Black2                     Black2     Deleted (B)  ||        Black2 (R)   Deleted (B)   
                       /  \                       /  \                   ||           /  \                      
                      /    \                     /    \                  ||          /    \                  
                    null   null                null   null               ||        null   null                
        -----------------------------------------------------------------||------------------------------------                 
         */


        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor(pNode->leftChild, RED);

        UpdateBlackHeight(pNode->leftChild);
        UpdateBlackHeight(pNode);
        UpdateBlackHeight(*pNodePtr);    
    } else if (hasRedRight(pNode->leftChild)) {
        /*
        
        Case 2: After right rotation, black sibling (Black2) and the red right nephew node (Red2) exist

        ----------------------------------------------------------------||------------------------------------------------------------------
                 Initial                  After right rotation (done)   ||    After left rotation      After right rotation and recoloring
        ----------------------------------------------------------------||------------------------------------------------------------------
                                                                        ||              
                   (*pNodePtr)                 (*pNodePtr)              ||       (*pNodePtr)                   (*pNodePtr)                                                    
                       |                           |                    ||           |                             |                                                        
                       |                           |                    ||           |                             |                                                         
                       V                           V                    ||           V                             V                                                                
                    Node (B)                   Sibling (R)              ||       Sibling (R)                   Sibling (B)                                                           
                    /      \                   /     \                  ||       /     \                       /     \                                                  
                   /        \                 /       \                 ||      /       \                     /       \                                                 
           Sibling (R)     Deleted (B)     Black1    Node (B)           ||   Black1    Node (B)            Black1     Red2                                       
              /      \                               /   \              ||             /   \                         /   \                                               
             /        \                             /     \             ||            /     \                       /     \                                              
         Black1       Black2                    Black2     Deleted (B)  ||         Red2     Deleted (B)        Black2     Node (B)                                 
                           \                         \                  ||          /                                        \                               
                            \                         \                 ||         /                                          \                                 
                            Red2                      Red2              ||      Black2                                      Deleted (B)                                   
        ----------------------------------------------------------------||-----------------------------------------------------------------   
         */

        BiTreeLeftRotate(&pNode->leftChild);
        GEN_ONE_IMAGE();

        BiTreeRightRotate(&(*pNodePtr)->rightChild);

        SetTreeNodeColor(*pNodePtr, BLACK);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight((*pNodePtr)->rightChild);
        UpdateBlackHeight(*pNodePtr);
    } else if (hasRedLeft(pNode->leftChild)) {
        /*

        Case 3: After right rotation, black sibling (Black2) and the red left nephew node (Red1) exist, 
                but the red right nephew node (Red2) does not exist.

        ---------------------------------------------------------------||--------------------------------------
                  Initial                After right rotation (done)   || After right rotation and recoloring
        ---------------------------------------------------------------||--------------------------------------
                                                                       ||           
                   (*pNodePtr)                (*pNodePtr)              ||           (*pNodePtr)                                                             
                       |                          |                    ||               |                                                                 
                       |                          |                    ||               |                                                                  
                       V                          V                    ||               V                                                                         
                    Node (B)                  Sibling (R)              ||           Sibling (B)                                                                    
                    /      \                  /     \                  ||           /     \                                                           
                   /        \                /       \                 ||          /       \                                                          
           Sibling (R)     Deleted (B)    Black1    Node (B)           ||       Black1    Black2 (R)                                              
              /      \                              /   \              ||                 /   \                                                        
             /        \                            /     \             ||                /     \                                                       
         Black1       Black2                   Black2     Deleted (B)  ||           Red1 (B)   Node (B)                                      
                        /                       /                      ||                        \                                               
                       /                       /                       ||                         \                                                   
                     Red1                    Red1                      ||                       Deleted (B)                                                
        ---------------------------------------------------------------||--------------------------------------                                                                                                                                 
         */

        BiTreeRightRotate(&(*pNodePtr)->rightChild);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->rightChild, RED);      
        SetTreeNodeColor((*pNodePtr)->rightChild->leftChild, BLACK);
        
        UpdateBlackHeight((*pNodePtr)->rightChild->leftChild);
        UpdateBlackHeight(pNode);
        UpdateBlackHeight((*pNodePtr)->rightChild);
        UpdateBlackHeight(*pNodePtr);        
    }    
}

/*
    The unbalance might be propagated upward.
 */
static void FixBlackRightSiblingInDeletion(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, char *graphName, char *fileName, long *pCnt) {
    BiTreeNodePtr pNode = *pNodePtr;

    if (hasBlackLeft(pNode->rightChild) && hasBlackRight(pNode->rightChild)) {
        /*
        Case 1:  black right sibling + two black nephews

        -----------------------------------------------------------------------
                Initial                         After recoloring
        -----------------------------------------------------------------------
    
               (*pNodePtr)                       (*pNodePtr)                                              
                   |                                 |                                                   
                   |                                 |                                                   
                  Node                             Node (B)                                                                                               
                /      \                          /      \                                                                                              
               /        \                        /        \                                                                                
        Deleted (B)    Sibling (B)        Deleted (B)    Sibling (R)                                                                  
                        /  \                              /  \                                                                         
                       /    \                            /    \                                                                             
                     null    null                      null    null                                    
        ----------------------------------------------------------------------- 

        We propagate the 'double black' (i.e., Deleted (B)) upwards if 'Node' was BLACK before recoloring.
        But if 'Node' was RED, then the red-black tree is balanced after recoloring.         
         */            
        SetTreeNodeColor(pNode->rightChild, RED);
        SetTreeNodeColor(pNode, BLACK);

        UpdateBlackHeight(pNode->rightChild);
        UpdateBlackHeight(pNode);
    } else if (hasRedRight(pNode->rightChild)) {
        NodeColor rootColor = pNode->color;
        /*
        Case 2: black right sibling + red right nephew (Red2)

        -----------------------------------------------------------------------
                 Initial                After left rotation and recoloring
        -----------------------------------------------------------------------
    
               (*pNodePtr)                       (*pNodePtr)                                              
                   |                                 |                                                   
                   |                                 |                                                   
                Node (rootColor)                   Sibling (rootColor)                                                                                               
                /      \                          /      \                                                                                              
               /        \                        /        \                                                                                
        Deleted (B)    Sibling (B)           Node (B)    Red2 (B)                                                                  
                           \                   /                                                                         
                            \                 /                                                                               
                            Red2           Deleted (B)                                              
        -----------------------------------------------------------------------          
         */
        BiTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor((*pNodePtr)->rightChild, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight((*pNodePtr)->rightChild);
        UpdateBlackHeight(*pNodePtr);
    } else if (hasRedLeft(pNode->rightChild)) {
        NodeColor rootColor = pNode->color;
        /*
        Case 3: black right sibling + red left nephew (Red1), but no red right nephew (Red2)

        ---------------------------------------------------------------------------------------------------------------
                 Initial                     After right rotation              After left rotation and  recoloring
        ---------------------------------------------------------------------------------------------------------------
    
               (*pNodePtr)                       (*pNodePtr)                          (*pNodePtr)                                        
                   |                                 |                                    |                                             
                   |                                 |                                    |                                             
                Node (rootColor)                   Node (rootColor)                     Red1 (rootColor)                                                                                         
                /      \                          /      \                             /      \                                                                                        
               /        \                        /        \                           /        \                                                                          
        Deleted (B)    Sibling (B)         Deleted (B)    Red1                  Node (B)    Sibling (B)                                                            
                        /  \                                 \                      /                                           
                       /    \                                 \                    /                                                
                     Red1   null                             Sibling (B)      Deleted (B)                                    
        ---------------------------------------------------------------------------------------------------------------                                     
         */
        BiTreeRightRotate(&(*pNodePtr)->rightChild);

        GEN_ONE_IMAGE();

        BiTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight(*pNodePtr);        
    }
}

/*
    The unbalance might be propagated upward.
 */
static void FixBlackLeftSiblingInDeletion(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, char *graphName, char *fileName, long *pCnt) {
    BiTreeNodePtr pNode = *pNodePtr;
    NodeColor rootColor = pNode->color;
    if (hasBlackLeft(pNode->leftChild) && hasBlackRight(pNode->leftChild)) {
        /*
        Case 1:  black left sibling + two black nephews

        -----------------------------------------------------------------------
                  Initial                        After recoloring
        -----------------------------------------------------------------------
    
               (*pNodePtr)                         (*pNodePtr)                                               
                   |                                   |                                                    
                   |                                   |                                                    
                  Node                                Node (B)                                                                                                  
                /      \                            /      \                                                                                               
               /        \                          /        \                                                                                 
        Sibling (B)    Deleted (B)          Sibling (R)    Deleted (B)                                                                   
            /  \                                /  \                                                                             
           /    \                              /    \                                                                                 
         null    null                        null    null                                        
        -----------------------------------------------------------------------

        We propagate the 'double black' (i.e., Deleted (B)) upwards if 'Node' was BLACK before recoloring.
        But if 'Node' was RED, then the red-black tree is balanced after recoloring.                
         */
        SetTreeNodeColor(pNode->leftChild, RED);
        SetTreeNodeColor(pNode, BLACK);
        UpdateBlackHeight(pNode->leftChild);
        UpdateBlackHeight(pNode);
    } else if (hasRedRight(pNode->leftChild)) {
        /*
        Case 2: black left sibling + red right nephew (Red2)

        ---------------------------------------------------------------------------------------------------------------
                  Initial                       After left rotation              After right rotation and  recoloring
        ---------------------------------------------------------------------------------------------------------------
    
               (*pNodePtr)                        (*pNodePtr)                      (*pNodePtr)                                        
                   |                                  |                                |                                             
                   |                                  |                                |                                             
              Node (rootColor)                    Node (rootColor)                   Red2 (rootColor)                                                                                         
                /      \                           /      \                         /      \                                                                                        
               /        \                         /        \                       /        \                                                                          
        Sibling (B)    Deleted (B)             Red2       Deleted (B)        Sibling (B)    Node (B)                                                            
            /  \                               /                                               \                             
           /    \                             /                                                 \                                
                Red2                   Sibling (B)                                              Deleted (B)                       
        --------------------------------------------------------------------------------------------------------------- 
         */
        BiTreeLeftRotate(&(*pNodePtr)->leftChild);
        GEN_ONE_IMAGE();

        BiTreeRightRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

        UpdateBlackHeight(pNode);
        UpdateBlackHeight(*pNodePtr);
    } else if (hasRedLeft(pNode->leftChild)) {
        /*
        Case 3: black left sibling + red left nephew (Red1), but no red right nephew (Red2)

        -----------------------------------------------------------------------
                  Initial                 After right rotation and recoloring
        -----------------------------------------------------------------------
    
               (*pNodePtr)                         (*pNodePtr)                                               
                   |                                   |                                                    
                   |                                   |                                                    
                 Node (rootColor)                  Sibling (rootColor)                                                                                                  
                /      \                            /      \                                                                                               
               /        \                          /        \                                                                                 
        Sibling (B)    Deleted (B)            Red1 (B)    Node (B)                                                                   
            /  \                                              \                                                                        
           /    \                                              \                                                                            
        Red1    null                                          Deleted (B)                                       
        -----------------------------------------------------------------------         
         */
        BiTreeRightRotate(pNodePtr);

        SetTreeNodeColor((*pNodePtr)->leftChild, BLACK);
        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

        UpdateBlackHeight((*pNodePtr)->leftChild);
        UpdateBlackHeight(pNode);
        UpdateBlackHeight(*pNodePtr);
    }
}


// The parameter pRoot is only used for generating the image of the binary search tree.
// In this recursive function, *pNodePtr might point to a sub-tree in the BST.
static void BiTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    //static long cnt = 0;
    assert(pCnt);
    char *graphName = "RBTree";
    char *fileName = "images/RBTree2Delete";
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
                //printf("deleting %ld\n", pNode->value.numVal);
                /*
                    If the node to be deleted is RED, nothing to do.
                 */
                if ((pNode->color == BLACK) && (pNode->rightChild)) {
                    // assert(pNode->rightChild->color == RED)
                    // This coloring operation can increase the black height of the subtree.
                    pNode->rightChild->color = BLACK;

                }              
                free(pNode);
                *pNodePtr = tmp;                
            } else if (pNode->rightChild == NULL) { // case 10
                BiTreeNodePtr tmp = pNode->leftChild;
                //printf("deleting %ld\n", pNode->value.numVal);
                /*
                    If the node to be deleted is RED, nothing to do.
                 */
                if ((pNode->color == BLACK) && (pNode->leftChild)) {
                    // assert(pNode->leftChild->color == RED)
                    pNode->leftChild->color = BLACK;
                }                 
                free(pNode);
                *pNodePtr = tmp;              
            } else {
#ifdef SWAP_IN_ORDER_SUCCESSOR                
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
                //(*pCnt)++;
                //GenOneImage(*pRoot, graphName, fileName, *pCnt);
                GEN_ONE_IMAGE();
                
                // Now, numVal is in right sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                BiTreeDelete(pRoot, &pNode->rightChild, pSuccessor->value.numVal, pCnt);
#else
                // case 11:  with two children
                // Get pNode's in-order pPredecessor, which is right-most node in its left sub-tree.
                BiTreeNodePtr pPredecessor = BiTreeMaxValueNode(pNode->leftChild);

                // (Swapping is done for clearer debugging output)
                // Swap the values of the node pointed to by pNode and its in-order predecessor              
                NodeValue val = pNode->value;
                // Copy the predecessor's value (this copy is necessary)
                pNode->value = pPredecessor->value;
                pPredecessor->value = val;

                // Display the inconsistent state
                //(*pCnt)++;
                //GenOneImage(*pRoot, graphName, fileName, *pCnt);
                GEN_ONE_IMAGE();
                
                // Now, numVal is in right sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                BiTreeDelete(pRoot, &pNode->leftChild, pPredecessor->value.numVal, pCnt);
#endif
            }
        }
        // 
        pNode = *pNodePtr;
        // If it is NULL, just return.
        if (pNode == NULL) {            
            return;
        }

        // recalculate and store its height
        UpdateBlackHeight(pNode);

        //(*pCnt)++;
        //GenOneImage(*pRoot, graphName, fileName, *pCnt);        
        GEN_ONE_IMAGE();

        int bFactor = BiTreeBalanceFactor(pNode);

        if (bFactor < 0) {
            /*
                      (*pNodePtr)
                          |      
                          |      
                         Node   
                        /      \
                       /        \
                Deleted (B)    Sibling (R or B)    
             */            
            if (hasRedRight(pNode)) {
                FixRedRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackRight(pNode) && pNode->rightChild) {
                FixBlackRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            }

        } else if (bFactor > 0) {
            /*
                          (*pNodePtr)
                              |      
                              |      
                              Node   
                            /      \
                           /        \
                Sibling (R or B)    Deleted (B)        
             */  
            if (hasRedLeft(pNode)) {
                FixRedLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackLeft(pNode) && pNode->leftChild) {
                FixBlackLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            }
        }    
        //
        //(*pCnt)++;
        //GenOneImage(*pRoot, graphName, fileName, *pCnt);
        GEN_ONE_IMAGE();

        // It is not on stack
        pNode->visited = 0;
    }
}

void RBTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    BiTreeDelete(pRoot, pNodePtr, numVal, pCnt);    
    BiTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        pNode->color = BLACK;
        UpdateBlackHeight(pNode);
    }
}

static int CheckRBTree(BiTreeNodePtr pNode) {
    if (pNode) {
        // Property 2: a node is either BLACK or RED
        if (pNode->color != BLACK && pNode->color != RED) {
            return 0;
        }        

        if (pNode->color == RED) {
            // Property 3: a red node shouldn't have any red child.
            if (hasRedLeft(pNode) || hasRedRight(pNode)) {
                return 0;
            }
        }

        // Property 4: The left and right subtrees have the same black height.
        if (RBTreeBlackHeight(pNode->leftChild) != RBTreeBlackHeight(pNode->rightChild)) {
            return 0;
        }

        // Property 5: All null nodes are black.  See hasBlackLeft() and hasBlackRight()

        // Recursively check left and right subtrees
        if (CheckRBTree(pNode->leftChild) == 0) {
            return 0;
        }
        
        if (CheckRBTree(pNode->rightChild) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
    This function returns 1 when root is a legal RB tree.
    Otherwise, it returns 0.
 */
int IsRBTree(BiTreeNodePtr root) {
    if (root) {
        // Property 1: the root is black.
        if (root->color != BLACK) {
            return 0;
        }
        // Check other properties
        return CheckRBTree(root);
    } else {
        return 1;
    }
}
