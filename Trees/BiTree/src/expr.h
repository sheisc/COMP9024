#ifndef EXPR_H
#define EXPR_H

// Max length of an identifier (e.g., a function or variable name) 
#define MAX_ID_LEN 127

// token value
typedef struct {
    // e.g, "year", "t0", "t1", "+", "-", "*", "/", "(", ")", ...
    char name[MAX_ID_LEN + 1];
    // value of an integer, e.g., 2024
    long numVal;
} Value;


// In C, an Enum/Enumeration is a custom data type where users can assign names to constant integers. 
// Using enums makes it easier for programmers to learn, understand, and maintain the program.
typedef enum {
    TK_NA,     // 0     By default, the first item has the value 0. Here, NA stands for Not Available.
    TK_ADD,    // 1     '+'
    TK_SUB,    // 2     '-'
    TK_MUL,    // 3     '*'
    TK_DIV,    // 4     '/'
    TK_NUM,    // 5     number 
    TK_LPAREN, // 6     '('  left parenthesis
    TK_RPAREN, // 7     ')'  right parenthesis
    TK_EOF,    // 8     end of file
} TokenKind;



/*
    The Abstract Syntax Tree Node for an expression.

    In an abstract syntax tree, syntactic details such as parentheses in expressions 
    like "(20 + 30) * 40" are considered redundant and thus ignored.

             * 
           /   \
          +     40
        /   \ 
       20   30
 */
struct astExprNode {
    /*
        1. The kind of an expression node: 
             an operand (e.g., 300) or an operator (e.g., '+', '-', '*' and '/')

        2. To keep it simple, we use the TokenKind as mentioned above  
             TK_NUM for 300, 400, ...
             TK_ADD for '+'
             TK_SUB for '-'
             TK_MUL for '*'
             TK_DIV for '/'
     */
    TokenKind op;
    /*
        The value of the token (a token is a word), consisting of two parts:

        1. an integer for representing the node's value (e.g., 300), 

        2. a C string for representing its name or value (e.g., "+", "t0", "t1", "(", ")", "300", ...)
     */
    Value value;

    /////////////////////////////////////////////
    // e.g.,  left and right operands of a binary operator (+, -, *, /)
    struct astExprNode *leftChild;
    struct astExprNode *rightChild; 
};

typedef struct astExprNode *AstExprNodePtr;


AstExprNodePtr Expression(void);

long EvalExpression(AstExprNodePtr root);

void ReleaseAstExpr(AstExprNodePtr root);

#endif
