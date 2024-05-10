#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#define MAX_ID_LEN 63


typedef enum {
    // '+', '-', '*', '/'
    BT_OPERATOR = 1,
    // integer value
    BT_OPERAND,
} BiTreeNodeKind;

typedef int BiTreeNodeValue;

/*
  (1) define a structure
      
      struct BiTree {

      };

  (2) define a pointer type which points to a BiTree

      typedef struct BiTree *BiTreePtr;   // BiTreePtr is a type, not a variable.

      SideTrack:

            struct BiTree *root;          // root is a variable.
    
  (3) The above two type definitions can be merged into one.

      typedef struct BiTree {

      } *BiTreePtr;
 */

#if 1
// A structure for describing a binary tree
struct BiTree {
    // left sub-tree
    struct BiTree *left;
    // right sub-tree
    struct BiTree *right;

    // An operand node or an operator node
    BiTreeNodeKind kind;
    // the value of the node
    BiTreeNodeValue val;
    // string representation of the node name.    
    // name[MAX_ID_LEN-1] is reserved for '\0', 
    // the end of of a C string.
    char name[MAX_ID_LEN + 1];
};

typedef struct BiTree *BiTreePtr;
// typedef struct BiTree * BiTreePtr;


#else

typedef struct BiTree {
    // left sub-tree
    struct BiTree *left;
    // right sub-tree
    struct BiTree *right;

    // An operand node or an operator node
    BiTreeNodeKind kind;
    // the value of the node
    BiTreeNodeValue val;
    // string representation of the node name.    
    // name[MAX_ID_LEN-1] is reserved for '\0', 
    // the end of of a C string.
    char name[MAX_ID_LEN + 1];
} *BiTreePtr;

#endif

BiTreePtr CreateBiTree(BiTreeNodeKind kind, 
                       BiTreeNodeValue val,
                       BiTreePtr left,
                       BiTreePtr right);

int TestBinaryTree(void);
void ReleaseBiTree(BiTreePtr root);

#endif


