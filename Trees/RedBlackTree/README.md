# Red-Black Tree


[AVL Trees](../SelfBalancingBST/README.md) require a strict balancing condition: the height difference between the left and right subtrees of any node (called the balance factor) must be at most 1 (i.e., -1, 0, and 1). Rotations are performed to maintain balance of an AVL tree.
Red-Black Trees, on the other hand, use color-based balancing with a much more relaxed set of rules and lighter operations (i.e., recoloring) to maintain balance. The key balancing rules for Red-Black Trees are:

- Every path from a node to its leaves must have the same number of black nodes.
- Red nodes cannot have red children (no two consecutive red nodes).

Together, these two rules ensure that the longest path in a Red-Black tree from the root to any leaf is no more than twice the length of the shortest path. 

For example, in the following diagram, 
the Red-Black tree uses the recoloring operation to restore its balance, but it tends to have a taller tree structure. 
The AVL Tree maintain a stricter balance, resulting in a generally shorter tree (i.e., shorter search time), 
but potentially more complex insertions due to its balancing constraints.

### Red-Black Tree and AVL Tree

| Initial (Red-Black Tree) | Insert | Self-Balancing (recoloring or rotation) |
|:-------------:|:-------------:|:-------------:|
| <img src="diagrams/RBTree1.png" width="80%" height="80%"> | <img src="diagrams/RBTree2.png" width="80%" height="80%"> |<img src="diagrams/RBTree3.png" width="80%" height="80%"> |

| Initial (AVL Tree) | Insert | Self-Balancing (Rotation) |
|:-------------:|:-------------:|:-------------:|
| <img src="diagrams/AVLTree1.png" width="80%" height="80%"> | <img src="diagrams/AVLTree2.png" width="80%" height="80%"> |<img src="diagrams/AVLTree3.png" width="80%" height="80%"> |

As a result, Red-Black trees do not need to be as tightly balanced as AVL Trees, meaning they can be more flexible when adjusting to changes (insertions and deletions). This relaxation reduces the overhead of maintaining strict balance.


<!-- (A dichromatic framework for balanced trees) -->
### A Red-Black Tree is a self-balancing [binary search tree](../BinarySearchTree/README.md) with the following properties.

- Property 1: the root is black.
- Property 2: a node is either BLACK or RED
- Property 3: a red node shouldn't have any red child.
- Property 4: the left and right subtrees have the same black height.
- Property 5: all null nodes are black.

```C
/*
    This function returns 1 when root is a legal RB tree.
    Otherwise, it returns 0.
 */
int IsRBTree(BiTreeNodePtr root) {
    if (root) {
        // Property 1: the root is black.
        if (root->color != BLACK) {
            return 0;
        }
        // Check other properties
        return CheckRBTree(root);
    } else {
        return 1;
    }
}

static int CheckRBTree(BiTreeNodePtr pNode) {
    if (pNode) {
        // Property 2: a node is either BLACK or RED
        if (pNode->color != BLACK && pNode->color != RED) {
            return 0;
        }        

        if (pNode->color == RED) {
            // Property 3: a red node shouldn't have any red child.
            if (hasRedLeft(pNode) || hasRedRight(pNode)) {
                return 0;
            }
        }

        // Property 4: The left and right subtrees have the same black height.
        if (RBTreeBlackHeight(pNode->leftChild) != RBTreeBlackHeight(pNode->rightChild)) {
            return 0;
        }

        // Property 5: All null nodes are black.  See hasBlackLeft() and hasBlackRight()

        // Recursively check left and right subtrees
        if (CheckRBTree(pNode->leftChild) == 0) {
            return 0;
        }
        
        if (CheckRBTree(pNode->rightChild) == 0) {
            return 0;
        }
    }
    return 1;
}

int hasRedRight(BiTreeNodePtr pNode) {
    return pNode->rightChild && pNode->rightChild->color == RED;
}

int hasBlackRight(BiTreeNodePtr pNode) {
    // Property 5: All null nodes are black
    return (!pNode->rightChild) || (pNode->rightChild->color == BLACK);
}

```

### RBTreeBlackHeight()
```C
static int GetMax(int h1, int h2) {
    return (h1 > h2) ? h1: h2;
}

static int RBTreeBlackHeight(BiTreeNodePtr root) {
    if (root) {
        return root->blackHeight;
    } else {
        return 0;
    }
}

static void UpdateBlackHeight(BiTreeNodePtr root) {
    if (root) {
        int max = GetMax(RBTreeBlackHeight(root->leftChild), RBTreeBlackHeight(root->rightChild));
        if (root->color == BLACK) {
            root->blackHeight = max + 1;
        } else {
            root->blackHeight = max;
        }
    }
}

int RBTreeBalanceFactor(RBTreeNodePtr root) {
    if (root) {
        return RBTreeBlackHeight(root->leftChild) - RBTreeBlackHeight(root->rightChild);
    } else {
        return 0;
    }
}
```



### Red-Black Tree and 2-3-4 Tree

A 2-3-4 Tree is a type of self-balancing search tree and a specific form of a [B-tree](../BPlusTree/README.md) (a generalization of binary search trees), where each node can have 2, 3, or 4 children and can store 1, 2, or 3 keys, respectively. 
A Red-Black Tree can be converted to a 2-3-4 Tree.
The converted 2-3-4 tree can be used to explain the height of a Red-Black Tree. The correspondence between a Red-Black Tree and a 2-3-4 Tree provides a simple and intuitive way to understand the height properties of a Red-Black Tree.

In the following red-black trees, 

- **H** is short for the black height of a tree node, returned by **RBTreeBlackHeight()**

- **B** stands for the balance factor of the tree node, calculated at run time via  **RBTreeBalanceFactor()**


| Red-Black Tree | 2-3-4 Tree |
|:-------------:|:-------------:|
| <img src="diagrams/RBTree1Insert_0001.png" width="30%" height="30%"> | <img src="diagrams/RBTree3BTree_0001.png" width="30%" height="30%"> |
| <img src="diagrams/RBTree1Insert_0002.png" width="40%" height="40%"> | <img src="diagrams/RBTree3BTree_0002.png" width="40%" height="40%"> |
| <img src="diagrams/RBTree1Insert_0003.png" width="50%" height="50%"> | <img src="diagrams/RBTree3BTree_0003.png" width="50%" height="50%"> |
| <img src="diagrams/RBTree1Insert_0004.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0004.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0005.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0005.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0006.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0006.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0007.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0007.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0008.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0008.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0009.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0009.png" width="80%" height="80%"> |
| <img src="diagrams/RBTree1Insert_0010.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0010.png" width="80%" height="80%"> |

### How to insert a node (as a red leaf) into a red-black tree

Two steps are needed:

- Perform the standard [Binary Search Tree](../BinarySearchTree/README.md) insertion

- Fix any violations of the Red-Black Tree properties that may arise

```C

static void RecursiveRBTreeInsert(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {
    RBTreeNodePtr pNode = *pNodePtr;
    assert(pCnt);

    char *graphName = "RBTree";
    char *fileName = "images/RBTree1Insert";

    if (pNode == NULL) {
        RBTreeNodePtr tmp = CreateRBTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;
        // printf("inserting %ld\n", numVal);
        return;
    } else {
        if (numVal < pNode->value.numVal) {
            RecursiveRBTreeInsert(pRoot, &pNode->leftChild, numVal, nodeName, pCnt);
        } else if (numVal > pNode->value.numVal) {
            RecursiveRBTreeInsert(pRoot, &pNode->rightChild, numVal, nodeName, pCnt);
        } else {
            // If numVal is already in the binary search tree, do nothing.
            return;
        }
        // The inserted node is RED node. We will update black heights in FixViolationsInInsertion().
        FixViolationsInInsertion(pRoot, pNodePtr, pCnt, graphName, fileName);
    }
}

/*
    The cases in FixViolationsInInsertion():

    e.g., RedParentRedUncle_RXXX represents

                Grandparent
                 /     \
                /       \
        Red Parent     Red Uncle
            /    
           /
        Red Child 
        (inserted)
 */
typedef enum {
    // R + R
    RedParentRedUncle_RXXX, // red parent, red uncle, left red child
    RedParentRedUncle_XRXX, // red parent, red uncle, right red child
    RedUncleRedParent_XXRX, // red uncle, red parent, left red child
    RedUncleRedParent_XXXR, // red uncle, red parent, right red child
    // R + B
    RedParentBlackUncle_RXXX, // red parent, black uncle, left red child
    RedParentBlackUncle_XRXX, // red parent, black uncle, right red child
    BlackUncleRedParent_XXRX, // black uncle, red parent, left red child
    BlackUncleRedParent_XXXR, // black uncle, red parent, right red child
    // other: e.g., B + B
    OtherRBState,
} RBTreeNodeState;
```

### How to delete a node from a red-black tree

Two steps are needed:

- Perform the standard [Binary Search Tree](../BinarySearchTree/README.md) deletion

- Fix any violations of the Red-Black Tree properties that may arise

```C
static void RecursiveRBTreeDelete(RBTreeNodePtr *pRoot, RBTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    // static long cnt = 0;
    assert(pCnt);
    char *graphName = "RBTree";
    char *fileName = "images/RBTree2Delete";
    RBTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        if (numVal < pNode->value.numVal) {
            RecursiveRBTreeDelete(pRoot, &(pNode->leftChild), numVal, pCnt);
        } else if (numVal > pNode->value.numVal) {
            RecursiveRBTreeDelete(pRoot, &(pNode->rightChild), numVal, pCnt);
        } else {
            /************************************************************************
                If the node (to be deleted) has:

                    0 child:

                        leftChild == NULL && rightChild == NULL    // case 00

                    1 child:

                        leftChild == NULL && rightChild != NULL    // case 01

                        or
                        leftChild != NULL && rightChild == NULL    // case 10

                    2 children:

                        leftChild != NULL && rightChild != NULL    // case 11

             **************************************************************************/
            if (pNode->leftChild == NULL) {   
                // case 00 and case 01
                RBTreeNodePtr tmp = pNode->rightChild;
                /*
                    If the node to be deleted is RED, nothing to do.
                 */
                if ((pNode->color == BLACK) && (pNode->rightChild)) {
                    // This coloring operation can increase the black height of the subtree.
                    pNode->rightChild->color = BLACK;
                }
                free(pNode);
                *pNodePtr = tmp;
            } else if (pNode->rightChild == NULL) {   
                // case 10
                RBTreeNodePtr tmp = pNode->leftChild;
                /*
                    If the node to be deleted is RED, nothing to do.
                 */
                if ((pNode->color == BLACK) && (pNode->leftChild)) {
                    pNode->leftChild->color = BLACK;
                }
                free(pNode);
                *pNodePtr = tmp;
            } else {
                // case 11:  with two children
                // Get pNode's in-order pPredecessor, which is right-most node in its left sub-tree.
                RBTreeNodePtr pPredecessor = BiTreeMaxValueNode(pNode->leftChild);

                // (Swapping is done for clearer debugging output)
                // Swap the values of the node pointed to by pNode and its in-order predecessor
                NodeValue val = pNode->value;
                // Copy the predecessor's value (this copy is necessary)
                pNode->value = pPredecessor->value;
                pPredecessor->value = val;

                // Now, numVal is in left sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                RecursiveRBTreeDelete(pRoot, &pNode->leftChild, pPredecessor->value.numVal, pCnt);
            }
        }
        //
        pNode = *pNodePtr;
        // If it is NULL, just return.
        if (pNode == NULL) {
            return;
        }
        // recalculate and store its height
        UpdateBlackHeight(pNode);

        int bFactor = RBTreeBalanceFactor(pNode);

        if (bFactor < 0) {
            /*
                      (*pNodePtr)
                          |
                          |
                          v
                         Node
                        /      \
                       /        \
                Deleted (B)    Sibling (R or B)
             */
            if (hasRedRight(pNode)) {
                FixRedRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackRight(pNode) && pNode->rightChild) {
                FixBlackRightSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            }
        } else if (bFactor > 0) {
            /*
                          (*pNodePtr)
                              |
                              |
                              V
                             Node
                            /      \
                           /        \
                Sibling (R or B)    Deleted (B)
             */
            if (hasRedLeft(pNode)) {
                FixRedLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            } else if (hasBlackLeft(pNode) && pNode->leftChild) {
                FixBlackLeftSiblingInDeletion(pRoot, pNodePtr, graphName, fileName, pCnt);
            }
        }
    }
}
```


### How to convert a red-black tree into a 2-3-4 tree


| Red-Black Tree | 2-3-4 Tree |
|:-------------:|:-------------:|
| <img src="diagrams/RBTree1Insert_0008.png" width="80%" height="80%"> | <img src="diagrams/RBTree3BTree_0008.png" width="80%" height="80%"> |


```C
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
            struct BTreeNode *red1Left = RBTreeTo234Tree(root->leftChild->leftChild);            
            struct BTreeNode *red1Right = RBTreeTo234Tree(root->leftChild->rightChild);
            struct BTreeNode *red2Left = RBTreeTo234Tree(root->rightChild->leftChild);
            struct BTreeNode *red2Right = RBTreeTo234Tree(root->rightChild->rightChild);            
            //
            SetKeyAtIndex(pNode, root->leftChild->value.numVal, 0);
            SetKeyAtIndex(pNode, root->value.numVal, 1);
            SetKeyAtIndex(pNode, root->rightChild->value.numVal, 2);

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
            struct BTreeNode *red1Left = RBTreeTo234Tree(root->leftChild->leftChild);            
            struct BTreeNode *red1Right = RBTreeTo234Tree(root->leftChild->rightChild);
            struct BTreeNode *black2 = RBTreeTo234Tree(root->rightChild);

            SetKeyAtIndex(pNode, root->leftChild->value.numVal, 0);
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
            struct BTreeNode *black1 = RBTreeTo234Tree(root->leftChild);
            struct BTreeNode *red2Left = RBTreeTo234Tree(root->rightChild->leftChild);
            struct BTreeNode *red2Right = RBTreeTo234Tree(root->rightChild->rightChild);

            SetKeyAtIndex(pNode, root->value.numVal, 0);
            SetKeyAtIndex(pNode, root->rightChild->value.numVal, 1);

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
            struct BTreeNode *black1 = RBTreeTo234Tree(root->leftChild);
            struct BTreeNode *black2 = RBTreeTo234Tree(root->rightChild);

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

```


### make view

```sh
RedBlackTree$ make view
```

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.


### RBTreeInsert(): {50, 20, 10, 30, 40, 70, 60, 100, 90, 80}

| Insert 50 | 
|:-------------:|
| <img src="images/RBTree1Insert_0001.png" width="80%" height="80%"> |

| Insert 20 |
|:-------------:|
| <img src="images/RBTree1Insert_0002.png" width="80%" height="80%"> |


| Insert 20 |
|:-------------:|
| <img src="images/RBTree1Insert_0004.png" width="80%" height="80%"> |


| Insert 10 |
|:-------------:|
| <img src="images/RBTree1Insert_0005.png" width="80%" height="80%"> |



| Insert 10 (red parent + black uncle + left child, right rotation at 50 and recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0007.png" width="80%" height="80%"> |

| Insert 10 | 
|:-------------:|
|  <img src="images/RBTree1Insert_0008.png" width="80%" height="80%"> |

| Insert 10 | 
|:-------------:|
| <img src="images/RBTree1Insert_0009.png" width="80%" height="80%"> |

| Insert 30 |
|:-------------:|
| <img src="images/RBTree1Insert_0010.png" width="80%" height="80%"> |



| Insert 30 (red uncle + red parent, recoloring)|
|:-------------:|
| <img src="images/RBTree1Insert_0012.png" width="80%" height="80%"> |

| Insert 30  (recoloring root, with the black height of the tree increased by one) |
|:-------------:|
| <img src="images/RBTree1Insert_0013.png" width="80%" height="80%"> |

| Insert 30  |
|:-------------:|
| <img src="images/RBTree1Insert_0014.png" width="80%" height="80%"> |

| Insert 40 |
|:-------------:|
| <img src="images/RBTree1Insert_0015.png" width="80%" height="80%"> |


| Insert 40 (red parent + black uncle + right child, left rotation at 30) |
|:-------------:|
| <img src="images/RBTree1Insert_0017.png" width="80%" height="80%"> |


| Insert 40 (red parent + black uncle + left child, right rotation at 50 + recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0018.png" width="80%" height="80%"> |



| Insert 40  |
|:-------------:|
| <img src="images/RBTree1Insert_0020.png" width="80%" height="80%"> |


| Insert 40 |
|:-------------:|
| <img src="images/RBTree1Insert_0022.png" width="80%" height="80%"> |

| Insert 70 |
|:-------------:|
| <img src="images/RBTree1Insert_0023.png" width="80%" height="80%"> |



| Insert 70 (red uncle + red parent, recoloring)|
|:-------------:|
| <img src="images/RBTree1Insert_0025.png" width="80%" height="80%"> |

| Insert 70  |
|:-------------:|
| <img src="images/RBTree1Insert_0026.png" width="80%" height="80%"> |

| Insert 70 |
|:-------------:|
| <img src="images/RBTree1Insert_0027.png" width="80%" height="80%"> |


| Insert 70 |
|:-------------:|
| <img src="images/RBTree1Insert_0029.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/RBTree1Insert_0030.png" width="80%" height="80%"> |


| Insert 60 (black uncle + red parent + left child, right rotation at 70)|
|:-------------:|
| <img src="images/RBTree1Insert_0032.png" width="80%" height="80%"> |

| Insert 60  (black uncle + red parent + right child, left rotation at 50 + recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0033.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/RBTree1Insert_0034.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/RBTree1Insert_0035.png" width="80%" height="80%"> |


| Insert 60 |
|:-------------:|
| <img src="images/RBTree1Insert_0038.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/RBTree1Insert_0039.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/RBTree1Insert_0040.png" width="80%" height="80%"> |



| Insert 100 (red uncle + red parent, recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0042.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/RBTree1Insert_0043.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/RBTree1Insert_0045.png" width="80%" height="80%"> |

| Insert 100 (black uncle + red parent + right child, left rotation at 20 + recoloring)|
|:-------------:|
| <img src="images/RBTree1Insert_0046.png" width="80%" height="80%"> |

| Insert 100  |
|:-------------:|
| <img src="images/RBTree1Insert_0047.png" width="80%" height="80%"> |


| Insert 100 |
|:-------------:|
| <img src="images/RBTree1Insert_0048.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/RBTree1Insert_0049.png" width="80%" height="80%"> |



| Insert 90 (black uncle + red parent + left child, right rotation at 100) |
|:-------------:|
| <img src="images/RBTree1Insert_0051.png" width="80%" height="80%"> |


| Insert 90 ((black uncle + red parent + right child, left rotation at 70 + recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0052.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/RBTree1Insert_0053.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/RBTree1Insert_0054.png" width="80%" height="80%"> |


| Insert 90 |
|:-------------:|
| <img src="images/RBTree1Insert_0056.png" width="80%" height="80%"> |


| Insert 90 |
|:-------------:|
| <img src="images/RBTree1Insert_0058.png" width="80%" height="80%"> |


| Insert 80 |
|:-------------:|
| <img src="images/RBTree1Insert_0059.png" width="80%" height="80%"> |


| Insert 80 (red parent + red uncle, recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0061.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/RBTree1Insert_0062.png" width="80%" height="80%"> |


| Insert 80 |
|:-------------:|
| <img src="images/RBTree1Insert_0064.png" width="80%" height="80%"> |

| Insert 80 (red uncle + red parent, recoloring) |
|:-------------:|
| <img src="images/RBTree1Insert_0065.png" width="80%" height="80%"> |

| Insert 80 (recoloring root, with the black height of the tree increased by one) |
|:-------------:|
| <img src="images/RBTree1Insert_0066.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/RBTree1Insert_0067.png" width="80%" height="80%"> |


### RBTreeDelete(): {50, 20, 10, 30, 40, 70, 60, 100, 90, 80}

| Delete 50 (red sibling + 2 black nephews, left rotation at 60 + recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0068.png" width="80%" height="80%"> |

| Delete 50 (black sibling + right red nephew, left rotation at 60 + recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0069.png" width="80%" height="80%"> |

| Delete 50 |
|:-------------:|
| <img src="images/RBTree2Delete_0070.png" width="80%" height="80%"> |




| Delete 50 |
|:-------------:|
| <img src="images/RBTree2Delete_0072.png" width="80%" height="80%"> |

| Delete 50 |
|:-------------:|
| <img src="images/RBTree2Delete_0073.png" width="80%" height="80%"> |

| Delete 20 (swap 10 and 20) |
|:-------------:|
| <img src="images/RBTree2Delete_0074.png" width="80%" height="80%"> |


| Delete 20 (black sibling at 30 + two black nephews, recoloring)|
|:-------------:|
| <img src="images/RBTree2Delete_0075.png" width="80%" height="80%"> |

| Delete 20 (black sibling at 90 + left red nephew at 70, right rotation at 90)|
|:-------------:|
| <img src="images/RBTree2Delete_0076.png" width="80%" height="80%"> |



| Delete 20 (black sibling at 90 + two black nephews, left rotation at 40 + recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0078.png" width="80%" height="80%"> |

| Delete 20 |
|:-------------:|
| <img src="images/RBTree2Delete_0079.png" width="80%" height="80%"> |

| Delete 20 |
|:-------------:|
| <img src="images/RBTree2Delete_0080.png" width="80%" height="80%"> |


| Delete 10 |
|:-------------:|
| <img src="images/RBTree2Delete_0081.png" width="80%" height="80%"> |


| Delete 10 |
|:-------------:|
| <img src="images/RBTree2Delete_0085.png" width="80%" height="80%"> |


| Delete 10 |
|:-------------:|
| <img src="images/RBTree2Delete_0087.png" width="80%" height="80%"> |


| Delete 30 (black sibling at 60 + two black nephews, recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0088.png" width="80%" height="80%"> |

| Delete 30 |
|:-------------:|
| <img src="images/RBTree2Delete_0089.png" width="80%" height="80%"> |


| Delete 30 (black sibling at 90 + two black nephews, recoloring)|
|:-------------:|
| <img src="images/RBTree2Delete_0090.png" width="80%" height="80%"> |


| Delete 30 (the black height of the tree decreased by 1) |
|:-------------:|
| <img src="images/RBTree2Delete_0091.png" width="80%" height="80%"> |


| Delete 30 |
|:-------------:|
| <img src="images/RBTree2Delete_0092.png" width="80%" height="80%"> |

| Delete 40 |
|:-------------:|
| <img src="images/RBTree2Delete_0093.png" width="80%" height="80%"> |


| Delete 40 |
|:-------------:|
| <img src="images/RBTree2Delete_0097.png" width="80%" height="80%"> |


| Delete 70 (swap 70 and 60) |
|:-------------:|
| <img src="images/RBTree2Delete_0098.png" width="80%" height="80%"> |


| Delete 70 (red sibling at 90, left rotation at 60 + recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0099.png" width="80%" height="80%"> |

| Delete 70 (black sibling at 80 + two black nephews, recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0100.png" width="80%" height="80%"> |

| Delete 70 |
|:-------------:|
| <img src="images/RBTree2Delete_0101.png" width="80%" height="80%"> |

| Delete 70 |
|:-------------:|
| <img src="images/RBTree2Delete_0102.png" width="80%" height="80%"> |

| Delete 60 |
|:-------------:|
| <img src="images/RBTree2Delete_0103.png" width="80%" height="80%"> |


| Delete 60 |
|:-------------:|
| <img src="images/RBTree2Delete_0107.png" width="80%" height="80%"> |

| Delete 100 (black sibling at 80 + two black nephews, recoloring) |
|:-------------:|
| <img src="images/RBTree2Delete_0108.png" width="80%" height="80%"> |

| Delete 100 (the black height of the tree decreased by 1)|
|:-------------:|
| <img src="images/RBTree2Delete_0109.png" width="80%" height="80%"> |


| Delete 100 |
|:-------------:|
| <img src="images/RBTree2Delete_0110.png" width="80%" height="80%"> |


| Delete 90 |
|:-------------:|
| <img src="images/RBTree2Delete_0111.png" width="80%" height="80%"> |


| Delete 80 |
|:-------------:|
| <img src="images/RBTree2Delete_0114.png" width="80%" height="80%"> |




