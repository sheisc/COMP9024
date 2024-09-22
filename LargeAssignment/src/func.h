#ifndef FUNC_H
#define FUNC_H
#include "stmt.h"

#define MAX_PARAMETERS_CNT 10
#define MAX_LOCAL_VARS_CNT 64

/*
    Abstract syntax tree node for a function definition.
 */
struct astFuncDefNode {
    // TK_FUNC
    TokenKind op;
    // Function's name
    Value value;

    ///////////////////////////////////////////////
    // The local symbol table of the function:
    //
    //     paras and local_vars
    //
    ///////////////////////////////////////////////

    // for function parameters
    struct astExprNode paras[MAX_PARAMETERS_CNT];
    // number of parameters defined
    int para_cnt;
    // for local variables
    struct astExprNode local_vars[MAX_LOCAL_VARS_CNT];
    // number of local variables defined in the function body
    int local_vars_cnt;

    // number of temporary variables
    int tmpVarNum;
    // the frame size (including stack space needed for local and temporary variables)
    int frameSize;

    // funcBody
    AstStmtNodePtr funcBody;
    // next function
    struct astFuncDefNode *next;
};

typedef struct astFuncDefNode *AstFuncDefNodePtr;

AstFuncDefNodePtr FunctionDefinitions(void);
void EmitFuncDefNode(AstFuncDefNodePtr func);

AstFuncDefNodePtr GetCurFuncDef(void);
void SetCurFuncDef(AstFuncDefNodePtr func);

#endif
