#include <stdio.h>
#include <stdlib.h>
#include "Trie.h"

static long imgCount = 1;

static char *words[] = { "ear", "apply", "ape", "apes", "earth", 
                         "east", "app",  "ace", "early", "earl", 
                         "aces" };                     

// The meaning of each word
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

    "the plural noun of the word ace"
};

void TestDictionary(void) {
    Trie t = CreateTrie();

    int n = sizeof(words)/sizeof(words[0]);   
    for (int i = 0; i < n; i++) {
        t = TrieInsert(t, words[i], meanings[i]);
        GenOneImage(t, "TrieInsert", "images/TrieInsert", imgCount);
        imgCount++;
    }
    
    printf("Our dictionary contains the following items:\n\n");
    ValueType value = NULL;
    for (int i = 0; i < n; i++) {
        if (SearchKey(t, words[i], &value)) {
            printf("%s\n\t%s\n", words[i], value);
        }
    }   
    printf("\n");

    char *newWord = "nullpointer";
    if (SearchKey(t, newWord, &value) == 0) {
        printf("Our dictionary does not contain the word \"%s\"\n\n", newWord);
    } else {
        printf("%s\n\t%s\n", newWord, value);
    }     
    ReleaseTrie(t);
}

/*
    IP Address Lookup Using Trie

    ------------------------------------------------------
        IP Address (binary)        Action
    ------------------------------------------------------
        1xxxxxxx                   Block
        0xxxxxxx                   Forward
        1111xxxx                   Block and alarm
        000xxxxx                   Forward and alarm
    ------------------------------------------------------
 */

static char *ipPrefix[] = { "1", "0", "1111", "000"}; 

static char *actions[] = {"Block", "Forward", "Block and alarm", "Forward and alarm"};

void TestLongestPrefixMatch(void) {
    Trie t = CreateTrie();
    int n = sizeof(ipPrefix)/sizeof(ipPrefix[0]);   
    for (int i = 0; i < n; i++) {
        t = TrieInsert(t, ipPrefix[i], actions[i]);
        GenOneImage(t, "TrieInsert", "images/TrieInsert", imgCount);
        imgCount++;
    }
    // To be simple, only 8 bits are used here, rather than 32 bits in IPv4
    char *ipAddrs[] = {"10000011", "00111111", "11110100", "11111000", "00011111"};
    ValueType value = NULL;
    for (int i = 0; i < sizeof(ipAddrs)/sizeof(ipAddrs[0]); i++) {
        if (LongestPrefixMatch(t, ipAddrs[i], &value)) {
            printf("%s ---> %s\n", ipAddrs[i], value);
        } else {
            printf("%s ---> NO Action Specified\n", ipAddrs[i]);
        }
    }  
    ReleaseTrie(t);
}


int main(void) {
    
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    TestDictionary();
    TestLongestPrefixMatch();
    return 0;
}



