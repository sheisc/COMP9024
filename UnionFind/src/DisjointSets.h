#ifndef DISJOINT_SETS_H
#define DISJOINT_SETS_H

typedef long DJSET_INT_T;

struct DisjointSets;

// Return the representative of the set containing i
struct DisjointSets *CreateDisjointSets(DJSET_INT_T n);

/*
    If x and y are in two disjoint sets, 
        merge the two sets
 */
void ReleaseDisjointSets(struct DisjointSets *djs);

// Return the representative of the set containing i
DJSET_INT_T DisjointSetsFind(struct DisjointSets *djs, DJSET_INT_T i);

/*
    If x and y are in two disjoint sets, 
        merge the two sets
 */
void DisjointSetsUnion(struct DisjointSets *djs, DJSET_INT_T x, DJSET_INT_T y);


void DisplayDisjointSets(struct DisjointSets *djs);


void DisjointSets2Dot(struct DisjointSets *djs,
                      char *filePath,
                      char *graphName,
                      int displayVisited);


void DisjointSetsGenOneImage(struct DisjointSets *djs, 
                             char *graphName, 
                             char *fileName, 
                             long seqNo);

#endif




