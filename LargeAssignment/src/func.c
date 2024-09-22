#include "func.h"
#include "decl.h"
#include "emit.h"
#include "error.h"
#include "expr.h"
#include "stmt.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define START_OFFSET_OF_PARAS 16
/* 
    https://en.wikipedia.org/wiki/X86_calling_conventions
    See "System V AMD64 ABI"
 
    funcName (parameter1, parameter2, parameter3, parameter4, parameter5, parameter6, 
              parameter7, parameter8, ...) {
 
    }
 
    --------------------------------------------------------------------
 
             The Stack Layout in SCC When A Function Is Called
 
   The offset of the 7th parameter, counting from 1
   
    ....
    parameter 8       --------  offset 24,  pushed by caller                 24(%rbp)
    parameter 7       --------  offset 16,  pushed by caller                 16(%rbp)
    return address    --------  offset 8                                     8 (%rbp)
    caller's rbp      --------  offset 0    <-------------------------------------------  callee's rbp           
    parameter 1       --------  offset -8,        for rdi, saved by SCC,     -8(%rbp)
    parameter 2       --------  offset -16,       for rsi, saved by SCC,     -16(%rbp)
    parameter 3       --------  offset -24,       for rdx, saved by SCC,     -24(%rbp) 
    parameter 4       --------  offset -32,       for rcx, saved by SCC,     -32(%rbp)
    parameter 5       --------  offset -40,       for r8,  saved by SCC,     -40(%rbp)
    parameter 6       --------  offset -48,       for r9,  saved by SCC,     -48(%rbp)     
    ....  
    local variables/temps

 */
#define OFFSET_OF_PARAMETER_7 16

static AstFuncDefNodePtr curFuncDef;

/*
   Create an AST node for one function definition.
 */
static AstFuncDefNodePtr CreateFuncDefNode(Token funcId) {
    AstFuncDefNodePtr pNode =
        (AstFuncDefNodePtr)malloc(sizeof(struct astFuncDefNode));
    assert(pNode != NULL);
    memset(pNode, 0, sizeof(*pNode));
    pNode->op = TK_FUNC;
    pNode->value = funcId.value;
    return pNode;
}

static void sanity_check(AstFuncDefNodePtr func) {
    (void)func;
    // FIXME
    // check whether there are redefinitions of local variables and parameters.
}

/*
    FunctionDefinition:
      id(id, id, id, ..., id) {  
  
      }
 */
static AstFuncDefNodePtr FunctionDefinition(void) {
    assert(curToken.kind == TK_ID);
    // curToken contains the function's name.
    AstFuncDefNodePtr func = CreateFuncDefNode(curToken);
    NEXT_TOKEN;
    Expect(TK_LPAREN);
    int para_cnt = 0;
    ResetFrameSize();
    // This loop handles the parameters.
    while (curToken.kind == TK_ID) {
        memset(&func->paras[para_cnt], 0, sizeof(func->paras[para_cnt]));
        func->paras[para_cnt].op = TK_ID;
        func->paras[para_cnt].value = curToken.value;
        /*
            https://en.wikipedia.org/wiki/X86_calling_conventions
     
             See "System V AMD64 ABI"
           
             Also have a look at the comments for OFFSET_OF_PARAMETER_7 in func.c.
             To keep it simple, we save this information in the AST.        
         */
        if (para_cnt < NUM_OF_ARGS_IN_REGS) {
            func->paras[para_cnt].offset =
                -((para_cnt + 1) * CPU_WORD_SIZE_IN_BYTES);
        } else {
            func->paras[para_cnt].offset =
                OFFSET_OF_PARAMETER_7 +
                (para_cnt - NUM_OF_ARGS_IN_REGS) * CPU_WORD_SIZE_IN_BYTES;
        }
        para_cnt++;
        if (para_cnt > MAX_PARAMETER_CNT) {
            Error(__FILE__, __LINE__, "only %d parameters allowed",
                  MAX_PARAMETER_CNT);
        }
        NEXT_TOKEN;
        if (curToken.kind == TK_COMMA) {
            NEXT_TOKEN;
            if (curToken.kind != TK_ID) {
                Error(__FILE__, __LINE__, "',' redundant");
            }
        } else if (curToken.kind == TK_RPAREN) {
            break;
        } else {
            Error(__FILE__, __LINE__, "',' or ')' expected");
        }
    }
    Expect(TK_RPAREN);
    func->para_cnt = para_cnt;
    // To be used by calling GetCurFuncDef()
    SetCurFuncDef(func);
    // Parse the function body
    func->funcBody = CompoundStatement();
    // To be simple,
    // after parsing a function definition, we also get its frame size in scc.
    func->frameSize = GetFrameSize();
    func->tmpVarNum = GetTmpVarNum();
    sanity_check(func);
    return func;
}

/* 
    FunctionDefinitions:
      FunctionDefinition  FunctionDefinition ...  FunctionDefinition
    
    One FunctionDefintion is as follows:

      id(id, id, id, ..., id) {

      }
 */
AstFuncDefNodePtr FunctionDefinitions(void) {
    // Two coding styles to create a linked list of function defintions
#if 0 // Method 1
    AstFuncDefNodePtr first = NULL, cur = NULL, pre = NULL;
    // traverse each function defintion
    while (curToken.kind == TK_ID) {
      cur = FunctionDefinition();
      if (pre == NULL) {
        first = cur;
      } else {
        pre->next = cur;
      }
      pre = cur;
    }
    return first;
#else // Method 2
    AstFuncDefNodePtr first = NULL;
    AstFuncDefNodePtr *pNodePtr = &first;
    // traverse each function defintion
    while (curToken.kind == TK_ID) {
        *pNodePtr = FunctionDefinition();
        pNodePtr = &((*pNodePtr)->next);
    }
    return first;
#endif
}

/*
  Get the AST node of the current function we are parsing
 */
AstFuncDefNodePtr GetCurFuncDef(void) { return curFuncDef; }

/*
  Save the AST node of the current function into a static variable.  
 */
void SetCurFuncDef(AstFuncDefNodePtr func) { curFuncDef = func; }

void OutputFrameInfo(AstFuncDefNodePtr func) {
#define SYMBOL_TABLE_OUTPUT_FMT "# %12s        %s"
    EmitComments("\n");
    EmitComments("# Table of Function Parameters");
    EmitComments("# ----------------------------");
    EmitComments(SYMBOL_TABLE_OUTPUT_FMT, "AccessName", "Name");
    EmitComments("# ----------------------------");
    for (int i = 0; i < func->para_cnt; i++) {
        EmitComments(SYMBOL_TABLE_OUTPUT_FMT,
                     GetNodeNameInAssembly(&func->paras[i]),
                     GetNodeNameInIR(&func->paras[i]));
    }
    EmitComments("# ----------------------------");
    EmitComments("\n");

    EmitComments("# Table of Local Variables");
    EmitComments("# ----------------------------");
    EmitComments(SYMBOL_TABLE_OUTPUT_FMT, "AccessName", "Name");
    EmitComments("# ----------------------------");
    EmitComments("#");
    for (int i = 0; i < func->local_vars_cnt; i++) {
        EmitComments(SYMBOL_TABLE_OUTPUT_FMT,
                     GetNodeNameInAssembly(&func->local_vars[i]),
                     GetNodeNameInIR(&func->local_vars[i]));
    }
    EmitComments("# ----------------------------");
    EmitComments("\n");
#undef SYMBOL_TABLE_OUTPUT_FMT
}

/*
    Generate x86_64 assembly code for a function definition.
 */
void EmitFuncDefNode(AstFuncDefNodePtr func) {
    /*
    The six physical registers specified in "System V AMD64 ABI"

        https://en.wikipedia.org/wiki/X86_calling_conventions

     */
    static char *x64ArgRegs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
    while (func) {
        SetCurFuncDef(func);
        /*********************************
         The following 'paraNames' is used to save the parameter names in a function defintion.
    
         e.g., 
          "argc,argv,env" in main(argc,argv,env)
        **********************************/
        char paraNames[MAX_PARAMETER_CNT * (MAX_ID_LEN + 1)];
        memset(paraNames, 0, sizeof(paraNames));
        for (int i = 0; i < func->para_cnt; i++) {
            strcat(paraNames, GetNodeNameInIR(&func->paras[i]));
            if (i < func->para_cnt - 1) {
                strcat(paraNames, ",");
            }
        }
        EmitComments("\n\n\n\n\n# *****************************  %s(%s)  "
                     "*****************************",
                     func->value.name, paraNames);
        EmitComments("# Code Section");

        /*********************************
          .text
          .globl	funcName
          funcName:
         **********************************/
        EmitLabel(".text");
        EmitLabel(".globl %s", func->value.name);
        EmitLabel("%s:", func->value.name);
        EmitPrologue(func);
        OutputFrameInfo(func);
        /****************************************************************************************     
         Simple policy:   
            Save the contents of the six register onto the call stack.
            Take a look at the comments for OFFSET_OF_PARAMETER_7 in func.c
    
            It is a simple policy, but not performant.        
            We have to reload these arguments from stack when they are needed (not performant).     
         ****************************************************************************************/
        EmitComments(
            "# Simple policy for saving arguments passed in registers");
        for (int i = 0; (i < func->para_cnt) && (i < NUM_OF_ARGS_IN_REGS);
             i++) {
            EmitAssembly("movq %%%s, %d(%%rbp)", x64ArgRegs[i],
                         -(i + 1) * CPU_WORD_SIZE_IN_BYTES);
        }
        EmitComments("\n");
        EmitStatementNode(func->funcBody);
        EmitEpilogue();
        func = func->next;
    }
}


