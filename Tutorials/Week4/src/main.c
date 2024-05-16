/*
                    How to use

    $ make

    $ ./main

    "3000 + (1000 + 2000) * 2 + 6 * 4" 

    t1 = 1000 + 2000
    t2 = t1 * 2
    t0 = 3000 + t2
    t4 = 6 * 4
    t3 = t0 + t4

    eval("3000 + (1000 + 2000) * 2 + 6 * 4") == 9024

 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "expr.h"

// scc source code in memory
//static const char *srcCode = "3000 + (1000 + 2000) * 2 + 6 * 4";
static const char *srcCode = "9000 + (6 * 4)";

// get the next char from memory
static int NextCharFromMem(void) {
  static int i = 0;  

  int ch = (unsigned char)(srcCode[i]);
  i++;
  /*
      A valid C string requires the presence of a terminating "null character" 
      (a character with ASCII value 0, usually represented by the character literal '\0' )
   */
  if (ch == 0) { 
    return EOF;
  } 
  else {
    return ch;
  }
}

#if 0
// get the next char from an input file
static int NextCharFromInputFile(void) {
  /*
    $ man fgetc

    int fgetc(FILE *stream);

    fgetc()  reads  the  next character from stream and 
    returns it as an unsigned char cast to an int, 
    or EOF on end of file or error.
   */
  int ch = fgetc(stdin);
  return ch;
}
#endif

int main(int argc, char **argv, char **env) {
  // The source code to be parsed
  printf("\"%s\" \n\n", srcCode);
  // Initialize the lexical analysis
  InitLexer(NextCharFromMem);
  // Get the first token
  NEXT_TOKEN;
  // Create a tree by parsing the arithmetic expression
  AstExprNodePtr expr = Expression();
  Expect(TK_EOF);
  // Perform a postorder traversal of the tree
  long result = EvalExpression(expr);  
  // Output the result
  printf("\neval(\"%s\") == %ld\n", srcCode, result);
  // Free the heap memory
  ReleaseAstExpr(expr);
  return 0;
}

