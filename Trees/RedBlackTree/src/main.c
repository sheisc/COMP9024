#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include <assert.h>
#include "BiTree.h"
#include "BTree.h"
#include "BTree2Dot.h"


void PrintNodeInfo(BiTreeNodePtr pNode) {
    printf("%s ", pNode->value.name);
    // For debugging
    fflush(stdout);
    //pNode->visited = 1;
}





static void TestRBTree(long *nums, long n, long *pCnt) {
    // Create an empty binary tree
    BiTreeNodePtr root = NULL;
    long errCnt = 0;
    assert(pCnt);
    long btreeCnt = 0;

    printf("*******************************  Testing RBTreeInsert() *******************************\n\n\n");
    for (long i = 0; i < n; i++) {
        RBTreeInsert(&root, &root, nums[i], NULL, pCnt);

        printf("Step %ld: After inserting %ld \n", i + 1, nums[i]);

        printf("\t----------------  InOrderTraversal() ----------------\n\t");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n\t-----------------------------------------------------\n\n");

        if (!IsRBTree(root)) {
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "diagrams/RBTree1Insert", *pCnt);
            errCnt++;
        }

        (*pCnt)++;              
        RBTreeGenOneImage(root, "RBTree", "images/RBTree1Insert", *pCnt);

        struct BTreeNode *pNode = RBTreeTo234Tree(root);
        btreeCnt++;
        BTreeGenOneImage(pNode, "RBTree", "images/RBTree3BTree", btreeCnt);
        ReleaseBTree(pNode);

    }

    assert(errCnt == 0);

    printf("*******************************  Testing RBTreeDelete() *******************************\n\n\n");
    for (long i = 0; i < n; i++) {
        RBTreeDelete(&root, &root, nums[i], pCnt);
        printf("Step %ld: After deleting %ld \n", i + 1, nums[i]);

        printf("\t----------------  InOrderTraversal() ----------------\n\t");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n\t-----------------------------------------------------\n\n");              

        if (!IsRBTree(root)) {
            (*pCnt)++;              
            RBTreeGenOneImage(root, "RBTree", "diagrams/RBTree2Delete", *pCnt);
            errCnt++;
        }
        (*pCnt)++;              
        RBTreeGenOneImage(root, "RBTree", "images/RBTree2Delete", *pCnt);

        //struct BTreeNode *pNode = RBTreeTo234Tree(root);       
    }

    assert(errCnt == 0);
    // Free the heap memory
    ReleaseBinaryTree(root);       
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
    TestRBTree(nums, sizeof(nums)/sizeof(nums[0]), &cnt);


    
    return 0;
}


