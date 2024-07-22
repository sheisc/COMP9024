#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Trie.h"
#include "Queue.h"

#define ALPHABET_SIZE 26
#define FIRST_CHAR 'a'

struct TrieNode {
    // Test whether it represents the last char in a key? 
    int wordEnd;
    // value for a key when wordEnd == 1  
    ValueType value;
    // Only support lowercase letters         
    Trie kids[ALPHABET_SIZE];
};

Trie CreateTrie() {
    return NULL;
}

void ReleaseTrie(Trie t) {
    if (t != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
	         ReleaseTrie(t->kids[i]);
        }
        free(t);
    }
}

/*
    Create a Trie node
 */
static Trie CreateTrieNode(void) {
    Trie pNode = malloc(sizeof(struct TrieNode));
    assert(pNode != NULL);
    pNode->wordEnd = 0;
    pNode->value = 0;   
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->kids[i] = NULL;
    }
    return pNode;
}

/*
    Insert 
 */
Trie TrieInsert(Trie t, char *key, ValueType value) {
    if (t == NULL) {
        t = CreateTrieNode();
    }
    if (key[0] == '\0') {
        t->wordEnd = 1;
        t->value = value;
    } else {
        int i = key[0] - FIRST_CHAR;
        assert((i >= 0) && (i <ALPHABET_SIZE));
        t->kids[i] = TrieInsert(t->kids[i], key+1, value);
    }
    return t;
}

/*
   SearchKey() returns 1 when it finds the key in the Trie.
           It will also stores the value in *pVal.
           
   SearchKey() returns 0 when the key doesn't exist.           
 */
int SearchKey(Trie t, char *key, ValueType *pVal) {
    Trie curNode = t;
    char *ptr = key;
    //
    while(*ptr) {
        int i = *ptr - FIRST_CHAR;
        assert((i >= 0) && (i <ALPHABET_SIZE));
        if (curNode->kids[i] == NULL) {
            return 0;
        }
        curNode = curNode->kids[i];
        ptr++;
    }
    //
    if (curNode->wordEnd) {
        if (pVal) {
            *pVal = curNode->value;
        }
        return 1;
    } else {
        return 0;
    }
}


//////////////////////////// Trie2Dot ////////////////////////////////////////

#define FILE_NAME_LEN  255

void GenOneImage(Trie root, char *graphName, char *fileName, long seqNo) {
    char dotFileName[FILE_NAME_LEN+1] = {0};
    char pngFileName[FILE_NAME_LEN+1] = {0};
    char command[(FILE_NAME_LEN+1)*4] = {0};
    
    snprintf(dotFileName, FILE_NAME_LEN, "%s_%04ld.dot", fileName, seqNo);
    snprintf(pngFileName, FILE_NAME_LEN, "%s_%04ld.png", fileName, seqNo);

    Trie2Dot(root, dotFileName, graphName);

    snprintf(command, FILE_NAME_LEN*4, "dot -T png %s -o %s", dotFileName, pngFileName);

    //printf("%s\n", command);
    
    // Execute the command in a child process (fork() + exec() on Linux)
    system(command);
}

/*
    digraph TrieInsert {
    "0x587a961a42a0" -> {"0x587a961a4390"} [label="e"]
    "0x587a961a4390" -> {"0x587a961a4480"} [label="a"]
    "0x587a961a4480" -> {"0x587a961a4570"} [label="r"]
    "0x587a961a4570" [color=red]
    }
 */
void Trie2Dot(Trie root, char *filePath, char *graphName) {

    FILE *dotFile = fopen(filePath, "w");
    /*
        FIXME:  check sanity of the parameters.
     */
    if (dotFile) {
        char *edgeConnectorStr = "->";
        fprintf(dotFile, "digraph %s {\n", graphName);
        
        struct Queue *pQueue = CreateQueue();
        if (root) {
            QueueEnqueue(pQueue, root);
            while (!QueueIsEmpty(pQueue)) {
                Trie curNode = QueueDequeue(pQueue);
                // output current node
                if (curNode->wordEnd) {
                    fprintf(dotFile, 
                            "\"%p\" [color=red]\n",
                            curNode);
                }
                // output the directed edge
                for (int i = 0; i < ALPHABET_SIZE; i++) {
                    if (curNode->kids[i]) {
                        fprintf(dotFile, "\"%p\" %s {\"%p\"} [label=\"%c\"]\n",
                                curNode,
                                edgeConnectorStr,                         
                                curNode->kids[i],
                                FIRST_CHAR + i);
                        QueueEnqueue(pQueue, curNode->kids[i]);                        
                    }
                }                
            }
        }
        ReleaseQueue(pQueue);     
        fprintf(dotFile, "}\n");
        fclose(dotFile);
    }                
}


