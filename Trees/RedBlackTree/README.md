# Red-Black Tree

``` sh
/*******************************************************************
                 Red-Black Tree

    1.  How to do an in-order traversal in a binary search tree

    2.  How to insert data in a red-black tree

    3.  How to delete data in a red-black tree

    4.  How to convert a red-black tree into a 2-3-4 tree.

                                             COMP9024

 *******************************************************************/
``` 
<!--
### Tree Height

We use BiTreeHeight(root) to represent the height of a BST tree.


### Balance Factor

### AVL Tree

To make an AVL tree into a 2-3-4 tree, simply color 
red exactly those links which go from a node at an 
even height to a node at an odd height.
-->

<!-- (A dichromatic framework for balanced trees) -->
### Red-Black Tree

- Property 1: the root is black.
- Property 2: a node is either BLACK or RED
- Property 3: a red node shouldn't have any red child.
- Property 4: the left and right subtrees have the same black height.
- Property 5: all null nodes are black.

```C

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

```


### Red-Black Tree and AVL Tree

| Initial (Red-Black Tree) | Insert | Self-Balancing (recoloring or rotation) |
|:-------------:|:-------------:|:-------------:|
| <img src="diagrams/RBTree1.png" width="80%" height="80%"> | <img src="diagrams/RBTree2.png" width="80%" height="80%"> |<img src="diagrams/RBTree3.png" width="80%" height="80%"> |

| Initial (AVL Tree) | Insert | Self-Balancing (Rotation) |
|:-------------:|:-------------:|:-------------:|
| <img src="diagrams/AVLTree1.png" width="80%" height="80%"> | <img src="diagrams/AVLTree2.png" width="80%" height="80%"> |<img src="diagrams/AVLTree3.png" width="80%" height="80%"> |

Red-Black Trees use lighter operations like recoloring, but they allow for a taller tree structure, 

while AVL Trees maintain a stricter balance, resulting in generally shorter trees 

but potentially more complex insertions and deletions due to their balancing constraints.

### Red-Black Tree and 2-3-4 Tree

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

<!--
### Self Balancing

An AVL tree ensures that the balance factor of every node is within the specified range by performing rotations whenever necessary after insertion or deletion operations. These rotations restore the balance of the tree while maintaining the BST property.

-->

We have discussed how to create a *.dot file in [COMP9024/Trees/Tree2Dot](../../Trees/Tree2Dot/README.md) for a binary tree.

In this project, we study how to insert and delete data in a self-balancing red-black tree.




## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Trees/RedBlackTree

RedBlackTree$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

RedBlackTree$ code

```

Two configuration files (RedBlackTree/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and RedBlackTree/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



#### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Trees/RedBlackTree", then click the "Open" button.


#### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


#### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 83).

Then, click **Run -> Start Debugging**

### 2.4 Directory

```sh
├── Makefile             defining set of tasks to be executed (the input file of the 'make' command)
|
├── README.md            introduction to this project
|
├── src                  containing *.c and *.h
|   |
|   |
│   ├── BiTree.c         Binary Search Tree
│   ├── BiTree.h
|   |
│   ├── Queue.c          used in a breadth-first tree traversal when generating *.dot files
│   ├── Queue.h
|   |
│   ├── Tree2Dot.c       Convert a red-black tree to *.dot and *.png files
|   |
│   └── main.c           main()
|
|── images               containing *.dot and *.png files
|
└── .vscode              containing configuration files for Visual Studio Code
    |
    ├── launch.json      specifying which program to debug and with which debugger,
    |                    used when you click "Run -> Start Debugging"
    |
    └── tasks.json       specifying which task to run (e.g., 'make' or 'make clean')
                         used when you click "Terminal -> Run Build Task" or "Terminal -> Run Task"
```

Makefile is discussed in [COMP9024/C/HowToMake](../../C/HowToMake/README.md).

## 3 The main procedure

### 3.1 make and ./main

**In addition to utilizing VS Code, we can also compile and execute programs directly from the command line interface as follows.**

``` sh

RedBlackTree$ make

RedBlackTree$ ./main

*******************************  Testing RBTreeInsert() *******************************


Step 1: After inserting 50 
	----------------  InOrderTraversal() ----------------
	50 
	-----------------------------------------------------

Step 2: After inserting 20 
	----------------  InOrderTraversal() ----------------
	20 50 
	-----------------------------------------------------

Step 3: After inserting 10 
	----------------  InOrderTraversal() ----------------
	10 20 50 
	-----------------------------------------------------

Step 4: After inserting 30 
	----------------  InOrderTraversal() ----------------
	10 20 30 50 
	-----------------------------------------------------

Step 5: After inserting 40 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 
	-----------------------------------------------------

Step 6: After inserting 70 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 70 
	-----------------------------------------------------

Step 7: After inserting 60 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 60 70 
	-----------------------------------------------------

Step 8: After inserting 100 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 60 70 100 
	-----------------------------------------------------

Step 9: After inserting 90 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 60 70 90 100 
	-----------------------------------------------------

Step 10: After inserting 80 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 50 60 70 80 90 100 
	-----------------------------------------------------

*******************************  Testing RBTreeDelete() *******************************


Step 1: After deleting 50 
	----------------  InOrderTraversal() ----------------
	10 20 30 40 60 70 80 90 100 
	-----------------------------------------------------

Step 2: After deleting 20 
	----------------  InOrderTraversal() ----------------
	10 30 40 60 70 80 90 100 
	-----------------------------------------------------

Step 3: After deleting 10 
	----------------  InOrderTraversal() ----------------
	30 40 60 70 80 90 100 
	-----------------------------------------------------

Step 4: After deleting 30 
	----------------  InOrderTraversal() ----------------
	40 60 70 80 90 100 
	-----------------------------------------------------

Step 5: After deleting 40 
	----------------  InOrderTraversal() ----------------
	60 70 80 90 100 
	-----------------------------------------------------

Step 6: After deleting 70 
	----------------  InOrderTraversal() ----------------
	60 80 90 100 
	-----------------------------------------------------

Step 7: After deleting 60 
	----------------  InOrderTraversal() ----------------
	80 90 100 
	-----------------------------------------------------

Step 8: After deleting 100 
	----------------  InOrderTraversal() ----------------
	80 90 
	-----------------------------------------------------

Step 9: After deleting 90 
	----------------  InOrderTraversal() ----------------
	80 
	-----------------------------------------------------

Step 10: After deleting 80 
	----------------  InOrderTraversal() ----------------
	
	-----------------------------------------------------

```

### 3.2 make view

**Ensure that you have executed 'make' and './main' before 'make view'.**


```sh
RedBlackTree$ make view
```

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.

Here, **feh** is an image viewer available in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/).

### Different cases in self balancing



#### 3.2.1 RBTreeInsert()

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


#### 3.2.2 RBTreeDelete()


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

## 4 Data structures

```C
typedef enum {
    NS_FROM_UP,      // 0   
    NS_FROM_LEFT,    // 1
    NS_FROM_RIGHT,   // 2
} NodeState;

typedef enum {
    RED,    // 0
    BLACK,  // 1
} NodeColor;

struct BiTreeNode {
    /*
     The value of a binary tree node:
  
     1. an integer for representing the node's value (e.g., 300), 
      
     2. a C string for representing its node name
     */
    NodeValue value;  
    // left subtree
    struct BiTreeNode *leftChild;
    // right subtree
    struct BiTreeNode *rightChild;
    // the current state when this node is pushed on the data stack (in non-recursive traversals)
    NodeState state;
    // whether this node has been visited
    int visited;
    //
    int blackHeight;
    // 
    NodeColor color;
};

typedef struct BiTreeNode *BiTreeNodePtr;

typedef BiTreeNodePtr RBTreeNodePtr;

```


## 5 Algorithms
<!--
### 5.1 IsRBTree()

```C

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

```
-->

### RBTreeTo234Tree()


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


<!--
### 5.3 BiTreeInsert()



```C


```

### 5.4 BiTreeDelete()

```C


```
### 5.5 BiTreeSelfBalance()

```C

```
-->

