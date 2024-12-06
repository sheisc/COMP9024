#include <stdio.h>
#include <stdlib.h>
#include "KMP.h"


int main(void) {
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    char *text = "acaadaaaababaaba";
    char *pattern = "aaba";
    KMPSearch(pattern, text);

    KMPSearch("aaa", "aaaabaaaa");

    KMPSearch("aabaabaab", "aaaabaaaabaabaabaa");

    KMPSearch("caba", "ddcabacc");       

    return 0;
}



