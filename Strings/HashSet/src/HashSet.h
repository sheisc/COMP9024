#ifndef HASHSET_H
#define HASHSET_H

typedef char *HashSetKeyTy;

struct HashSet;

struct HashSet *CreateHashSet(void);

void ReleaseHashSet(struct HashSet* pSet);

// test whether a HashMap contains a key
int HashSetContains(struct HashSet *pSet, HashSetKeyTy key);

// insert a key
void HashSetInsert(struct HashSet *pSet, HashSetKeyTy key);

// delete a key
void HashSetDelete(struct HashSet *pSet, HashSetKeyTy key);

// Save the HashSet in a *.dot file
void HashSet2Dot(struct HashSet* pSet, char *filePath, char *graphName);

void GenOneImage(struct HashSet* pSet, char *graphName, char *fileName, long seqNo);

#endif

