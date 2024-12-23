#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

typedef enum {
    RED,
    BLACK,
} NodeColor;

typedef long TreeNodeKeyTy;

struct RBTreeNode {
    TreeNodeKeyTy k;
    NodeColor color;
    struct RBTreeNode *left;
    struct RBTreeNode *right;
    struct RBTreeNode *parent;
};


// typedef struct RBTreeNode *RBTreeNodePtr;



struct RBTree;

struct RBTree *CreateRBTree(void);

void ReleaseRBTree(struct RBTree *pRBTree);

struct RBTreeNode *RBTreeSearch(struct RBTree *pRBTree, TreeNodeKeyTy k);

void RBTreeDelete(struct RBTree *pRBTree, TreeNodeKeyTy k);

void RBTreeInsert(struct RBTree *pRBTree, TreeNodeKeyTy k);

void RBTreePrintInOrder(struct RBTree *pRBTree);

struct RBTreeNode *RBTreeGetRoot(struct RBTree *pRBTree);

int IsRBTree(struct RBTree *pRBTree);

long RBTreeBlackHeight(struct RBTreeNode *pNode);

long GetLeftRotationCount(void);

long GetRightRotationCount(void);

#endif



