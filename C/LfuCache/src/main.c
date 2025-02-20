#include <stdio.h>
#include <string.h>
#include "LfuCache.h"

#define INFO_STR_LEN    1024

static char info[INFO_STR_LEN];

int main(void) {
    long n = 4;
    // put()
    struct LfuCache *cache = CreateLfuCache(n);
    for (long i = 1; i <= n; i++) {
        LfuCachePut(cache, i, i * 10);   
        snprintf(info, INFO_STR_LEN, "put(%ld, %ld)", i, i * 10);
        PrintLfuCache(cache, info);     
    }
    // get(), repeat twice
    long value = 0;
    for (long repeat = 1; repeat < 2; repeat++) {
        for (long i = 1; i <= n; i++) {   
            int found = LfuCacheGet(cache, i, &value);         
            snprintf(info, INFO_STR_LEN, "get(%ld)", i);
            PrintLfuCache(cache, info);
            if (found) {
                printf("<%ld, %ld>\n", i, value);
            }            
        }
    }
    // put new keys
    for (long i = n + 1; i <= 2 * n; i++) {
        LfuCachePut(cache, i, i * 10);   
        snprintf(info, INFO_STR_LEN, "put(%ld, %ld)", i, i * 10);
        PrintLfuCache(cache, info);     
    }    
    ReleaseLfuCache(cache);    
    return 0;
}

