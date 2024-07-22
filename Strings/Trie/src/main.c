#include <stdio.h>
#include <stdlib.h>
#include "Trie.h"

// To be simple, support lowercases only
static char *words[] = { "ear", "apply", "ape", "apes", "earth", "east",
		  "app", "ace", "early", "earl", "aces" };

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

int main(void) {
    long cnt = 1;
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    Trie t = CreateTrie();

    int n = sizeof(words)/sizeof(words[0]);   
    for (int i = 0; i < n; i++) {
        t = TrieInsert(t, words[i], meanings[i]);
        GenOneImage(t, "TrieInsert", "images/TrieInsert", cnt);
        cnt++;
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
    return 0;
}



