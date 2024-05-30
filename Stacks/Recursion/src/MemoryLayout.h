// MEMORY_LAYOUT_H is a macro guard, further discussed in COMP9024/C/HowToMake/src/myadd.h
#ifndef MEMORY_LAYOUT_H
#define MEMORY_LAYOUT_H


// The declaration of a global variable, not its definition
extern long year;
//  
extern char *cptr;

// declaration of a global function
int test1(void);
// 
void TestMemoryLayout(void);

#endif