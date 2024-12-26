#include <stdio.h>
#include <stdlib.h> // srandom(), random()
#include <time.h>   // time()
#include <assert.h>
#include "SplayTree.h"

#define  ILLEGAL_TREE_HEIGHT        -1

/////////////////////////////// SplayTree ///////////////////////////////////////////
typedef enum {
    SPLAY_TREE_SEARCH,
    SPLAY_TREE_INSERT,
    SPLAY_TREE_DELETE,
} SPLAY_TREE_OPERATION;


static struct SplayTreeNode *CreateSplayTreeNode(TreeNodeKeyTy k) {
    struct SplayTreeNode *pNode = (struct SplayTreeNode *) malloc(sizeof(struct SplayTreeNode));
    pNode->k = k;
    pNode->left = NULL;
    pNode->right = NULL;
    pNode->height = ILLEGAL_TREE_HEIGHT;
    return pNode;
}

static void FreeOneNode(struct SplayTreeNode *pNode) {
    free(pNode);
}

struct SplayTreeNode *CreateSplayTree(void) {
    return NULL;
}

void ReleaseSplayTree(struct SplayTreeNode *pNode) {
    if (pNode) {
        ReleaseSplayTree(pNode->left);
        ReleaseSplayTree(pNode->right);
        FreeOneNode(pNode);
    }
}

static void UpdateHeight(struct SplayTreeNode *pNode) {
    if (pNode) {
        int max = 0;
        if (pNode->left && pNode->left->height > max) {
            max = pNode->left->height;
        }
        if (pNode->right && pNode->right->height > max) {
            max = pNode->right->height;
        }
        pNode->height = max + 1;
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
void SplayTreeRightRotate(SplayTreeNodePtr *pNodePtr) {
    SplayTreeNodePtr pNodeC = *pNodePtr;
    SplayTreeNodePtr pNodeA = pNodeC->left;
    SplayTreeNodePtr pNodeB = pNodeA->right;

    pNodeA->right = pNodeC;
    pNodeC->left = pNodeB;
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
void SplayTreeLeftRotate(SplayTreeNodePtr *pNodePtr) {
    SplayTreeNodePtr pNodeA = *pNodePtr;
    SplayTreeNodePtr pNodeC = pNodeA->right;
    SplayTreeNodePtr pNodeB = pNodeC->left;

    pNodeC->left = pNodeA;
    pNodeA->right = pNodeB;
    UpdateHeight(pNodeA);
    UpdateHeight(pNodeC);
    // Let *pNodePtr point to NodeC
    *pNodePtr = pNodeC;
}

/*
    Splay() acts like a standard BST search(),
    but performs insertions, deletions, and rotations during stack unwinding in recursion.
 */
static void Splay(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k, SPLAY_TREE_OPERATION operation) {    
    if (*pNodePtr == NULL) {
        if (operation == SPLAY_TREE_INSERT) {
            *pNodePtr = CreateSplayTreeNode(k);
        }
        return;
    }
    struct SplayTreeNode *root = *pNodePtr;
    //
    if (root->k == k) {
        // Found one
        if (operation == SPLAY_TREE_DELETE) {
            if (root->left == NULL) { // case 00 and case 01
                *pNodePtr = root->right;                
            } else { // case 10 and case 11
                /*
                    k is larger than any key in the subtree pointed to by root->left.
                    So after splay(&(root->left), k), the updated subtree root->left
                    will have no right child tree.
                    Then we can delete the node and merge its two subtrees.
                */            
                Splay(&root->left, k, SPLAY_TREE_DELETE);
                root->left->right = root->right;                
                *pNodePtr = root->left;                
            }
            FreeOneNode(root);
        }
    } else if ((root->k) > k) {
        if (root->left == NULL) {
            if (operation == SPLAY_TREE_INSERT) {
                root->left = CreateSplayTreeNode(k);                
            } else {
                return;
            }
        }
        Splay(&(root->left), k, operation);
        // when action == SPLAY_TREE_DELETE, root->left might be NULL after Splay()
        if (root->left != NULL) {
            SplayTreeRightRotate(pNodePtr);
        }
    } else {
        if (root->right == NULL) {
            if (operation == SPLAY_TREE_INSERT) {
                root->right = CreateSplayTreeNode(k);
            } else {
                return;
            }
        }
        Splay(&(root->right), k, operation);
        // when action == SPLAY_TREE_DELETE, root->right might be NULL after Splay()
        if (root->right != NULL) {
            SplayTreeLeftRotate(pNodePtr);
        }
    }
}

void SplayTreeInsert(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k) {
    Splay(pNodePtr, k, SPLAY_TREE_INSERT);
}

int SplayTreeSearch(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k) {
    Splay(pNodePtr, k, SPLAY_TREE_SEARCH);
    struct SplayTreeNode *root = *pNodePtr;
    if (root && (root->k == k)) {
        return 1;
    } else {
        return 0;
    }
}

void SplayTreeDelete(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k) {
    Splay(pNodePtr, k, SPLAY_TREE_DELETE);  
}

void SplayTreePrintInOrder(struct SplayTreeNode *root) {
    if (root != NULL) {
        SplayTreePrintInOrder(root->left);
        printf("%ld ", (long)root->k);
        SplayTreePrintInOrder(root->right);
    }
}

long SplayTreeGetHeight(struct SplayTreeNode *root) {
    if (root == NULL) {
        return 0;
    } else {
        if (root->height != ILLEGAL_TREE_HEIGHT) {
            return root->height;
        }
        long leftDepth = SplayTreeGetHeight(root->left);
        long rightDepth = SplayTreeGetHeight(root->right);
        long max = leftDepth > rightDepth ? leftDepth : rightDepth;
        root->height = max + 1;
        return max + 1;
    }
}

