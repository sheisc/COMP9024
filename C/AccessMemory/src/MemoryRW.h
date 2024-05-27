#ifndef MEMORY_RW_H
#define MEMORY_RW_H

// assume sizeof(long) == 8, ...
// With the keyword typedef, we can give a type (e.g., char) a new name (e.g., MM_INT_8). 
typedef char  MM_INT_8;
typedef short MM_INT_16;
typedef int   MM_INT_32;
typedef long  MM_INT_64;

// Memory Read: for explaining low-level pointer arithmetic in C
MM_INT_8  MemoryRead8(MM_INT_64 address);
MM_INT_16 MemoryRead16(MM_INT_64 address);
MM_INT_32 MemoryRead32(MM_INT_64 address);
MM_INT_64 MemoryRead64(MM_INT_64 address);

// Memory Write for explaining low-level pointer arithmetic in C
void MemoryWrite8(MM_INT_64 address, MM_INT_8 val);
void MemoryWrite16(MM_INT_64 address, MM_INT_16 val);
void MemoryWrite32(MM_INT_64 address, MM_INT_32 val);
void MemoryWrite64(MM_INT_64 address, MM_INT_64 val);

#endif

