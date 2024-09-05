#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "lex.h"
#include "error.h"


static int defaultNextChar(void);


static int curChar = ' ';

static NEXT_CHAR_FUNC NextChar = defaultNextChar;

/*
  Run the following command and 
  take a look at the output (i.e., src/lex.i) of the C preprocessor.

  Tutorials/Week4$ gcc -E src/lex.c -o src/lex.i

 */
static char *tokenNames[] = {
  #define TOKEN(kind, name) name,
  #include "tokens.txt"
  #undef TOKEN
};

Token curToken;

////////////////////////////////////////////////////////////////////////

// Test whether ch is a white space character
static int IsWhiteSpace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

// Default function for get next char.
static int defaultNextChar(void) { 
  return EOF; 
}

// Get the token kind for a single character such as '+', '-' , '*', '/'
// See tokens.txt and the macro TOKEN()
static TokenKind GetTokenKindOfChar(char ch) {
  int i = 0;
  for (i = 0; i < sizeof(tokenNames) / sizeof(tokenNames[0]); i++) {
    if ((strlen(tokenNames[i]) == 1) && (tokenNames[i][0] == ch)) {
      return i;
    }
  }
  return TK_NA;
}

// Get the token name of a token kind
const char *GetTokenName(TokenKind tk) { 
  return tokenNames[tk]; 
}

void Expect(TokenKind tk) {
  if (curToken.kind == tk) {
    NEXT_TOKEN;
  } 
  else {
    Error("\"%s\" expected", GetTokenName(tk));
  }
}

// Get a token (like an English word, but a token consists of a kind and a value.)
Token GetToken(void) {
  Token token;
  memset(&token, 0, sizeof(token));
 
  // skip white space
  while (IsWhiteSpace(curChar)) {
    curChar = NextChar();
  }

  if (curChar == EOF) { // end of file
    token.kind = TK_EOF;
  } 
  else if (isdigit(curChar)) { // number
    // FIXME: 
    // checking integer overflow; 
    long numVal = 0;
    token.kind = TK_NUM;
    do {
      numVal = numVal * 10 + (curChar - '0');
      curChar = NextChar();
    } while (isdigit(curChar));
    token.value.numVal = numVal;
    snprintf(token.value.name, MAX_ID_LEN, "%ld", numVal);
  } 
  else { // '+','-','*','/'
    token.kind = GetTokenKindOfChar(curChar);
    if (token.kind != TK_NA) { 
      token.value.name[0] = curChar;
      curChar = NextChar();
    } 
    else {
      Error("Out of our alphabet: char '%c' or \'0x%x\'", curChar, curChar);    
    }
  }
  return token;
}

// Init our scanner
void InitLexer(NEXT_CHAR_FUNC next) {
  if (next) {
    NextChar = next;
  }
}
