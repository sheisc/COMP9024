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
    long pos = KMPSearch(pattern, text);
    if (pos != -1) {
        printf("%s found at index %ld in %s \n", pattern, pos, text);
    } else {
        printf("%s is not int %s\n", pattern, text);
    }
    return 0;
}



