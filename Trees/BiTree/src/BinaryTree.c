#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "BinaryTree.h"

#define EmitIR  printf

// static area
static int tmpVarNum = 0; 


BiTreePtr CreateBiTree(BiTreeNodeKind kind, 
                       BiTreeNodeValue val,
                       BiTreePtr left,
                       BiTreePtr right) {
    // Allocate a BiTree object in the heap (no name)
    BiTreePtr root = (BiTreePtr) malloc(sizeof(struct BiTree));

    assert(root != NULL);
    // zero out the heap object    
    memset(root, 0, sizeof(*root));
    // Initialize different fields.
    root->kind = kind;
    root->val = val;
    root->left = left;
    root->right = right;
    return root;    
}

void ReleaseBiTree(BiTreePtr root) {
    if (root) {
        ReleaseBiTree(root->left);
        ReleaseBiTree(root->right);
        free(root);
    }
}
/*
    An intermediate representation (IR) is the data structure or code 
    used internally by a compiler or virtual machine to represent source code. 
    Here, we simply output the IR instructions.
 */
static void GenerateIR(BiTreePtr root) {     
    if (root == NULL)
        return;
    
    if (root->kind == BT_OPERAND) {
        snprintf(root->name, MAX_ID_LEN, "%d", root->val);        
    }
    else if (root->kind == BT_OPERATOR) {     
        GenerateIR(root->left);
        GenerateIR(root->right);
        // Postorder traversal
        tmpVarNum++;
        // Use a temporary variable to save the result
        snprintf(root->name, MAX_ID_LEN, "t%d", tmpVarNum);        
        EmitIR("\t %s = %s %c %s\n", root->name, root->left->name, root->val, root->right->name);        
    }    
}


// In fact, it is an interpreter
static BiTreeNodeValue EvalExpression(BiTreePtr root) {    
    assert(root);
    if (root->kind == BT_OPERAND) {        
        return root->val;
    }
    else if (root->kind == BT_OPERATOR) {
        //
        assert(root->left);
        assert(root->right);        
        BiTreeNodeValue leftOperand = EvalExpression(root->left);
        BiTreeNodeValue rightOperand = EvalExpression(root->right); 
        // Postorder traversal
        BiTreeNodeValue result = 0;
        switch (root->val) {
            case '+':
                result = leftOperand + rightOperand;
                break;
            case '-':
                result = leftOperand - rightOperand;
                break;
            case '*':
                result = leftOperand * rightOperand;
                break;
            case '/':
                result = leftOperand / rightOperand;
                break;
            default:
                break;              
        }
        return result;
    }
}

int TestBinaryTree(void) {

    printf("\n\n*************** COMP9024 TestBinaryTree() ***************\n\n\n");
    ///////////////////////////////////////////////////////////////////
    //
    //  An expression (i.e., a string) from a user:
    //       
    //      "9000 + 6 * 4" 
    //
    //  Its binary-tree representation:
    //
    //        +
    //      /   \
    //    9000   *
    //          /  \
    //         6    4
    // 
    //
    // Q1. How to create a binary tree?
    // Q2. How to visit a binary tree?
    //
    ///////////////////////////////////////////////////////////////////
    char *srcCode = "9000 + 6 * 4";
    /*
        Q1. 

            Now, let's manually create the binary tree. 
            
            We can see why pointers (addresses) are so important.
            A tree node (oneNode) can save/keep/store another node's address,
            thus establishing a connection from oneNode to anotherNode.
            
            =====================================================================
                    Pointers are just like hyper links in web pages.
            =====================================================================

                                via a pointer           
            A tree node   ------------------------------->  another tree node 
            
            oneNode.left = address of anotherNode


                                via a hyperlink
            one.html      ------------------------------->  two.html
            
            <a href="two.html">Neo</a>

            ======================================================================

            We will write a parser to create the tree for us later.       
     */
    BiTreePtr left = CreateBiTree(BT_OPERAND, 9000, NULL, NULL);
    BiTreePtr rightLeft = CreateBiTree(BT_OPERAND, 6, NULL, NULL);
    BiTreePtr rightRight = CreateBiTree(BT_OPERAND, 4, NULL, NULL);   
    BiTreePtr right = CreateBiTree(BT_OPERATOR, '*', rightLeft, rightRight);
    BiTreePtr root = CreateBiTree(BT_OPERATOR, '+', left, right);

    // Q2.
    BiTreeNodeValue result = EvalExpression(root);
    printf("An expression:\n\n");
    printf("   \"%s\" \n\n", srcCode);
    printf("An interpreter based on a binary tree:\n\n");
    char * strTree = "\n"
                    "        + \n"
                    "      /   \\ \n"
                    "    9000   * \n"
                    "          /  \\ \n"
                    "         6    4 \n";
    printf("%s", strTree);
    printf("\n\n   eval(\"%s\") = %d\n\n\n", srcCode, result);

    // Q2.
    printf("Its intermediate representation (IR): \n\n");
    GenerateIR(root);

    // Do you see the tricky part of malloc() and free()?    
    // In larger C/C++ programs, use-after-free and double-free bugs are common.
    ReleaseBiTree(root);
    printf("\n");
    return 0;
}

