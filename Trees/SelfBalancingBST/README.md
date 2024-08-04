# Self-Balancing Binary Search Tree

``` sh
/*******************************************************************
                 Self-Balancing Binary Search Tree

    1.  How to do an in-order traversal in a binary search tree

    2.  How to insert data in a self-balancing binary search tree

    3.  How to delete data in a self-balancing binary search tree


                                             COMP9024 24T2

 *******************************************************************/
``` 

## An example of an unbalanced binary search tree

| Unbalanced BST ( degenerating into a structure that resembles a linked list) | 
|:-------------:|
| <img src="diagrams/UnbalancedBST45.png" width="80%" height="80%"> |


A Binary Search Tree (BST) is a type of data structure that organizes data efficiently.

Each node has at most two children, with values smaller than the node on the left and values larger on the right. 

However, in worst case, an unbalanced BST might degenerate into a structure that resembles a linked list, with O(n) time complexity in searching.

Self-balancing trees are introduced to resolve this issue.

In computer science, an AVL tree (named after inventors Adelson-Velsky and Landis) is a self-balancing binary search tree.

An AVL tree with n nodes maintains a logarithmic height O(log(n)), ensuring efficient operations such as insertion, deletion, and search.

| Self-Balancing Binary Search Tree (after inserting 50, 20, 10, 30, 40, 70, 60, 100, 90, and 80) |
|:-------------:|
| <img src="diagrams/AVLTree.png" width="80%" height="80%"> |

### Tree Height

We use BiTreeHeight(root) to represent the height of a BST tree.

In this project, its height is defined as follows.
we define the height of an empty tree (NULL node) to be 0.

For a non-empty tree, the height is the maximum of the heights of its left and right subtrees, plus one (to account for the current node).

```sh
If root is NULL
    its height is 0
else
    leftHeight = BiTreeHeight(root->leftChild)
    rightHeight = BiTreeHeight(root->rightChild)
    its height is Max(leftHeight, rightHeight) + 1
```
Note that its height can be also defined as -1 when root is NULL.

| A binary tree with 7 nodes (height = 3 = log($2^{3}$) $\approx$ log(7)) |
|:-------------:|
| <img src="diagrams/Nodes7.png" width="80%" height="80%"> |

| A binary tree with 15 nodes (height = 4 = log($2^{4}$) $\approx$ log(15)) |
|:-------------:|
| <img src="diagrams/Nodes15.png" width="80%" height="80%"> |

| A binary tree with 31 nodes (height = 5 = log($2^{5}$) $\approx$ log(31))|
|:-------------:|
| <img src="diagrams/Nodes31.png" width="100%" height="100%"> |

```C
int GetMax(int h1, int h2) {
    return (h1 > h2) ? h1: h2;
}

int BiTreeHeight(BiTreeNodePtr root) {
    if (root) {
        return root->height;
    } else {
        return 0;
    }
}

static void UpdateHeight(BiTreeNodePtr root) {
    if (root) {
        int max = GetMax(BiTreeHeight(root->leftChild), BiTreeHeight(root->rightChild));
        root->height = max + 1;
    }
}
``` 

### Balance Factor
Each node in an AVL tree has a balance factor, which is calculated as the difference between the heights of its left and right subtrees. The balance factor can be -1, 0, or 1.

The tree is balanced (i.e., no self-balancing is required) if every node’s balance factor is within the range {-1, 0, 1}.

In this project, the balance factor is defined as follows.

```C
int BiTreeBalanceFactor(BiTreeNodePtr root) {
    if (root) {        
        return BiTreeHeight(root->leftChild) - BiTreeHeight(root->rightChild);
    } else {
        return 0;
    }
}
```
In terms of our definition, 

a tree node is **left-heavy** when its balance factor is larger than 0  (bf > 0);

a tree node is **right-heavy** when its balance factor is smaller than 0 (bf < 0);

a tree node is **in-balance** when its balance factor is 0 (bf == 0);

a tree node is **unbalanced** (i.e., it requires self-balancing) when its balance factor is not in {-1, 0, 1} (that is, bf < -1 || bf > 1).

### Self Balancing

An AVL tree ensures that the balance factor of every node is within the specified range by performing rotations whenever necessary after insertion or deletion operations. These rotations restore the balance of the tree while maintaining the BST property.

We have discussed how to create a *.dot file in [COMP9024/Trees/Tree2Dot](../../Trees/Tree2Dot/README.md) for a binary tree.

In this project, we study how to insert and delete data in a self-balancing BST.




## 1 How to download this project in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/)

Open a terminal (Applications -> Terminal Emulator)

```sh

$ git clone https://github.com/sheisc/COMP9024.git

$ cd COMP9024/Trees/SelfBalancingBST

SelfBalancingBST$ 

```


## 2 How to start [Visual Studio Code](https://code.visualstudio.com/) to browse/edit/debug a project.


```sh

SelfBalancingBST$ code

```

Two configuration files (SelfBalancingBST/.vscode/[launch.json](https://code.visualstudio.com/docs/cpp/launch-json-reference) and SelfBalancingBST/.vscode/[tasks.json](https://code.visualstudio.com/docs/editor/tasks)) have been preset.



#### 2.1 Open the project in VS Code

In the window of Visual Studio Code, please click "File" and "Open Folder",

select the folder "COMP9024/Trees/SelfBalancingBST", then click the "Open" button.


#### 2.2 Build the project in VS Code

click **Terminal -> Run Build Task**


#### 2.3 Debug the project in VS Code

Open src/main.c, and click to add a breakpoint (say, line 45).

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

SelfBalancingBST$ make

SelfBalancingBST$ ./main

*******************************  Testing BiTreeInsert() *******************************


inserting 50
After inserting 50 
----------------  InOrderTraversal() ----------------
50 
-----------------------------------------------------

inserting 20
After inserting 20 
----------------  InOrderTraversal() ----------------
20 50 
-----------------------------------------------------

inserting 10
Left-Left Case: Right Rotate
After inserting 10 
----------------  InOrderTraversal() ----------------
10 20 50 
-----------------------------------------------------

inserting 30
After inserting 30 
----------------  InOrderTraversal() ----------------
10 20 30 50 
-----------------------------------------------------

inserting 40
Left-Right Case: Left Rotate + Right Rotate
After inserting 40 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 
-----------------------------------------------------

inserting 70
Right-Right Case: Left Rotate
After inserting 70 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 70 
-----------------------------------------------------

inserting 60
Right-Left Case: Right Rotate + Left Rotate
After inserting 60 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 60 70 
-----------------------------------------------------

inserting 100
After inserting 100 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 60 70 100 
-----------------------------------------------------

inserting 90
Right-Left Case: Right Rotate + Left Rotate
After inserting 90 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 60 70 90 100 
-----------------------------------------------------

inserting 80
Right-Left Case: Right Rotate + Left Rotate
After inserting 80 
----------------  InOrderTraversal() ----------------
10 20 30 40 50 60 70 80 90 100 
-----------------------------------------------------




*******************************  Testing BiTreeDelete() *******************************


deleting 50
After deleting 50 
----------------  InOrderTraversal() ----------------
10 20 30 40 60 70 80 90 100 
-----------------------------------------------------

deleting 20
After deleting 20 
----------------  InOrderTraversal() ----------------
10 30 40 60 70 80 90 100 
-----------------------------------------------------

deleting 10
Right-Right Case: Left Rotate
After deleting 10 
----------------  InOrderTraversal() ----------------
30 40 60 70 80 90 100 
-----------------------------------------------------

deleting 30
After deleting 30 
----------------  InOrderTraversal() ----------------
40 60 70 80 90 100 
-----------------------------------------------------

deleting 40
After deleting 40 
----------------  InOrderTraversal() ----------------
60 70 80 90 100 
-----------------------------------------------------

deleting 70
After deleting 70 
----------------  InOrderTraversal() ----------------
60 80 90 100 
-----------------------------------------------------

deleting 60
Right-Right Case: Left Rotate
After deleting 60 
----------------  InOrderTraversal() ----------------
80 90 100 
-----------------------------------------------------

deleting 100
After deleting 100 
----------------  InOrderTraversal() ----------------
80 90 
-----------------------------------------------------

deleting 90
After deleting 90 
----------------  InOrderTraversal() ----------------
80 
-----------------------------------------------------

deleting 80
After deleting 80 
----------------  InOrderTraversal() ----------------

-----------------------------------------------------


```

### 3.2 make view

**Ensure that you have executed 'make' and './main' before 'make view'.**


```sh
SelfBalancingBST$ make view
```

**Click on the window of 'feh' or use your mouse scroll wheel to view images**.

Here, **feh** is an image viewer available in [CSE VLAB](https://vlabgateway.cse.unsw.edu.au/).

### Four cases in self balancing


**(1) Insert 30, 20, and 10**


| Case | Rotation |  Before   | Intermediate | After   |
|:-------------:|:-------------:|:-------------:|:-------------:|:-------------:|
| Left-Left | Right Rotation | <img src="images/BiTreeBiTreeInsert_0072.png" width="80%" height="80%"> | |<img src="images/BiTreeBiTreeInsert_0073.png" width="80%" height="80%"> |

```C
/*
------------------------------------------------------------------------
  Before rotation                           After right rotation
------------------------------------------------------------------------                                         
      *pNodePtr                                *pNodePtr
          .                                        .
          .                                        .
          .                                        .
          V                                        V
        NodeC                                    NodeA      
       /                                         /    \
    NodeA                                     Node0   NodeC    
    /   \                                             /
 Node0   NodeB (or NULL)                            NodeB (or NULL)      
------------------------------------------------------------------------        
 */
void BiTreeRightRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeC = *pNodePtr;
    BiTreeNodePtr pNodeA = pNodeC->leftChild;
    BiTreeNodePtr pNodeB = pNodeA->rightChild;

    pNodeA->rightChild = pNodeC;
    pNodeC->leftChild = pNodeB;
    // NodeC's height should be updated before NodeA's height
    UpdateHeight(pNodeC);
    UpdateHeight(pNodeA);
    // Let *pNodePtr point to NodeA
    *pNodePtr = pNodeA;
}
```

**(2) Insert 10, 20 and 30**


| Case | Rotation |  Before   | Intermediate | After   |
|:-------------:|:-------------:|:-------------:|:-------------:|:-------------:|
| Right-Right | Left Rotation|<img src="images/BiTreeBiTreeInsert_0083.png" width="80%" height="80%">| | <img src="images/BiTreeBiTreeInsert_0084.png" width="80%" height="80%">|

```C
/*
-----------------------------------------------------------------------
  Before rotation                After left rotation
-----------------------------------------------------------------------                              
      *pNodePtr                     *pNodePtr
          .                             .
          .                             .
          .                             .
          V                             V
        NodeA                         NodeC      
            \                         /   \
           NodeC                  NodeA   Node0
           /   \                      \
       NodeB   Node0                  NodeB 
     (or NULL)                        (or NULL)  
-----------------------------------------------------------------------        
 */
void BiTreeLeftRotate(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNodeA = *pNodePtr;
    BiTreeNodePtr pNodeC = pNodeA->rightChild;
    BiTreeNodePtr pNodeB = pNodeC->leftChild;

    pNodeC->leftChild = pNodeA;
    pNodeA->rightChild = pNodeB;
    // NodeA's height should be updated before NodeC's height
    UpdateHeight(pNodeA);
    UpdateHeight(pNodeC);
    // Let *pNodePtr point to NodeC
    *pNodePtr = pNodeC;
}
```
**(3) Insert 30, 10 and 20**

| Case | Rotation |  Before   | Intermediate | After   |
|:-------------:|:-------------:|:-------------:|:-------------:|:-------------:|
| Left-Right | Left Rotation, Right Rotation |<img src="images/BiTreeBiTreeInsert_0094.png" width="100%" height="100%">|<img src="images/BiTreeBiTreeInsert_0095.png" width="100%" height="100%">|<img src="images/BiTreeBiTreeInsert_0096.png" width="100%" height="100%">|

**(4) Insert 10, 30 and 20**

| Case | Rotation |  Before   | Intermediate | After   |
|:-------------:|:-------------:|:-------------:|:-------------:|:-------------:|
| Right-Left | Right Rotation, Left Rotation |<img src="images/BiTreeBiTreeInsert_0106.png" width="100%" height="100%">|<img src="images/BiTreeBiTreeInsert_0107.png" width="100%" height="100%">|<img src="images/BiTreeBiTreeInsert_0108.png" width="100%" height="100%">|


#### 3.2.1 BiTreeInsert()

| Insert 50 | 
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0001.png" width="80%" height="80%"> |

| Insert 20 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0002.png" width="80%" height="80%"> |

| Insert 20 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0003.png" width="80%" height="80%">  |

| Insert 10 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0004.png" width="80%" height="80%"> |

| Insert 10 (Left-Left Case: Right Rotate)|
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0005.png" width="80%" height="80%"> |

| Insert 10 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0006.png" width="80%" height="80%"> |

| Insert 30 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0007.png" width="80%" height="80%"> |

| Insert 30 | 
|:-------------:|
|  <img src="images/BiTreeBiTreeInsert_0008.png" width="80%" height="80%"> |

| Insert 30 | 
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0009.png" width="80%" height="80%"> |

| Insert 40 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0010.png" width="80%" height="80%"> |

| Insert 40 (Left-Right Case: Left Rotate + Right Rotate) |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0011.png" width="80%" height="80%"> |

| Insert 40 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0012.png" width="80%" height="80%"> |

| Insert 40 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0013.png" width="80%" height="80%"> |

| Insert 40 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0014.png" width="80%" height="80%"> |

| Insert 70 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0015.png" width="80%" height="80%"> |

| Insert 70  |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0016.png" width="80%" height="80%"> |


| Insert 70 (Right-Right Case: Left Rotate)|
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0017.png" width="80%" height="80%"> |


| Insert 70 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0018.png" width="80%" height="80%"> |


| Insert 60 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0019.png" width="80%" height="80%"> |

| Insert 60 (Right-Left Case: Right Rotate + Left Rotate) |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0020.png" width="80%" height="80%"> |


| Insert 60 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0021.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0022.png" width="80%" height="80%"> |

| Insert 60 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0023.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0024.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0025.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0026.png" width="80%" height="80%"> |

| Insert 100 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0027.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0028.png" width="80%" height="80%"> |

| Insert 90 (Right-Left Case: Right Rotate + Left Rotate) |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0029.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0030.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0031.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0032.png" width="80%" height="80%"> |

| Insert 90 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0033.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0034.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0035.png" width="80%" height="80%"> |

| Insert 80 (Right-Left Case: Right Rotate + Left Rotate)|
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0036.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0037.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0038.png" width="80%" height="80%"> |

| Insert 80 |
|:-------------:|
| <img src="images/BiTreeBiTreeInsert_0039.png" width="80%" height="80%"> |

#### 3.2.2 BiTreeDelete()


| Delete 50 |
|:-------------:|
| <img src="images/BiTreeDelete_0039.png" width="80%" height="80%"> |

| Delete 50 |
|:-------------:|
| <img src="images/BiTreeDelete_0040.png" width="80%" height="80%"> |

| Delete 50 |
|:-------------:|
| <img src="images/BiTreeDelete_0041.png" width="80%" height="80%"> |


| Delete 50 |
|:-------------:|
| <img src="images/BiTreeDelete_0042.png" width="80%" height="80%"> |

| Delete 50 |
|:-------------:|
| <img src="images/BiTreeDelete_0043.png" width="80%" height="80%"> |

| Delete 20 (Swap 20 and 30) |
|:-------------:|
| <img src="images/BiTreeDelete_0044.png" width="80%" height="80%"> |

| Delete 20 |
|:-------------:|
| <img src="images/BiTreeDelete_0045.png" width="80%" height="80%"> |


| Delete 20 |
|:-------------:|
| <img src="images/BiTreeDelete_0046.png" width="80%" height="80%"> |

| Delete 20 |
|:-------------:|
| <img src="images/BiTreeDelete_0047.png" width="80%" height="80%"> |


| Delete 10 |
|:-------------:|
| <img src="images/BiTreeDelete_0048.png" width="80%" height="80%"> |


| Delete 10 (Right-Right Case: Left Rotate)|
|:-------------:|
| <img src="images/BiTreeDelete_0049.png" width="80%" height="80%"> |

| Delete 10 |
|:-------------:|
| <img src="images/BiTreeDelete_0050.png" width="80%" height="80%"> |

| Delete 30 |
|:-------------:|
| <img src="images/BiTreeDelete_0051.png" width="80%" height="80%"> |


| Delete 30 |
|:-------------:|
| <img src="images/BiTreeDelete_0052.png" width="80%" height="80%"> |

| Delete 30 |
|:-------------:|
| <img src="images/BiTreeDelete_0053.png" width="80%" height="80%"> |


| Delete 40 |
|:-------------:|
| <img src="images/BiTreeDelete_0054.png" width="80%" height="80%"> |


| Delete 70 (Swap 70 and 80)|
|:-------------:|
| <img src="images/BiTreeDelete_0057.png" width="80%" height="80%"> |


| Delete 70 |
|:-------------:|
| <img src="images/BiTreeDelete_0058.png" width="80%" height="80%"> |

| Delete 70 |
|:-------------:|
| <img src="images/BiTreeDelete_0059.png" width="80%" height="80%"> |

| Delete 70 |
|:-------------:|
| <img src="images/BiTreeDelete_0060.png" width="80%" height="80%"> |

| Delete 60 (Right-Right Case: Left Rotate) |
|:-------------:|
| <img src="images/BiTreeDelete_0061.png" width="80%" height="80%"> |

| Delete 60 |
|:-------------:|
| <img src="images/BiTreeDelete_0062.png" width="80%" height="80%"> |


| Delete 100 |
|:-------------:|
| <img src="images/BiTreeDelete_0063.png" width="80%" height="80%"> |


| Delete 100 |
|:-------------:|
| <img src="images/BiTreeDelete_0064.png" width="80%" height="80%"> |


| Delete 90 |
|:-------------:|
| <img src="images/BiTreeDelete_0065.png" width="80%" height="80%"> |

| Delete 80 |
|:-------------:|
| <img src="images/BiTreeDelete_0067.png" width="80%" height="80%"> |



## 4 Data structures

```C

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

    // ...
};

typedef struct BiTreeNode *BiTreeNodePtr;
```


## 5 Algorithms

### 5.1 main()

```C

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

```

### 5.2 BiTreeInsert()

```C


void BiTreeInsert(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, char *nodeName, long *pCnt) {  
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pCnt);
    char *graphName = "BiTreeBiTreeInsert";
    char *fileName = "images/BiTreeBiTreeInsert";
    if (pNode == NULL) {
        BiTreeNodePtr tmp = CreateBinaryTreeNode(numVal, nodeName, NULL, NULL);
        *pNodePtr = tmp;
        printf("inserting %ld\n", numVal);
        return;
    } else {
        // on stack
        pNode->visited = 1;
        if (numVal < pNode->value.numVal) {
            BiTreeInsert(pRoot, &pNode->leftChild, numVal, nodeName, pCnt);
        } else if (numVal > pNode->value.numVal) {
            BiTreeInsert(pRoot, &pNode->rightChild, numVal, nodeName, pCnt);
        } else {
            // If numVal is already in the binary search tree, do nothing.
            // off stack
            pNode->visited = 0;
            return;
        }

        // recalculate and store its height
        pNode->height = 1 + GetMax(BiTreeHeight(pNode->leftChild), BiTreeHeight(pNode->rightChild));

        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);        

        BiTreeSelfBalance(pRoot, pNodePtr, pCnt, graphName, fileName);
        // off stack
        pNode->visited = 0;   
    }
}
```

### 5.3 BiTreeDelete()

```C
BiTreeNodePtr BiTreeMinValueNode(BiTreeNodePtr root) {
    BiTreeNodePtr cur = root;
    // Get the left-most node
    while ((cur != NULL) && (cur->leftChild != NULL)) {
        cur = cur->leftChild;
    }
    return cur;
}

// The parameter pRoot is only used for generating the image of the binary search tree.
// In this recursive function, *pNodePtr might point to a sub-tree in the BST.
void BiTreeDelete(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long numVal, long *pCnt) {
    //static long cnt = 0;
    assert(pCnt);
    char *graphName = "BiTreeDelete";
    char *fileName = "images/BiTreeDelete";
    BiTreeNodePtr pNode = *pNodePtr;
    if (pNode) {
        // It is on stack
        pNode->visited = 1;
        if (numVal < pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->leftChild), numVal, pCnt);
        } else if (numVal > pNode->value.numVal) {
            BiTreeDelete(pRoot, &(pNode->rightChild), numVal, pCnt);
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
            
            if (pNode->leftChild == NULL) {   // case 00 and case 01
                BiTreeNodePtr tmp = pNode->rightChild;
                printf("deleting %ld\n", pNode->value.numVal);
                free(pNode);
                *pNodePtr = tmp;                
            } else if (pNode->rightChild == NULL) { // case 10
                BiTreeNodePtr tmp = pNode->leftChild;
                printf("deleting %ld\n", pNode->value.numVal);      
                free(pNode);
                *pNodePtr = tmp;              
            } else {
                // case 11:  with two children
                // Get pNode's in-order successor, which is left-most node in its right sub-tree.
                BiTreeNodePtr pSuccessor = BiTreeMinValueNode(pNode->rightChild);

                // (Swapping is done for clearer debugging output)
                // Swap the values of the node pointed to by pNode and its in-order successor              
                NodeValue val = pNode->value;
                // Copy the successor's value (this copy is necessary)
                pNode->value = pSuccessor->value;
                pSuccessor->value = val;

                // Display the inconsistent state
                (*pCnt)++;
                GenOneImage(*pRoot, graphName, fileName, *pCnt);
                
                // Now, numVal is in right sub-tree. Let us recursively delete it.
                // Temporarily, the whole binary search tree is at an inconsistent state.
                // It will become consistent when the deletion is really done.
                BiTreeDelete(pRoot, &pNode->rightChild, pSuccessor->value.numVal, pCnt);
            }
        }
        // 
        pNode = *pNodePtr;
        // If it is NULL, just return.
        if (pNode == NULL) {            
            return;
        }

        // recalculate and store its height
        pNode->height = 1 + GetMax(BiTreeHeight(pNode->leftChild), BiTreeHeight(pNode->rightChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);        

        BiTreeSelfBalance(pRoot, pNodePtr, pCnt, graphName, fileName);
        // It is not on stack
        pNode->visited = 0;    
    }
}

```
### 5.4 BiTreeSelfBalance()

```C

/*
    Rotate the subtree pointed to by *pNodePtr when necessary.

    pRoot and pCnt are used in generating images for visualizing the algorithms.

    We define:

    The balance factor of a tree node:

        BalanceFactor(pNode) = Height(pNode->left) - Height(pNode->right)

    The height of an empty tree (pNode == NULL) is 0.
    BTW, some people define the height of the node (pointed to by pNode) to be -1 when pNode is NULL.

    A tree node is left-heavy when its balance factor is larger than 0  (bf > 0).
    A tree node is right-heavy when its balance factor is smaller than 0 (bf < 0).
 */

static void BiTreeSelfBalance(BiTreeNodePtr *pRoot, BiTreeNodePtr *pNodePtr, long *pCnt, char *graphName, char *fileName) {
//static void BiTreeSelfBalance(BiTreeNodePtr *pNodePtr) {
    BiTreeNodePtr pNode = *pNodePtr;
    assert(pNode);

    // calculate its balance factor
    int bFactor = BiTreeBalanceFactor(pNode);

    if (bFactor > 1 && BiTreeBalanceFactor(pNode->leftChild) >= 0) {
        /*
            Left-Left Case:  the unbalanced node is left-heavy, and its left child is left-heavy or in-balance

                             Suppose NodeC has a right child, NodeD.
                             When NodeD is deleted from the AVL Tree, NodeC becomes unbalanced.
                             But NodeC's left child (NodeA) can be in-balance when NodeB exists.

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeC  (unbalanced node)
                           /    
                        NodeA  
                        /   \
                    Node0   NodeB (or NULL)  
         */
        printf("Left-Left Case: Right Rotate\n");           
        BiTreeRightRotate(pNodePtr);
    }
    else if (bFactor < -1 && BiTreeBalanceFactor(pNode->rightChild) <= 0) {
        /*
            Right-Right Case: the unbalanced node is right-heavy, and its right child is right-heavy or in-balance

                    *pNodePtr
                        .
                        .
                        .
                        V
                      NodeA  (unbalanced node)  
                         \
                        NodeC         
                        /   \
                    NodeB   Node0
                  (or NULL)        
         */
        printf("Right-Right Case: Left Rotate\n");  
        BiTreeLeftRotate(pNodePtr);
    }
    else if (bFactor > 1 && BiTreeBalanceFactor(pNode->leftChild) < 0) {
        /*
            Left-Right Case:  the unbalanced node is left-heavy, and its left child is right-heavy

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeC   (unbalanced) 
                           /    
                        NodeA     
                           \
                          NodeB 

         */
        printf("Left-Right Case: Left Rotate + Right Rotate\n");  
        BiTreeLeftRotate(&(pNode->leftChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);   
        BiTreeRightRotate(pNodePtr);
    }
    else if (bFactor < -1 && BiTreeBalanceFactor(pNode->rightChild) > 0) {
        /*
            Right-Left Case:  the unbalanced node is right-heavy, and its right child is left-heavy

                        *pNodePtr
                            .
                            .
                            .
                            V
                          NodeA  (unbalanced)   
                             \
                            NodeC   
                            /   
                        NodeB                                     
         */
        printf("Right-Left Case: Right Rotate + Left Rotate\n");  
        BiTreeRightRotate(&(pNode->rightChild));
        (*pCnt)++;
        GenOneImage(*pRoot, graphName, fileName, *pCnt);          
        BiTreeLeftRotate(pNodePtr);
    }     
}
```