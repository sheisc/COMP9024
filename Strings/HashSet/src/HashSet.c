#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HashSet.h"

#define BUCKET_COUNT    2

#define LOAD_FACTOR_THRESHOLD       0.75


struct BucketEntry {
    HashSetKeyTy key;
    struct BucketEntry* next;
};

struct HashSet{
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
    // the number of slots/buckets in the HashSet
    int capacity;
    // the number of keys in the HashSet
    int n;
};

/////////////////////////////// key strings ///////////////////////////////////////////////

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

static HashSetKeyTy CopyKey(HashSetKeyTy key) {
    return CopyString(key);
}

static void ReleaseKey(HashSetKeyTy key) {
    ReleaseString(key);
}

static int IsEqual(HashSetKeyTy k1, HashSetKeyTy k2) {
    return strcmp(k1, k2) == 0;
}

static char *KeyToString(HashSetKeyTy key) {
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

struct BucketEntry *CreateBucketEntry(HashSetKeyTy key) {
    struct BucketEntry *pEntry = (struct BucketEntry *) malloc(sizeof(struct BucketEntry));
    assert(pEntry);
    // Copy key. Heap space will be allocated if necessary.
    pEntry->key = CopyKey(key);
    pEntry->next = NULL;
    return pEntry;
}

void ReleaseBucketEntry(struct BucketEntry *pEntry) {
    if (pEntry) {
        ReleaseKey(pEntry->key);
        free(pEntry);
    }    
}


struct HashSet *CreateHashSet(void) {
    struct HashSet *pSet = (struct HashSet *) malloc(sizeof(struct HashSet));
    struct BucketEntry **buckets = 
        (struct BucketEntry **) malloc(sizeof(struct BucketEntry *) * BUCKET_COUNT);
    assert(pSet && buckets);

    pSet->buckets = buckets;
    pSet->capacity = BUCKET_COUNT;
    pSet->n = 0;
    for (int i = 0; i < pSet->capacity; i++) {
        pSet->buckets[i] = NULL;
    }    
    return pSet;
}

static void HashSetResize(struct HashSet *pSet) {
    int newCapacity = pSet->capacity * 2;
    assert(newCapacity > pSet->capacity);
    struct BucketEntry **newBuckets = 
        (struct BucketEntry **) malloc(sizeof(struct BucketEntry *) * newCapacity);
    assert(newBuckets);
    for (int i = 0; i < newCapacity; i++) {
        newBuckets[i] = NULL;
    }

    // move the existing bucket entries 
    for (int i = 0; i < pSet->capacity; i++) {
        struct BucketEntry *current = pSet->buckets[i];
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
    free(pSet->buckets);
    // resize
    pSet->buckets = newBuckets;
    pSet->capacity = newCapacity;
}

double HashSetLoadFactor(struct HashSet *pSet) {
    assert(pSet->capacity > 0);
    double loadFactor = ((double) (pSet->n)) / pSet->capacity;
    return loadFactor;
}

// insert a key in a HashSet
void HashSetInsert(struct HashSet *pSet, HashSetKeyTy key) {
    unsigned int index = GetHash(key) % pSet->capacity;
    struct BucketEntry *current = pSet->buckets[index];    
    
    while (current != NULL) {
        // Do nothing if the key exists
        if (IsEqual(current->key, key)) {
            return;
        }
        current = current->next;
    }

    // Add a key
    struct BucketEntry *pEntry = CreateBucketEntry(key);
    // add the entry at the front of the bucket
    pEntry->next = pSet->buckets[index];
    pSet->buckets[index] = pEntry;
    //increase the number of keys
    pSet->n++;

    // Test whether we need to resize the HashSet for better performance
    if (HashSetLoadFactor(pSet) >= LOAD_FACTOR_THRESHOLD) {
        HashSetResize(pSet);
    }
}

// test whether the HashSet contains the key
int HashSetContains(struct HashSet *pSet, HashSetKeyTy key) {
    unsigned int index = GetHash(key) % pSet->capacity;
    struct BucketEntry *current = pSet->buckets[index];
    
    while (current != NULL) {
        if (IsEqual(current->key, key)) {
            return 1;
        }
        current = current->next;
    }
    // Not found
    return 0; 
}

// delete a key
void HashSetDelete(struct HashSet *pSet, HashSetKeyTy key) {
    unsigned int index = GetHash(key) % pSet->capacity;
    struct BucketEntry *current = pSet->buckets[index];
    struct BucketEntry *prev = NULL;
    
    while (current != NULL) {
        if (IsEqual(current->key, key)) {
            if (prev != NULL) { 
                // current is not the first element
                prev->next = current->next;
            } else { 
                // current is the first element
                pSet->buckets[index] = current->next;
            }
            ReleaseBucketEntry(current);
            // decrease the number of keys
            pSet->n--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void ReleaseHashSet(struct HashSet* pSet) {
    for (int i = 0; i < pSet->capacity; i++) {
        struct BucketEntry *current = pSet->buckets[i];
        // Release the elements in each bucket
        while (current != NULL) {
            struct BucketEntry *tmp = current;
            current = current->next;
            ReleaseBucketEntry(tmp);
        }
    }
    free(pSet->buckets);
    free(pSet);
}

//////////////////////////// HashSet2Dot ////////////////////////////////////////

#define FILE_NAME_LEN  255

void GenOneImage(struct HashSet* pSet, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    HashSet2Dot(pSet, dotFileName, graphName);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}

/*
    digraph OurHashSet {
    "HashSet_c=8_n=3"
    "HashSet_c=8_n=3" -> {"ear"} [label="0"]
    "HashSet_c=8_n=3" -> {"apply"} [label="6"]
    "apply" -> {"ape"}
    }
 */
void HashSet2Dot(struct HashSet* pSet, char *filePath, char *graphName) {
    
    FILE *dotFile = fopen(filePath, "w");
    assert(pSet);
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        // Node for the whole HashSet
        fprintf(dotFile, 
                "\"HashSet_c=%d_n=%d\"\n",
                pSet->capacity,
                pSet->n);
        for (int i = 0; i < pSet->capacity; i++) {
            if (pSet->buckets[i]) {
                fprintf(dotFile, 
                        "\"HashSet_c=%d_n=%d\" %s {\"%s\"} [label=\"%d\"]\n",
                        pSet->capacity,
                        pSet->n,
                        edgeConnectorStr,
                        KeyToString(pSet->buckets[i]->key),
                        i);
                struct BucketEntry *next = pSet->buckets[i]->next;
                struct BucketEntry *current = pSet->buckets[i];
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


