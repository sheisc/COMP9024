#ifndef HASHMAP_H
#define HASHMAP_H

struct HashMap;

struct HashMap *CreateHashMap(void);

void ReleaseHashMap(struct HashMap* pMap);

// insert or update
void HashMapPut(struct HashMap *pMap, const char* key, const char* value);

// retrieve
const char *HashMapGet(struct HashMap *pMap, const char* key);

// delete
void HashMapDelete(struct HashMap *pMap, const char* key);

void HashMapPrint(struct HashMap* pMap);

// Save the HashMap in a *.dot file
void HashMap2Dot(struct HashMap* pMap, char *filePath, char *graphName);

void GenOneImage(struct HashMap* pMap, char *graphName, char *fileName, long seqNo);

#endif

