#include <stdio.h>
#include <stdlib.h>
#include "BiTree.h"


void PrintNodeInfo(BiTreeNodePtr pNode) {
    printf("Visiting %s\n", pNode->value.name);
    pNode->visited = 1;
}

int main(int argc, char **argv, char **env) {

    // Create a binary tree
    BiTreeNodePtr root = CreateBinaryTree();

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");

    // remove the *.dot and *.png files in the directory 'images'
    //system("rm -f images/*.dot images/*.png");
      
    //GenOneImage(root, "OurBiTree", "images/OurBiTree", 0);
    
    printf("*****************  PreOrderTraversal() **********************\n");
    PreOrderTraversal(root, PrintNodeInfo);
    printf("\n\n");

    printf("*****************  InOrderTraversal() **********************\n");
    InOrderTraversal(root, PrintNodeInfo);
    printf("\n\n");


    printf("*****************  PostOrderTraversal() **********************\n");
    PostOrderTraversal(root, PrintNodeInfo);
    printf("\n\n");

    printf("*****************  PreOrderTraversal2() **********************\n");
    ResetNodeState(root);
    PreOrderTraversal2(root, PrintNodeInfo);
    printf("\n\n");

    printf("*****************  InOrderTraversal2() **********************\n");
    ResetNodeState(root);
    printf("After completing the code in Q1-Q5 (BiTree.c), please also uncomment line %d in %s:\n\n", (__LINE__ + 1), __FILE__);
    //InOrderTraversal2(root, PrintNodeInfo);
    printf("\n\n");

    printf("*****************  PostOrderTraversal2() **********************\n");
    ResetNodeState(root);
    PostOrderTraversal2(root, PrintNodeInfo);
    printf("\n\n");  

    // Free the heap memory
    ReleaseBinaryTree(root);

    return 0;
}

