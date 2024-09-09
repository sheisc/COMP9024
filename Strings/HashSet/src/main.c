#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HashSet.h"

// keys
static char *words[] = { 
    "ear", "apply", "ape", "apes", "earth", 
    "east", "app", "ace", "early", "earl", 
    "aces", "apple"
};
   

int main(void) {
    long count = 0;
    struct HashSet *pSet = CreateHashSet();

    int n = sizeof(words)/sizeof(words[0]);

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    for (int i = 0; i < n; i++) {
        HashSetInsert(pSet, words[i]);
        GenOneImage(pSet, "OurHashSet", "images/OurHashSet", count);
        count++;
    }

    HashSetKeyTy key = "earl";

    if (HashSetContains(pSet, key)) {
        printf("\tThe HashSet contains \"%s\"\n", key);
    }

    HashSetDelete(pSet, key);
    GenOneImage(pSet, "OurHashSet", "images/OurHashSet", count);
    count++;
    
    printf("After HashSetDelete(pSet, key)\n");
    if (!HashSetContains(pSet, key)) {
        printf("\tHashSet doesn't contain \"%s\"\n", key);
    }

    ReleaseHashSet(pSet);
    return 0;
}



