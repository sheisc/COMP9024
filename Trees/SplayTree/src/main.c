#include <stdio.h>
#include <stdlib.h> // srandom(), random()
#include <time.h>   // time()
#include <assert.h>
#include <math.h>
#include "SplayTree.h"

#define MAX_COMMENT_LEN 1024

#define MAX_NUM_OF_RANDOM_KEYS      (1024 * 1024)

static long imgCount = 0;

static char comment[MAX_COMMENT_LEN];

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


/* Driver program to test above function*/
int TestSplayTree(long nums[], long n, long hasOutput) {
    struct SplayTreeNode *root = CreateSplayTree();
    long max = 0;

    for (long i = 0; i < n; i++) {
        SplayTreeInsert(&root, nums[i]);
        long depth = SplayTreeGetHeight(root);
        if (depth > max) {
            max = depth;
        }
        if (hasOutput) {
            SplayTreePrintInOrder(root);
            printf("\n");
            snprintf(comment, MAX_COMMENT_LEN, "SplayTreeInsert(%ld)", (long)nums[i]);
            SplayTreeGenOneImage(root, "SplayTree", "images/SplayTree", imgCount, comment);
            imgCount++;
        }
    }

    for (long i = 0; i < n; i++) {
        SplayTreeSearch(&root, nums[i]);
        long depth = SplayTreeGetHeight(root);
        if (depth > max) {
            max = depth;
        }
        if (hasOutput) {
            SplayTreePrintInOrder(root);
            printf("\n");
            snprintf(comment, MAX_COMMENT_LEN, "SplayTreeSearch(%ld)", (long)nums[i]);
            SplayTreeGenOneImage(root, "SplayTree", "images/SplayTree", imgCount, comment);
            imgCount++;
        }
    }

    for (long i = 0; i < n; i++) {
        SplayTreeDelete(&root, nums[i]);
        long depth = SplayTreeGetHeight(root);
        if (depth > max) {
            max = depth;
        }
        if (hasOutput) {
            SplayTreePrintInOrder(root);
            printf("\n");
            snprintf(comment, MAX_COMMENT_LEN, "After SplayTreeDelete(%ld)", (long)nums[i]);
            SplayTreeGenOneImage(root, "SplayTree", "images/SplayTree", imgCount, comment);
            imgCount++;
        }
    }
    // | $2^{26}$ | 81|
    printf("| $2^{%ld}$ | %ld |\n", (long) log2((double) n), max);
    ReleaseSplayTree(root);
    return 0;
}

int main(void) {
    srandom(time(NULL));
    system("mkdir -p diagrams");
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    long nums[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    long n = sizeof(nums) / sizeof(nums[0]);
    TestSplayTree(nums, n, 1);

    // random inputs    
    n = 16;
    while (n <= MAX_NUM_OF_RANDOM_KEYS) {
        long *indexes = (long *)malloc(sizeof(long) * n);
        GenRandomIndexes(indexes, n, 1);
        // PrintArr(indexes, n);
        TestSplayTree(indexes, n, 0);
        free(indexes);
        n *= 4;
    }

    return 0;
}
