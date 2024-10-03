#ifndef RBTREE2DOT_H
#define RBTREE2DOT_H

#include "BiTree.h"

void RBTreeGenOneImage(BiTreeNodePtr root, char *graphName, char *fileName, long seqNo);

void BiTree2Dot(BiTreeNodePtr root, 
               char *filePath,
               char *graphName,
               int displayVisited);

#endif

