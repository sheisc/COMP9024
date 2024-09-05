#ifndef LEX_H
#define LEX_H
#include <stdio.h>

// Max length of an identifier (e.g., a function or variable name) 
#define MAX_ID_LEN 127

// token value
typedef struct {
  // e.g, "year", "t0", "t1", "+", "-", "*", "/", "(", ")", ...
  char name[MAX_ID_LEN + 1];
  // value of an integer, e.g., 2024
  long numVal;
} Value;

/*

In C, an Enum/Enumeration is a custom data type where users can assign names to constant integers. 
Using enums makes it easier for programmers to learn, understand, and maintain the program.

typedef enum {
  TK_NA,    // 0     By default, the first item has the value 0. Here, NA stands for Not Available.
  TK_ADD,   // 1
  TK_SUB,   // 2
  TK_MUL,
  TK_DIV,
  TK_NUM, 
  TK_LPAREN,
  TK_RPAREN, 
  TK_EOF, 
} TokenKind;

 */
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

// Define a function pointer type
typedef int (*NEXT_CHAR_FUNC)(void);

// // end of file
// #define SCC_EOF (-1)

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

void Expect(TokenKind tk);


#endif
