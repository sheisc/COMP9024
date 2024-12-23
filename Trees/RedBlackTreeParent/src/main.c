#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include <assert.h>
#include "RedBlackTree.h"
#include "RBTree2Dot.h"

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

static long imgCount = 0;

static void TestRBTree(long nums[], long n, long checkRBTree, long hasOutput) {
    struct RBTree *pRBTree = CreateRBTree();
    
    for (long i = 0; i < n; i++) {
        RBTreeInsert(pRBTree, nums[i]);
        if (checkRBTree) {
            assert(IsRBTree(pRBTree));
        }
        if (hasOutput) {            
            RBTreePrintInOrder(pRBTree);
            RBTreeGenOneImage(pRBTree, "RBTree", "images/RBTree", imgCount);
            imgCount++;
        }
    }
    for (long i = 0; i < n; i++) {
        RBTreeDelete(pRBTree, nums[i]);
        if (checkRBTree) {
            assert(IsRBTree(pRBTree));
        }
        if (hasOutput) {            
            RBTreePrintInOrder(pRBTree);
            RBTreeGenOneImage(pRBTree, "RBTree", "images/RBTree", imgCount);
            imgCount++;
        }     
    }

    ReleaseRBTree(pRBTree);
}

void PrintArr(long arr[], long n) {
    for(long i = 0; i < n; i++) {
        printf("%ld,", arr[i]);
    }
    printf("\n");
}

int main(void) {
    srandom(time(NULL));

    system("mkdir -p diagrams");
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");  

    long nums[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    long n = sizeof(nums)/sizeof(nums[0]);
    TestRBTree(nums, n, 1, 1);

#if 1
    // random inputs
    n = 100000;
    long *indexes = (long *) malloc(sizeof(long) * n);
    GenRandomIndexes(indexes, n, 1);
    //PrintArr(indexes, n);
    TestRBTree(indexes, n, 0, 0);
    free(indexes);
#endif
    // statistics
    printf("GetLeftRotationCount(): %ld\n", GetLeftRotationCount());
    printf("GetRightRotationCount(): %ld\n", GetRightRotationCount());
    return 0;
}


