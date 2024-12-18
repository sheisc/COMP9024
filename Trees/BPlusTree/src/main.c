#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include <assert.h>
#include "BPlusTree.h"
#include "BPlusTree2Dot.h"

// At least md-way 
#define   BPlusTree_MINIMUM_DEGREE  2

//#define   ENABLE_OUT_OF_ORDER_TEST 

static void Swap(long *pa, long *pb) {
    long tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}

/* 
    Randomly generate n integers in {0, 1, 2, ..., n-1} and store them in indexes one by one.
 */
void GenRandomIndexes(long *indexes, long n) {
    assert(n > 0);
    // Generate a sorted list {0, 1, 2, ..., n-1}
    for (long i = 0; i < n; i++) {
        indexes[i] = i;
    }
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


static void TestBPlusTree(long md, long *randKeys1, long *randKeys2, long n) {
    struct BPlusTree * pBPlusTree = CreateBPlusTree(md);
    long max = 0;
    // insert + traverse + range query
    for (long i = 0; i < n; i++) {
        BPlusTreeInsert(pBPlusTree, randKeys1[i], randKeys1[i] + KEY_VALUE_GAP_FOR_TESTING); 
        BPlusTreeTraverse(pBPlusTree);
        if (max < randKeys1[i]) {
            max = randKeys1[i];
        }
        BPlusTreeRangeQuery(pBPlusTree, 0, max);      
    }
 
    // delete + traverse + range query
    for (long i = 0; i < n; i++) {
        BPlusTreeDelete(pBPlusTree, randKeys2[i]);
        BPlusTreeTraverse(pBPlusTree);
        BPlusTreeRangeQuery(pBPlusTree, 0, max);      
    }    
    ReleaseBPlusTree(pBPlusTree);
}

#define MAX_N       100000

void PrintArr(long *arr, long n) {
    for (long i = 0; i < n; i++) {
        printf("%ld, ", arr[i]);
    }
    printf("\n");
}

int main(void) { 
    srandom(time(NULL)); 
    // BPlusTreeValueTy represents the data pointer (e.g. page/block offset), not DB record itself 
    assert(sizeof(BPlusTreeValueTy) <= sizeof(struct BPlusTreeNode *));

    system("mkdir -p diagrams");
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");
     
    //

#if 0
    long arr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    TestBPlusTree(BPlusTree_MINIMUM_DEGREE, arr1, arr1, sizeof(arr1)/sizeof(arr1[0]));

    long n = 20;
    long *arr = (long *) malloc(sizeof(long) * n);
    for (long i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    TestBPlusTree(BPlusTree_MINIMUM_DEGREE, arr, arr, n);    
    free(arr);

    long randKeys1[] = {5, 9, 3, 0, 8, 1, 2, 7, 6, 4};
    long randKeys2[] = {1, 5, 6, 7, 3, 0, 2, 9, 8, 4};    
    TestBPlusTree(BPlusTree_MINIMUM_DEGREE, randKeys1, randKeys2, sizeof(randKeys1)/sizeof(randKeys1[0]));    
#endif

    long arr2[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    TestBPlusTree(BPlusTree_MINIMUM_DEGREE, arr2, arr2, sizeof(arr2)/sizeof(arr2[0]));

#ifdef   ENABLE_OUT_OF_ORDER_TEST 
    for (long md = 2; md <= 256; md *= 2) {
        for (long n = 10; n <= MAX_N; n *= 10) {
            printf("md = %ld, n = %ld\n", md, n);
            long *randKeys1 = (long *) malloc(sizeof(long) * n);
            GenRandomIndexes(randKeys1, n);
            //PrintArr(randKeys1, n);

            long *randKeys2 = (long *) malloc(sizeof(long) * n);
            GenRandomIndexes(randKeys2, n);
            //PrintArr(randKeys2, n);

            TestBPlusTree(md, randKeys1, randKeys2, n);
            free(randKeys1);
            free(randKeys2);
        }
    }
#endif      
    return 0;
}




