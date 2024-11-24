#include <stdio.h>    // printf()
#include <stdlib.h>   // system()

#include "DisjointSets.h"

static void TestDisjointSets(void) {
    struct DisjointSets *djs = CreateDisjointSets(5);
    
    DisplayDisjointSets(djs);

    DisjointSetsUnion(djs, 0, 2);
    DisplayDisjointSets(djs);

    DisjointSetsUnion(djs, 4, 2);
    DisplayDisjointSets(djs);

    DisjointSetsUnion(djs, 3, 1);
    DisplayDisjointSets(djs);

    DisjointSetsUnion(djs, 3, 4);
    DisplayDisjointSets(djs);    

    DJSET_INT_T rep = DisjointSetsFind(djs, 3);
    DisplayDisjointSets(djs);
    
    printf("DisjointSetsFind(djs, 3): %ld\n", rep);

    ReleaseDisjointSets(djs);
}

int main(void) {
    printf("\n########################### TestDisjointSets() ######################\n\n\n");

    TestDisjointSets();

    return 0;    
}



