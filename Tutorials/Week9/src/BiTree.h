#ifndef BI_TREE_H
#define BI_TREE_H

// Max length of an identifier (e.g., the name for a tree node) 
#define MAX_ID_LEN 127

// token value
typedef struct {
    // e.g, "9000", "Node A", "Node B"
    char name[MAX_ID_LEN + 1];
    // value of an integer, e.g., 2024
    long numVal;
} NodeValue;


/*
    During the depth-first traversal of a binary tree, 
    there are three times we arrive at a binary tree node:

    (1) from its parent 
    (2) from its left child
    (3) from its right child

    We can use the following node states to represent different stages 
    when we walk through a binary tree node in a non-recursive algorithm.


                 |
                 |  NS_FROM_UP
                 |
                 v
          ----------------
          |              |
          | BiTree Node  |
          ----------------
                ^  ^
               /    \
              /      \
             /        \
    NS_FROM_LEFT        NS_FROM_RIGHT

    It is a simple finite-state machine, with the following state transition:

     NS_FROM_UP  --->  NS_FROM_LEFT  ---> NS_FROM_RIGHT

    For more information about the finite-state machine (Finite Automata, including DFA/NFA), 
    please refer to COMP9102 (Programming Languages and Compilers).

    For a non-recursive pre-order traversal,
      we need to visit the top node on the stack when it is at state NS_FROM_UP.
    For a non-recursive in-order traversal,
      we need to visit the top node on the stack when it is at state NS_FROM_LEFT.
    For a non-recursive post-order traversal,
      we need to visit the top node on the stack when it is at state NS_FROM_RIGHT.
 */

typedef enum {
    NS_FROM_UP,      // 0   
    NS_FROM_LEFT,    // 1
    NS_FROM_RIGHT,   // 2
} NodeState;


struct BiTreeNode {
    /*
     The value of a binary tree node:
  
     1. an integer for representing the node's value (e.g., 300), 
      
     2. a C string for representing its node name
     */
    NodeValue value;  
    // left subtree
    struct BiTreeNode *leftChild;
    // right subtree
    struct BiTreeNode *rightChild;
    // the current state when this node is pushed on the data stack (in non-recursive traversals)
    NodeState state;
    // whether this node has been visited
    int visited;
};

typedef struct BiTreeNode *BiTreeNodePtr;

typedef void (* NodeVisitor)(BiTreeNodePtr pNode);

void PreOrderTraversal(BiTreeNodePtr root, NodeVisitor visit);
void InOrderTraversal(BiTreeNodePtr root, NodeVisitor visit);
void PostOrderTraversal(BiTreeNodePtr root, NodeVisitor visit);



BiTreeNodePtr CreateBinaryTreeNode(long numVal, char *nodeName, BiTreeNodePtr left, BiTreeNodePtr right);

//BiTreeNodePtr CreateBinaryTree(void);

void ReleaseBinaryTree(BiTreeNodePtr root);

void BiTree2Dot(BiTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited);

// Based on Graph2Dot()
void GenOneImage(BiTreeNodePtr root, char *graphName, char *fileName, long seqNo);

/////////////////////////////////// Binary Search Tree ///////////////////////////////////////////

void BiTreeInsert(BiTreeNodePtr *p2NodePtr, long numVal, char *nodeName);


BiTreeNodePtr BiTreeSearch(BiTreeNodePtr root, long numVal);


BiTreeNodePtr BiTreeMinValueNode(BiTreeNodePtr root);

//void BiTreeDelete(BiTreeNodePtr *p2NodePtr, long numVal);
void BiTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *p2NodePtr, long numVal);

#endif
