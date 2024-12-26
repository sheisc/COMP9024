#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

// Max length of an identifier (e.g., the name for a tree node) 
#define MAX_ID_LEN 127

// Node value
typedef struct {
    // e.g, "9000", "Node A", "Node B"
    // char name[MAX_ID_LEN + 1];
    // value of an integer, e.g., 2024
    long numVal;
} NodeValue;


typedef enum {
    RED,    // 0
    BLACK,  // 1
} NodeColor;

struct RBTreeNode {
    // left subtree
    struct RBTreeNode *left;
    // right subtree
    struct RBTreeNode *right;
    // red or black
    NodeColor color;
    //The value of a binary tree node:
    NodeValue value; 

    // For testing and algorithm visualization
    // Speed up CheckRBTree()
    int blackHeight;
    // whether this node has been visited
    int visited;          
};

typedef struct RBTreeNode *RBTreeNodePtr;

typedef void (* NodeVisitor)(RBTreeNodePtr pNode);

/////////////////////////////////// Red Black Tree ///////////////////////////////////////////

RBTreeNodePtr CreateRBTree(void);

void ReleaseRBTree(RBTreeNodePtr root);

void InOrderTraversal(RBTreeNodePtr root, NodeVisitor visit);

//
void RBTreeInsert(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt);

//
// RBTreeNodePtr RBTreeSearch(RBTreeNodePtr root, long numVal);

//
void RBTreeDelete(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, long *pCnt);

int IsRBTree(RBTreeNodePtr root);

int hasRedLeft(RBTreeNodePtr pNode);
int hasBlackLeft(RBTreeNodePtr pNode);
int hasRedRight(RBTreeNodePtr pNode);
int hasBlackRight(RBTreeNodePtr pNode);

long RBTreeBlackHeight(struct RBTreeNode *pNode);

void RBTreeResetBlackHeight(struct RBTreeNode *pNode);

// for debugging
void SetIntermediateImages(long enableState);

// Ignore node color
long RBTreeGetRealHeight(struct RBTreeNode *pNode);

#endif


