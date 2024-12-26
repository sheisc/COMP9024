#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "RedBlackTree.h"
#include "BTree.h"




struct BTreeNode *CreateBTree(void) {
    struct BTreeNode *pNode = (struct BTreeNode *) malloc(sizeof(struct BTreeNode));
    assert(pNode);
    
    for (long i = 0; i < NUM_OF_KEYS; i++) {
        pNode->keys[i] = 0;
    }

    for (long i = 0; i < NUM_OF_CHILDREN; i++) {
        pNode->children[i] = NULL;        
    }

    pNode->nk = 0;

    pNode->leaf = 0;

    pNode->visited = 0;

    return pNode;

}

void ReleaseBTree(struct BTreeNode *root) {
    if (root) {
        long nk = root->nk;
        for (long i = 0; i <= nk; i++) {
            ReleaseBTree(root->children[i]);            
        }
        free(root);
    }
}

static void SetKeyAtIndex(struct BTreeNode *pNode, TreeNodeKeyTy key, long i) {
    assert(i >= 0 && i < NUM_OF_KEYS);
    pNode->keys[i] = key;
}

static void SetChildAtIndex(struct BTreeNode *pNode, struct BTreeNode *child, long i) {
    assert(i >= 0 && i < NUM_OF_CHILDREN);
    pNode->children[i] = child;
}

static void SetNumberOfKeys(struct BTreeNode *pNode, long nk) {
    pNode->nk = nk;
}

static void SetLeafStatus(struct BTreeNode *pNode, int leaf) {
    pNode->leaf = leaf;
}


long GetNumberOfChildren(struct BTreeNode *root) {
    if (root) {
        long nc = root->nk + 1;
        // if (nc > NUM_OF_CHILDREN) {
        //     nc = NUM_OF_CHILDREN;
        // }
        return nc;
    } else {
        return 0;
    }
}

char *GetBTreeNodeName(struct BTreeNode *root, char *nodeName, long n) {
    long nk = root->nk;
    char *buf = nodeName;
    memset(nodeName, 0, n);
    long availableLen = n - 1;
    /*
        These functions return the number of characters printed  
        (excluding  the  null  byte used to end output to strings).

        The functions snprintf() and vsnprintf() write at most size  bytes  
        (including  the  terminating  null  byte).   
     */
    for (long i = 0; i < nk; i++) {
        long len;
        if (i != nk - 1) {
            len = snprintf(buf, availableLen, "%ld_", root->keys[i]);
        } else {
            len = snprintf(buf, availableLen, "%ld", root->keys[i]);
        }
        buf += len;
        availableLen -= len;
    }

    return nodeName;
}

struct BTreeNode *RBTreeTo234Tree(RBTreeNodePtr root) {
    if (root) {
        assert(root->color == BLACK);
        struct BTreeNode *pNode = CreateBTree();
        if (hasRedLeft(root) && hasRedRight(root)) {
            /*
                       root (Black)                                                   
                       /   \                         (red1,  root,  red2)                            
                      /     \                        /     |       |    \                     
                    red1    red2        ---->       /      |       |     \                   
                   / \      / \                    /       |       |      \             
                  /   \    /   \                  r1L     r1R      r2L    r2R                     
                r1L   r1R r2L   r2R
             */
            struct BTreeNode *red1Left = RBTreeTo234Tree(root->left->left);            
            struct BTreeNode *red1Right = RBTreeTo234Tree(root->left->right);
            struct BTreeNode *red2Left = RBTreeTo234Tree(root->right->left);
            struct BTreeNode *red2Right = RBTreeTo234Tree(root->right->right);            
            //
            SetKeyAtIndex(pNode, root->left->value.numVal, 0);
            SetKeyAtIndex(pNode, root->value.numVal, 1);
            SetKeyAtIndex(pNode, root->right->value.numVal, 2);

            SetNumberOfKeys(pNode, 3);

            SetChildAtIndex(pNode, red1Left, 0);
            SetChildAtIndex(pNode, red1Right, 1);
            SetChildAtIndex(pNode, red2Left, 2);
            SetChildAtIndex(pNode, red2Right, 3);            

            SetLeafStatus(pNode, !(red1Left || red1Right || red2Left || red2Right));

        } else if (hasRedLeft(root)) {
            /*
                       root (Black)                                                        
                       /   \                           (red1, root)                                          
                      /     \                          /     |     \                         
                    red1    black2       ---->        /      |      \                                   
                    / \                              /       |       \                                   
                   /   \                           r1L      r1R      black2                          
                 r1L   r1R                                                                                      
             */          
            struct BTreeNode *red1Left = RBTreeTo234Tree(root->left->left);            
            struct BTreeNode *red1Right = RBTreeTo234Tree(root->left->right);
            struct BTreeNode *black2 = RBTreeTo234Tree(root->right);

            SetKeyAtIndex(pNode, root->left->value.numVal, 0);
            SetKeyAtIndex(pNode, root->value.numVal, 1);

            SetNumberOfKeys(pNode, 2);

            SetChildAtIndex(pNode, red1Left, 0);
            SetChildAtIndex(pNode, red1Right, 1);
            SetChildAtIndex(pNode, black2, 2);

            SetLeafStatus(pNode, !(red1Left || red1Right || black2));


        } else if (hasRedRight(root)) {
            /*
                       root (Black)                                                           
                       /   \                             (root, red2)                                    
                      /     \                            /     |     \                             
                   black1    red2         ---->         /      |      \                                 
                             / \                       /       |       \                                  
                            /   \                    black1    r2L      r2R                                    
                          r2L   r2R                                                                       
             */
            struct BTreeNode *black1 = RBTreeTo234Tree(root->left);
            struct BTreeNode *red2Left = RBTreeTo234Tree(root->right->left);
            struct BTreeNode *red2Right = RBTreeTo234Tree(root->right->right);

            SetKeyAtIndex(pNode, root->value.numVal, 0);
            SetKeyAtIndex(pNode, root->right->value.numVal, 1);

            SetNumberOfKeys(pNode, 2);

            SetChildAtIndex(pNode, black1, 0);
            SetChildAtIndex(pNode, red2Left, 1);
            SetChildAtIndex(pNode, red2Right, 2);

            SetLeafStatus(pNode, !(black1 || red2Left || red2Right));

        } else {
            /*
                       root (Black)                    (root)                              
                       /   \             ---->         /    \                               
                      /     \                         /      \                         
                   black1    black2                 black1    black2                                                  
             */
            struct BTreeNode *black1 = RBTreeTo234Tree(root->left);
            struct BTreeNode *black2 = RBTreeTo234Tree(root->right);

            SetKeyAtIndex(pNode, root->value.numVal, 0);

            SetNumberOfKeys(pNode, 1);

            SetChildAtIndex(pNode, black1, 0);
            SetChildAtIndex(pNode, black2, 1);

            SetLeafStatus(pNode, !(black1 || black2));
        }
        return pNode;

    } else {
        return NULL;
    }
}



