#ifndef STMT_H
#define STMT_H
#include "expr.h"
#include "lex.h"

/*
    AST node for different statements.
    We use the field op to distinguish different statements.  
 */
struct astStmtNode {
    // The kind of a statement.
    // To keep it simple, we reuse the TokenKind defined in tokens.txt.
    TokenKind op;
    // The value got from lex.c
    //      name of id,  integer values, ...
    Value value;
    /////////////////////////////
    /*
    1. Used in IfStatement and WhileStatement for labels
        See the comments for IfStatement() in stmt.c.  
    2. Other statements might use these fields for other purposes.
        see ExpressionStatement()
    */
    struct astExprNode *kids[2];
    /*
    1. See IfStatement() in stmt.c
          if (expr)
            thenStmt
          else
            elseStmt
          
          nextStmt
     2. Other statements might reuse part of these fields.
        For example, WhileStatement only use expr, thenStmt, and next.
    */
    struct astExprNode *expr;
    struct astStmtNode *thenStmt;
    struct astStmtNode *elseStmt;
    struct astStmtNode *next;
};

typedef struct astStmtNode *AstStmtNodePtr;

AstStmtNodePtr CompoundStatement(void);
AstStmtNodePtr GlobalDeclStatements(void);
AstStmtNodePtr Statement(void);
void EmitStatementNode(AstStmtNodePtr stmt);
#endif
