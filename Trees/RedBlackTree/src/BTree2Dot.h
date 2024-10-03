#ifndef BTREE2DOT_H
#define BTREE2DOT_H

#include "BiTree.h"



void BTreeGenOneImage(struct BTreeNode *root, char *graphName, char *fileName, long seqNo);

/*
    2-3-4 Tree to dot
 */
void BTree2Dot(struct BTreeNode *root, 
               char *filePath,
               char *graphName,
               int displayVisited);



#endif

