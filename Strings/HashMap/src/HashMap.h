#ifndef HASHMAP_H
#define HASHMAP_H

typedef char *HashMapKeyTy;
typedef char *HashMapValueTy;

struct HashMap;

struct HashMap *CreateHashMap(void);

void ReleaseHashMap(struct HashMap* pMap);

// insert or update a key-value pair in a HashMap
void HashMapPut(struct HashMap *pMap, HashMapKeyTy key, HashMapValueTy value);

// get the value associated with a key
HashMapValueTy HashMapGet(struct HashMap *pMap, HashMapKeyTy key);

// delete a key-value pair
void HashMapDelete(struct HashMap *pMap, HashMapKeyTy key);

// Save the HashMap in a *.dot file
void HashMap2Dot(struct HashMap* pMap, char *filePath, char *graphName);

void GenOneImage(struct HashMap* pMap, char *graphName, char *fileName, long seqNo);

#endif

