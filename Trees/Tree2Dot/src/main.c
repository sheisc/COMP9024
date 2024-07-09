#include <stdio.h>
#include <stdlib.h>
#include "BiTree.h"


int main(int argc, char **argv, char **env) {
    // Create a binary tree
    BiTreeNodePtr root = CreateBinaryTree();

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");
    // generate one image  
    GenOneImage(root, "OurBiTree", "images/OurBiTree", 0);
    
    // Free the heap memory
    ReleaseBinaryTree(root);

    return 0;
}



