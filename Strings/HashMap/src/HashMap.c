#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HashMap.h"

#define BUCKET_COUNT    2

#define LOAD_FACTOR_THRESHOLD       0.75


struct BucketEntry {
    HashMapKeyTy key;
    HashMapValueTy value;
    struct BucketEntry* next;
};

struct HashMap{
    /*
                             ------------------------
                                buckets[capacity-1]    ---->  BucketEntry  ----> ....   ---> BucketEntry
                                buckets[capacity-2]
                                     ...
            buckets   ---->     buckets[0]             ---->  BucketEntry  ----> ....   ---> BucketEntry 
                             ------------------------
                              Array of pointers on the heap
     */
    struct BucketEntry **buckets;
    // the number of slots/buckets in the HashMap
    int capacity;
    // the number of key-value pairs in the HashMap
    int n;
};

/////////////////////////////// Both key and value are strings ///////////////////////////////////////////////

static char * CopyString(char *str) {
    size_t len = strlen(str);
    char *str2 = (char *) malloc(len+1);
    strncpy(str2, str, len);
    str2[len] = 0;
    return str2;    
}

static void ReleaseString(char *str) {
    free(str);
}

static HashMapKeyTy CopyKey(HashMapKeyTy key) {
    return CopyString(key);
}

static void ReleaseKey(HashMapKeyTy key) {
    ReleaseString(key);
}

static HashMapValueTy CopyValue(HashMapValueTy value) {
    return CopyString(value);
}

static void ReleaseValue(HashMapValueTy value) {
    free(value);
}

static int IsEqual(HashMapKeyTy k1, HashMapKeyTy k2) {
    return strcmp(k1, k2) == 0;
}

static char *KeyToString(HashMapKeyTy key) {
    return key;
}
/*
    For more about hash functions, please refer to 

        https://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16.html
 */
static unsigned int GetHash(char *key) {
    unsigned int sum = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        sum += ((unsigned char) key[i]);
    }
    return sum;  
}

///////////////////////////////////////////////////////////////////////////////////

struct BucketEntry *CreateBucketEntry(HashMapKeyTy key, HashMapValueTy value) {
    struct BucketEntry *pEntry = (struct BucketEntry *) malloc(sizeof(struct BucketEntry));
    assert(pEntry);
    // Copy key and value. Heap space will be allocated if necessary.
    pEntry->key = CopyKey(key);
    pEntry->value = CopyValue(value);
    pEntry->next = NULL;
    return pEntry;
}

void ReleaseBucketEntry(struct BucketEntry *pEntry) {
    if (pEntry) {
        ReleaseKey(pEntry->key);
        ReleaseValue(pEntry->value);
        free(pEntry);
    }    
}


struct HashMap *CreateHashMap(void) {
    struct HashMap *pMap = (struct HashMap *) malloc(sizeof(struct HashMap));
    struct BucketEntry **buckets = 
        (struct BucketEntry **) malloc(sizeof(struct BucketEntry *) * BUCKET_COUNT);
    assert(pMap && buckets);

    pMap->buckets = buckets;
    pMap->capacity = BUCKET_COUNT;
    pMap->n = 0;
    for (int i = 0; i < pMap->capacity; i++) {
        pMap->buckets[i] = NULL;
    }    
    return pMap;
}

static void HashMapResize(struct HashMap *pMap) {
    int newCapacity = pMap->capacity * 2;
    assert(newCapacity > pMap->capacity);
    struct BucketEntry **newBuckets = 
        (struct BucketEntry **) malloc(sizeof(struct BucketEntry *) * newCapacity);
    assert(newBuckets);
    for (int i = 0; i < newCapacity; i++) {
        newBuckets[i] = NULL;
    }

    // move the existing bucket entries 
    for (int i = 0; i < pMap->capacity; i++) {
        struct BucketEntry *current = pMap->buckets[i];
        while (current != NULL) {
            struct BucketEntry *tmp = current;
            // rehash the element into a larger table.
            unsigned int index = GetHash(tmp->key) % newCapacity;
            current = current->next;
            // insert the BucketEntry at the head of the linked list pointed to by newBuckets[index]
            tmp->next = newBuckets[index];
            newBuckets[index] = tmp;
        }
    }
    // release the old buckets
    free(pMap->buckets);
    // resize
    pMap->buckets = newBuckets;
    pMap->capacity = newCapacity;
}

double HashMapLoadFactor(struct HashMap *pMap) {
    assert(pMap->capacity > 0);
    double loadFactor = ((double) (pMap->n)) / pMap->capacity;
    return loadFactor;
}

// insert or update a key-value pair in a HashMap
void HashMapPut(struct HashMap *pMap, HashMapKeyTy key, HashMapValueTy value) {
    unsigned int index = GetHash(key) % pMap->capacity;
    struct BucketEntry *current = pMap->buckets[index];
    
    // Update the value if its key already exists
    while (current != NULL) {
        if (IsEqual(current->key, key)) {
            // Release the heap space pointed to by current->value
            ReleaseValue(current->value);
            // Copy the value and allocate heap space if necessary
            current->value = CopyValue(value);
            return;
        }
        current = current->next;
    }

    // Add a key-value pair
    struct BucketEntry *pEntry = CreateBucketEntry(key, value);
    // add the entry at the front of the bucket
    pEntry->next = pMap->buckets[index];
    pMap->buckets[index] = pEntry;
    //increase the number of key-value pairs
    pMap->n++;

    // Test whether we need to resize the HashMap for better performance
    if (HashMapLoadFactor(pMap) >= LOAD_FACTOR_THRESHOLD) {
        HashMapResize(pMap);
    }
}

// get the value associated with a key
HashMapValueTy HashMapGet(struct HashMap *pMap, HashMapKeyTy key) {
    unsigned int index = GetHash(key) % pMap->capacity;
    struct BucketEntry *current = pMap->buckets[index];
    
    while (current != NULL) {
        if (IsEqual(current->key, key)) {
            return current->value;
        }
        current = current->next;
    }
    // Not found
    return NULL; 
}

// delete a key-value pair
void HashMapDelete(struct HashMap *pMap, HashMapKeyTy key) {
    unsigned int index = GetHash(key) % pMap->capacity;
    struct BucketEntry *current = pMap->buckets[index];
    struct BucketEntry *prev = NULL;
    
    while (current != NULL) {
        if (IsEqual(current->key, key)) {
            if (prev != NULL) { 
                // current is not the first element
                prev->next = current->next;
            } else { 
                // current is the first element
                pMap->buckets[index] = current->next;
            }
            ReleaseBucketEntry(current);
            // decrease the number of key-value pairs
            pMap->n--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void ReleaseHashMap(struct HashMap* pMap) {
    for (int i = 0; i < pMap->capacity; i++) {
        struct BucketEntry *current = pMap->buckets[i];
        // Release the elements in each bucket
        while (current != NULL) {
            struct BucketEntry *tmp = current;
            current = current->next;
            ReleaseBucketEntry(tmp);
        }
    }
    free(pMap->buckets);
    free(pMap);
}

//////////////////////////// HashMap2Dot ////////////////////////////////////////

#define FILE_NAME_LEN  255

void GenOneImage(struct HashMap* pMap, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    HashMap2Dot(pMap, dotFileName, graphName);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}

/*
    digraph OurHashMap {
    "HashMap_c=8_n=3"
    "HashMap_c=8_n=3" -> {"ear"} [label="0"]
    "HashMap_c=8_n=3" -> {"apply"} [label="6"]
    "apply" -> {"ape"}
    }
 */
void HashMap2Dot(struct HashMap* pMap, char *filePath, char *graphName) {
    
    FILE *dotFile = fopen(filePath, "w");
    assert(pMap);
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        // Node for the whole HashMap
        fprintf(dotFile, 
                "\"HashMap_c=%d_n=%d\"\n",
                pMap->capacity,
                pMap->n);
        for (int i = 0; i < pMap->capacity; i++) {
            if (pMap->buckets[i]) {
                fprintf(dotFile, 
                        "\"HashMap_c=%d_n=%d\" %s {\"%s\"} [label=\"%d\"]\n",
                        pMap->capacity,
                        pMap->n,
                        edgeConnectorStr,
                        KeyToString(pMap->buckets[i]->key),
                        i);
                struct BucketEntry *next = pMap->buckets[i]->next;
                struct BucketEntry *current = pMap->buckets[i];
                while (next) {
                    fprintf(dotFile, 
                            "\"%s\" %s {\"%s\"}\n",
                            KeyToString(current->key),
                            edgeConnectorStr,
                            KeyToString(next->key));
                    current = next;
                    next = current->next;                   
                }
            }
        }
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


