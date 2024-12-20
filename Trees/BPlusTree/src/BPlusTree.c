#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "BPlusTree.h"
#include "BPlusTree2Dot.h"

static void BPlusTreeNodeDelete(struct BPlusTreeNode *pNode, BPlusTreeKeyTy k);
static void BPlusTreeNodeFill(struct BPlusTreeNode *pNode, long i);
static struct BPlusTreeNode * BPlusTreeNodeGetPredecessor(struct BPlusTreeNode *pNode, long i);
static struct BPlusTreeNode * BPlusTreeNodeGetSuccessor(struct BPlusTreeNode *pNode, long i);
static void BPlusTreeNodeMerge(struct BPlusTreeNode *pNode, long index);



#ifdef ENABLE_TEST_OUTPUT
static long imgCount = 0;
#endif

///////////////////////////// BPlusTreeNode ///////////////////////////////////////////////////////////////////////
static BPlusTreeValueTy GetValue(struct BPlusTreeNode *pNode, int i) {
    if (pNode->isLeaf) {
        return (BPlusTreeValueTy) (pNode->pChildren[i]);
    } else {
        struct BPlusTreeNode *cur = pNode->pChildren[i];
        while (!cur->isLeaf) {
            cur = cur->pChildren[cur->nk];
        }
        return (BPlusTreeValueTy) cur->pChildren[cur->nk];
    }   
}

/*    
    A leaf node contains an extra slot to store a key-value pair from one of its ancestor nodes.
    Update the extra slot with (pNode->keys[i], val).
 */
static void UpdateExtraSlotInLeaf(struct BPlusTreeNode *pNode, int i, BPlusTreeValueTy val) {
    if (!pNode->isLeaf) {
        struct BPlusTreeNode *cur = pNode->pChildren[i];
        while (!cur->isLeaf) {
            cur = cur->pChildren[cur->nk];
        }
        cur->keys[cur->nk] = pNode->keys[i];  
        cur->pChildren[cur->nk] = (struct BPlusTreeNode *) val;   
    }
}

static struct BPlusTreeNode *CreateBPlusTreeNode(long minDegree, int isLeaf) {
    size_t bytesOfMem = sizeof(struct BPlusTreeNode);    
    struct BPlusTreeNode *pNode = (struct BPlusTreeNode *) malloc(bytesOfMem);
    memset(pNode, 0, bytesOfMem);
    
    pNode->md = minDegree;
    pNode->nk = 0;

    // FIXME: B+ Tree
    // one more key
    bytesOfMem = sizeof(BPlusTreeKeyTy) * (MAX_NUM_OF_KEYS(minDegree) + 1);
    pNode->keys = (BPlusTreeKeyTy *) malloc(bytesOfMem);
    memset(pNode->keys, 0, bytesOfMem);


    bytesOfMem = sizeof(struct BPlusTreeNode *) * MAX_NUM_OF_CHILDREN(minDegree);
    pNode->pChildren = (struct BPlusTreeNode **) malloc(bytesOfMem);
    memset(pNode->pChildren, 0, bytesOfMem);

    pNode->isLeaf = isLeaf;
    pNode->visited = 0;

    return pNode;
}

static void FreeOneNode(struct BPlusTreeNode *pNode) {
    free(pNode->pChildren);
    free(pNode->keys);
    free(pNode);
}


static void ReleaseBPlusTreeNode(struct BPlusTreeNode *pNode) {
    if (pNode) {
        for (long i = 0; i <= pNode->nk; i++) {
            if (!pNode->isLeaf) {
                ReleaseBPlusTreeNode(pNode->pChildren[i]);
            }         
        }
        FreeOneNode(pNode);
    }
}

#if 0
/*
    This function returns 0 if k is not found.
    It returns 1 if k exists.
 */
static long BPlusTreeNodeFindKeyInCurNode(struct BPlusTreeNode *pNode, BPlusTreeKeyTy k, long *pIndex) {
    long i = 0;
    while (i < pNode->nk && pNode->keys[i] < k) {
        i++;
    }
    *pIndex = i;
    if (i >= pNode->nk || pNode->keys[i] > k) { // not found
        return 0;
    } else {
        return 1;
    }
}
#else
/*
    Binary search.

    The array elements pNode->keys[0 .. pNode->nk - 1] are in ascending order.

    This function returns 1 when @k is found,
    *pIndex is the index of the found element.
    If there are more than one @k in pNode->keys[], it returns the leftmost one.

    Otherwise it returns 0, with *pIndex being either pNode->nk or the index of 
    the first element in pNode->keys[] which is larger than @k.
 */
static long BPlusTreeNodeFindKeyInCurNode(struct BPlusTreeNode *pNode, BPlusTreeKeyTy k, long *pIndex) {
    // [left, right]
    long left = 0;
    long right = pNode->nk - 1;
    while (left <= right) {
        long mid = (left + right) / 2;
        if (pNode->keys[mid] == k) {   
            // find the leftmost target         
            while (mid - 1 >= 0 && pNode->keys[mid - 1] == k) {
                mid--;
            }
            *pIndex = mid;
            return 1;
        } else if (pNode->keys[mid] < k) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }                       
    }
    assert(left >= pNode->nk || pNode->keys[left] > k);
    *pIndex = left;
    return 0;      
}
#endif

static void PrintKV(BPlusTreeKeyTy k, BPlusTreeValueTy v) {
#ifdef ENABLE_TEST_OUTPUT             
            printf("%9ld  %9ld \n", (long) k, (long) v);
#endif

#ifdef USE_KEY_VALUE_GAP_FOR_TESTING         
            assert(((long) v) == ((long) k) + KEY_VALUE_GAP_FOR_TESTING); 
#endif 
}

static long BPlusTreeNodeIsFull(struct BPlusTreeNode *pNode) {
    assert(pNode);
    return pNode->nk == 2 * pNode->md - 1;
}

static void BPlusTreeNodeTraverse(struct BPlusTreeNode *pNode) {
    if (pNode) {
        long i;
        for (i = 0; i < pNode->nk; i++) {
            if (!pNode->isLeaf) {
                BPlusTreeNodeTraverse(pNode->pChildren[i]);
            }
            BPlusTreeKeyTy k = pNode->keys[i];
            BPlusTreeValueTy v = GetValue(pNode, i);
            PrintKV(k, v);
          
        }
        // right-most child
        if (!pNode->isLeaf) {
            BPlusTreeNodeTraverse(pNode->pChildren[i]);
        }
    }
}


static struct BPlusTreeNode * BPlusTreeNodeSearch(struct BPlusTreeNode *pNode, BPlusTreeKeyTy k, BPlusTreeValueTy *pVal) {
    if (!pNode) {
        return NULL;
    } else {
        long i = 0;
        if (BPlusTreeNodeFindKeyInCurNode(pNode, k, &i)) {
            if (pVal) {
                *pVal = GetValue(pNode, i);
            }
            return pNode;
        } else {
            // not found:  recursively search the child
            if (!pNode->isLeaf) {
                return BPlusTreeNodeSearch(pNode->pChildren[i], k, pVal);
            } else {
                return NULL;
            }
        }
    }
}

/////////////////////////////////////// Insert() //////////////////////////////////////////////////

/*
    Split the keys in a full child node pChild into three parts.
    (1) the first (md - 1) keys are kept in the child
    (2) the middle key is moved to its parent
    (3) create a right sibling for saving the last (md - 1) keys.
 */
static void BPlusTreeNodeSplitFullChild(struct BPlusTree *pBPlusTree, struct BPlusTreeNode *pParent, int i, struct BPlusTreeNode *pChild) {
    long md = pChild->md;
    assert(pChild->nk == (2 * md - 1));
    struct BPlusTreeNode *pSibling = CreateBPlusTreeNode(md, pChild->isLeaf);
    // set the number of keys in pSibling to be (md - 1)
    pSibling->nk = md - 1;

    // Copy md slots from pChild to pSibling.        
    for (long j = 0; j <= pSibling->nk; j++) {
        pSibling->keys[j] = pChild->keys[j + md];
        pSibling->pChildren[j] = pChild->pChildren[j + md];
    }

    // create space for pSibling in pParent->pChildren[]
    for (long j = pParent->nk; j >= i + 1; j--) {
        pParent->pChildren[j + 1] = pParent->pChildren[j];
    }
    // insert pSibling at index (i+1)
    pParent->pChildren[i + 1] = pSibling;

    // create space for pChild->keys[md - 1] in pParent->keys[]
    for (long j = pParent->nk - 1; j >= i; j--) {
        pParent->keys[j + 1] = pParent->keys[j];
    }
    // insert pChild->keys[md-1] at index i
    pParent->keys[i] = pChild->keys[md - 1];

    // one more key in the parent node
    pParent->nk += 1;
    // only (md - 1) left in the child node
    pChild->nk = md - 1;

    // If it is leaf, update the linked list of leaf nodes.
    if (pChild->isLeaf) {
        pSibling->next = pChild->next;
        pChild->next = pSibling;
    }
#ifdef ENABLE_TEST_OUTPUT
    BPlusTreeGenOneImage(pBPlusTree, "BPlusTree", "images/BPlusTree", imgCount);
    imgCount++;      
#endif    
}


void BPlusTreeNodeInsertNonFull(struct BPlusTree *pBPlusTree, struct BPlusTreeNode *pNode, BPlusTreeKeyTy k, BPlusTreeValueTy v) {
    assert(pNode);

    long i = 0;
    if (BPlusTreeNodeFindKeyInCurNode(pNode, k, &i)) {
        return;
    }
    assert(i >= pNode->nk || pNode->keys[i] > k);
    if (pNode->isLeaf) {
        // create space when needed    
        for (long j = pNode->nk; j >= i; j--) {
            pNode->keys[j + 1] = pNode->keys[j];
            pNode->pChildren[j + 1] = pNode->pChildren[j];
        }
        pNode->keys[i] = k;
        pNode->pChildren[i] = (struct BPlusTreeNode *) v;
        pNode->nk++;
    } else {        
        if (BPlusTreeNodeIsFull(pNode->pChildren[i])) {
            BPlusTreeNodeSplitFullChild(pBPlusTree, pNode, i, pNode->pChildren[i]);
            /*
                After split, pNode->keys[i] has been changed. 
             */
            if (pNode->keys[i] == k) {
                return;
            }
            // See which child to follow.
            if (pNode->keys[i] < k) {
                i++;
            }            
        }
        BPlusTreeNodeInsertNonFull(pBPlusTree, pNode->pChildren[i], k, v);
    }
}

////////////////////// delete() ///////////////////////////////////////////////////////////////////////

/*
    Delete pNode->keys[i] in a leaf node.

    (1) If it has a parent node, to make sure there are more than (md - 1) keys left, 

        BPlusTreeNodeFill() is called when necessary while traversing a B+ tree from the root to the leaf.
    
    (2) If it is the root node,
        no constraint on the number of keys in the root node.

 */
void BPlusTreeNodeDeleteFromLeafNode(struct BPlusTreeNode *pNode, long i) {
    assert(pNode->isLeaf);
    // FIXME: B+ Tree
    for (long j = i + 1; j <= pNode->nk; j++) {
        pNode->keys[j - 1] = pNode->keys[j];
        pNode->pChildren[j - 1] = pNode->pChildren[j];
    }
    pNode->nk--;
}

/*
    Delete pNode->keys[i] when pNode is not a leaf.

    (1) Replace pNode->keys[i] with its predecessor in traversal if pNode->pChildren[i] has at least md keys.

    (2) Replace pNode->keys[i] with its successor in traversal if pNode->pChildren[i+1] has at least md keys.

    (3) If both (1) and (2) fail, merge pNode->pChildren[i] and pNode->pChildren[i + 1].
        Then recursively delete the key from pNode->pChildren[i].
 */
void BPlusTreeNodeDeleteFromNonLeafNode(struct BPlusTreeNode *pNode, long i) {
    assert(!pNode->isLeaf);

    BPlusTreeKeyTy k = pNode->keys[i];
    long md = pNode->md;

    if (pNode->pChildren[i]->nk >= md) {
        struct BPlusTreeNode *predNode = BPlusTreeNodeGetPredecessor(pNode, i);
        BPlusTreeKeyTy predKey = predNode->keys[predNode->nk - 1];
        pNode->keys[i] = predKey;
        UpdateExtraSlotInLeaf(pNode, i, (BPlusTreeValueTy) predNode->pChildren[predNode->nk - 1]);
        BPlusTreeNodeDelete(pNode->pChildren[i], predKey);
    } else if (pNode->pChildren[i + 1]->nk >= md) {
        struct BPlusTreeNode *succNode = BPlusTreeNodeGetSuccessor(pNode, i);
        BPlusTreeKeyTy succKey = succNode->keys[0];
        pNode->keys[i] = succKey;
        UpdateExtraSlotInLeaf(pNode, i, (BPlusTreeValueTy) succNode->pChildren[0]);
        BPlusTreeNodeDelete(pNode->pChildren[i + 1], succKey);
    } else {
        // Merge pNode->pChildren[i], pNode->keys[i], and pNode->pChildren[i+1]
        BPlusTreeNodeMerge(pNode, i);
        // the key k is now in pNode->pChildren[i]
        BPlusTreeNodeDelete(pNode->pChildren[i], k);
    }
}

static void BPlusTreeNodeDelete(struct BPlusTreeNode *pNode, BPlusTreeKeyTy k) {
    long i = 0;
    if (BPlusTreeNodeFindKeyInCurNode(pNode, k, &i)) {
        // k is at pNode->keys[i]
        if (pNode->isLeaf) {
            BPlusTreeNodeDeleteFromLeafNode(pNode, i);
        } else {
            BPlusTreeNodeDeleteFromNonLeafNode(pNode, i);
        }
    } else {
        // k is not in the current node pNode->keys[]
        if (pNode->isLeaf) {
            return;
        }
        // 
        long md = pNode->md;
        long last = (i == pNode->nk);
        //
        if (pNode->pChildren[i]->nk < md) {
            BPlusTreeNodeFill(pNode, i);
        }
        // After BPlusTreeNodeFill(), pNode->nk (the number of keys in pNode) might change.
        // When merging two children, one key node in the parent node is also pulled down.
        if (last && i > pNode->nk) {
            BPlusTreeNodeDelete(pNode->pChildren[i - 1], k);
        } else {
            BPlusTreeNodeDelete(pNode->pChildren[i], k);
        }
    }
}

/*
    Return the predecessor of pNode->keys[i] in a BPlusTree
 */
static struct BPlusTreeNode * BPlusTreeNodeGetPredecessor(struct BPlusTreeNode *pNode, long i) {
    struct BPlusTreeNode *curNode = pNode->pChildren[i];
    while (!curNode->isLeaf) {
        // move to the right-most child
        curNode = curNode->pChildren[curNode->nk];
    }
    return curNode;
}

/*
    Return the successor of pNode->keys[i] in a BPlusTree
 */
static struct BPlusTreeNode *  BPlusTreeNodeGetSuccessor(struct BPlusTreeNode *pNode, long i) {
    struct BPlusTreeNode *curNode = pNode->pChildren[i + 1];
    while (! curNode->isLeaf) {
        // move to the left-most child
        curNode = curNode->pChildren[0];
    }
    return curNode;
}

/*
    Borrow one key from pNode->children[index - 1] to pNode->children[index].

    pNode:
                        keys[index - 1]              keys[index]
        pChildren[index-1]             pChildren[index]
          (pSibling)                     (pChild)
 */
static void BPlusTreeNodeBorrowFromPrevious(struct BPlusTreeNode *pNode, long index) {
    struct BPlusTreeNode *pChild = pNode->pChildren[index];
    struct BPlusTreeNode *pSibling = pNode->pChildren[index - 1];
    assert(pChild->isLeaf == pSibling->isLeaf);

    // Shift all elements one position to the right
    for (long i = pChild->nk; i >= 0; i--) {
        pChild->keys[i + 1] = pChild->keys[i];
        pChild->pChildren[i + 1] = pChild->pChildren[i];
    }
    // insert pNode->keys[index - 1] in pChild->keys[0]
    pChild->keys[0] = pNode->keys[index - 1];
    // insert pSibling->pChildren[pSibling->nk] in pChild->pChildren[0]
    pChild->pChildren[0] = pSibling->pChildren[pSibling->nk];

    // move pSibling->keys[pSibling->nk - 1] to pNode->keys[index - 1]
    pNode->keys[index - 1] = pSibling->keys[pSibling->nk - 1];
    //

    pChild->nk += 1;
    pSibling->nk -= 1;
}

/*
    Borrow one key from pNode->children[index + 1] to pNode->children[index].

    pNode:
                        keys[index]              keys[index + 1]
        pChildren[index]             pChildren[index + 1]
          (pChild)                     (pSibling)
 */
static void BPlusTreeNodeBorrowFromNext(struct BPlusTreeNode *pNode, long index) {
    struct BPlusTreeNode *pChild = pNode->pChildren[index];
    struct BPlusTreeNode *pSibling = pNode->pChildren[index + 1];
    // they are both internal or both leaf nodes
    assert(pChild->isLeaf == pSibling->isLeaf);

    // insert pNode->keys[index] the end of pChild->keys[]
    pChild->keys[pChild->nk] = pNode->keys[index];

    // move pSibling->keys[0] to pNode->keys[index]
    pNode->keys[index] = pSibling->keys[0];      

    /*
        If pChild is leaf, 
            save its ancestor's key and value.

        If it is internal, only pSibling->pChildren[0] needs to be saved,
            but the copy of  pSibling->keys[0] is redundant.
     */
    pChild->keys[pChild->nk + 1] = pSibling->keys[0];
    pChild->pChildren[pChild->nk + 1] = pSibling->pChildren[0];

    // remove slot 0 in its sibling
    for (long i = 1; i <= pSibling->nk; i++) {
        pSibling->pChildren[i - 1] = pSibling->pChildren[i];
        pSibling->keys[i - 1] = pSibling->keys[i];
    }

    pChild->nk += 1;
    pSibling->nk -= 1;    
}

/*
    Merge pNode->pChildren[index], pNode->keys[i], and  pNode->pChildren[index + 1] 
    into pNode->pChildren[index].

    pNode:
                        keys[index]              keys[index + 1]
        pChildren[index]             pChildren[index + 1]
          (pChild)                     (pSibling)    
 */
static void BPlusTreeNodeMerge(struct BPlusTreeNode *pNode, long index) {
    struct BPlusTreeNode *pChild = pNode->pChildren[index];
    struct BPlusTreeNode *pSibling = pNode->pChildren[index + 1];    
    long md = pChild->md;
    assert(pChild->isLeaf == pSibling->isLeaf);
    assert(pChild->nk == md - 1);

    // Insert pNode->keys[index] at the end of pChild->keys[]
    pChild->keys[md - 1] = pNode->keys[index];
    // Add pSibling->keys[] at the end of pChild->keys[]
    for (long i = 0; i <= pSibling->nk; i++) {
        pChild->keys[i + md] = pSibling->keys[i];
        pChild->pChildren[i + md] = pSibling->pChildren[i];
    }

    // delete pNode->keys[index]
    for (long i = index + 1; i <= pNode->nk; i++) {
        pNode->keys[i - 1] = pNode->keys[i];
    }
    // delete pNode->pChildren[index + 1]
    for (long i = index + 2; i <= pNode->nk; i++) {
        pNode->pChildren[i - 1] = pNode->pChildren[i];
    }
    // all the keys from pSibling + one key from pNode
    pChild->nk += pSibling->nk + 1;
    pNode->nk -= 1;
    // update the linked list of leaf nodes
    if (pSibling->isLeaf) {
        pChild->next = pSibling->next;
    }
    // Only free one node
    FreeOneNode(pSibling);
}

/*
    One key in the subtree pNode->pChildren[i] will be deleted.
    But pNode->pChildren[i] has only (m - 1) keys.
    Try one of the follow three ways to add more keys in pNode->pChildren[i].

    (1) Borrow from its prvious sibling pNode->pChildren[i - 1] when i > 0
    (2) Borrow from its next sibling pNode->pChildren[i + 1] when i < pNode->nk
    (3) If both (1) and (2) fail, 
        merge pNode->pChildren[i] and pNode->pChildren[i + 1] when i < pNode->nk
        or
        merge pNode->pChildren[i - 1] and pNode->pChildren[i] when i == pNode->nk
 */
static void BPlusTreeNodeFill(struct BPlusTreeNode *pNode, long i) {
    long md = pNode->md;
    if (i > 0 && (pNode->pChildren[i - 1]->nk) >= md) {
        // pNode->pChildren[i - 1] has more than md - 1 keys
        BPlusTreeNodeBorrowFromPrevious(pNode, i);
    } else if (i < pNode->nk && (pNode->pChildren[i + 1]->nk) >= md) {
        // pNode->pChildren[i + 1] has more than md - 1 keys
        BPlusTreeNodeBorrowFromNext(pNode, i);
    } else {
        if (i < pNode->nk ) {
            // merge pNode->pChildren[i] and pNode->pChildren[i + 1]
            BPlusTreeNodeMerge(pNode, i);
        } else {
            // merge pNode->pChildren[i-1] and pNode->pChildren[i]
            BPlusTreeNodeMerge(pNode, i - 1);
        }
    }
}

///////////////////////////////////// BPlusTree /////////////////////////////////////////////////////////////

struct BPlusTree *CreateBPlusTree(long minDegree) {
    assert(minDegree > 0);
    struct BPlusTree *pBPlusTree = (struct BPlusTree *) malloc(sizeof(struct BPlusTree));
    memset(pBPlusTree, 0, sizeof(struct BPlusTree));

    pBPlusTree->minDegree = minDegree;
    pBPlusTree->root = NULL;

    return pBPlusTree;
}

void ReleaseBPlusTree(struct BPlusTree *pBPlusTree) {
    if (pBPlusTree) {
        ReleaseBPlusTreeNode(pBPlusTree->root);
        free(pBPlusTree);
    }
}

/*
    A new key k is always inserted at the leaf node in a BPlusTree.
 */
void BPlusTreeInsert(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k, BPlusTreeValueTy v) {
    if (pBPlusTree->root == NULL) {
        pBPlusTree->root = CreateBPlusTreeNode(pBPlusTree->minDegree, 1);
        pBPlusTree->root->keys[0] = k;
        pBPlusTree->root->pChildren[0] = (struct BPlusTreeNode *) v;
        //
        pBPlusTree->root->keys[1] = UNUSED_SLOT_KEY_VALUE;
        pBPlusTree->root->pChildren[1] = (struct BPlusTreeNode *) UNUSED_SLOT_KEY_VALUE;        
        pBPlusTree->root->nk++;
    } else {
        
        if (BPlusTreeNodeIsFull(pBPlusTree->root)) {
            struct BPlusTreeNode *newRoot = CreateBPlusTreeNode(pBPlusTree->minDegree, 0);
            newRoot->pChildren[0] = pBPlusTree->root;

            pBPlusTree->root = newRoot;

#ifdef ENABLE_TEST_OUTPUT  
            BPlusTreeGenOneImage(pBPlusTree, "BPlusTree", "images/BPlusTree", imgCount);
            imgCount++;
#endif            

            BPlusTreeNodeSplitFullChild(pBPlusTree, newRoot, 0, newRoot->pChildren[0]);
            long i = 0;        
            if (newRoot->keys[0] == k) {
                return;
            }    
            // After split, see which child to follow
            if (newRoot->keys[0] < k) {
                i++;
            }
             
            BPlusTreeNodeInsertNonFull(pBPlusTree, newRoot->pChildren[i], k, v);
            // pBPlusTree->root = newRoot;
        } else {
            BPlusTreeNodeInsertNonFull(pBPlusTree, pBPlusTree->root, k, v);
        }
    }
#ifdef ENABLE_TEST_OUTPUT    
    BPlusTreeGenOneImage(pBPlusTree, "BPlusTree", "images/BPlusTree", imgCount);
    imgCount++;
#endif
}

void BPlusTreeTraverse(struct BPlusTree *pBPlusTree) {
    if (pBPlusTree) {
        BPlusTreeNodeTraverse(pBPlusTree->root);
#ifdef ENABLE_TEST_OUTPUT        
        printf("\n");
#endif        
    }
}

struct BPlusTreeNode * BPlusTreeSearch(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k, BPlusTreeValueTy *pVal) {
    if (pBPlusTree->root) {
        return BPlusTreeNodeSearch(pBPlusTree->root, k, pVal);
    } else {
        return NULL;
    }
}

void BPlusTreeDelete(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy k) {
    if (pBPlusTree->root == NULL) {
        return;
    }
    BPlusTreeNodeDelete(pBPlusTree->root, k);

    if (pBPlusTree->root->nk == 0) {
#ifdef ENABLE_TEST_OUTPUT    
        BPlusTreeGenOneImage(pBPlusTree, "BPlusTree", "images/BPlusTree", imgCount);
        imgCount++;
#endif        
        struct BPlusTreeNode *tmp = pBPlusTree->root;
        if (pBPlusTree->root->isLeaf) {
            pBPlusTree->root = NULL;
        } else {
            pBPlusTree->root = pBPlusTree->root->pChildren[0];
        }
        // Only free one node
        FreeOneNode(tmp);
    }
#ifdef ENABLE_TEST_OUTPUT    
    BPlusTreeGenOneImage(pBPlusTree, "BPlusTree", "images/BPlusTree", imgCount);
    imgCount++;    
#endif    
}

/*  
    Visit the linked list of leaf nodes.   
    Output the key-value pairs where the keys are in [startKey, endKey]
 */
void BPlusTreeRangeQuery(struct BPlusTree *pBPlusTree, BPlusTreeKeyTy startKey, BPlusTreeKeyTy endKey) {
    struct BPlusTreeNode *curNode = pBPlusTree->root;
    if (!curNode) {
        return;
    }
    // find the leaf node which contains a key larger than or equal to startKey.
    while (!curNode->isLeaf) {
        long i = 0;
        while (i < curNode->nk && curNode->keys[i] < startKey) {
            i++;
        }
        curNode = curNode->pChildren[i];        
    }
    
    while (curNode) {
        assert(curNode->isLeaf);
        // the last leaf node does not have the extra slot
        long lastIndex = (curNode->next) ? curNode->nk : (curNode->nk - 1);
        for (long i = 0; i <= lastIndex; i++) {
            BPlusTreeKeyTy k = curNode->keys[i];
            if (k > endKey) {
                goto func_exit;
            }
            if (k >= startKey && k <= endKey) {
                BPlusTreeValueTy v = GetValue(curNode, i);
                PrintKV(k, v);                
            }
        }
        curNode = curNode->next;
    }

func_exit:
#ifdef ENABLE_TEST_OUTPUT     
    printf("\n");
#endif
    return;    
}


