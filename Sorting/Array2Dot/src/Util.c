#include <stdio.h>
#include <stdlib.h>

/*
    Swap the values of the two integer variables pointed to by pa and pb, respectively.  
 */
void Swap(int *pa, int *pb) {
    int tmp = *pa;
    *pa = *pb;
    *pb = tmp;
}


/*
    void PrintArray(int *ptr, int n);

        Print the values of the n integer variables pointed to by an pointer ptr:

            ptr[0],  ptr[1],    ptr[2],    ...,  ptr[n-1]

        or 
            *ptr,   *(ptr+1),  *(ptr+2),   ...,  *(ptr + n -1)

 */
void PrintArray(int *ptr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", ptr[i]);
    }
    printf("\n");
}

