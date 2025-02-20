#ifndef LFU_CACHE_H
#define LFU_CACHE_H

struct LfuCache;

struct LfuCache *CreateLfuCache(long capacity);

void ReleaseLfuCache(struct LfuCache *cache);

void LfuCachePut(struct LfuCache *cache, long key, long value);

int LfuCacheGet(struct LfuCache *cache, long key, long *pValue);

void PrintLfuCache(struct LfuCache *cache, const char *info);

#endif
