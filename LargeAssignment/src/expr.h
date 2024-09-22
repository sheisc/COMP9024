#ifndef EXPR_H
#define EXPR_H
#include "lex.h"

#define MAX_ARG_CNT 10
#define MAX_PARAMETER_CNT MAX_ARG_CNT
// 8 bytes on x86_64
#define CPU_WORD_SIZE_IN_BYTES 8
// Only support integers; NO floating point numbers in scc.
// Number of arguments passed in physical registers.
#define NUM_OF_ARGS_IN_REGS 6

/*
    The Abstract Syntax Tree Node for an expression.
 */
struct astExprNode {
    // The kind of an expression.
    // To keep it simple, we reuse the TokenKind defined in tokens.txt.
    TokenKind op;
    // The value got from lex.c
    //      name of id,  integer values, ...
    Value value;

    /////////////////////////////////////////////
    // e.g.,  left and right operands of an binary operator (+, -, *, /)
    struct astExprNode *kids[2];
    // assembly  name  for this node
    char accessName[MAX_ID_LEN + 1];
    // the offset for local/temporary variables, see The Stack Layout in func.c
    int offset;
    // for a function call in an expression
    struct astExprNode *args[MAX_ARG_CNT];
    // number of arguments in a function call
    int arg_cnt;
    // "t1" in a function call "t1 = f()".
    char retValTmp[MAX_ID_LEN + 1];
};
typedef struct astExprNode *AstExprNodePtr;

// void Expect(TokenKind tk);
void DoExpect(char *sccFile, int sccLine, TokenKind tk);

/*
__FILE__ 
    a preprocessor macro that expands to full path to the current file,
    in the form of a C string constant (e.g., "src/expr.c"). 
__LINE__
    a preprocessor macro that expands to current line number in the C source file, 
    as an integer (e.g., 30). 
 */
#define Expect(tk) DoExpect(__FILE__, __LINE__, tk)

AstExprNodePtr Expression(void);

void EmitAstExprNode(AstExprNodePtr pNode);
char *GetNodeNameInAssembly(AstExprNodePtr pNode);
char *GetNodeNameInIR(AstExprNodePtr pNode);
int GetTmpVarNum(void);
int GetFrameSize(void);
void ResetFrameSize(void);
int AllocFrameOffset(void);
AstExprNodePtr CreateAstExprNode(TokenKind tk, Value *pVal, AstExprNodePtr left,
                                 AstExprNodePtr right);
AstExprNodePtr FunctionCallExpression(Token savedToken);

#endif
