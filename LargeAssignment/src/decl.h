#ifndef DECL_H
#define DECL_H
#include "expr.h"
#include "lex.h"
AstExprNodePtr Declaration(void);
void EmitGlobalDeclarationNode(AstExprNodePtr pNode);
#endif
