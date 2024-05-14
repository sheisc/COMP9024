#include "Int2Str.h"

int main(void) {
    STACK_ITEM_T x = 20249024;

    PrintInteger(x, 10);
    PrintInteger(x, 16);    
    PrintInteger(x, 8);
    PrintInteger(x, 2);
    return 0;
}

