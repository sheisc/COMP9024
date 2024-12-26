#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

typedef long TreeNodeKeyTy;

struct SplayTreeNode {
    TreeNodeKeyTy k;
    struct SplayTreeNode *left;
    struct SplayTreeNode *right;
    // for speeding up calculation of tree height during testing
    long height;
};

typedef struct SplayTreeNode *SplayTreeNodePtr;

struct SplayTreeNode *CreateSplayTree(void);

void ReleaseSplayTree(struct SplayTreeNode *pNode);

void SplayTreeInsert(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k);

int SplayTreeSearch(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k);

void SplayTreeDelete(SplayTreeNodePtr *pNodePtr, TreeNodeKeyTy k);

void SplayTreePrintInOrder(struct SplayTreeNode *root);

void SplayTreeGenOneImage(struct SplayTreeNode *root, char *graphName, char *fileName, long seqNo, char *comment);

long SplayTreeGetHeight(struct SplayTreeNode *root);

#endif



