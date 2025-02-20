#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "LfuCache.h"

// doubly linked node
struct ListHead {
    struct ListHead *prev;
    struct ListHead *next;
};

// Node in the Lfu list
struct LfuNode {
    struct ListHead lfuLink;          // to form the Lfu List, in ascending order of frequency
    struct ListHead listWithSameFreq; // head node of the list with the same frequency
    long n;                           // number of nodes in listWithSameFreq
    long freq;
};

// ItemNode, which is in a bucket, and the Lfu list
struct ItemNode {
    long key;
    long value;
    long freq; // access frequency

    struct ListHead bucketLink; // in the bucket
    struct ListHead freqLink;   // in the the list which has nodes with the same frequency

    struct LfuNode *pLfuNode; // points to the Lfu node which contains the list
};

struct LfuCache {
    long capacity;
    long num_of_keys;

    struct ListHead lfuList;
    struct ListHead buckets[];
};

#define container_of(ptr, type, member) (type *)((char *)(ptr) - (size_t)(&((type *)0)->member))

#define for_each_entry_safe(pCur, pNext, pList, member)                           \
    for ((pCur) = container_of((pList)->next, __typeof(*(pCur)), member),         \
        (pNext) = container_of(((pCur)->member).next, __typeof(*(pCur)), member); \
         &((pCur)->member) != (pList);                                            \
         (pCur) = (pNext), (pNext) = container_of((pNext)->member.next, __typeof(*(pCur)), member))

/*
    Init the doubly linked list
 */
static void InitListHead(struct ListHead *list) {
    list->prev = list;
    list->next = list;
}

static int IsTailNode(struct ListHead *list, struct ListHead *pEntry) {
    return pEntry->next == list;
}

// static int IsFrontNode(struct ListHead *list, struct ListHead *pEntry) {
//     return pEntry == list->next;
// }

/*
    Insert pNode in the middle of prev and next
 */
static void InsertNode(struct ListHead *prev, struct ListHead *pNode, struct ListHead *next) {
    prev->next = pNode;
    pNode->prev = prev;
    next->prev = pNode;
    pNode->next = next;
}

/*
    list points to the head node.
    pNode is the node to be inserted into the front of the list
 */
static void AddToFront(struct ListHead *list, struct ListHead *pNode) {
    InsertNode(list, pNode, list->next);
}

static void AddToTail(struct ListHead *list, struct ListHead *pNode) {
    InsertNode(list->prev, pNode, list);
}

/*
    Remove the node from the list
 */
static void RemoveFromList(struct ListHead *pNode) {
    pNode->prev->next = pNode->next;
    pNode->next->prev = pNode->prev;
}

struct LfuCache *CreateLfuCache(long capacity) {
    assert(capacity > 0);
    long nBytes = sizeof(struct LfuCache) + sizeof(struct ListHead) * capacity;
    struct LfuCache *cache = (struct LfuCache *)malloc(nBytes);
    cache->num_of_keys = 0;
    cache->capacity = capacity;
    InitListHead(&cache->lfuList);
    for (long i = 0; i < capacity; i++) {
        InitListHead(&cache->buckets[i]);
    }
    return cache;
}

void ReleaseLfuCache(struct LfuCache *cache) {
    struct LfuNode *pCurLfuNode, *pNextLfuNode;
    struct ListHead *plfuList = &(cache->lfuList);
    //
    for_each_entry_safe(pCurLfuNode, pNextLfuNode, plfuList, lfuLink) {
        RemoveFromList(&pCurLfuNode->lfuLink);
        //
        struct ItemNode *pCurItemNode, *pNextItemNode;
        struct ListHead *pListWithSameFreq = &pCurLfuNode->listWithSameFreq;
        for_each_entry_safe(pCurItemNode, pNextItemNode, pListWithSameFreq, freqLink) {
            RemoveFromList(&pCurItemNode->freqLink);
            free(pCurItemNode);
        }

        free(pCurLfuNode);
    }
    free(cache);
}

static struct LfuNode *CreateLfuNode(long freq) {
    struct LfuNode *newLfuNode = (struct LfuNode *)malloc(sizeof(struct LfuNode));
    assert(newLfuNode);
    memset(newLfuNode, 0, sizeof(struct LfuNode));
    newLfuNode->freq = freq;
    newLfuNode->n = 0;
    InitListHead(&newLfuNode->lfuLink);
    InitListHead(&newLfuNode->listWithSameFreq);
    return newLfuNode;
}

static void AddNewItemNode(struct LfuCache *cache, long key, long value, struct LfuNode *pLfuNode) {
    struct ItemNode *pItemNode = (struct ItemNode *)malloc(sizeof(struct ItemNode));
    assert(pItemNode);
    memset(pItemNode, 0, sizeof(struct ItemNode));
    pItemNode->key = key;
    pItemNode->value = value;
    pItemNode->freq = 1;
    pItemNode->pLfuNode = pLfuNode;
    
    AddToFront(&pLfuNode->listWithSameFreq, &pItemNode->freqLink);
    pLfuNode->n++;
    assert(pLfuNode->freq == 1);
    long i = key % cache->capacity;
    AddToFront(&cache->buckets[i], &pItemNode->bucketLink);
    cache->num_of_keys++;
}

/*

 */
static void UpdateLfuList(struct ListHead *lfuList, struct ItemNode *pCur) {
    // curList is the head node of the list with the same frequency
    struct LfuNode *pCurLfuNode = pCur->pLfuNode;
    struct LfuNode *pNextLfuNode = NULL;
    if (!IsTailNode(lfuList, &pCurLfuNode->lfuLink)) {
        pNextLfuNode = container_of((pCurLfuNode->lfuLink.next), struct LfuNode, lfuLink);
    }

    pCur->freq++;
    RemoveFromList(&pCur->freqLink);
    pCurLfuNode->n--;
    if (pCurLfuNode->n == 0) {
        RemoveFromList(&pCurLfuNode->lfuLink);
        free(pCurLfuNode);
    }

    if (pNextLfuNode && pNextLfuNode->freq == pCur->freq) {
        AddToFront(&pNextLfuNode->listWithSameFreq, &pCur->freqLink);
        pNextLfuNode->n++;
    } else {
        struct LfuNode *newLfuNode = CreateLfuNode(pCur->freq);
        AddToFront(&newLfuNode->listWithSameFreq, &pCur->freqLink);
        newLfuNode->n++;
        if (pNextLfuNode) {
            InsertNode(pNextLfuNode->lfuLink.prev, &newLfuNode->lfuLink, &pNextLfuNode->lfuLink);
        } else {
            AddToTail(lfuList, &newLfuNode->lfuLink);
        }
    }
}

static void RemoveOneKeyFromCache(struct LfuCache *cache)
{
    // the first LfuNode in Lfulist, in ascending order by frequency
    struct LfuNode *pCurLfuNode = container_of(cache->lfuList.next, struct LfuNode, lfuLink);
    // the last item in the list with the same frequency
    struct ItemNode *pItemNode = container_of(pCurLfuNode->listWithSameFreq.prev, struct ItemNode, freqLink);

    RemoveFromList(&pItemNode->bucketLink);
    RemoveFromList(&pItemNode->freqLink);
    pCurLfuNode->n--;
    if (pCurLfuNode->n == 0) {
        RemoveFromList(&pCurLfuNode->lfuLink);
        free(pCurLfuNode);
    }
    free(pItemNode);
    cache->num_of_keys--;
}

void PrintLfuCache(struct LfuCache *cache, const char *info) {
    struct LfuNode *pCurLfuNode, *pNextLfuNode;
    struct ListHead *plfuList = &(cache->lfuList);
    printf("\n\n\n--------  %s(): %s -----------\n\n", __FUNCTION__, info);
    //
    for_each_entry_safe(pCurLfuNode, pNextLfuNode, plfuList, lfuLink) {    
        printf("\t ======  freq = %ld, n = %ld ====== \n\n", pCurLfuNode->freq, pCurLfuNode->n);    
        //
        struct ItemNode *pCurItemNode, *pNextItemNode;
        struct ListHead *pListWithSameFreq = &pCurLfuNode->listWithSameFreq;
        for_each_entry_safe(pCurItemNode, pNextItemNode, pListWithSameFreq, freqLink) {
            printf("\t\t key = %ld, value = %ld\n", pCurItemNode->key, pCurItemNode->value);              
        }
    }
}

void LfuCachePut(struct LfuCache *cache, long key, long value) {
    long index = key % cache->capacity;

    struct ItemNode *pCur, *pNext;
    struct ListHead *pList = &(cache->buckets[index]);
    //
    for_each_entry_safe(pCur, pNext, pList, bucketLink) {
        if (pCur->key == key) {
            UpdateLfuList(&cache->lfuList, pCur);
            return;
        }
    }

    if (cache->num_of_keys == cache->capacity) {
        // remove one node from the cache
        RemoveOneKeyFromCache(cache);
    }

    struct LfuNode *pLfuNode = NULL;
    if (cache->num_of_keys > 0) {
        pLfuNode = container_of(cache->lfuList.prev, struct LfuNode, lfuLink);
        if (pLfuNode->freq != 1) {
            pLfuNode = NULL;
        }
    }
    // If there is no LfuNode which has the frequency 1
    if (pLfuNode == NULL) {
        pLfuNode = CreateLfuNode(1);
        AddToFront(&cache->lfuList, &pLfuNode->lfuLink);
    }
    AddNewItemNode(cache, key, value, pLfuNode);
}

/*
    If found
        *pValue = foundValue
        return 1
    else
        return 0
 */
int LfuCacheGet(struct LfuCache *cache, long key, long *pValue) {
    long index = key % cache->capacity;

    struct ItemNode *pCur, *pNext;
    struct ListHead *pList = &(cache->buckets[index]);
    //
    for_each_entry_safe(pCur, pNext, pList, bucketLink) {
        if (pCur->key == key) {
            UpdateLfuList(&cache->lfuList, pCur);
            *pValue = pCur->value;
            return 1;
        }
    }    
    return 0;
}

