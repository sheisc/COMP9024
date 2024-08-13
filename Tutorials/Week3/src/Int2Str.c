#include <stdio.h>
#include <stdlib.h>
#include "Int2Str.h"

/*
    Print the prefix

    0x
        hex  
    0b 
        bin  
    0o
        oct
 */
static void PrintPrefix(STACK_ITEM_T base) {
    if (base != 10) {        
        switch (base) {
        case 2:  // 0b
            printf("0b");
            break;
        case 8:  // 0o
            printf("0o");
            break;
        case 16: // 0x
            printf("0x");
            break;
        default:
            break;
        }
    }
}

/*
    Print the hexadecimal/decimal/octal/binary representation (specified by base) of an integer x.

    @x
        the integer to be printed
    @base
        2, 8, 10, 16


    Similar with the following python code.

        $ python3

        >>> x = 20249024

        >>> hex(x)
        '0x134f9c0'

        >>> oct(x)
        '0o115174700'

        >>> bin(x)
        '0b1001101001111100111000000'
 */
void PrintInteger(STACK_ITEM_T x, int base) {
    struct Stack *pStack = CreateStack();
    int r;

    x = (x >= 0 ? x : -x);
    base = (base >= 0 ? base: -base);

    printf("------------- x = %ld, base = %d -------------\n\n", (long) x, base);
    
    // push the remainders onto the stack
    do {
        r = x % base;
        x = x / base;
        printf("push %d\n", r);
        StackPush(pStack, r);
    } while (x != 0);

    printf("\n\nAfter popping (First In Last Out):\n\n");
    PrintPrefix(base);
    // output the remainders in the FILO order
    while(!StackIsEmpty(pStack)) {
        r = StackPop(pStack);
        // see https://www.asciitable.com/
        if (r <= 9) {
            // 1 --> '1' (0x31, 49); ...
            // 1 + 48 == 49
            printf("%c", r + 48);
        } else if (r <= 15) {
            // 10 --> 'A' (0x41, 65);  11 --> 'B', ...
            // 10 + 55 == 65
            printf("%c", r + 55);
        } else {
            // unknown base
            printf("?");
        }
    }
    ReleaseStack(pStack);
    printf("\n");
}

