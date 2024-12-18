#ifndef BPlusTree_H
#define BPlusTree_H

/*
    For non-root BPlusTree nodes, the range of the number of keys

        [MIN_NUM_OF_KEYS, MAX_NUM_OF_KEYS]
 */
#define MIN_NUM_OF_KEYS(md)         ((md) - 1)

#define MAX_NUM_OF_KEYS(md)         (2 * (md) - 1)   

// md-way tree, except the root
#define MIN_NUM_OF_CHILDREN(md)     (md)

#define MAX_NUM_OF_CHILDREN(md)     (2 * (md))


#define UNUSED_SLOT_KEY_VALUE              (-1)

#define KEY_VALUE_GAP_FOR_TESTING           9

// for BPlusTree-related API, visible to users
typedef long BPlusTreeKeyTy;
typedef long BPlusTreeValueTy;


struct BPlusTreeNode {
    // keys points to an array of (BPlusTreeKeyTy)
    BPlusTreeKeyTy *keys;
    /*
        union {
            struct BPlusTreeNode **pChildren;
            BPlusTreeKeyTy *values;
        }

        (1) For internal nodes, 
            'pChildren' points to an array of (struct BPlusTreeNode *).
            It acts as 'pChildren' in BPlusTree.

        (2) For leaf nodes,
            'pChildren' points to an array of DataPointers.
     */
    struct BPlusTreeNode **pChildren;

    /*
        md:  minimum degree.

             at least md-way tree
        
        For non-root nodes, md is the minimum number of children of a BPlusTree node.
     */
    long md;
    /*
        The number of keys stored in this node.

        md - 1 <= nk <= 2 * md - 1
     */
    long nk;    
    // whether this node is the leaf node
    int isLeaf;
    // whether the node has been visited
    int visited;
    //
    struct BPlusTreeNode *next;
};

struct BPlusTree {
    /*
        The root node of a BPlusTree is treated differently from other nodes.
     */
    struct BPlusTreeNode *root;
    /*
        For non-root nodes, minDegree is the minimum number of children of a BPlusTree node
     */
    long minDegree;    
};


struct BPlusTree *CreateBPlusTree(long minDegree);

void ReleaseBPlusTree(struct BPlusTree *pBPlusTree) ;

void BPlusTreeInsert(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k, BPlusTreeValueTy v);

void BPlusTreeTraverse(struct BPlusTree *pBPlusTree) ;

struct BPlusTreeNode * BPlusTreeSearch(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k, BPlusTreeValueTy *pVal);

void BPlusTreeDelete(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k);

void BPlusTreeRangeQuery(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy startKey, BPlusTreeKeyTy endKey);

#endif


