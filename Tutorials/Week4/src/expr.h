#ifndef EXPR_H
#define EXPR_H
#include "lex.h"


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
    
    2. To keep it simple, we use the TokenKind defined in tokens.txt.  
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
  struct astExprNode *kids[2];  
};

typedef struct astExprNode *AstExprNodePtr;


AstExprNodePtr Expression(void);

long EvalExpression(AstExprNodePtr root);

void ReleaseAstExpr(AstExprNodePtr root);

#endif
