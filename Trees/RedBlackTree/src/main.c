#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include <assert.h>
#include <math.h>       // log2()
#include "RedBlackTree.h"
#include "BTree.h"
#include "BTree2Dot.h"
#include "RBTree2Dot.h"

#define MAX_NUM_OF_RANDOM_KEYS      (1024 * 1024* 16)

void PrintNodeInfo(RBTreeNodePtr pNode) {
    printf("%ld ", (long) pNode->value.numVal);
    // For debugging
    fflush(stdout);
    //pNode->visited = 1;
}

static void Swap(long *pa, long *pb) {
    long tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

/*
    Randomly generate n integers in {0, 1, 2, ..., n-1} and store them in indexes one by one.
 */
void GenRandomIndexes(long *indexes, long n, long shuffle) {
    assert(n > 0);
    // Generate a sorted list {0, 1, 2, ..., n-1}
    for (long i = 0; i < n; i++) {
        indexes[i] = i;
    }
    if (shuffle) {
        long x;
        long count = n;
        // randomly generate an integer x in {0, 1, 2, 3, count-1},
        while (count > 0) {
            x = random();
            x %= count;
            Swap(indexes + x, indexes + count - 1);
            count--;
        }
    }
}

static void TestRBTree(long *nums, long n, long *pCnt, long checkRBTree, long hasOutput) {
    // Create an empty binary tree
    RBTreeNodePtr root = CreateRBTree();
    long errCnt = 0;
    assert(pCnt);
    long btreeCnt = 0;

    SetIntermediateImages(hasOutput);

    // printf("*******************************  Testing RBTreeInsert() *******************************\n\n\n");
    for (long i = 0; i < n; i++) {
        if (hasOutput) {
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "images/RBTree1Insert", *pCnt, "Before RBTreeInsert()", nums[i]);
        }
        RBTreeInsert(&root, &root, nums[i], NULL, pCnt);
        if (checkRBTree) {
            if (!IsRBTree(root)) {
                errCnt++;
            }
        }

        if (hasOutput) {
            InOrderTraversal(root, PrintNodeInfo);
            printf("\n"); 
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "images/RBTree1Insert", *pCnt, "After RBTreeInsert()", nums[i]); 
            // convert the red-black tree to a 2-3-4 tree.
            struct BTreeNode *pNode = RBTreeTo234Tree(root);
            btreeCnt++;
            BTreeGenOneImage(pNode, "RBTree", "images/RBTree3BTree", btreeCnt);
            ReleaseBTree(pNode);                       
        }
    }

    printf("| $2^{%ld}$ | %ld | %ld |\n", (long) log2((double) n), RBTreeBlackHeight(root), RBTreeGetRealHeight(root));

    assert(errCnt == 0);

    // printf("*******************************  Testing RBTreeDelete() *******************************\n\n\n");
    for (long i = 0; i < n; i++) {
        if (hasOutput) {
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "images/RBTree2Delete", *pCnt, "Before RBTreeDelete()", nums[i]);           
        }
        RBTreeDelete(&root, &root, nums[i], pCnt);
        if (checkRBTree) {        
            if (!IsRBTree(root)) {
                errCnt++;
            }
        }
        if (hasOutput) {
            InOrderTraversal(root, PrintNodeInfo);
            printf("\n");   
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "images/RBTree2Delete", *pCnt, "After RBTreeDelete()", nums[i]);
        }
        //struct BTreeNode *pNode = RBTreeTo234Tree(root);       
    }

    assert(errCnt == 0);
    // Free the heap memory
    ReleaseRBTree(root);       
}

int main(void) {
    long cnt = 0;
    srandom(time(NULL));

    system("mkdir -p diagrams");
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");
     
    long nums[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    TestRBTree(nums, sizeof(nums)/sizeof(nums[0]), &cnt, 1, 1);

    // random inputs 
    long n = 16;
    while (n <= MAX_NUM_OF_RANDOM_KEYS) {
        long *indexes = (long *)malloc(sizeof(long) * n);
        GenRandomIndexes(indexes, n, 1);
        TestRBTree(indexes, n, &cnt, 0, 0);
        free(indexes);
        n *= 4;
    }
    return 0;
}


