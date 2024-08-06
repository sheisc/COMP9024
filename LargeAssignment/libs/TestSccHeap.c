/******************************************************************************************

1. After completing the code in Q1-Q10 in SccHeap.c, we can run the following commands in VLAB.

    LargeAssignment$ pwd

    /path/to/LargeAssignment

    LargeAssignment$ make testHeap


2. If everything goes well, we will see the output as follows.
   The address might differ due to Address space layout randomization (ASLR).

    [0x564728956040,4194296]

    ************** SccMalloc() *****************

    void * ptr16 = SccMalloc(16)
    [0x564728956040,4194272]

    void * ptr32 = SccMalloc(32)
    [0x564728956040,4194232]

    void * ptr48 = SccMalloc(48)
    [0x564728956040,4194176]

    void * ptr64 = SccMalloc(64)
    [0x564728956040,4194104]

    void * ptr80 = SccMalloc(80)
    [0x564728956040,4194016]

    ************** SccFree() *****************

    SccFree(ptr32)
    [0x564728956040,4194016]-->[0x564728d56000,32]

    SccFree(ptr64)
    [0x564728956040,4194016]-->[0x564728d55f80,64]-->[0x564728d56000,32]

    SccFree(ptr16)
    [0x564728956040,4194016]-->[0x564728d55f80,64]-->[0x564728d56000,56]

    SccFree(ptr48)
    [0x564728956040,4194016]-->[0x564728d55f80,184]

    SccFree(ptr80)
    [0x564728956040,4194296]

 3. If we want all the test cases in the directory 'tests' to use OurMalloc() and OurFree(),
    just add the compiler flag '-D USE_OUR_MALLOC_FREE' into line 62 in the Makefile.    
    The same flag has already been used in line 68.
 ******************************************************************************************/

#include "SccHeap.h"
#include <stdio.h>
#include <stdlib.h>

#define ALLOC(n)                                                               \
  printf("void * ptr%d = SccMalloc(%d)\n", (n), (n));                          \
  void *ptr##n = SccMalloc(n);                                                 \
  printList();

#define FREE(n)                                                                \
  printf("SccFree(ptr%d)\n", (n));                                             \
  SccFree(ptr##n);                                                             \
  printList();

int main(void) {
  printList();

  printf("************** SccMalloc() *****************\n");
  ALLOC(16);
  ALLOC(32);
  ALLOC(48);
  ALLOC(64);
  ALLOC(80);

  printf("************** SccFree() *****************\n");

  FREE(32);
  FREE(64);
  FREE(16);
  FREE(48);
  FREE(80);

  return 0;
}
