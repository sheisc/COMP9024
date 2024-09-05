#include <stdlib.h>
#include "Int2Str.h"

int main(void) {
    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");
    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png");
        
    STACK_ITEM_T x = 20249024;

    PrintInteger(x, 10);
    PrintInteger(x, 16);    
    //PrintInteger(x, 8);
    //PrintInteger(x, 2);
    return 0;
}

