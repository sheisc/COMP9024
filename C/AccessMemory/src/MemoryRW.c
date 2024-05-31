#include "MemoryRW.h"

// read one byte (8 bits) from the memory block at the specified address
MM_INT_8  MemoryRead8(MM_INT_64 address) {
    MM_INT_8 *cptr = (MM_INT_8 *) address;
    MM_INT_8 val = *cptr;
    return val;
}

// read two bytes (16 bits) from the memory block at the specified address
MM_INT_16 MemoryRead16(MM_INT_64 address) {
    MM_INT_16 *sptr = (MM_INT_16 *) address;
    MM_INT_16 val = *sptr;
    return val;    
}

// read four byte (32 bits) from the memory block at the specified address
MM_INT_32 MemoryRead32(MM_INT_64 address) {
    MM_INT_32 *iptr = (MM_INT_32 *) address;
    MM_INT_32 val = *iptr;
    return val;    
}

// read eight byte (64 bits) from the memory block at the specified address
MM_INT_64 MemoryRead64(MM_INT_64 address) {
    MM_INT_64 *lptr = (MM_INT_64 *) address;
    MM_INT_64 val = *lptr;
    return val; 
}

// write the 8-bit data to the memory block at the specified address
void MemoryWrite8(MM_INT_64 address, MM_INT_8 val) {
    MM_INT_8 *cptr = (MM_INT_8 *) address;
    *cptr = val;

    //*((MM_INT_8 *) address) = val;
}

// write the 16-bit data to the memory block at the specified address
void MemoryWrite16(MM_INT_64 address, MM_INT_16 val) {
    MM_INT_16 *sptr = (MM_INT_16 *) address;
    *sptr = val;

    //*((MM_INT_16 *) address) = val;
}

// write the 32-bit data to the memory block at the specified address
void MemoryWrite32(MM_INT_64 address, MM_INT_32 val) {
    MM_INT_32 *iptr = (MM_INT_32 *) address;
    *iptr = val;

    //*((MM_INT_32 *) address) = val;
}

// write the 64-bit data to the memory block at the specified address
void MemoryWrite64(MM_INT_64 address, MM_INT_64 val) {
    MM_INT_64 *lptr = (MM_INT_64 *) address;
    *lptr = val;

    //*((MM_INT_64 *) address) = val;
}
