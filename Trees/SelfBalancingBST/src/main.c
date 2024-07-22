#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BiTree.h"

void PrintNodeInfo(BiTreeNodePtr pNode) {
    printf("%s ", pNode->value.name);
    // For debugging
    fflush(stdout);
    //pNode->visited = 1;
}

void TestAVLTree(long *nums, int n, long *pCnt) {
    // Create an empty binary tree
    BiTreeNodePtr root = NULL;    
    assert(pCnt);

    printf("*******************************  Testing BiTreeInsert() *******************************\n\n\n");
    for (int i = 0; i < n; i++) {
        BiTreeInsert(&root, &root, nums[i], NULL, pCnt);

        printf("After inserting %ld \n", nums[i]);

        printf("----------------  InOrderTraversal() ----------------\n");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n-----------------------------------------------------\n\n");

        (*pCnt)++;              
        GenOneImage(root, "BiTreeBiTreeInsert", "images/BiTreeBiTreeInsert", *pCnt);        
    } 

    printf("\n\n\n*******************************  Testing BiTreeDelete() *******************************\n\n\n");

    GenOneImage(root, "BiTreeDelete", "images/BiTreeDelete", *pCnt);
    for (int i = 0; i < n; i++) {                        
        BiTreeDelete(&root, &root, nums[i], pCnt);
        printf("After deleting %ld \n", nums[i]);
        printf("----------------  InOrderTraversal() ----------------\n");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n-----------------------------------------------------\n\n");

        (*pCnt)++;              
        GenOneImage(root, "BiTreeDelete", "images/BiTreeDelete", *pCnt);        
    }
    printf("\n");

    // Free the heap memory
    ReleaseBinaryTree(root);       
}

int main(void) {
    long cnt = 0;

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");

    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");
  
    
    long nums[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    //long nums[] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    TestAVLTree(nums, sizeof(nums)/sizeof(nums[0]), &cnt);

    long LLnums[] = {30, 20, 10};
    TestAVLTree(LLnums, sizeof(LLnums)/sizeof(LLnums[0]), &cnt);    

    long RRnums[] = {10, 20, 30};
    TestAVLTree(RRnums, sizeof(RRnums)/sizeof(RRnums[0]), &cnt);

    long LRnums[] = {30, 10, 20};
    TestAVLTree(LRnums, sizeof(LRnums)/sizeof(LRnums[0]), &cnt);

    long RLnums[] = {10, 30, 20};
    TestAVLTree(RLnums, sizeof(RLnums)/sizeof(RLnums[0]), &cnt);


    return 0;
}


