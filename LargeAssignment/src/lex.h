#ifndef LEX_H
#define LEX_H
#include <stdio.h>

// Max length of an identifier (e.g., a function or variable name) 
#define MAX_ID_LEN 127

// token value
typedef struct {
  // name for id, e.g, "year"
  char name[MAX_ID_LEN + 1];
  // value of an integer, e.g., 2024
  long numVal;
} Value;

// Different token kinds are defined in tokens.txt
typedef enum {
#define TOKEN(kind, name) kind,
#include "tokens.txt"
#undef TOKEN
} TokenKind;

// A token is like an English word.
// But a token = (token kind, token value)
// For example, 300 and 400 are of the same token kind (i.e., TK_NUM),
// but with different values.
typedef struct {
  TokenKind kind;
  Value value;
} Token;

// Define a funtion pointer type
typedef int (*NEXT_CHAR_FUNC)(void);

// end of file
#define SCC_EOF (-1)

// declaration of a global variable in C
extern Token curToken;

// Read the next token from the input file and save it in the global variable curToken.
#define NEXT_TOKEN                                                             \
  do {                                                                         \
    curToken = GetToken();                                                     \
  } while (0)

Token GetToken(void);
const char *GetTokenName(TokenKind tk);
void InitLexer(NEXT_CHAR_FUNC next);

// A structure for describing the input/output file information.
typedef struct {
  // e.g., "./tests/Factorial.scc"
  char *inputFileName;
  // current line in the open sccSrcFile (e.g., "./tests/Factorial.scc")
  int curLine;
  // e.g., sccSrcFile = fopen("./tests/Factorial.scc", "r")
  FILE *sccSrcFile;
  // sccAssemblyFile = fopen("./tests/Factorial.scc.s", "w")
  FILE *sccAssemblyFile;
} SccIOFileInfo;

SccIOFileInfo *GetSccIOFileInfo(void);

#endif
