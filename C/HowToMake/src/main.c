#include <stdio.h>  // Standard library header files, printf(), scanf()
#include "myadd.h"  // User-defined header files
#include "mysub.h"
#include "main.h"

//int main(int argc, char **argv, char **env) {
//int main(int argc, char **argv) {
//int main(int argc) {
int main(void) {    
    int a = 9000;
    int b = 24;
    printf("Hello COMP9024\n");
    printf("add(%d, %d) = %d\n", a, b, add(a, b));
    printf("sub(%d, %d) = %d\n", a, b, sub(a, b));   
    return 0;
}

