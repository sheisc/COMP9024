#include "SccLib.h"


SCC_INT_T SccLess(SCC_INT_T a, SCC_INT_T b) { 
  return a < b; 
}

SCC_INT_T SccLessEqual(SCC_INT_T a, SCC_INT_T b) { 
  return a <= b; 
}

SCC_INT_T SccLarger(SCC_INT_T a, SCC_INT_T b) { 
  return a > b; 
}

SCC_INT_T SccLargerEqual(SCC_INT_T a, SCC_INT_T b) { 
  return a >= b; 
}

SCC_INT_T SccEqual(SCC_INT_T a, SCC_INT_T b) { 
  return a == b; 
}

SCC_INT_T SccNotEqual(SCC_INT_T a, SCC_INT_T b) { 
  return a != b; 
}

SCC_INT_T SccMod(SCC_INT_T a, SCC_INT_T b) {
  return a % b;
}

// Logic Operation

SCC_INT_T SccLogicAnd(SCC_INT_T a, SCC_INT_T b) { 
  return a && b; 
}


SCC_INT_T SccLogicOr(SCC_INT_T a, SCC_INT_T b) { 
  return a || b; 
}

SCC_INT_T SccLogicNot(SCC_INT_T a) { 
  return !a; 
}

// Memory Read

SCC_INT_8  SccRead8(char *base, SCC_INT_T offset) {
  return *((SCC_INT_8 *)(base + offset));
}

SCC_INT_16  SccRead16(char *base, SCC_INT_T offset) {
  return *((SCC_INT_16 *)(base + offset));
}

SCC_INT_32 SccRead32(char *base, SCC_INT_T offset) {
  return *((SCC_INT_32 *)(base + offset));
}


SCC_INT_64  SccRead64(char *base, SCC_INT_T offset) {
  return *((SCC_INT_64 *)(base + offset));
}

// Memory Write

void SccWrite8(char *base, SCC_INT_T offset, SCC_INT_8 val) {
  *((SCC_INT_8 *)(base + offset)) = val;
}

void SccWrite16(char *base, SCC_INT_T offset, SCC_INT_16 val) {
  *((SCC_INT_16 *)(base + offset)) = val;
}

void SccWrite32(char *base, SCC_INT_T offset, SCC_INT_32 val) {
  *((SCC_INT_32 *)(base + offset)) = val;
}

void SccWrite64(char *base, SCC_INT_T offset, SCC_INT_64 val) {
  *((SCC_INT_64 *)(base + offset)) = val;
}

#if 0
void SccStore(char *base, SCC_INT_T offset, SCC_INT_T val) { 
  *((SCC_INT_T *)(base + offset)) = val; 
}

SCC_INT_T SccLoad(char *base, SCC_INT_T offset) { 
  return *((SCC_INT_T *)(base + offset)); 
}
#endif

