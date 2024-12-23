#include <assert.h> // assert()
#include <stdio.h>  // printf() and snprintf()
#include <stdlib.h> // malloc() and free()
#include <string.h> // memset()
#include "RedBlackTree.h"

//////////////////////////////// RBTreeNode //////////////////////////////////////////////////////

static struct RBTreeNode *CreateRBTreeNode(TreeNodeKeyTy k, NodeColor c) {
    struct RBTreeNode *pNode = (struct RBTreeNode *) malloc(sizeof(struct RBTreeNode));
    pNode->k = k;
    pNode->color = c;
    pNode->parent = NULL;
    pNode->left = NULL;
    pNode->right = NULL;
    return pNode;
}

static void ReleaseRBTreeNode(struct RBTreeNode *pNode) {
    if (pNode) {
        ReleaseRBTreeNode(pNode->left);
        ReleaseRBTreeNode(pNode->right);
        free(pNode);
    }
}

static int RBTreeNodeHasRedChild(struct RBTreeNode *pNode) {
    return (pNode->left && pNode->left->color == RED) || (pNode->right && pNode->right->color == RED);
}

static int RBTreeNodeIsOnLeft(struct RBTreeNode *pNode) {
    assert(pNode->parent);
    return pNode->parent->left == pNode;
}

// FIXME: if the node has a value, both the key-value pair should be swapped.
static void RBTreeNodeSwapKeys(struct RBTreeNode *pNode1, struct RBTreeNode *pNode2) {
    TreeNodeKeyTy tmp = pNode1->k;
    pNode1->k = pNode2->k;
    pNode2->k = tmp;
}

static struct RBTreeNode *RBTreeNodeGetSuccessor(struct RBTreeNode *pNode) {
    struct RBTreeNode *cur = pNode;
    while (cur->left) {
        cur = cur->left;
    }
    return cur;
}

static struct RBTreeNode *RBTreeNodeGetSibling(struct RBTreeNode *pNode) {
    if (pNode->parent == NULL) {
        return NULL;
    }
    
    if (RBTreeNodeIsOnLeft(pNode)) {
        return pNode->parent->right;
    } else {
        return pNode->parent->left;
    }
}

static struct RBTreeNode *RBTreeNodeGetUncle(struct RBTreeNode *pNode) {
    if (!pNode->parent ||!(pNode->parent->parent)) {
        return NULL;
    }

    if (RBTreeNodeIsOnLeft(pNode->parent)) {
        return pNode->parent->parent->right;
    } else {
        return pNode->parent->parent->left;
    }
}

//////////////////////////////// RBTree  /////////////////////////////////////////////////////////

struct RBTree {
    struct RBTreeNode *root;
};

struct RBTreeNode *RBTreeGetRoot(struct RBTree *pRBTree) {
    return pRBTree->root;
}


struct RBTree *CreateRBTree(void) {
    struct RBTree *pRBTree = (struct RBTree *) malloc(sizeof(struct RBTree));
    pRBTree->root = NULL;
    return pRBTree;
}

void ReleaseRBTree(struct RBTree *pRBTree) {
    if (pRBTree->root) {
        ReleaseRBTreeNode(pRBTree->root);        
    }
    free(pRBTree);
}

static long leftRotationCount = 0;

static long rightRotationCount = 0;

long GetLeftRotationCount(void) {
    return leftRotationCount;
}

long GetRightRotationCount(void) {
    return rightRotationCount;
}

/*
    -----------------------------------------------------------------------
      Before rotation                After left rotation
    -----------------------------------------------------------------------
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
static void RBTreeLeftRotate(struct RBTree *pRBTree, struct RBTreeNode *pNode) {
    struct RBTreeNode *pNodeA = pNode;
    struct RBTreeNode *pNodeC = pNodeA->right;
    struct RBTreeNode *pNodeB = pNodeC->left;
    // pRBTree->root
    if (pNodeA == pRBTree->root) {
        pRBTree->root = pNodeC;
    }
    // pNodeC
    pNodeC->parent = pNodeA->parent;
    pNodeC->left = pNodeA;
    if (pNodeA->parent) {
        if (RBTreeNodeIsOnLeft(pNodeA)) {
            pNodeA->parent->left = pNodeC;
        } else {
            pNodeA->parent->right = pNodeC;
        }
    }
    // pNodeA
    pNodeA->parent = pNodeC;
    pNodeA->right = pNodeB;
    // pNodeB, if it is not NULL
    if (pNodeB) {
        pNodeB->parent = pNodeA;
    }
    leftRotationCount++;
}

/*
    ------------------------------------------------------------------------
      Before rotation                           After right rotation
    ------------------------------------------------------------------------
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
static void RBTreeRightRotate(struct RBTree *pRBTree, struct RBTreeNode *pNode) {
    struct RBTreeNode *pNodeC = pNode;
    struct RBTreeNode *pNodeA = pNodeC->left;
    struct RBTreeNode *pNodeB = pNodeA->right;    
    // pRBTree->root
    if (pNodeC == pRBTree->root) {
        pRBTree->root = pNodeA;
    }
    // pNodeA
    pNodeA->parent = pNodeC->parent;
    if (pNodeC->parent) {
        if (RBTreeNodeIsOnLeft(pNodeC)) {
            pNodeC->parent->left = pNodeA;
        } else {
            pNodeC->parent->right = pNodeA;
        }
    }
    pNodeA->right = pNodeC;
    // pNodeC
    pNodeC->parent = pNodeA;
    pNodeC->left = pNodeB;
    // pNodeB
    if (pNodeB) {
        pNodeB->parent = pNodeC;
    }
    rightRotationCount++;
}

/*
    pNode is either the newly-inserted red node or a grandparent converted from black to red 
    in fixing its grandchild's red-uncle red-red violation.
 */
static void RBTreeFixRedRed(struct RBTree *pRBTree, struct RBTreeNode *pNode) {
    if (pNode == pRBTree->root) {
        pNode->color = BLACK;
        return;
    }

    struct RBTreeNode *parent = pNode->parent;

    if (parent->color == RED) {
        struct RBTreeNode *grandParent = pNode->parent->parent;
        struct RBTreeNode *uncle = RBTreeNodeGetUncle(pNode);        
        if (uncle && uncle->color == RED) { // red uncle
            parent->color = BLACK;
            uncle->color = BLACK;
            grandParent->color = RED;
            RBTreeFixRedRed(pRBTree, grandParent);
        } else { // black uncle (null is treated as black)
            if (RBTreeNodeIsOnLeft(parent)) {
                if (RBTreeNodeIsOnLeft(pNode)) {
                    /*
                        Left-Left
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (one rotation + recoloring)
                        ------------------------------------------------------------------------
                             grandParent (black)                         parent(black) 
                               /                                         /    \
                            parent (red)                       pNode (red)  grandParent(red)           
                            /                                               
                         pNode (red)                            
                        ------------------------------------------------------------------------               
                     */
                    RBTreeRightRotate(pRBTree, grandParent);
                    parent->color = BLACK;
                    grandParent->color = RED;
                } else {
                    /*
                        Left-Right
                        ------------------------------------------------------------------------
                          Before fixing                  After fixing (two rotations + recoloring)
                        ------------------------------------------------------------------------
                             grandParent (black)                       pNode (black) 
                               /                                         /    \
                            parent (red)                       parent(red)    grandParent(red)           
                               \                                            
                               pNode (red)                            
                        ------------------------------------------------------------------------               
                     */
                    RBTreeLeftRotate(pRBTree, parent);
                    RBTreeRightRotate(pRBTree, grandParent);
                    pNode->color = BLACK;
                    grandParent->color = RED;
                }
            } else {
                if (RBTreeNodeIsOnLeft(pNode)) {
                    /*
                        Right-Left
                        ------------------------------------------------------------------------
                          Before fixing                  After fixing (two rotations + recoloring)
                        ------------------------------------------------------------------------
                             grandParent (black)                        pNode (black) 
                                   \                                         /    \
                                 parent (red)                   grandParent(red)   parent(red)           
                                  /                                           
                               pNode (red)                            
                        ------------------------------------------------------------------------               
                     */
                    RBTreeRightRotate(pRBTree, parent);
                    RBTreeLeftRotate(pRBTree, grandParent);
                    pNode->color = BLACK;
                    grandParent->color = RED;
                } else {
                    /*
                        Right-Right
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (one rotation + recoloring)
                        ------------------------------------------------------------------------
                             grandParent (black)                         parent(black) 
                                   \                                         /    \
                                  parent (red)                 grandParent(red)   pNode (red)             
                                     \                                               
                                    pNode (red)                            
                        ------------------------------------------------------------------------               
                     */
                    RBTreeLeftRotate(pRBTree, grandParent);
                    parent->color = BLACK;
                    grandParent->color = RED;
                }
            }
        }
    }
}

/*
    pNode points to a node marked as double black indicating where the imbalance occurs.
    It can either be the node to be deleted or its ancestor when the imbalance propagated 
    from bottom to up.
 */
static void RBTreeFixDoubleBlack(struct RBTree *pRBTree, struct RBTreeNode *pNode) {   
    if (pNode == pRBTree->root) {
        return;
    } 

    struct RBTreeNode *sibling = RBTreeNodeGetSibling(pNode);
    struct RBTreeNode *parent = pNode->parent;

    assert(sibling != NULL);
    assert(pNode->color == BLACK);
    if (sibling->color == RED) {
        /*
            Since pNode is black and sibling is red, we have the following constraints.

            1 sibling must have two black nephews (not NULL)
            2 parent is BLACK
         */
        assert(parent->color == BLACK);        
        assert(sibling->left && sibling->left->color == BLACK);
        assert(sibling->right && sibling->right->color == BLACK);   
        if (RBTreeNodeIsOnLeft(sibling)) {
            /*
                ------------------------------------------------------------------------
                  Before fixing                 After fixing (one rotation + recoloring)
                ------------------------------------------------------------------------
                       parent (B)                              sibling (B)
                        /    \                                 /    \
                 sibling (R)  pNode (B)                           parent (R)              
                       \                                             /    \
                    nephew (B)                                 nephew (B)   pNode (B)
                ------------------------------------------------------------------------               
             */              
            RBTreeRightRotate(pRBTree, parent);
        } else {
            /*
                ------------------------------------------------------------------------
                  Before fixing                 After fixing (one rotation + recoloring)
                ------------------------------------------------------------------------
                      parent (B)                                  sibling (B)
                       /    \                                     /    
                 pNode (B)    sibling (R)                   parent (R)              
                             /                                 /  \
                          nephew (B)                   pNode (B)   nephew (B)
                ------------------------------------------------------------------------               
             */                          
            RBTreeLeftRotate(pRBTree, parent);
        }
        parent->color = RED;
        sibling->color = BLACK;
        /*
            After left/right rotation, 
            the red-sibling problem has been converted to the black-sibling and red-parent problem.
         */
        RBTreeFixDoubleBlack(pRBTree, pNode);
    } else {
        if (RBTreeNodeHasRedChild(sibling)) {
            // the imbalance of black height can be fixed locally
            struct RBTreeNode *nephew = NULL;
            if (sibling->left && sibling->left->color == RED) {
                nephew = sibling->left;
                // one red nephew is on the sibling's left (one or two red nephews)
                if (RBTreeNodeIsOnLeft(sibling)) {
                    /*
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (one rotation + recoloring)
                        ------------------------------------------------------------------------
                                   parent                            sibling                                              
                                /         \                        /         \                                                 
                          sibling (B)     pNode (B)          nephew (B)       parent (B)                                      
                              /                                                 \                                                 
                          nephew (R)                                          pNode(B)                                                                        
                        ------------------------------------------------------------------------               
                     */                  
                    RBTreeRightRotate(pRBTree, parent);
                    nephew->color = BLACK;                    
                    sibling->color = parent->color;
                    parent->color = BLACK;                    
                } else {
                    /*
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (two rotation + recoloring)
                        ------------------------------------------------------------------------
                                   parent                            nephew                                              
                                /         \                        /         \                                                 
                          pNode (B)     sibling (B)           parent (B)     sibling (B)                                   
                                        /                       /                                                                 
                                     nephew (R)             pNode(B)                                                                        
                        ------------------------------------------------------------------------               
                     */                       
                    RBTreeRightRotate(pRBTree, sibling);
                    RBTreeLeftRotate(pRBTree, parent);
                    nephew->color = parent->color;
                    parent->color = BLACK;                    
                }
            } else {
                nephew = sibling->right;
                // the only red nephew is on the sibling's right
                if (RBTreeNodeIsOnLeft(sibling)) {
                    /*
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (two rotations + recoloring)
                        ------------------------------------------------------------------------
                                   parent                            nephew                                              
                                /         \                        /         \                                                 
                          sibling (B)    pNode (B)           sibling (B)     parent (B)                                   
                                \                                               \                                                 
                                nephew (R)                                     pNode (B)                                                             
                        ------------------------------------------------------------------------               
                     */                     
                    RBTreeLeftRotate(pRBTree, sibling);
                    RBTreeRightRotate(pRBTree, parent);
                    nephew->color = parent->color;
                    parent->color = BLACK;                    
                } else {
                    /*
                        ------------------------------------------------------------------------
                          Before fixing                 After fixing (one rotation + recoloring)
                        ------------------------------------------------------------------------
                                   parent                            sibling                                              
                                /         \                        /         \                                                 
                          pNode (B)     sibling (B)           parent (B)     nephew (B)                                   
                                            \                    /                                                                 
                                          nephew (R)          pNode(B)                                                                        
                        ------------------------------------------------------------------------               
                     */ 
                    RBTreeLeftRotate(pRBTree, parent);
                    nephew->color = BLACK;
                    sibling->color = parent->color;
                    parent->color = BLACK;                    
                }
            }
        } else { // no red nephew
            sibling->color = RED;
            if (parent->color == BLACK) {
                RBTreeFixDoubleBlack(pRBTree, parent);
            } else {
                // compensate the loss of black height in its children
                parent->color = BLACK;
            }
        }
    }
}

/*
    pNode is the node to be deleted.
 */
static void RBTreeDeleteNode(struct RBTree *pRBTree, struct RBTreeNode *pNode) {
    struct RBTreeNode *pReplacement = NULL; 
    struct RBTreeNode *parent = pNode->parent;
    
    if (pNode->left == NULL && pNode->right == NULL) {  // pNode has no child
        if (pNode == pRBTree->root) {
            pRBTree->root = NULL;
        } else {
            if (pNode->color == BLACK) { // Need to fix doubl black when deleting a black node
                RBTreeFixDoubleBlack(pRBTree, pNode);
            } 
            // deattach from its parent
            if (RBTreeNodeIsOnLeft(pNode)) {
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        }
        free(pNode);
    } else if (pNode->left && pNode->right) { // pNode has two children
        /*
            pReplacment is pNode's in-order successor, the left-most node in pNode's right subtree.
            So, pReplacement has no more than one child.
            After swapping, a two-child problem is converted into one-child or zero-child problem.
         */    
        pReplacement = RBTreeNodeGetSuccessor(pNode->right);
        RBTreeNodeSwapKeys(pReplacement, pNode);
        assert(!(pReplacement->left && pReplacement->right));        
        RBTreeDeleteNode(pRBTree, pReplacement);
    } else { // pNode has only one child. 
        /*
            According to the red-black tree's properties, it means that
            
            1 pNode's color must be black.
            2 Its child's color must be RED when pNode has only one child.
            3 There are only two nodes in the subtree pointed to by pNode.
         */
        pReplacement = pNode->left ? pNode->left : pNode->right;
        assert(pNode->color == BLACK);
        assert(pReplacement->color == RED);

        if (pNode == pRBTree->root) {
            // After deletion, there will be only one node.
            pNode->k = pReplacement->k;
            pNode->left = NULL;
            pNode->right = NULL;
            free(pReplacement);            
        } else {
            /*
                pNode is not root. 
                So its parent is not NULL. 
                Replace pNode with pReplacement
             */
            if (RBTreeNodeIsOnLeft(pNode)) {
                parent->left = pReplacement;
            } else {
                parent->right = pReplacement;
            }
            free(pNode);
            pReplacement->parent = parent;
            pReplacement->color = BLACK;
        }        
    }
}

static void RBTreeInorderTraverse(struct RBTree *pRBTree, struct RBTreeNode *pNode) {
    if (pNode) {
        RBTreeInorderTraverse(pRBTree, pNode->left);
        printf("%ld ", (long) pNode->k);
        RBTreeInorderTraverse(pRBTree, pNode->right);
    }
}

/*
    If k is found
        return the node which contains k (e.g., see RBTreeDelete()). 
    else
        return the last node while traversing (e.g., see RBTreeInsert()).
 */
struct RBTreeNode *RBTreeSearch(struct RBTree *pRBTree, TreeNodeKeyTy k) {
    struct RBTreeNode *cur = pRBTree->root;
    while (cur) {
        if (k < cur->k) {
            if (cur->left) {
                cur = cur->left;
            } else {
                break;
            }
        } else if (k == cur->k) {
            break;
        } else {
            if (cur->right) {
                cur = cur->right;
            } else {
                break;
            }
        }
    }
    return cur;
}

void RBTreeInsert(struct RBTree *pRBTree, TreeNodeKeyTy k) {
    if (pRBTree->root == NULL) {
        pRBTree->root = CreateRBTreeNode(k, BLACK);
    } else {
        struct RBTreeNode *pNode = RBTreeSearch(pRBTree, k);
        if (pNode->k == k) {
            return;
        }
        // create newNode and insert it into pNode
        struct RBTreeNode *newNode = CreateRBTreeNode(k, RED);
        newNode->parent = pNode;
        if (k < pNode->k) {
            pNode->left = newNode;
        } else {
            pNode->right = newNode;
        }
        // Fix violations
        RBTreeFixRedRed(pRBTree, newNode);
    }
}

void RBTreeDelete(struct RBTree *pRBTree, TreeNodeKeyTy k) {
    if (pRBTree->root == NULL) {
        return;
    }

    struct RBTreeNode *pNode = RBTreeSearch(pRBTree, k);

    if (pNode->k != k) {
        return;
    }
    RBTreeDeleteNode(pRBTree, pNode);
}

void RBTreePrintInOrder(struct RBTree *pRBTree) {
    RBTreeInorderTraverse(pRBTree, pRBTree->root);
    printf("\n");
}

///////////////////////////////////////////// IsRBTree() //////////////////////////////////

long RBTreeBlackHeight(struct RBTreeNode *pNode) {
    if (pNode) {
        long leftLen = RBTreeBlackHeight(pNode->left);
        long rightLen = RBTreeBlackHeight(pNode->right);
        assert(leftLen == rightLen);
        if (pNode->color == BLACK) {
            return 1 + leftLen;
        } else {
            return leftLen;
        }
    } else {
        return 0;
    }
}

static int CheckRBTree(struct RBTreeNode *pNode) {
    if (pNode) {
        // Property 2: a node is either BLACK or RED
        if (pNode->color != BLACK && pNode->color != RED) {
            return 0;
        }

        if (pNode->color == RED) {
            // Property 3: a red node shouldn't have any red child.
            if (RBTreeNodeHasRedChild(pNode)) {
                return 0;
            }
        }

        // Property 4: The left and right subtrees have the same black height.
        if (RBTreeBlackHeight(pNode->left) != RBTreeBlackHeight(pNode->right)) {
            return 0;
        }

        // Property 5: All null nodes are black.  

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

    This function can be improved by using a table black_height[].
 */
int IsRBTree(struct RBTree *pRBTree) {
    struct RBTreeNode *root = pRBTree->root;
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

