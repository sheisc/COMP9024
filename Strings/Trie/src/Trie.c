#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Trie.h"
#include "Queue.h"

// from '0' to 'z'
// (from 48  to  122)
#define ALPHABET_SIZE 75
#define FIRST_CHAR '0'

struct TrieNode {
    // Test whether it represents the last char in a key? 
    int wordEnd;
    // value for a key when wordEnd == 1  
    ValueType value;
    // from '0' to 'z' in ASCII table        
    Trie kids[ALPHABET_SIZE];
};

//////////////////////////////////// C String ////////////////////////////////////////////////

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

static ValueType CopyValue(ValueType value) {
    return CopyString(value);
}

static void ReleaseValue(ValueType value) {
    ReleaseString(value);
}


////////////////////////////////////   Trie //////////////////////////////////////////////////



void ReleaseTrie(Trie t) {
    if (t != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            ReleaseTrie(t->kids[i]);
        }
        if (t->value) {
            ReleaseValue(t->value);
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
    pNode->value = NULL;   
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        pNode->kids[i] = NULL;
    }
    return pNode;
}

#if 0
Trie CreateTrie() {
    return NULL;
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
        t->value = CopyValue(value);
    } else {
        int i = key[0] - FIRST_CHAR;
        assert((i >= 0) && (i < ALPHABET_SIZE));
        t->kids[i] = TrieInsert(t->kids[i], key+1, value);
    }
    return t;
}
#endif

Trie CreateTrie() {
    // Create a trie node, representing the special string prefix "", an empty string
    return CreateTrieNode();
}

/*
    Insert 
 */
Trie TrieInsert(Trie t, char *key, ValueType value) {
    assert(t);
    Trie curNode = t;
    char *ptr = key;
    /*
        ptr ---> "ear" 

        "ear" can be seen as:

                "" + "a" + "b" + "c"

        We use four nodes to represent "ear" in a Trie.
     */
    while (1) {
        if(*ptr) {
            int i = *ptr - FIRST_CHAR;
            assert((i >= 0) && (i < ALPHABET_SIZE));
            if (curNode->kids[i] == NULL) {
                curNode->kids[i] = CreateTrieNode();
            }
            curNode = curNode->kids[i];
            ptr++;
        } else { // the end of the C string            
            curNode->wordEnd = 1;
            curNode->value = CopyValue(value);
            break;
        }       
    }
    return t;
}

/*
   SearchKey() returns 1 when it finds the key in the Trie.
           It will also stores the value in *pVal.
           
   SearchKey() returns 0 when the key doesn't exist.           
 */
int SearchKey(Trie t, char *key, ValueType *pVal) {
    // if (t == NULL) {
    //     return 0;
    // }

    assert(t);
    Trie curNode = t;
    char *ptr = key;
    //
    while (*ptr) {
        int i = *ptr - FIRST_CHAR;
        assert((i >= 0) && (i < ALPHABET_SIZE));
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

/*
   LongestPrefixMatch() returns 1 when it finds the longest prefix match in the Trie.
           It will also stores the value in *pVal.
           
   LongestPrefixMatch() returns 0 when the key doesn't exist.           
 */
int LongestPrefixMatch(Trie t, char *key, ValueType *pVal) {
    // if (t == NULL) {
    //     return 0;
    // }
    assert(t);
    Trie curNode = t;
    char *ptr = key;

    ValueType valueFound = NULL;
    if (t->wordEnd) {
        valueFound = t->value;
    }
    // Try to find the longest prefix match
    while (*ptr) {
        int i = *ptr - FIRST_CHAR;
        assert((i >= 0) && (i < ALPHABET_SIZE));
        // 
        if (curNode->kids[i] == NULL) {
            break;
        } else if (curNode->kids[i]->wordEnd) {
            valueFound = curNode->kids[i]->value;            
        }        
        curNode = curNode->kids[i];
        ptr++;
    }
    //
    if (valueFound) {
        if (pVal) {
            *pVal = valueFound;
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


