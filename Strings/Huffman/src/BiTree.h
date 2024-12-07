#ifndef BI_TREE_H
#define BI_TREE_H

// Max length of an identifier (e.g., the name for a tree node) 
#define MAX_ID_LEN 127

typedef struct {
    // e.g, "9000", "Node A", "Node B"
    char name[MAX_ID_LEN + 1];
    // value of an integer, e.g., 2024
    long frequency;
    //
    long charValue;
} NodeValue;


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
    // whether this node has been visited
    int visited;
};

typedef struct BiTreeNode *BiTreeNodePtr;

typedef void (* NodeVisitor)(BiTreeNodePtr pNode);


BiTreeNodePtr CreateBinaryTreeNode(long numVal, char *nodeName, BiTreeNodePtr left, BiTreeNodePtr right);

void BiTree2Dot(BiTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited);

// Based on BiTree2Dot()
void GenOneImage(BiTreeNodePtr root, char *graphName, char *fileName, long seqNo);

#endif

