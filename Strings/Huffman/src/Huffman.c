#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BiTree.h"
#include "Huffman.h"

// ASCII table
#define   SIZE_OF_ALPHABET    256

// Including internal nodes and leaves in a Huffman tree (a full binary tree)
#define   NUM_OF_ALL_NODES    (2 * SIZE_OF_ALPHABET)

//
#define   INTERNAL_NODE_CHARVAL        (SIZE_OF_ALPHABET + 1)

// 
#define   ILLEGAL_HUFFMAN_CODE                  -1

// when the text is "aaaa", only one kind of character 
#define     SPECIAL_CASE_ENCODED_CHAR       '0'

struct HuffmanInfo {
    /*
        Huffman tree is a full binary tree, with (numOfLeaves + numOfInternalNodes) nodes

        nodes point to an array of binary tree nodes.
     */
    struct BiTreeNode *nodes;
    // the number of leaves
    long numOfLeaves;
    // the number of internal nodes
    long numOfInternalNodes;
    /*
        For a charater with the value charVal,

        convertTable[charVal] is its string representation in Huffman encoding.        
     */
    char convertTable[SIZE_OF_ALPHABET][SIZE_OF_ALPHABET];
};

static void InitBinaryTreeNode(struct BiTreeNode *pNode, 
                               long charValue,
                               long frequency, 
                               char *nodeName, 
                               BiTreeNodePtr left, 
                               BiTreeNodePtr right) {
    //
    memset(pNode, 0, sizeof(*pNode));

    pNode->value.charValue = charValue;
    pNode->value.frequency = frequency;
    if (nodeName == NULL) {
        if (charValue != INTERNAL_NODE_CHARVAL) {
            snprintf(pNode->value.name, MAX_ID_LEN, "\'%c\'_%ld", (char) charValue, frequency);
        } else {
            snprintf(pNode->value.name, MAX_ID_LEN, "%ld", frequency);
        }
    } else {
        snprintf(pNode->value.name, MAX_ID_LEN, "%s", nodeName);
    }
    pNode->leftChild = left;
    pNode->rightChild = right;
    pNode->visited = 0;   
}

/*
    There are n elements in pNode[0 .. n-1],
    the first nSorted elements pNode[0 .. nSorted-1] are sorted.
 */

static void InsertionSort(struct BiTreeNode *pNode, long numOfSorted, long n) {
    for (long i = numOfSorted; i < n; i++) {
        struct BiTreeNode node = pNode[i];

        long j = i - 1;

        while (j >= 0 && pNode[j].value.frequency > node.value.frequency) {
            pNode[j + 1] = pNode[j];
            j = j - 1;
        }

        pNode[j + 1] = node;
    }
}

/*
    When the text is "aaaa", 
    the root node of a Huffman tree is not a node with the INTERNAL_NODE_CHARVAL
 */

static struct BiTreeNode *BuildHuffmanTree(struct HuffmanInfo *pHuffmanInfo) {
    long nodeCount = pHuffmanInfo->numOfLeaves;
    struct BiTreeNode *pNode = pHuffmanInfo->nodes;

    InsertionSort(pNode, 1, nodeCount);
    while (nodeCount > 1) {                    
        long sum = pNode[0].value.frequency + pNode[1].value.frequency;
        // pNode[nodeCount] is used as an internal node
        InitBinaryTreeNode(pNode + nodeCount, INTERNAL_NODE_CHARVAL, sum, NULL, pNode, pNode+1);
        // skip the first two nodes
        pNode += 2;
        // Two leaf nodes removed, but one internal node added
        nodeCount--;
        // The first (nodeCount-1) elements are sorted
        InsertionSort(pNode, nodeCount-1, nodeCount);
    }

    return pNode;
}

static int IsLeafNode(struct BiTreeNode *root) {
    if (root) {
        return !root->leftChild && !root->rightChild;
    } else {
        return 0;
    }
}


static void GenerateConvertTable(struct BiTreeNode *root, long depth, char trace[], char convertTable[][SIZE_OF_ALPHABET]) {
    if (root) {
        if (!root->leftChild && !root->rightChild) {
            // leaf node
            trace[depth] = 0;
            long charVal = root->value.charValue;
            strncpy(convertTable[charVal], trace, depth + 1);
            //printf("\'%c\': %s\n", (char) charVal, convertTable[charVal]);
        } else {
            trace[depth] = '0';
            GenerateConvertTable(root->leftChild, depth + 1, trace, convertTable);

            trace[depth] = '1';
            GenerateConvertTable(root->rightChild, depth + 1, trace, convertTable);
        } 
    }
}

/*
    Assume pEncodeStr is legal.
 */
static long DecodeOneChar(struct HuffmanInfo *pHuffmanInfo, struct BiTreeNode *root, char **pEncodedStr) {
    assert(root);
    // Special case: "aaaa"
    if (pHuffmanInfo->numOfLeaves == 1) {
        if (**pEncodedStr == SPECIAL_CASE_ENCODED_CHAR) {
            (*pEncodedStr)++;
            return (unsigned char) root->value.charValue;
        } else {
            return ILLEGAL_HUFFMAN_CODE;
        }
    }
    struct BiTreeNode *curNode = root;
    char ch;
    while (!IsLeafNode(curNode) && ((ch = **pEncodedStr) != 0)) {        
        if (ch == '0') {
            curNode = curNode->leftChild;
        } else {
            assert(ch == '1');
            curNode = curNode->rightChild;
        }
        // points to the next encoded char
        (*pEncodedStr)++;
    }
    if (IsLeafNode(curNode)) {
        return (unsigned char) curNode->value.charValue;
    } else {
        return ILLEGAL_HUFFMAN_CODE;
    }
}

/*
    The caller is responsible for releasing the heap space.
 */
char *HuffmanDecode(struct HuffmanInfo *pHuffmanInfo, char *encodedStr) {
    char **pEncodedStr = &encodedStr;
    long n = strlen(encodedStr);
    assert(n > 0);
    // FIXME: vector<char> needed
    long lenOfDecodedStr = sizeof(char) * (n + 1);
    char *decodedStr = (char *) malloc(lenOfDecodedStr);
    memset(decodedStr, 0, lenOfDecodedStr);

    struct BiTreeNode *root = GetHuffmanTree(pHuffmanInfo);
    long k = 0;
    while (**pEncodedStr) {
        long ch = DecodeOneChar(pHuffmanInfo, root, pEncodedStr);
        if (ch != ILLEGAL_HUFFMAN_CODE) {
            decodedStr[k] = (char) ch;
            k++;
        } else {
            break;
        }          
    }
    return decodedStr;
}


/*
    The caller is responsible for releasing the heap space.
 */
char *HuffmanEncode(struct HuffmanInfo *pHuffmanInfo, char *text) {
    long n = strlen(text);
    assert(n > 0);
    // worst case 
    long lenOfEncodedStr = sizeof(char) * n * SIZE_OF_ALPHABET + 1;
    
    char *encodedStr = (char *) malloc(lenOfEncodedStr);
    memset(encodedStr, 0, lenOfEncodedStr);

    char *ptr = text;
    long val;
    while ((val = ((unsigned char) *ptr)) != 0) {
        strcat(encodedStr, pHuffmanInfo->convertTable[val]);
        ptr++;
    }
    return encodedStr;
}

static void PrintConvertTable(struct HuffmanInfo *pHuffmanInfo) {
    for (long charValue = 0; charValue < SIZE_OF_ALPHABET; charValue++) {
        if (pHuffmanInfo->convertTable[charValue][0] != 0) {
            printf("\'%c\': %s\n", (char) charValue, pHuffmanInfo->convertTable[charValue]);     
        }
    } 
}
 
struct HuffmanInfo *CreateHuffmanInfoFromText(char *text) {
    long n = strlen(text);
    assert(n > 0);
    
    long numOfLeaves = 0;
    long *frequencies = (long *) malloc(sizeof(long) * SIZE_OF_ALPHABET);
    memset(frequencies, 0, sizeof(long) * SIZE_OF_ALPHABET);   
    
    for (long i = 0; i < n; i++) {
        long val = (unsigned char) text[i];
        if (frequencies[val] == 0) {
            numOfLeaves++;
        }
        frequencies[val]++;
    }

    long numOfInternalNodes = numOfLeaves - 1;

    struct HuffmanInfo *pHuffmanInfo = (struct HuffmanInfo *) malloc(sizeof(struct HuffmanInfo));
    memset(pHuffmanInfo, 0, sizeof(struct HuffmanInfo));

    struct BiTreeNode *nodes = malloc(sizeof(struct BiTreeNode) * (numOfLeaves + numOfInternalNodes));
    memset(nodes, 0, sizeof(struct BiTreeNode) * (numOfLeaves + numOfInternalNodes));

    pHuffmanInfo->nodes = nodes;
    pHuffmanInfo->numOfLeaves = numOfLeaves;
    pHuffmanInfo->numOfInternalNodes = numOfInternalNodes;

    // Init the leaf nodes
    long k = 0;
    for (long charValue = 0; charValue < SIZE_OF_ALPHABET; charValue++) {
        if (frequencies[charValue] > 0) {
            InitBinaryTreeNode(nodes + k, charValue, frequencies[charValue], NULL, NULL, NULL);  
            k++;          
        }
    }   

    BuildHuffmanTree(pHuffmanInfo);
    struct BiTreeNode *root = GetHuffmanTree(pHuffmanInfo);

    char *trace = (char *) malloc(sizeof(char) * SIZE_OF_ALPHABET);

    GenerateConvertTable(root, 0, trace, pHuffmanInfo->convertTable);
    // When the text is like "aaaa", only one kind of character in the text
    if (pHuffmanInfo->numOfLeaves == 1) {
        char *encPtr = pHuffmanInfo->convertTable[(unsigned char) *text];
        encPtr[0] = SPECIAL_CASE_ENCODED_CHAR;
        encPtr[1] = 0;
    }
    PrintConvertTable(pHuffmanInfo);

    GenOneImage(root, "Huffman", "images/Huffman", 0);

    free(trace);
    free(frequencies);
    return pHuffmanInfo;
}

void ReleaseHuffmanInfo(struct HuffmanInfo *pHuffmanInfo) {
    if (pHuffmanInfo) {
        free(pHuffmanInfo->nodes);
        free(pHuffmanInfo);
    }
}


struct BiTreeNode *GetHuffmanTree(struct HuffmanInfo *pHuffmanInfo) {
    if (pHuffmanInfo) {
        long n = pHuffmanInfo->numOfInternalNodes + pHuffmanInfo->numOfLeaves;
        return &(pHuffmanInfo->nodes[n-1]);
    } else {
        return NULL;
    }
}

