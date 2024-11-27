#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "DisjointSets.h"

/*
    DisjointSetsUnion(djs, 0, 2);

    DisjointSetsUnion(djs, 4, 2);

    DisjointSetsUnion(djs, 3, 1);

       2              1
      /  \           /
     0    4         3
 */
struct DisjointSets {
    /*
        If parents[i] == i, 
            node i is the representative of a disjoint set.
        
        It points to an array; node parents[i] is the parent of node i
     */
    DJSET_INT_T *parents;
    /*
        When parents[i] == i,
            ranks[i] is approximately the height of the tree (representing a disjoint set).
        Due to path compression in find(), the height is not precise enough.
        That is why it is named as rank, rather than height.
     */
    DJSET_INT_T *ranks;
    // total number of nodes
    DJSET_INT_T n;
};

/*
    Create disjoint sets which contain n node ids.
    At the beginning, there are n disjoint sets, where parents[i] is initialized with i.
    The rank of each disjoint set is set to be 1.
 */
struct DisjointSets *CreateDisjointSets(DJSET_INT_T n) {
    struct DisjointSets *djs = (struct DisjointSets *) malloc(sizeof(struct DisjointSets));
    assert(djs);
    
    djs->n = n;
    djs->parents = (DJSET_INT_T *) malloc(sizeof(DJSET_INT_T) * n);
    djs->ranks = (DJSET_INT_T *) malloc(sizeof(DJSET_INT_T) * n);
    assert(djs->parents && djs->ranks);

    for (DJSET_INT_T i = 0; i < n; i++) {
        djs->parents[i] = i;
        djs->ranks[i] = 1;
    }

    return djs;
}

/*
    Release the heap space.
 */
void ReleaseDisjointSets(struct DisjointSets *djs) {
    if (djs) {
        free(djs->parents);
        free(djs->ranks);
        free(djs);
    }
}

/*
    Return the representative of the set containing node i.
 */
DJSET_INT_T DisjointSetsFind(struct DisjointSets *djs, DJSET_INT_T i) {
    assert(djs);

    if (djs->parents[i] == i) {
        return i;
    } else {
        DJSET_INT_T rep = DisjointSetsFind(djs, djs->parents[i]);
        // path compression
        djs->parents[i] = rep;
        return rep;
    }
}

/*
    If x and y are in two disjoint sets, 
        merge the two disjoint sets into one.
 */
void DisjointSetsUnion(struct DisjointSets *djs, DJSET_INT_T x, DJSET_INT_T y) {
    assert(djs);

    DJSET_INT_T xRep = DisjointSetsFind(djs, x);
    DJSET_INT_T yRep = DisjointSetsFind(djs, y);

    if (xRep == yRep) {
        return;
    }

    if (djs->ranks[xRep] < djs->ranks[yRep]) {
        djs->parents[xRep] = yRep;
    } else if (djs->ranks[xRep] > djs->ranks[yRep]) {
        djs->parents[yRep] = xRep;        
    } else {
        djs->parents[xRep] = yRep;
        djs->ranks[yRep]++;
    }
}

static long imgCount;

void DisplayDisjointSets(struct DisjointSets *djs) {
    DisjointSetsGenOneImage(djs, "DisjointSets", "images/DisjointSets", imgCount);
    imgCount++;    
}


////////////////////////////// DisjointSets2Dot (for visualizing the algorithm) ///////////////////////////////////////
/*
    Dot Files
 */
void DisjointSets2Dot(struct DisjointSets *djs,
                      char *filePath,
                      char *graphName,
                      int displayVisited) {
    (void) displayVisited;

    FILE *dotFile = fopen(filePath, "w");
    if (dotFile) {
        fprintf(dotFile, "digraph %s {\n", graphName);
        //fprintf(dotFile, "rankdir=\"LR\";\n");

        DJSET_INT_T i = 0;

        while (i < djs->n) {
            if (djs->parents[i] != i) {
                // one edge
                fprintf(dotFile, 
                        "\"%p\" -> \"%p\" \n", 
                        &(djs->parents[i]), 
                        &(djs->parents[djs->parents[i]]));
            }
            // one node
            fprintf(dotFile, 
                    "\"%p\" [label=\"i=%ld_r=%ld\"] [shape=box]\n", 
                    &(djs->parents[i]), 
                    (long) i, 
                    (long) djs->ranks[i]);            
            i++;
        } 
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}

#define FILE_NAME_LEN  255

void DisjointSetsGenOneImage(struct DisjointSets *djs, 
                             char *graphName, 
                             char *fileName, 
                             long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    DisjointSets2Dot(djs, dotFileName, graphName, 1);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command); 
}


