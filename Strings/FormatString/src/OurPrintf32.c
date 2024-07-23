#include <stdio.h>
#include <stdarg.h>

// 32-bit x86
//#define ON_A_32_BIT_SYSTEM

/*
    Use va_start() and va_arg() to access the unnamed parameters of a variadic function in C.

    int printf(const char *format, ...);
 */
void OurPrintf_V1(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("OurPrintf_V1(): ");        
    while (*fmt) {
        if (*fmt == '%') { // %: This is the leading sign that denotes the beginning of the format specifier
            fmt++;
            if (*fmt == 'd') {
                int val = va_arg(ap, int);
                printf("%d", val);
                // flush the buffered data, for debugging
                fflush(stdout);
                fmt++;
            }
            else if (*fmt == 'f') {
                double val = va_arg(ap, double);
                printf("%f", val);
                fflush(stdout);
                fmt++;
            }              
            else {
                //...
            }
        }
        else {  // regular character
            printf("%c", *fmt);
            fflush(stdout);
            fmt++;
        }
    }
    va_end(ap);
}


/*
    Pass a va_list to vprintf()

        int vprintf(const char *format, va_list ap);
        
 */
void OurPrintf_V2(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    printf("OurPrintf_V2(): ");
    vprintf(fmt, ap);
    va_end(ap);
}

#ifdef ON_A_32_BIT_SYSTEM
/*
    In OurPrintf32_V3(), we do the pointer arithmetic by ourselves on a 32-bit system.
    It is not portable.
    But it can help us get a big picture of what is going on under the hood.   
 */
void OurPrintf32_V3(char *fmt, ...) {
    char *ap;                                   // similar with but not necessarily equal to 'va_list ap';
    ap = ((char *) &fmt) + sizeof(char *);      // similar with va_start(ap, fmt);
    printf("OurPrintf_V3(): ");
    // Now ap points to the unnamed parameters.               
    while (*fmt) {
        if (*fmt == '%') { // %: This is the leading sign that denotes the beginning of the format specifier
            fmt++;
            if (*fmt == 'd') {
                int ival = *((int *) ap);        // similar with va_arg(ap, int)
                ap += sizeof(int);

                printf("%d", ival);
                // flush the buffered data, for debugging
                fflush(stdout);
                fmt++;
            }
            else if (*fmt == 'f') {  
                double fval = *((double *) ap);  // similar with va_arg(ap, double)
                ap += sizeof(double);
                printf("%f", fval);
                fflush(stdout);
                fmt++;
            }
            else {
                //...
            }
        }
        else {  // regular character
            printf("%c", *fmt);
            fflush(stdout);
            fmt++;
        }
    }
    ap = NULL;                                  // similar with va_end(ap)
}
#endif

int main(void) {
    int a = 30;
    int b = 40;
    int c = 50;
    double d = 3.14;
    printf("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
    OurPrintf_V1("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
    OurPrintf_V2("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
#ifdef  ON_A_32_BIT_SYSTEM   
    OurPrintf32_V3("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
#endif
    return 0;
}
