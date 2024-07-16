/******************************************************************************************

1. After completing the code in Q1-Q10 in SccHeap.c, we can run the following commands in CSE VLAB.

    HeapAllocator$ make

    HeapAllocator$ ./main


2. If everything goes well, we will see the output as follows.
   The addresses might differ due to Address Space Layout Randomization (ASLR).

[0x55797dcdc040,4088]

************** SccMalloc() *****************

void * ptr16 = SccMalloc(16)

	[0x55797dcdc040,4064]

void * ptr32 = SccMalloc(32)

	[0x55797dcdc040,4024]

void * ptr48 = SccMalloc(48)

	[0x55797dcdc040,3968]

void * ptr64 = SccMalloc(64)

	[0x55797dcdc040,3896]

void * ptr80 = SccMalloc(80)

	[0x55797dcdc040,3808]

************** SccFree() *****************

SccFree(ptr32)

	[0x55797dcdc040,3808]-->[0x55797dcdd000,32]

SccFree(ptr64)

	[0x55797dcdc040,3808]-->[0x55797dcdcf80,64]-->[0x55797dcdd000,32]

SccFree(ptr16)

	[0x55797dcdc040,3808]-->[0x55797dcdcf80,64]-->[0x55797dcdd000,56]

SccFree(ptr48)

	[0x55797dcdc040,3808]-->[0x55797dcdcf80,184]

SccFree(ptr80)

	[0x55797dcdc040,4088]

 ******************************************************************************************/

#include "SccHeap.h"
#include <stdio.h>
#include <stdlib.h>

/*
  Concatenation

  https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html

  Concatenation is often useful to merge two tokens into one while expanding macros.

  The ‘##’ preprocessing operator performs token pasting. 
  When a macro is expanded, 
  the two tokens on either side of each ‘##’ operator are combined into a single token.
 */

#define ALLOC(n)                                                               \
  printf("\nvoid * ptr%d = SccMalloc(%d)\n\n\t", (n), (n));                    \
  void *ptr##n = SccMalloc(n);                                                 \
  printList();

#define FREE(n)                                                                \
  printf("\nSccFree(ptr%d)\n\n\t", (n));                                       \
  SccFree(ptr##n);                                                             \
  printList();

int main() {
  printList();

  printf("\n************** SccMalloc() *****************\n");
  ALLOC(16);
  ALLOC(32);
  ALLOC(48);
  ALLOC(64);
  ALLOC(80);

  printf("\n************** SccFree() *****************\n");

  FREE(32);
  FREE(64);
  FREE(16);
  FREE(48);
  FREE(80);

  return 0;
}


