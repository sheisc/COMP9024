#ifndef RBTREE2DOT_H
#define RBTREE2DOT_H

#include "RedBlackTree.h"

void RBTreeGenOneImage(RBTreeNodePtr root, char *graphName, char *fileName, long seqNo);

void RedBlackTree2Dot(RBTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited);

#endif

