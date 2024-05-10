/*************************************************************
 *		Declaration
 *				long id;
 *
 *************************************************************/
#include "decl.h"
#include "emit.h"
#include "error.h"
#include "expr.h"
#include "lex.h"
#include "stmt.h"
#include <stdio.h>

/*
  Declaration:
		long id;

  To declare a global variable or a local variable.
 */

AstExprNodePtr Declaration(void) {
  AstExprNodePtr dec = NULL;
  Expect(TK_INT);
  if (curToken.kind == TK_ID) {	
    // FIXME: the default type is integer
    dec = CreateAstExprNode(TK_ID, &curToken.value, NULL, NULL);
    NEXT_TOKEN;
  } else {
    Expect(TK_ID);
  }  
  return dec;
}

static void OutputGlobalVarInfo(AstExprNodePtr globalVar) {
  #define  SYMBOL_TABLE_OUTPUT_FMT   "# %12s        %s"
  EmitComments("\n");
  EmitComments("# ----------------------------");
  EmitComments(SYMBOL_TABLE_OUTPUT_FMT, "AccessName", "Global Var");
  EmitComments("# ----------------------------");
   
  EmitComments(SYMBOL_TABLE_OUTPUT_FMT, 
               GetNodeNameInAssembly(globalVar), 
               GetNodeNameInIR(globalVar));
  EmitComments("# ----------------------------");
  #undef  SYMBOL_TABLE_OUTPUT_FMT
  EmitComments("\n");
}

/*
	Generate assembly code for a global variable (only supporting long integers).
		
  
  Put it in the data section (global area) in the assembly file.

  Example:
      long year;

  Assembly:

.data
	.comm	year, 8

  ------------------------------------------------------------------------------

  .comm 
          Declares a common memory area for data that is not initialized.
          For a global variable.
  .lcomm 
          Declares a local common memory area for data that is not initialized.
          For a static variable.
  Syntax

  .comm symbol, length

 */
void EmitGlobalDeclarationNode(AstExprNodePtr pNode) {
  if (pNode && pNode->op == TK_ID) {
    OutputGlobalVarInfo(pNode);
    EmitAssembly(".comm	%s, %d", GetNodeNameInAssembly(pNode), CPU_WORD_SIZE_IN_BYTES);
  }
}
