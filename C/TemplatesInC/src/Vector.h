#ifndef VECTOR_H
#define VECTOR_H

/*
    Template function names, used in declarations and defintions
 */
#define Vector(type)        type##Vector
// template function names
#define CreateVector(type)  Create##type##Vector
#define ReleaseVector(type) Release##type##Vector
#define VectorAdd(type)     type##VectorAdd
#define VectorAppend(type)  type##VectorAppend
#define VectorRemove(type)  type##VectorRemove
#define VectorGet(type)     type##VectorGet
#define VectorSet(type)     type##VectorSet
#define VectorSize(type)    type##VectorSize

/*
    Declaration of a vector template in C
 */
#define VectorDeclaration(type) \
\
struct Vector(type);\
\
struct Vector(type) *CreateVector(type)(void);\
\
void ReleaseVector(type)(struct Vector(type) *pVec);\
\
void VectorAdd(type)(struct Vector(type) *pVec, long index, type element);\
\
void VectorAppend(type)(struct Vector(type) *pVec, type element);\
\
type VectorRemove(type)(struct Vector(type) *pVec, long index);\
\
type VectorGet(type)(struct Vector(type) *pVec, long index);\
\
type VectorSet(type)(struct Vector(type) *pVec, long index, type element);\
\
long VectorSize(type)(struct Vector(type) *pVec);

VectorDeclaration(long);
VectorDeclaration(double);

#if 0
struct BinaryTree {
    struct BinaryTree *left;
    struct BinaryTree *right;
    long key;
};
typedef struct BinaryTree *BinaryTreePtr;
VectorDeclaration(BinaryTreePtr);
#endif

/*
    Definition of a vector template in C
 */
#define VectorDefinition(type, INITIAL_VECTOR_SIZE) \
\
struct Vector(type) { \
    long capacity; \
    long count; \
    type *pItems; \
}; \
\
struct Vector(type) *CreateVector(type)(void) {\
    struct Vector(type) *pVec = (struct Vector(type) *) malloc(sizeof(struct Vector(type))); \
    type *pItems = (type *) malloc(sizeof(type) * INITIAL_VECTOR_SIZE); \
    assert(pVec && pItems); \
    pVec->capacity = INITIAL_VECTOR_SIZE; \
    pVec->count = 0; \
    pVec->pItems = pItems; \
    return pVec; \
} \
void ReleaseVector(type)(struct Vector(type) *pVec) { \
    if (pVec) { \
        free(pVec->pItems); \
        free(pVec); \
    }   \
} \
static void type##VectorIncreaseCapacity(struct Vector(type) *pVec) { \
     type *newItems = (type *) malloc(sizeof(type) * pVec->capacity * 2); \
     assert(newItems); \
     memcpy(newItems, pVec->pItems, sizeof(type) * pVec->capacity); \
     free(pVec->pItems); \
     pVec->capacity *= 2; \
     pVec->pItems = newItems; \
} \
\
void VectorAdd(type)(struct Vector(type) *pVec, long index, type element) { \
    assert(index >= 0 && index <= pVec->count); \
    if (pVec->capacity == pVec->count) { \
        type##VectorIncreaseCapacity(pVec); \
    } \
    for (long i = pVec->count - 1; i >= index; i--) { \
        pVec->pItems[i + 1] = pVec->pItems[i]; \
    } \
    pVec->pItems[index] = element; \
    pVec->count++;   \
}\
\
void VectorAppend(type)(struct Vector(type) *pVec, type element){ \
    VectorAdd(type)(pVec, pVec->count, element);\
}\
\
type VectorRemove(type)(struct Vector(type) *pVec, long index){\
    assert(index >= 0 && index < pVec->count);\
    type element = pVec->pItems[index];\
    for (long i = index + 1; i < pVec->count; i++) {\
        pVec->pItems[i - 1] = pVec->pItems[i];\
    }\
    pVec->count--;\
    return element;\
}\
\
type VectorGet(type)(struct Vector(type) *pVec, long index){\
    assert(index >= 0 && index < pVec->count);\
    return pVec->pItems[index];\
}\
\
type VectorSet(type)(struct Vector(type) *pVec, long index, type element){\
    assert(index >= 0 && index < pVec->count);\
    type old = pVec->pItems[index];\
    pVec->pItems[index] = element;\
    return old;\
}\
\
long VectorSize(type)(struct Vector(type) *pVec){\
    return pVec->count;\
}

#endif


