#ifndef SCC_LIB_H
#define SCC_LIB_H

typedef char SCC_INT_8;
typedef short SCC_INT_16;
typedef int SCC_INT_32;
typedef long SCC_INT_64;

typedef long SCC_INT_T;

// <, <=, >, >=, ==, !=
SCC_INT_T SccLess(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccLessEqual(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccLarger(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccLargerEqual(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccEqual(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccNotEqual(SCC_INT_T a, SCC_INT_T b);

// a % b
SCC_INT_T SccMod(SCC_INT_T a, SCC_INT_T b);

// Logic Operation
SCC_INT_T SccLogicAnd(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccLogicOr(SCC_INT_T a, SCC_INT_T b);
SCC_INT_T SccLogicNot(SCC_INT_T a);

// Memory Read
// SCC_INT_8  SccRead8(char *base, SCC_INT_T offset);
// SCC_INT_16 SccRead16(char *base, SCC_INT_T offset);
// SCC_INT_32 SccRead32(char *base, SCC_INT_T offset);
// SCC_INT_64 SccRead64(char *base, SCC_INT_T offset);

SCC_INT_T SccRead8(char *base, SCC_INT_T offset);
SCC_INT_T SccRead16(char *base, SCC_INT_T offset);
SCC_INT_T SccRead32(char *base, SCC_INT_T offset);
SCC_INT_T SccRead64(char *base, SCC_INT_T offset);

// Memory Write
void SccWrite8(char *base, SCC_INT_T offset, SCC_INT_8 val);
void SccWrite16(char *base, SCC_INT_T offset, SCC_INT_16 val);
void SccWrite32(char *base, SCC_INT_T offset, SCC_INT_32 val);
void SccWrite64(char *base, SCC_INT_T offset, SCC_INT_64 val);

#endif
