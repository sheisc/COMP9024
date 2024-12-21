#ifndef BTREE_H
#define BTREE_H

// 2-3-4 Tree:  a special case of a B-tree
#define  NUM_OF_KEYS      3
#define  NUM_OF_CHILDREN  4

#define BTREE_NODE_NAME_LEN     1024

typedef long TreeNodeKeyTy;

struct BTreeNode {
    // one slot more than NUM_OF_KEYS
    TreeNodeKeyTy keys[NUM_OF_KEYS+1];
    struct BTreeNode *children[NUM_OF_CHILDREN];
    // the number of keys stored in this node
    long nk;    
    // whether this node is the leaf node
    int leaf;
    //
    int visited;
};

struct BTreeNode *CreateBTree(void);

void ReleaseBTree(struct BTreeNode *root) ;

struct BTreeNode *RBTreeTo234Tree(RBTreeNodePtr root);

long GetNumberOfChildren(struct BTreeNode *root);

char *GetBTreeNodeName(struct BTreeNode *root, char *nodeName, long n);

#endif


