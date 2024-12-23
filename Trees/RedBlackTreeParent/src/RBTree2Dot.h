#ifndef RBTREE2DOT_H
#define RBTREE2DOT_H

#include "RedBlackTree.h"


void RBTreeGenOneImage(struct RBTree *pRBTree, char *graphName, char *fileName, long seqNo);

void RedBlackTree2Dot(struct RBTree *pRBTree, char *filePath, char *graphName, int displayVisited);


#endif

