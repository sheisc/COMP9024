/***************************************************************************************
                A customized heap allocator.

  (1) Based on a free memory-block list (i.e., a singly linked list)
      
  (2) Some low-level pointer arithmetic involved.
  
  (3) void *OurMalloc(heap_size_t size);    

  (4) void OurFree(void *addr);

  (5) Please complete the code in Q1-Q10 in this file.

  (6) Refer to the comments in TestSccHeap.c for an overview.

                                                                COMP9024 24T2
 ***************************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memset(...)
#include "SccHeap.h"
//
#define HEAP_SIZE (4 * 1024)

// 
#define RESIDUE 8

// see the comments for STACK_SIZE_ALIGNED() in src/emit.c
#define ALIGN 3
#define ROUND(s) (((1 << ALIGN) - 1) + (s)) & (~((1 << ALIGN) - 1))

// use it as heap space via OurMalloc() and OurFree()
static char heap[HEAP_SIZE]; 

// whether the heap allocator has been initialized
static int init = 0;   

// Represents a free memory block in the linked list.
struct HeapMemBlock {
  // The size of the memory block managed, including the 'next' field, but excluding the 'size' field.
  heap_size_t size;
  // Pointing to its successor
  struct HeapMemBlock *next;
};

static struct HeapMemBlock *first;

//__attribute__ ((__constructor__))

// Initialize the heap allocator
static void initialHeap() {
  first = (struct HeapMemBlock *)&heap;
  //memset(first, 0, HEAP_SIZE); 
  // excluding the memory occupied by the 'size' field
  first->size = HEAP_SIZE - sizeof(heap_size_t);
  first->next = NULL;
}

/*
  
  1. The OurMalloc() function allocates @size bytes and returns a pointer to the allocated memory.
     
  2. The memory allocated is not initialized. 

 */
static void *OurMalloc(heap_size_t size) {
  if (!init) {
    init = 1;
    initialHeap();
  }
  if (size <= 0)
    return NULL;
  //
  heap_size_t alignedSize = ROUND(size);

  struct HeapMemBlock *cur = NULL, *pre = NULL;
  heap_size_t *ptr = NULL;

  for (cur = first; cur != NULL; pre = cur, cur = cur->next) {
    if (cur->size >= alignedSize) {
      if (cur->size >= (alignedSize + RESIDUE + sizeof(heap_size_t))) { 
        /*
          When the memory block is large enough after cutting, 
            just cut (alignedSize + sizeof(heap_size_t)) bytes of memory from the end of the block.
            
             (1) alignedSize bytes of memory will be returned and used as heap memory.

             (2) sizeof(heap_size_t) bytes of memory will be used to 
                 save the size of memory returned (i.e., alignedSize).

                 In this way, when the allocated memory is returned via OurFree(void *addr),
                 our heap allocator can know its size (i.e., alignedSize).

                 Please refer to:

                      static void OurFree(void *addr).

          Q1. Reset cur->size to be (cur->size - alignedSize - sizeof(heap_size_t)).
          Q2. Let ptr be (heap_size_t *)((char *)cur + cur->size + sizeof(heap_size_t)),
             i.e., skipping the HeapMemBLock managed by cur.
          Q3. Save the value of alignedSize into the memory location pointed to by ptr.  
         */

        ////////////////////////////////////////////////////////////////////////////////////
        //
        //    Please read the above comments first and then complete the following code.
        //
        ////////////////////////////////////////////////////////////////////////////////////

        // Q1. ___________________
        // Q2. ___________________
        // Q3. ___________________

        //memset(ptr + 1, 0, alignedSize);
        return (ptr + 1);
      } 
      else { 
        /* 
          When the memory block is not large, no cutting is performed any more.
          Just remove cur from the linked list.

          If pre is NULL (i.e., cur is the first node in the list),
              reset first (i.e., the original second node now becomes the first node)
          else
              let cur's successor be pre's successor.              
         */
        if (pre == NULL) {
          first = first->next;
        } else {
          pre->next = cur->next;
        }
        //memset((char *)cur + sizeof(heap_size_t), 0, cur->size);
        return (char *)cur + sizeof(heap_size_t);
      }
    }
  }
  return NULL;
}

/*  
    If the two nodes are adjacent,
      merge them into one node.
 */
static void mergeIfAdjacent(struct HeapMemBlock *one, struct HeapMemBlock *two) {
  if (one > two) {
    struct HeapMemBlock *tmp = one;
    one = two;
    two = tmp;
    //printf("one > two\n");
  }
  char *cptr = (char *)one;
  if (cptr + one->size + sizeof(heap_size_t) == (char *)two) {
    /*
        When @two is merged into @one, 
        sizeof(heap_size_t) bytes of memory space occupied by its field 'size' 
        can be released and used as heap space.

        So, we have 
            one->size += two->size + sizeof(heap_size_t);
     */
    one->size += two->size + sizeof(heap_size_t);
    one->next = two->next;
  }
}

/*

  The OurFree() function frees the memory space pointed to by @addr, 
  which must have been returned by a previous call to OurMalloc().

  If addrs is NULL, no operation is performed.

 */
static void OurFree(void *addr) {
  if (addr == NULL)
    return;
  
  heap_size_t *ptr = (heap_size_t *)addr; 
  // Let freedNode point to the memory block
  struct HeapMemBlock *freedNode = (struct HeapMemBlock *)(ptr - 1);  
  // Initialize its next field to be NULL.
  freedNode->next = NULL;
  /*
                WARNING

    The field freeNode->size has already been set to 'alignedSize' in OurMalloc().
    and it will be used in mergeIfAdjacent().
   */  

  // If the free memory block list is empty, then just need to insert the node
  if (first == NULL) {
    first = freedNode;
    return;
  } 
  
  struct HeapMemBlock *pre = NULL, *cur = NULL;
  /*
    1. When the control flow gets here, we are sure that 'first' is not NULL

    2. The linked list is sorted by ascending memory block addresses (from low to high).
    
    3. The following loop tries to insert the freedNode into the linked list.      
   */
  assert(first != NULL);
  for (cur = first; cur != NULL; pre = cur, cur = cur->next) {
    if (cur > freedNode) { // The insert point found
      if (pre == NULL) { 
        /*
          When pre is NULL (no predecessor node),
            cur is the first node in the list.
          
          Q4. Let the freedNode be the new first node. 
          Q5. Let cur be the successor of freedNode
          Q6. Merge freedNode and cur if they are adjacent         
         */
        ////////////////////////////////////////////////////////////////////////////////////
        //
        //    Please read the above comments first and then complete the following code (Q4-Q6).
        //
        ////////////////////////////////////////////////////////////////////////////////////

        // Q4. ___________________
        // Q5. ___________________
        // Q6. ___________________
      } 
      else {
        /*
          When pre is not NULL,
            cur is not the first node in the list.    

          Let us insert freedNode into the position between pre and cur.     
          
          Q7. Let freedNode be the successor of pre
          Q8. Let cur be the successor of freedNode
          Q9. Merge freedNode and cur if they are adjacent
          Q10. Merge pre and freedNode if they are adjacent         
         */        
        ////////////////////////////////////////////////////////////////////////////////////
        //
        //    Please read the above comments first and then complete the following code (Q7-Q10).
        //
        ////////////////////////////////////////////////////////////////////////////////////

        // Q7. ___________________
        // Q8. ___________________
        // Q9. ___________________
        // Q10. __________________ 
      }
      return;
    }
  }  
  /*
    1. When the control flow reaches here, it means that 
       no insert point has been found in the above loop.

    2. It also means the freedNode should be inserted at the end of the linked list.
       (i.e., its memory address is larger than that of any node in the list).
    
    3. The pointer variable 'pre' must point to the last node of the linked list
       after the above loop.        
   */
  assert(pre != NULL);
  pre->next = freedNode;
  mergeIfAdjacent(pre, freedNode);  
}

/*
  For testing.
 */
void printList() {
  if (!init) {
    init = 1;
    initialHeap();
  }
  struct HeapMemBlock *cur = first;
  for (; cur != NULL; cur = cur->next) {
    printf("[%p,%ld]", (cur), cur->size);
    if (cur->next)
      printf("-->");
    else
      printf("\n");
  }
}


/*
  if the macro USE_OUR_MALLOC_FREE is defined
      Use OurMalloc() and OurFree()
  else
      Use malloc() and free() in the C library.
 */

#define USE_OUR_MALLOC_FREE

void *SccMalloc(heap_size_t size) {
#ifdef USE_OUR_MALLOC_FREE
  return OurMalloc(size);
#else
  return malloc(size);
#endif    

}

void SccFree(void *addr) {
#ifdef USE_OUR_MALLOC_FREE
  return OurFree(addr);
#else
  return free(addr);
#endif
}

