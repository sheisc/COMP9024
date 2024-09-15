/****************************************************************************************
                Recursive functions and call stack

    1.  Observe the stack frames when calling a recursive function

    2.  Different versions/instances of the local variable 'n' in Factorial(n)

    3.  The Memory Layout of a Linux Process (userspace)

                                                                    COMP9024

 ******************************************************************************************/

#include <stdio.h>     // printf(), scanf()
#include "MemoryLayout.h"

long Factorial(long n) {
    printf("Factorial(): n = %ld, &n = %p\n", n, &n);
    if (n <= 1) {
        // the base case for the recursive function
        return 1;
    } else {
        long result = Factorial(n - 1);
        result = result * n;
        return result;
    }
}

int main(void) {
    long n = 4;
    printf("main(): n = %ld, &n = %p\n", n, &n);

    long x = Factorial(n);

    printf("\nFactorial(%ld) = %ld\n", n, x);    

    TestMemoryLayout();
    return 0;
}
