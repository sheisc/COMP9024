#include "stmt.h"
#include "decl.h"
#include "emit.h"
#include "error.h"
#include "func.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IS_PREFIX_OF_DECL(tk) (tk == TK_INT)

// #ifdef _WIN32
// #define snprintf _snprintf
// #endif

// int snprintf(char *str, size_t size, const char *format, ...);

static AstStmtNodePtr ExpressionStatement(void);
static AstStmtNodePtr IfStatement(void);
static AstStmtNodePtr WhileStatement(void);

static int isPrefixOfStatement(TokenKind tk);
static int NewLabel(void);
/////////////////////////////////////////////////////////////////////
// The first token of a statement
static TokenKind prefixOfStmt[] = {TK_ID,  TK_IF,    TK_WHILE,  TK_LBRACE,
                                   TK_INT, TK_INPUT, TK_OUTPUT, TK_RETURN, TK_DO};

///////////////////////////////////////////////////////////////////////////////
static int NewLabel(void) {
  static int labelNo;
  return labelNo++;
}

static AstExprNodePtr CreateLabelNode(void) {
  Value value;
  memset(&value, 0, sizeof(Value));
  snprintf(value.name, MAX_ID_LEN, "Label_%d", NewLabel());
  return CreateAstExprNode(TK_LABEL, &value, NULL, NULL);
}

AstStmtNodePtr CreateStmtNode(TokenKind op) {
  AstStmtNodePtr pNode = (AstStmtNodePtr) malloc(sizeof(struct astStmtNode));
  assert(pNode != NULL);
  memset(pNode, 0, sizeof(*pNode));
  pNode->op = op;
  return pNode;
}

// Test whether the token tk can be the first token of a statement in scc.
static int isPrefixOfStatement(TokenKind tk) {
  int i = 0;
  for (i = 0; i < sizeof(prefixOfStmt) / sizeof(prefixOfStmt[0]); i++) {
    if (tk == prefixOfStmt[i]) {
      return 1;
    }
  }
  return 0;
}

/*
  GlobalDeclStatement:
      Declaration ;
 */
static AstStmtNodePtr GlobalDeclStatement(void) {
  if (IS_PREFIX_OF_DECL(curToken.kind)) {
    // declaration ;
    AstStmtNodePtr decl = CreateStmtNode(TK_GLOBAL_DECL_STMT);
    decl->expr = Declaration();
    Expect(TK_SEMICOLON);
    return decl;
  } 
  else {
    Error(__FILE__,
          __LINE__,
          "'long' expected");
    return NULL;
  }
}

/*
  GlobalDeclStatements:
      GlobalDeclStatement  GlobalDeclStatement  ...  GlobalDeclStatement
 */
AstStmtNodePtr GlobalDeclStatements(void) {
  AstStmtNodePtr decls;
  AstStmtNodePtr *pStmt; 

  decls = CreateStmtNode(TK_GLOBAL_DECL_STMTS);
  pStmt = &(decls->next);
  while (IS_PREFIX_OF_DECL(curToken.kind)) {
    *pStmt = GlobalDeclStatement();
    pStmt = &((*pStmt)->next);
  }
  return decls;
}

// Save the local declaration in current function definition (i.e., GetCurFuncDef())
static void HandleOneLocalDecl(AstExprNodePtr pNode) {
  if (pNode && pNode->op == TK_ID) {
    /**********************************************************
      FIXME:
        need to check whether there are redefinitions of local variables
     **********************************************************/
    AstFuncDefNodePtr func = GetCurFuncDef();
    int cnt = func->local_vars_cnt;
    memset(&func->local_vars[cnt], 0, sizeof(func->local_vars[cnt]));
    func->local_vars[cnt].op = TK_ID;
    func->local_vars[cnt].value = pNode->value; // curToken.value;
    // minus offset for local variables
    func->local_vars[cnt].offset = AllocFrameOffset();
    func->local_vars_cnt++;
  }
}

/*
  ExpressionStatement:
        ID = Expression;
        ID (Expression, Expression, ..., Expression);
        Declaration;
  */
static AstStmtNodePtr ExpressionStatement(void) {
  if (curToken.kind == TK_ID) {
    AstStmtNodePtr res_stmt = NULL;
    Token savedToken = curToken;
    NEXT_TOKEN;
    if (curToken.kind == TK_ASSIGN) {
      //	id = expression;
      AstStmtNodePtr assign = CreateStmtNode(TK_ASSIGN);
      assign->kids[0] = CreateAstExprNode(TK_ID, &savedToken.value, NULL, NULL);
      NEXT_TOKEN;
      assign->expr = Expression();
      res_stmt = assign;
    } 
    else if (curToken.kind == TK_LPAREN) {
      // FUNCTION CALL
      // id (expr, expr, ...)
      AstStmtNodePtr call = CreateStmtNode(TK_CALL);
      call->expr = FunctionCallExpression(savedToken);
      res_stmt = call;
    } 
    else {
      Error(__FILE__,
            __LINE__,
            "'=' expected");      
    }
    Expect(TK_SEMICOLON);
    return res_stmt;
  }
#if 1                                          // 	allow local variables.
  else if (IS_PREFIX_OF_DECL(curToken.kind)) { // long var;
    // declaration ;
    AstStmtNodePtr decl = CreateStmtNode(TK_LOCAL_DECL_STMT);
    decl->expr = Declaration();
    Expect(TK_SEMICOLON);
    HandleOneLocalDecl(decl->expr);
    return decl;
  }
#endif
  else {
    Error(__FILE__,
          __LINE__,
          "'id' or 'long' expected");
    return NULL;
  }
}

/**************************************************
 IfStatement:
    if (Expression) Statement
    if (Expression) Statement else Statement

 Semantics:
 
    (1)  if(Expression) Statement_1 else Statement_2

            if(!Expression) 
                goto label_false

            Statement_1
            goto label_next

    label_false:  //  this label is saved in IfStatement.kids[0]
            Statement_2

    label_next:   //  this label is saved in IfStatement.kids[1]
            ...

    (2)  if(Expression) Statement
    
            if(!Expression)	
                goto label_next
            Statement
    label_next:   // this label is saved in IfStatement.kids[0]
            ...
 **************************************************/
static AstStmtNodePtr IfStatement(void) {
  AstStmtNodePtr ifStmt = NULL;

  ifStmt = CreateStmtNode(TK_IF);
  Expect(TK_IF);
  Expect(TK_LPAREN);
  ifStmt->expr = Expression();
  Expect(TK_RPAREN);

  ifStmt->thenStmt = Statement();
  ifStmt->kids[0] = CreateLabelNode();
  if (curToken.kind == TK_ELSE) {
    NEXT_TOKEN;
    ifStmt->elseStmt = Statement();
    // label for the statement after if-statement
    ifStmt->kids[1] = CreateLabelNode();
  }

  return ifStmt;
}

/**********************************************
  WhileStatement:
      while (Expression) Statement

  Semantics:

        label_begin:         // saved in WhileStatement.kids[0]

              if(!Expression)  
                  goto	label_next
              
              Statement
              
              goto label_begin

        label_next:         //  saved in WhileStatement.kids[1]
              ...
 **********************************************/
static AstStmtNodePtr WhileStatement(void) {
  AstStmtNodePtr whileStmt = NULL;
  whileStmt = CreateStmtNode(TK_WHILE);

  whileStmt->kids[0] = CreateLabelNode();
  Expect(TK_WHILE);
  Expect(TK_LPAREN);
  whileStmt->expr = Expression();
  Expect(TK_RPAREN);
  whileStmt->thenStmt = Statement();
  // label for the statement after while
  whileStmt->kids[1] = CreateLabelNode();
  return whileStmt;
}

/**********************************************
   DoWhileStatement:
         do Statement while (Expression);

   Semantics:   
        label_begin:     // saved in DoWhileStatement.kids[0]

            Statement

            if(Expression)
                goto label_begin

        label_next:     //  saved in DoWhileStatement.kids[1]
            ....
 **********************************************/
static AstStmtNodePtr DoWhileStatement(void) {
  AstStmtNodePtr doWhileStmt = NULL;

  /*
    Create a sub-tree for the do-while statement.
    It is similar with the code in WhileStatement().

    Q11. call CreateStmtNode(TK_DO) to create an AST node and save the return value in doWhileStmt
    Q12. call Expect() to match TK_DO in the input file
    Q13. call CreateLabelNode() to create an AST node for label_begin, and save the return value in doWhileStmt->kids[0]
    Q14. call Statement() to create an AST node for the Statement, and save the return value in doWhileStmt->thenStmt
    Q15. call Expect() to match TK_WHILE in the input file
    Q16. call Expect() to match TK_LPAREN in the input file
    Q17. call Expression() to create an AST node for the Expression in do-while, and save the return value in doWhileStmt->expr.
    Q18. call Expect() to match TK_RPAREN in the input file
    Q19. call Expect() to match TK_SEMICOLON in the input file
    Q20. call CreateLabelNode() to create an AST node for label_next, and save the return value in doWhileStmt->kids[1]     
   */
  ////////////////////////////////////////////////////////////////////////////////////
  //
  //    Please read the above comments first and then complete the following code (Q11-Q20).
  //
  ////////////////////////////////////////////////////////////////////////////////////

  // Q11. ___________________
  // Q12. ___________________
  // Q13. ___________________
  // Q14. ___________________
  // Q15. ___________________
  // Q16. ___________________
  // Q17. ___________________
  // Q18. ___________________
  // Q19. ___________________
  // Q20. ___________________  

  return doWhileStmt;
}

/*
  CompoundStatement:
      {Statement  Statement   ...  Statement}

  Semantics:
      The list of statements is saved in CompoundStatement.next.
 */
AstStmtNodePtr CompoundStatement(void) {
  AstStmtNodePtr comStmt;
  AstStmtNodePtr *pStmt;
  Value value;

  comStmt = CreateStmtNode(TK_COMPOUND);
  pStmt = &(comStmt->next);

  Expect(TK_LBRACE);
  while (isPrefixOfStatement(curToken.kind)) {
    *pStmt = Statement();
    pStmt = &((*pStmt)->next);
  }
  Expect(TK_RBRACE);
  return comStmt;
}

/*
    IOStatement:

        output(ID);
        input(ID);
  */
AstStmtNodePtr IOStatement(TokenKind tk) {
  AstStmtNodePtr ioStmt;
  ioStmt = CreateStmtNode(tk);
  Expect(tk);
  Expect(TK_LPAREN);
  if (curToken.kind == TK_ID) {
    ioStmt->expr = CreateAstExprNode(curToken.kind, &curToken.value, NULL, NULL);
    NEXT_TOKEN;
  } 
  else {
    Error(__FILE__,
          __LINE__,
          "'id' expected");
  }
  Expect(TK_RPAREN);
  Expect(TK_SEMICOLON);
  return ioStmt;
}

/* 
  ReturnStatement:
      return Expresion;
 */
AstStmtNodePtr ReturnStatement() {
  AstStmtNodePtr returnStmt = NULL;
  returnStmt = CreateStmtNode(TK_RETURN);
  Expect(TK_RETURN);
  returnStmt->expr = Expression();
  // FIXME: allocate a temporary for the return value
  // returnStmt->expr->offset = AllocFrameOffset();
  Expect(TK_SEMICOLON);
  return returnStmt;
}

/*
    Statement:
          IfStatement
          WhileStatement
          DoWhileStatement
          CompoundStatement
          ExpressionStatement
          IOStatement
          ReturnStatement
 */
AstStmtNodePtr Statement(void) {
  switch (curToken.kind) {
  case TK_IF:
    return IfStatement();
  case TK_WHILE:
    return WhileStatement();
  case TK_DO:
    return DoWhileStatement();    
  case TK_LBRACE:
    return CompoundStatement();
  case TK_ID:
  case TK_INT:
    return ExpressionStatement();
  case TK_INPUT:
  case TK_OUTPUT:
    return IOStatement(curToken.kind);
  case TK_RETURN:
    return ReturnStatement();
  }

  assert(0);
  return NULL;
}

/*
    Generate assembly code for a statement
 */
void EmitStatementNode(AstStmtNodePtr stmt) {
  if (!stmt) {
    return;
  }
  switch (stmt->op) {
  case TK_IF:
    /*****************************************************
            Refer to comments for IfStatement(void)
    ******************************************************/
    if (stmt->kids[0] && stmt->kids[1]) {

      EmitAstExprNode(stmt->expr);
      EmitComments("\n");
      EmitIR("# start of if(%s) {} else {}",
                   GetNodeNameInIR(stmt->expr));
      EmitIR("# if(!%s) goto %s", 
                    GetNodeNameInIR(stmt->expr),
                    stmt->kids[0]->value.name);
      EmitAssembly("cmpq $0, %s", GetNodeNameInAssembly(stmt->expr));
      EmitAssembly("je %s", stmt->kids[0]->value.name);
      EmitStatementNode(stmt->thenStmt);
      EmitIR("# goto %s", stmt->kids[1]->value.name); 
      EmitAssembly("jmp %s", stmt->kids[1]->value.name);
      EmitComments("\n");
      EmitIR("# else");
      EmitLabel("%s:", stmt->kids[0]->value.name);
      EmitStatementNode(stmt->elseStmt); 
      EmitIR("# end of if(%s) {} else {}",
                   GetNodeNameInIR(stmt->expr));   
      EmitLabel("%s:", stmt->kids[1]->value.name);
    } 
    else {
      EmitComments("\n");
      EmitIR("# start of if(%s) {}",
                   GetNodeNameInIR(stmt->expr));
      EmitIR("# if(!%s) goto %s", 
                    GetNodeNameInIR(stmt->expr),
                    stmt->kids[0]->value.name);      
      EmitAstExprNode(stmt->expr);
      EmitAssembly("cmpq $0, %s", GetNodeNameInAssembly(stmt->expr));
      EmitAssembly("je %s", stmt->kids[0]->value.name);
      EmitStatementNode(stmt->thenStmt);
      EmitIR("# end of if(%s) {}",
                   GetNodeNameInIR(stmt->expr));
      EmitLabel("%s:", stmt->kids[0]->value.name);
    }
    break;
  case TK_DO:
    /*****************************************************
            Refer to comments for DoWhileStatement(void)
    ******************************************************/
    EmitComments("\n");
    EmitIR("# start of do {} while (%s);", GetNodeNameInIR(stmt->expr));
    EmitLabel("%s:", stmt->kids[0]->value.name);
    EmitStatementNode(stmt->thenStmt);
    EmitAstExprNode(stmt->expr);
    EmitComments("\n");
    EmitIR("# if(%s) goto %s", 
            GetNodeNameInIR(stmt->expr),
            stmt->kids[0]->value.name);    
    EmitAssembly("cmpq $0, %s", GetNodeNameInAssembly(stmt->expr));
    EmitAssembly("jne %s", stmt->kids[0]->value.name); 
    EmitComments("\n");
    EmitIR("# end of do {} while(%s)",
                 GetNodeNameInIR(stmt->expr));   
    EmitLabel("%s:", stmt->kids[1]->value.name);
    break;    
  case TK_WHILE:
    /*****************************************************
            Refer to comments for WhileStatement(void)
    ******************************************************/
    EmitComments("\n");
    EmitIR("# start of while (%s) {}", GetNodeNameInIR(stmt->expr));     
    EmitLabel("%s:", stmt->kids[0]->value.name);
    EmitAstExprNode(stmt->expr);
    EmitIR("# if(!%s) goto %s", 
                 GetNodeNameInIR(stmt->expr),
                 stmt->kids[1]->value.name);      
    EmitAssembly("cmpq $0, %s", GetNodeNameInAssembly(stmt->expr));
    EmitAssembly("je %s", stmt->kids[1]->value.name);
    EmitComments("\n");
    EmitStatementNode(stmt->thenStmt);
    EmitComments("\n");
    EmitIR("# goto %s", stmt->kids[0]->value.name);     
    EmitAssembly("jmp %s", stmt->kids[0]->value.name);
    EmitComments("\n");
    EmitIR("# end of while(%s) do {}",
                 GetNodeNameInIR(stmt->expr));  
    EmitLabel("%s:", stmt->kids[1]->value.name);

    break;
  case TK_COMPOUND:
    /*****************************************************
            {Statement Statement ... Statement}
    ******************************************************/  
    while (stmt->next) {
      EmitStatementNode(stmt->next);
      stmt = stmt->next;
    }
    break;
  case TK_GLOBAL_DECL_STMTS: // global variables
    while (stmt->next) {
      EmitGlobalDeclarationNode(stmt->next->expr);
      stmt = stmt->next;
    }
    break;
  case TK_ASSIGN:  // id = expression;
    EmitAstExprNode(stmt->expr);
    if (stmt->kids[0] && stmt->expr) {
      EmitIR("# %s = %s;", GetNodeNameInIR(stmt->kids[0]), GetNodeNameInIR(stmt->expr));
      EmitAssembly("movq %s, %%rax", GetNodeNameInAssembly(stmt->expr));
      EmitAssembly("movq %%rax, %s", GetNodeNameInAssembly(stmt->kids[0]));
    }
    break;
  case TK_INPUT: // input(id);
    // printf("\tinput(%s) \n",stmt->expr->value.name);
    /****************************************************
     Example:

        leaq .input_fmtstr, %rdi
        leaq a, %rsi
        movq $0, %rax
        call scanf
    *****************************************************/
    EmitIR("# input(%s);", GetNodeNameInIR(stmt->expr));
    EmitAssembly("leaq %s, %%rdi", INPUT_FORMAT_STR_NAME);
    EmitAssembly("leaq %s, %%rsi", GetNodeNameInAssembly(stmt->expr));
    EmitAssembly("movq $0, %%rax");
    EmitAssembly("call scanf");
    break;
  case TK_OUTPUT: // output(id);
    // printf("\toutput(%s) \n",stmt->expr->value.name);
    /*****************************************************
     Example:

        leaq .output_fmtstr, %rdi
        movq a, %rsi
        movq $0, %rax      // No floating point numbers in SCC
        call printf
     *****************************************************/
    EmitIR("# output(%s);", GetNodeNameInIR(stmt->expr));
    EmitAssembly("leaq %s, %%rdi", OUTPUT_FORMAT_STR_NAME);
    EmitAssembly("movq %s, %%rsi", GetNodeNameInAssembly(stmt->expr));
    EmitAssembly("movq $0, %%rax");
    EmitAssembly("call printf");
    break;
  case TK_CALL:  // function call:  f(expr, expr, ...);
    /*****************************************************
     Refer to 
          FunctionCallExpression() in expr.c
          ExpressionStatement() in stmt.c
    *****************************************************/
    EmitAstExprNode(stmt->expr);
    break;
  case TK_RETURN:  // return expression;
    EmitAstExprNode(stmt->expr);
    EmitIR("# return %s;", GetNodeNameInIR(stmt->expr));
    EmitComments("# Save current function's return value %s in register rax", GetNodeNameInIR(stmt->expr));
    EmitAssembly("movq %s, %%rax", GetNodeNameInAssembly(stmt->expr));
    break;
  default:
    break;
  }
}
