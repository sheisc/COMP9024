#include "lex.h"
#include "error.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    TokenKind kind;
    char *name;
} KeywordInfo;

static int defaultNextChar(void);

static SccIOFileInfo sccIOFileInfo; //  = {.curLine = 1};
static int curChar = ' ';
static NEXT_CHAR_FUNC NextChar = defaultNextChar;
// FIXME: consistency with tokens.txt
static KeywordInfo keywords[] = {{TK_INT, "long"},      {TK_IF, "if"},
                                 {TK_ELSE, "else"},     {TK_WHILE, "while"},
                                 {TK_INPUT, "input"},   {TK_OUTPUT, "output"},
                                 {TK_RETURN, "return"}, {TK_DO, "do"}};
static char *tokenNames[] = {
#define TOKEN(kind, name) name,
#include "tokens.txt"
#undef TOKEN
};
Token curToken;

////////////////////////////////////////////////////////////////////////

/*
  If @id is a keyword in scc (see the keywords[] in lex.c)
    return its kind (such as TK_INT, TK_IF, TI_WHILE, ...)
  else
    @id is a TK_ID.
 */
static TokenKind GetKeywordKind(char *id) {
    int i = 0;
    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(id, keywords[i].name) == 0) {
            return keywords[i].kind;
        }
    }
    return TK_ID;
}

// Test whether ch is a white space character
static int IsWhiteSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

// Default function for get next char.
static int defaultNextChar(void) { return SCC_EOF; }

// Get the token kind for a single character such as '+', '-' , ...
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

SccIOFileInfo *GetSccIOFileInfo(void) { return &sccIOFileInfo; }

// Get the token name of a token kind
const char *GetTokenName(TokenKind tk) { return tokenNames[tk]; }

// Get a token (like an English word, but a token consists of a kind and a value.)
Token GetToken(void) {
    Token token;
    int len = 0;
    memset(&token, 0, sizeof(token));
TryAgain:
    // skip white space
    while (IsWhiteSpace(curChar)) {
        if (curChar == '\n') {
            //curLineNum++;
            sccIOFileInfo.curLine++;
        }
        curChar = NextChar();
    }
    if (curChar == SCC_EOF) {
        token.kind = TK_EOF;
    } else if (isalpha(curChar) || curChar == '_') { // id or keyword
        len = 0;
        do {
            token.value.name[len] = curChar;
            curChar = NextChar();
            len++;
        } while ((isalnum(curChar) || curChar == '_') && len < MAX_ID_LEN);
        // check whether it is a keyword or not
        token.kind = GetKeywordKind(token.value.name);
    } else if (isdigit(curChar)) { // number
        // FIXME:
        //  checking integer overflow;
        //  no "unary plus or minus" yet;
        //  decimal only.
        long numVal = 0;
        token.kind = TK_NUM;
        do {
            numVal = numVal * 10 + (curChar - '0');
            curChar = NextChar();
        } while (isdigit(curChar));
        token.value.numVal = numVal;
        snprintf(token.value.name, MAX_ID_LEN, "%ld", numVal);
    } else { // '+','-','*','/',...
        token.kind = GetTokenKindOfChar(curChar);
        if (token.kind != TK_NA) {
            token.value.name[0] = curChar;
            curChar = NextChar();
            // It is a line comment "//" when two '/' found.
            if (token.kind == TK_DIV && curChar == '/') {
                // skip the comments between "//" and '\n'
                do {
                    curChar = NextChar();
                } while (curChar != '\n');
                goto TryAgain;
            }
        } else {
            Error(__FILE__, __LINE__,
                  "Out of scc's alphabet: char '%c' or \'0x%x\'", curChar,
                  curChar);
            curChar = NextChar();
            goto TryAgain;
        }
    }
    return token;
}

// Init our scanner
void InitLexer(NEXT_CHAR_FUNC next) {
    if (next) {
        NextChar = next;
    }
    sccIOFileInfo.curLine = 1;
}
