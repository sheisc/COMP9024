#ifndef BPLUSTREE2DOT_H
#define BPLUSTREE2DOT_H




void BPlusTreeGenOneImage(struct BPlusTree *pBPlusTree, char *graphName, char *fileName, long seqNo);

/*
    Convert a BPlusTree to a dot file
 */
void BPlusTree2Dot(struct BPlusTree *pBPlusTree, char *filePath, char *graphName, int displayVisited);

#endif

