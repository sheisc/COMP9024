#ifndef HEAP_H
#define HEAP_H

typedef unsigned long heap_size_t;

void *SccMalloc(heap_size_t size);
void SccFree(void *addr);
void printList(); //

//void *OurMalloc(heap_size_t size);
//void OurFree(void *addr);
#endif
