#include <stdio.h>
#include "expr.h"


int main(int argc, char **argv, char **env) {
    
    // Create a binary tree
    AstExprNodePtr expr = Expression();
    
    // Perform a postorder traversal of the binary tree
    long result = EvalExpression(expr);  
    
    // Output the result
    printf("\n9000 + 6 * 4 == %ld\n", result);
    
    // Free the heap memory
    ReleaseAstExpr(expr);
    
    return 0;
}

