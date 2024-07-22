#ifndef TRIE_H
#define TRIE_H

typedef char *ValueType;
//typedef char *KeyType;

struct TrieNode;
typedef struct TrieNode *Trie;

// create an empty Trie
Trie CreateTrie();

// free memory associated with Trie
void ReleaseTrie(Trie t);


// insert a new item into a Trie
Trie TrieInsert(Trie t, char *key, ValueType value);  

// Search a key in a Trie
int SearchKey(Trie t, char *key, ValueType *pVal);

// For visualizing the data structure
void Trie2Dot(Trie root, char *filePath, char *graphName);
void GenOneImage(Trie root, char *graphName, char *fileName, long seqNo);

#endif


