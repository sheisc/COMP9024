#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HashMap.h"

// keys
static char *words[] = { 
    "ear", "apply", "ape", "apes", "earth", 
    "east", "app", "ace", "early", "earl", 
    "aces", "apple"
};

// values
static char *meanings[] = { 
    "the sense organ for hearing", 
    "put into service", 
    "a large primate that lacks a tail", 
    "the plural noun of the word ape", 
    "the planet on which we live", 
    
    "the eastern part of the world",
    "an application, especially as downloaded by a user to a mobile device", 
    "a playing card with a single spot on it", 
    "happening or done before the usual or expected time", 
    "a British nobleman",
    
    "the plural noun of the word ace",
    "a round fruit with firm, white flesh and a green, red, or yellow skin"
};    

int main(void) {
    long count = 0;
    struct HashMap *pMap = CreateHashMap();

    int n = sizeof(words)/sizeof(words[0]);
    assert(n == (sizeof(meanings)/sizeof(meanings[0])));   

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    for (int i = 0; i < n; i++) {
        HashMapPut(pMap, words[i], meanings[i]);
        //
        GenOneImage(pMap, "OurHashMap", "images/OurHashMap", count);
        count++;
    }

    // Demonstrate get and put
    const char *key = words[0];
    const char *value = NULL;
    value = HashMapGet(pMap, key);
    if (value) {
        printf("\nThe meaning of \"%s\": %s\n", key, value);
    }
  
    printf("\nUpdating the meaning of \"%s\"", key);
    if (value) {
        HashMapPut(pMap, key, "The sense organ for hearing");
    }

    value = HashMapGet(pMap, key);
    if (value) {
        printf("\nThe meaning of \"%s\": %s\n", key, value);
    }

    key = "earl";
    printf("\nHashMapDelete(pMap, \"%s\")\n", key);
    HashMapDelete(pMap, key);

    GenOneImage(pMap, "OurHashMap", "images/OurHashMap", count);
    count++;

    value = HashMapGet(pMap, key);
    if (!value) {
        printf("\nThe hash map does not contain the key \"%s\"\n\n", key);
    }

    for (int i = 0; i < n; i++) {
        value = HashMapGet(pMap, words[i]);
        if (!value) {
            printf("\t \"%s\" not found\n", words[i]);
        } else {
            printf("\t %s: %s\n", words[i], value);
        }        
    }

    ReleaseHashMap(pMap);
    return 0;
}



