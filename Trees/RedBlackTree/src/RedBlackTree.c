#include <assert.h> // assert()
#include <stdio.h>  // printf() and snprintf()
#include <stdlib.h> // malloc() and free()
#include <string.h> // memset()
#include "RedBlackTree.h"
#include "RBTree2Dot.h"
#include "Queue.h"

// 
typedef enum {
    // no violation in deletion or fixed
    NO_DOUBLE_BACK = 0,
    // violation in deletion generated
    DOUBLE_BLACK_GENERATED = 1,
    // violation in deletion propagated from its left child
    DOUBLE_BLACK_FROM_LEFT = 2,
    // violation in deletion propagated from its right child
    DOUBLE_BLACK_FROM_RIGHT = 3,
} DoubleBlackState;

typedef enum {
    // the node being visited is a left node
    TURN_LEFT = 2,
    // the node being visited is a right node
    TURN_RIGHT = 3
} TraversalDirection;

// whether to generate intermediate images in inserting or deleting nodes
#define ENABLE_GEN_INTERMEDIATE_IMAGES

#ifdef ENABLE_GEN_INTERMEDIATE_IMAGES
#define GEN_ONE_IMAGE(comment, nodeKey)                         \
    do                                                          \
    {                                                           \
        if (hasIntermediateImages) {                            \
            (*pCnt)++;                                                                   \
            RBTreeGenOneImage(*pRoot, graphName, fileName, *pCnt, (comment), (nodeKey)); \
        }                                                                                \
    } while (0)
#else
#define GEN_ONE_IMAGE(comment, nodeKey) \
    do                  \
    {                   \
    } while (0)
#endif

#define  ILLEGAL_TREE_HEIGHT        -1

/*
    The cases in FixViolationsInInsertion():

    e.g., RedParentRedUncle_RXXX represents

                Grandparent
                 /     \
                /       \
        Red Parent     Red Uncle
            /
           /
        Red Child (inserted)
 */
typedef enum {
    // R + R
    RedParentRedUncle_RXXX, // red parent, red uncle, left red child
    RedParentRedUncle_XRXX, // red parent, red uncle, right red child
    RedUncleRedParent_XXRX, // red uncle, red parent, left red child
    RedUncleRedParent_XXXR, // red uncle, red parent, right red child
    // R + B
    RedParentBlackUncle_RXXX, // red parent, black uncle, left red child
    RedParentBlackUncle_XRXX, // red parent, black uncle, right red child
    BlackUncleRedParent_XXRX, // black uncle, red parent, left red child
    BlackUncleRedParent_XXXR, // black uncle, red parent, right red child
    // other: e.g., B + B
    OtherRBState,
} RBTreeNodeState;

static int FixBlackRightSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                           char *graphName, char *fileName, long *pCnt);

static int FixBlackLeftSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                           char *graphName, char *fileName, long *pCnt);                                          

static long hasIntermediateImages = 0;

void SetIntermediateImages(long enableState) {
    hasIntermediateImages = enableState;
}

static void SetTreeNodeColor(RBTreeNodePtr pNode, NodeColor color) {
    pNode->color = color;
}

int hasRedLeft(RBTreeNodePtr pNode) {
    return pNode->left && pNode->left->color == RED;
}

int hasBlackLeft(RBTreeNodePtr pNode) {
    return (!pNode->left) || (pNode->left->color == BLACK);
}

int hasRedRight(RBTreeNodePtr pNode) {
    return pNode->right && pNode->right->color == RED;
}

int hasBlackRight(RBTreeNodePtr pNode) {
    return (!pNode->right) || (pNode->right->color == BLACK);
}

static RBTreeNodeState GetRBTreeNodeState(RBTreeNodePtr pNode) {
    assert(pNode);

    if (hasRedLeft(pNode) && hasRedRight(pNode)) {
        if (hasRedLeft(pNode->left)) {
            return RedParentRedUncle_RXXX;
        } else if (hasRedRight(pNode->left)) {
            return RedParentRedUncle_XRXX;
        } else if (hasRedLeft(pNode->right)) {
            return RedUncleRedParent_XXRX;
        } else if (hasRedRight(pNode->right)) {
            return RedUncleRedParent_XXXR;
        }
    } else if (hasRedLeft(pNode) && hasBlackRight(pNode)) {
        if (hasRedLeft(pNode->left)) {
            return RedParentBlackUncle_RXXX;
        } else if (hasRedRight(pNode->left)) {
            return RedParentBlackUncle_XRXX;
        }
    } else if (hasBlackLeft(pNode) && hasRedRight(pNode)) {
        if (hasRedLeft(pNode->right)) {
            return BlackUncleRedParent_XXRX;
        } else if (hasRedRight(pNode->right)) {
            return BlackUncleRedParent_XXXR;
        }
    }
    return OtherRBState;
}

/////////////////////////////////// Red-Black Tree ///////////////////////////////////////////

void RBTreeResetBlackHeight(struct RBTreeNode *pNode) {
    if (pNode) {
        pNode->blackHeight = ILLEGAL_TREE_HEIGHT;
        RBTreeResetBlackHeight(pNode->left);
        RBTreeResetBlackHeight(pNode->right);
    }
}

long RBTreeBlackHeight(struct RBTreeNode *pNode) {
    if (pNode) {
        if (pNode->blackHeight != ILLEGAL_TREE_HEIGHT) {
            return pNode->blackHeight;
        }        
        long leftLen = RBTreeBlackHeight(pNode->left);
        long rightLen = RBTreeBlackHeight(pNode->right);
        assert(leftLen == rightLen);
        if (pNode->color == BLACK) {
            pNode->blackHeight = 1 + leftLen;
            return 1 + leftLen;
        } else {
            pNode->blackHeight = leftLen;
            return leftLen;
        }
    } else {
        return 0;
    }
}

// Ignore node color
long RBTreeGetRealHeight(struct RBTreeNode *pNode) {
    if (pNode) {
        long leftLen = RBTreeGetRealHeight(pNode->left);
        long rightLen = RBTreeGetRealHeight(pNode->right);
        return leftLen > rightLen ? (leftLen + 1) : (rightLen + 1);     
    } else {
        return 0;
    }
}

static RBTreeNodePtr CreateRBTreeNode(long numVal, char *nodeName, RBTreeNodePtr left, RBTreeNodePtr right) {
    RBTreeNodePtr pNode = (RBTreeNodePtr)malloc(sizeof(struct RBTreeNode));
    assert(pNode != NULL);

    memset(pNode, 0, sizeof(*pNode));
    pNode->value.numVal = numVal;
    pNode->left = left;
    pNode->right = right;
    pNode->visited = 0;
    pNode->blackHeight = ILLEGAL_TREE_HEIGHT;
    SetTreeNodeColor(pNode, RED);
    return pNode;
}

RBTreeNodePtr CreateRBTree(void) {
    return NULL;
}

void ReleaseRBTree(RBTreeNodePtr root) {
    if (root) {
        ReleaseRBTree(root->left);
        ReleaseRBTree(root->right);
        free(root);
    }
}

void InOrderTraversal(RBTreeNodePtr root, NodeVisitor visit) {
    if (root) {
        InOrderTraversal(root->left, visit);
        visit(root);
        InOrderTraversal(root->right, visit);
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
static void RBTreeRightRotate(RBTreeNodePtr *pNodePtr) {
    RBTreeNodePtr pNodeC = *pNodePtr;
    RBTreeNodePtr pNodeA = pNodeC->left;
    RBTreeNodePtr pNodeB = pNodeA->right;

    pNodeA->right = pNodeC;
    pNodeC->left = pNodeB;
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
static void RBTreeLeftRotate(RBTreeNodePtr *pNodePtr) {
    RBTreeNodePtr pNodeA = *pNodePtr;
    RBTreeNodePtr pNodeC = pNodeA->right;
    RBTreeNodePtr pNodeB = pNodeC->left;

    pNodeC->left = pNodeA;
    pNodeA->right = pNodeB;
    *pNodePtr = pNodeC;
}

//////////////////////////////////////////  RBTreeInsert() //////////////////////////////////////////////

/*
    The unbalance might be propagated upward.
 */
static void FixViolationsInInsertion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                     long *pCnt, char *graphName, char *fileName) {
    RBTreeNodePtr pNode = *pNodePtr;
    assert(pNode);

    RBTreeNodeState state = GetRBTreeNodeState(pNode);
    if (state == OtherRBState) {
        return;
    }
    //GEN_ONE_IMAGE("Before FixViolationsInInsertion()", (*pNodePtr)->value.numVal);
    switch (state){
    case RedParentRedUncle_RXXX:
    case RedParentRedUncle_XRXX:
    case RedUncleRedParent_XXRX:
    case RedUncleRedParent_XXXR:
        GEN_ONE_IMAGE("red uncle + black grandparent:\n"
                    "after coloring, propagate it upwards to", 
                    pNode->value.numVal);    
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
        SetTreeNodeColor(pNode->left, BLACK);
        SetTreeNodeColor(pNode->right, BLACK);

        break;
    case RedParentBlackUncle_RXXX:
        GEN_ONE_IMAGE("black right uncle + black grandparent:\n"
                    "right rotation and coloring, to fix the red-red violation at the RXXX grandchild of", 
                    pNode->value.numVal);      
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
        RBTreeRightRotate(pNodePtr);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->right, RED);

        break;
    case RedParentBlackUncle_XRXX:
        GEN_ONE_IMAGE("black right uncle + black grandparent:\n"
                    "LR rotations and coloring, to fix the red-red violation at the XRXX grandchild of", 
                    pNode->value.numVal);      
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
        RBTreeLeftRotate(&pNode->left);
        GEN_ONE_IMAGE("After left rotation", pNode->left->value.numVal);

        RBTreeRightRotate(pNodePtr);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->right, RED);

        break;
    case BlackUncleRedParent_XXRX:
        GEN_ONE_IMAGE("black left uncle + black grandparent:\n"
                    "RL rotations and coloring, to fix the red-red violation at the XXRX grandchild of", 
                    pNode->value.numVal);      
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
        RBTreeRightRotate(&pNode->right);
        GEN_ONE_IMAGE("After right rotation", pNode->right->value.numVal);

        RBTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->left, RED);

        break;
    case BlackUncleRedParent_XXXR:
        GEN_ONE_IMAGE("black left uncle + black grandparent:\n"
                    "left rotation and coloring, to fix the red-red violation at the XXXR grandchild of", 
                    pNode->value.numVal);      
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
        RBTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(*pNodePtr, BLACK);
        SetTreeNodeColor((*pNodePtr)->left, RED);

        break;
    default:
        break;
    }
    GEN_ONE_IMAGE("After fixing the red-red violation at the grandchild of", (pNode)->value.numVal);
}

static void RecursiveRBTreeInsert(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;
    assert(pCnt);

    char *graphName = "RBTree";
    char *fileName = "images/RBTree1Insert";

    if (pNode == NULL) {
        RBTreeNodePtr tmp = CreateRBTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;
        // printf("inserting %ld\n", numVal);
        return;
    } else {
        // on stack
        pNode->visited = 1;
        if (numVal < pNode->value.numVal) {
            RecursiveRBTreeInsert(pRoot, &pNode->left, numVal, nodeName, pCnt);
        } else if (numVal > pNode->value.numVal) {
            RecursiveRBTreeInsert(pRoot, &pNode->right, numVal, nodeName, pCnt);
        } else {
            // If numVal is already in the binary search tree, do nothing.
            // off stack
            pNode->visited = 0;
            return;
        }
        // The inserted node is RED node. We will update black heights in FixViolationsInInsertion().        
        FixViolationsInInsertion(pRoot, pNodePtr, pCnt, graphName, fileName);
        // off stack
        pNode->visited = 0;
    }
}

void RBTreeInsert(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {
    RecursiveRBTreeInsert(pRoot, pNodePtr, numVal, nodeName, pCnt);
    RBTreeNodePtr pNode = *pNodePtr;
    pNode->color = BLACK;
}

//////////////////////////////////////////  RBTreeDelete() //////////////////////////////////////////////

static RBTreeNodePtr BiTreeMaxValueNode(RBTreeNodePtr root) {
    RBTreeNodePtr cur = root;
    // Get the right-most node
    while ((cur != NULL) && (cur->right != NULL)) {
        cur = cur->right;
    }
    return cur;
}

/*
    The unbalance can be fixed in this function.
 */
static int FixRedRightSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                         char *graphName, char *fileName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;
    GEN_ONE_IMAGE("red right sibling + black parent:\n"
                  "left rotation and coloring, to convert it into a black-sibling-red-parent violation", 
                  pNode->value.numVal);    
    // sibling will become a black grandparent
    SetTreeNodeColor(pNode->right, BLACK);
    SetTreeNodeColor(pNode, RED);
    /*

        -----------------------------------------------------------------------
                  Initial                      After left rotation + coloring
        -----------------------------------------------------------------------

                  (*pNodePtr)                              (*pNodePtr)
                      |                                        |
                      |                                        |
                      V                                        V
                  Node (B)                                Sibling (B)
                  /      \                                 /        \
                 /        \                               /          \
        Deleted (B)     Sibling (R)                  Node (R)      Black2
                         /      \                    /     \
                        /        \                  /       \
                    Black1       Black2       Deleted (B)    Black1

        =======================================================================
        Black1 and Black2 may have 0, 1, or 2 red children.

        The red-black tree needs to be rebalanced later.
        -----------------------------------------------------------------------
     */
    RBTreeLeftRotate(pNodePtr);
    GEN_ONE_IMAGE("After left rotation and coloring,\n"
                  "a black-sibling-red-parent violation at the left child of", 
                  pNode->value.numVal);

    return FixBlackRightSiblingInDeletion(pRoot, &((*pNodePtr)->left), graphName, fileName, pCnt);  
}

/*
    The unbalance can be fixed in this function.
 */
static int FixRedLeftSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                        char *graphName, char *fileName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;
    GEN_ONE_IMAGE("red left sibling + black parent:\n"
                  "right rotation and coloring, to convert it into a black-sibling-red-parent violation", 
                  pNode->value.numVal);      
    // sibling will become a black grandparent
    SetTreeNodeColor(pNode->left, BLACK);
    SetTreeNodeColor(pNode, RED);  
    /*
        -----------------------------------------------------------------------
                     Initial               After right rotation and recoloring
        -----------------------------------------------------------------------

                     (*pNodePtr)                         (*pNodePtr)
                         |                                   |
                         |                                   |
                         V                                   V
                      Node (B)                           Sibling (B)
                      /      \                           /     \
                     /        \                         /       \
             Sibling (R)     Deleted (B)             Black1    Node (R)
                /      \                                       /   \
               /        \                                     /     \
           Black1       Black2                            Black2     Deleted (B)

        =======================================================================
        Black1 and Black2 may have 0, 1, or 2 red children.

        The red-black tree needs to be rebalanced later.
    -----------------------------------------------------------------------
     */
    RBTreeRightRotate(pNodePtr);
    GEN_ONE_IMAGE("After right rotation and coloring,\n"
                  "a black-sibling-red-parent violation at the right child of", 
                  pNode->value.numVal);
    return FixBlackLeftSiblingInDeletion(pRoot, &((*pNodePtr)->right), graphName, fileName, pCnt);  
}

/*
    The unbalance might be propagated upward.
 */
static int FixBlackRightSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                           char *graphName, char *fileName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;

    if (hasBlackLeft(pNode->right) && hasBlackRight(pNode->right)) {
        if (pNode->color == RED) {
            GEN_ONE_IMAGE("black right sibling + two black nephews + red parent:\n"
                          "coloring, to fix double black at the left child of ", pNode->value.numVal);
        } else {
            GEN_ONE_IMAGE("black right sibling + two black nephews + black parent:\n"
                          "after coloring, propagate the double black upwards from the left child of", pNode->value.numVal);
        }
        /*
            Case 1:  black right sibling + two black nephews

            -----------------------------------------------------------------------
                    Initial                         After recoloring
            -----------------------------------------------------------------------

                   (*pNodePtr)                       (*pNodePtr)
                       |                                 |
                       |                                 |
                       v                                 v
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
        int fixed = 0;
        if (pNode->color == RED) {
            fixed = 1;
        }
        SetTreeNodeColor(pNode->right, RED);
        SetTreeNodeColor(pNode, BLACK);

        return fixed;
    } else if (hasRedRight(pNode->right)) {
        GEN_ONE_IMAGE("black right sibling + red right nephew:\n"
                      "left rotation and coloring, to fix the double black at the left child of", pNode->value.numVal);
        NodeColor rootColor = pNode->color;
        /*
            Case 2: black right sibling + red right nephew (Red2)

            -----------------------------------------------------------------------
                     Initial                After left rotation and recoloring
            -----------------------------------------------------------------------

                   (*pNodePtr)                       (*pNodePtr)
                       |                                 |
                       |                                 |
                       v                                 v
                    Node (rootColor)                   Sibling (rootColor)
                    /      \                          /      \
                   /        \                        /        \
            Deleted (B)    Sibling (B)           Node (B)    Red2 (B)
                               \                   /
                                \                 /
                                Red2           Deleted (B)
            -----------------------------------------------------------------------
         */
        RBTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor((*pNodePtr)->right, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

    } else if (hasRedLeft(pNode->right)) {
        GEN_ONE_IMAGE("black right sibling + red left nephew:\n"
                      "RL rotations and coloring, to fix the double black at the left child of", pNode->value.numVal);
        NodeColor rootColor = pNode->color;
        /*
            Case 3: black right sibling + red left nephew (Red1), but no red right nephew (Red2)

            ---------------------------------------------------------------------------------------------------------------
                     Initial                     After right rotation              After left rotation and  recoloring
            ---------------------------------------------------------------------------------------------------------------

                   (*pNodePtr)                       (*pNodePtr)                          (*pNodePtr)
                       |                                 |                                    |
                       |                                 |                                    |
                       v                                 v                                    v
                    Node (rootColor)                   Node (rootColor)                     Red1 (rootColor)
                    /      \                          /      \                             /      \
                   /        \                        /        \                           /        \
            Deleted (B)    Sibling (B)         Deleted (B)    Red1                  Node (B)    Sibling (B)
                            /  \                                 \                      /
                           /    \                                 \                    /
                         Red1   null                             Sibling (B)      Deleted (B)
            ---------------------------------------------------------------------------------------------------------------
         */
        RBTreeRightRotate(&(*pNodePtr)->right);

        GEN_ONE_IMAGE("After right rotation", (*pNodePtr)->right->value.numVal);

        RBTreeLeftRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);
   
    }
    return 1;
}

/*
    The unbalance might be propagated upward.
 */
static int FixBlackLeftSiblingInDeletion(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                          char *graphName, char *fileName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;
    NodeColor rootColor = pNode->color;
    if (hasBlackLeft(pNode->left) && hasBlackRight(pNode->left)) {
        if (pNode->color == RED) {
            GEN_ONE_IMAGE("black left sibling + two black nephews + red parent:\n "
                          "coloring, to fix the double black at the right child of", pNode->value.numVal);
        } else {
            GEN_ONE_IMAGE("black left sibling + two black nephews + black parent:\n"
                          "after coloring, propagate the double black upwards from the right child of", pNode->value.numVal);
        }        
        /*
            Case 1:  black left sibling + two black nephews

            -----------------------------------------------------------------------
                      Initial                        After recoloring
            -----------------------------------------------------------------------

                   (*pNodePtr)                         (*pNodePtr)
                       |                                   |
                       |                                   |
                       v                                   v
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
        int fixed = 0;
        if (pNode->color == RED) {
            fixed = 1;
        }
        SetTreeNodeColor(pNode->left, RED);
        SetTreeNodeColor(pNode, BLACK);
        return fixed;
    } else if (hasRedRight(pNode->left)) {
        GEN_ONE_IMAGE("black left sibling + red right nephew:\n"
                      "LR rotations and coloring, to fix the double black at the right child of", pNode->value.numVal);
        /*
            Case 2: black left sibling + red right nephew (Red2)

            ---------------------------------------------------------------------------------------------------------------
                      Initial                       After left rotation              After right rotation and  recoloring
            ---------------------------------------------------------------------------------------------------------------

                   (*pNodePtr)                        (*pNodePtr)                      (*pNodePtr)
                       |                                  |                                |
                       |                                  |                                |
                       v                                  v                                v
                  Node (rootColor)                    Node (rootColor)                   Red2 (rootColor)
                    /      \                           /      \                         /      \
                   /        \                         /        \                       /        \
            Sibling (B)    Deleted (B)             Red2       Deleted (B)        Sibling (B)    Node (B)
                /  \                               /                                               \
               /    \                             /                                                 \
                    Red2                   Sibling (B)                                              Deleted (B)
            ---------------------------------------------------------------------------------------------------------------
         */
        RBTreeLeftRotate(&(*pNodePtr)->left);
        GEN_ONE_IMAGE("After left rotation", (*pNodePtr)->left->value.numVal);

        RBTreeRightRotate(pNodePtr);

        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

    } else if (hasRedLeft(pNode->left)) {
        GEN_ONE_IMAGE("black left sibling + red left nephew:\n"
                      "right rotation and coloring, to fix the double black at the right child of", pNode->value.numVal);
        /*
            Case 3: black left sibling + red left nephew (Red1), but no red right nephew (Red2)

            -----------------------------------------------------------------------
                      Initial                 After right rotation and recoloring
            -----------------------------------------------------------------------

                   (*pNodePtr)                         (*pNodePtr)
                       |                                   |
                       |                                   |
                       v                                   v
                     Node (rootColor)                  Sibling (rootColor)
                    /      \                            /      \
                   /        \                          /        \
            Sibling (B)    Deleted (B)            Red1 (B)    Node (B)
                /  \                                              \
               /    \                                              \
            Red1    null                                          Deleted (B)
            -----------------------------------------------------------------------
         */
        RBTreeRightRotate(pNodePtr);

        SetTreeNodeColor((*pNodePtr)->left, BLACK);
        SetTreeNodeColor(pNode, BLACK);
        SetTreeNodeColor(*pNodePtr, rootColor);

    }
    return 1;
}

// The parameter pRoot is only used for generating the image of the binary search tree.
// In this recursive function, *pNodePtr might point to a sub-tree in the BST.
static DoubleBlackState RecursiveRBTreeDelete(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, 
                                              long numVal, long *pCnt, TraversalDirection direction) {
    // static long cnt = 0;
    assert(pCnt);
    char *graphName = "RBTree";
    char *fileName = "images/RBTree2Delete";
    RBTreeNodePtr pNode = *pNodePtr;

    DoubleBlackState dbState = NO_DOUBLE_BACK;
    
    if (pNode) {
        // It is on stack
        pNode->visited = 1;

        if (numVal < pNode->value.numVal) {
            dbState = RecursiveRBTreeDelete(pRoot, &(pNode->left), numVal, pCnt, TURN_LEFT);
        } else if (numVal > pNode->value.numVal) {
            dbState = RecursiveRBTreeDelete(pRoot, &(pNode->right), numVal, pCnt, TURN_RIGHT);
        } else {
            if (pNode->left == NULL && pNode->right == NULL) {   //// case 00: no child
                if (pNode->color == BLACK) {
                    if (direction == TURN_LEFT) {
                        dbState = DOUBLE_BLACK_FROM_LEFT;
                    } else {
                        dbState = DOUBLE_BLACK_FROM_RIGHT;
                    }
                }
                free(pNode);
                *pNodePtr = NULL;
                GEN_ONE_IMAGE("After deleting", numVal);   
                return dbState;  
            } else if (pNode->left == NULL && pNode->right != NULL) { // case 01: only right  
                RBTreeNodePtr tmp = pNode->right;
                assert(pNode->color == BLACK && pNode->right->color == RED);
                pNode->right->color = BLACK;
                free(pNode);
                *pNodePtr = tmp; 
                pNode = tmp;               
            } else if (pNode->left != NULL && pNode->right == NULL) { // case 10: only left
                // case 10
                RBTreeNodePtr tmp = pNode->left;
                assert(pNode->color == BLACK && pNode->left->color == RED);
                pNode->left->color = BLACK;
                free(pNode);
                *pNodePtr = tmp;
                pNode = tmp;
            } else { // case 11:  with two children                
                // Get pNode's in-order pPredecessor, which is right-most node in its left sub-tree.
                RBTreeNodePtr pPredecessor = BiTreeMaxValueNode(pNode->left);

                // (Swapping is done for clearer debugging output)
                // Swap the values of the node pointed to by pNode and its in-order predecessor
                NodeValue val = pNode->value;
                // Copy the predecessor's value (this copy is necessary)
                pNode->value = pPredecessor->value;
                pPredecessor->value = val;

                GEN_ONE_IMAGE("After swapping, recursively delete", pPredecessor->value.numVal);

                // Now, numVal is in left sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                dbState = RecursiveRBTreeDelete(pRoot, &pNode->left, pPredecessor->value.numVal, pCnt, TURN_LEFT);
            }
        }
        assert(pNode);
        // The deleted node has been replaced with a non-null node. 
        // If no double black or already fixed in lower tree nodes, return NO_DOUBLE_BACK
        if (dbState == NO_DOUBLE_BACK) {
            pNode->visited = 0;
            return NO_DOUBLE_BACK;            
        } 
        // Try to fix the double black propagated from downwards
        int fixed = 0;
        if (dbState == DOUBLE_BLACK_FROM_LEFT) { // from its left child
            /*
                      (*pNodePtr)
                          |
                          |
                          v
                         Node
                        /      \
                       /        \
                Deleted (B)    Sibling (R or B)
             */            
            if (hasRedRight(pNode)) {
                fixed = FixRedRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackRight(pNode) && pNode->right) {
                fixed = FixBlackRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } 
        } else if (dbState == DOUBLE_BLACK_FROM_RIGHT) { // from its right child
            /*
                          (*pNodePtr)
                              |
                              |
                              v
                             Node
                            /      \
                           /        \
                Sibling (R or B)    Deleted (B)
             */            
            if (hasRedLeft(pNode)) {
                fixed = FixRedLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackLeft(pNode) && pNode->left) {
                fixed = FixBlackLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            }            
        } 
        // If fixed, reset the state to be NO_DOUBLE_BACK
        if (fixed) {
            dbState = NO_DOUBLE_BACK;
        }
        // For algorithm visualization
        if (dbState != NO_DOUBLE_BACK) {
            // double black's view point
            GEN_ONE_IMAGE("Propagate the double black upwards to", pNode->value.numVal);
        } else {
            GEN_ONE_IMAGE("After fixing the double black at the child of ", pNode->value.numVal);
        }
        // The violation cannot be fixed, propagate it upwards.
        if (!fixed) {
            if (direction == TURN_LEFT) {
                dbState = DOUBLE_BLACK_FROM_LEFT;
            } else {
                dbState = DOUBLE_BLACK_FROM_RIGHT;
            }
        }     
        // It is not on stack
        pNode->visited = 0;        
    }
    return dbState;     
}

void RBTreeDelete(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    RecursiveRBTreeDelete(pRoot, pNodePtr, numVal, pCnt, TURN_LEFT);
    RBTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        pNode->color = BLACK;
    }
}

//////////////////////////////////////////  IsRBTree() //////////////////////////////////////////////

static int CheckRBTree(RBTreeNodePtr pNode) {
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
        if (RBTreeBlackHeight(pNode->left) != RBTreeBlackHeight(pNode->right)) {
            return 0;
        }

        // Property 5: All null nodes are black.  See hasBlackLeft() and hasBlackRight()

        // Recursively check left and right subtrees
        if (CheckRBTree(pNode->left) == 0) {
            return 0;
        }

        if (CheckRBTree(pNode->right) == 0) {
            return 0;
        }
    }
    return 1;
}

/*
    This function returns 1 when root is a legal RB tree.
    Otherwise, it returns 0.
 */
int IsRBTree(RBTreeNodePtr root) {
    if (root) {
        // Property 1: the root is black.
        if (root->color != BLACK) {
            return 0;
        }
        RBTreeResetBlackHeight(root);
        // Check other properties
        return CheckRBTree(root);
    } else {
        return 1;
    }
}

