#include <stdio.h>
#include <stdlib.h>
#include "BiTree.h"

void PrintNodeInfo(BiTreeNodePtr pNode) {
    printf("%s ", pNode->value.name);
    // For debugging
    fflush(stdout);
    pNode->visited = 1;
}

int main(int argc, char **argv, char **env) {
    static long cnt = 0;

    // Create an empty binary tree
    BiTreeNodePtr root = NULL;

        // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");

    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");     
 
    /*
     'nums[]' contains the pre-order traversal of the following binary search tree.

                       50  
                    /      \
                  20        70
                 /  \      /  \  
                10  30    60  100
                      \        /
                      40      90
                             /
                            80
     */       
    printf("*******************************  Testing BiTreeInsert() *******************************\n\n\n");
    long nums[] = {50, 20, 10, 30, 40, 70, 60, 100, 90, 80};
    for (int i = 0; i < sizeof(nums)/sizeof(nums[0]); i++) {
        BiTreeInsert(&root, nums[i], NULL);

        printf("After inserting %ld \n", nums[i]);

        printf("*****************  InOrderTraversal() **********************\n");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n\n");

        cnt++;              
        GenOneImage(root, "BiTreeBiTreeInsert", "images/BiTreeBiTreeInsert", cnt);        
    }
    printf("\n");


    printf("\n\n\n*******************************  Testing BiTreeSearch() *******************************\n\n\n");
    long targetVal = 80;
    
    BiTreeNodePtr node = NULL;
    printf("After completing the code in Q1-Q5 (BiTree.c), please also uncomment line %d in %s:\n\n", (__LINE__ + 1), __FILE__);    
    //node = BiTreeSearch(root, targetVal);
    if (node) {
        printf("Found: BiTreeSearch(root, %ld) == %ld\n\n", node->value.numVal, targetVal);
    } else {
        printf("Not Found: BiTreeSearch(root, %ld) == NULL\n\n", targetVal);
    }

    //
    node = BiTreeMinValueNode(root);

    if (node) {
        printf("BiTreeMinValueNode(root) == %ld\n\n", node->value.numVal);
    }

    printf("\n\n\n*******************************  Testing BiTreeDelete() *******************************\n\n\n");
    cnt = 0;
    GenOneImage(root, "BiTreeDelete", "images/BiTreeDelete", cnt);
    for (int i = 0; i < sizeof(nums)/sizeof(nums[0]); i++) {                        
        BiTreeDelete(&root, &root, nums[i]);
        printf("After deleting %ld \n", nums[i]);
        printf("*****************  InOrderTraversal() **********************\n");
        InOrderTraversal(root, PrintNodeInfo);
        printf("\n\n");

        //cnt++;              
        //GenOneImage(root, "BiTreeDelete", "images/BiTreeDelete", cnt);        
    }
    printf("\n");      

    // Free the heap memory
    ReleaseBinaryTree(root);

    return 0;
}

