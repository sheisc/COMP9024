#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

// Max length of an identifier (e.g., the name for a tree node) 
#define MAX_ID_LEN 127

// Node value
typedef struct {
    // e.g, "9000", "Node A", "Node B"
    char name[MAX_ID_LEN + 1];
    // value of an integer, e.g., 2024
    long numVal;
} NodeValue;


typedef enum {
    RED,    // 0
    BLACK,  // 1
} NodeColor;

struct RBTreeNode {
    // left subtree
    struct RBTreeNode *leftChild;
    // right subtree
    struct RBTreeNode *rightChild;
    //
    int blackHeight;
    // 
    NodeColor color;

    // whether this node has been visited; for algorithm visualization
    int visited;
    /*
        The value of a binary tree node:
        
        1. an integer for representing the node's value (e.g., 300), 
          
        2. a C string for representing its node name
     */
    NodeValue value;         
};

typedef struct RBTreeNode *RBTreeNodePtr;

typedef void (* NodeVisitor)(RBTreeNodePtr pNode);

/////////////////////////////////// Red Black Tree ///////////////////////////////////////////

RBTreeNodePtr CreateRBTreeNode(long numVal, char *nodeName, RBTreeNodePtr left, RBTreeNodePtr right);

void ReleaseRBTree(RBTreeNodePtr root);

void RedBlackTree2Dot(RBTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited);

void RBTreeGenOneImage(RBTreeNodePtr root, char *graphName, char *fileName, long seqNo);

int RBTreeBalanceFactor(RBTreeNodePtr root);

void PreOrderTraversal(RBTreeNodePtr root, NodeVisitor visit);

void InOrderTraversal(RBTreeNodePtr root, NodeVisitor visit);

void PostOrderTraversal(RBTreeNodePtr root, NodeVisitor visit);

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

#endif


