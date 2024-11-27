#include <stdio.h>
#include <stdlib.h>
#include "RadixSort.h"


int main(void) {
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");

    TestRadixSort();
    return 0;
}



