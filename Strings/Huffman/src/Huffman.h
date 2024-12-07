#ifndef HUFFMAN_H
#define HUFFMAN_H

struct BiTreeNode;

typedef struct BiTreeNode *BiTreeNodePtr;

struct HuffmanInfo;

struct HuffmanInfo *CreateHuffmanInfoFromText(char *text);

/*
    The caller is responsible for releasing the heap space.
 */
char *HuffmanEncode(struct HuffmanInfo *pHuffmanInfo, char *text);

/*
    The caller is responsible for releasing the heap space.
 */
char *HuffmanDecode(struct HuffmanInfo *pHuffmanInfo, char *encodedStr);


void ReleaseHuffmanInfo(struct HuffmanInfo *pHuffmanInfo);


//struct BiTreeNode *GetHuffmanTree(struct HuffmanInfo *pHuffmanInfo);

void HuffmanInfo2Dot(struct HuffmanInfo *pHuffmanInfo, 
                     char *filePath,
                     char *graphName,
                     int displayVisited);

// Based on HuffmanInfo2Dot()
void GenOneImage(struct HuffmanInfo *pHuffmanInfo, char *graphName, char *fileName, long seqNo);

#endif
