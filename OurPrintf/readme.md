# Variadic Functions in C

## 1. Please install gcc-multilib and g++-multilib as follows.

```
sudo apt-get install gcc-multilib g++-multilib
```

## 2. Variadic functions on a 32-bit system

### Call Stack Memory Layout

```C

void OurPrintf32_V3(char *fmt, ...) {
    char *ap;                                   // similar with but not necessarily equal to 'va_list ap';
    ap = ((char *) &fmt) + sizeof(char *);      // similar with va_start(ap, fmt);
    // Now ap points to the unnamed parameters.               
    while (*fmt) {
        if (*fmt == '%') { // %: This is the leading sign that denotes the beginning of the format specifier
            fmt++;
            if (*fmt == 'd') {
                int ival = *((int *) ap);        // similar with va_arg(ap, int)
                ap += sizeof(int);

                printf("%d", ival);
                fmt++;
            }
            else if (*fmt == 'f') {  
                double fval = *((double *) ap);  // similar with va_arg(ap, double)
                ap += sizeof(double);
                printf("%f", fval);
                fmt++;
            }          
            else {
                //...
            }                 
        }
        else {  // regular character
            printf("%c", *fmt);
            fmt++;
        }
    }
    ap = NULL;                                  // similar with va_end(ap)
}

int main(void) {
    int a = 30;
    int b = 40;
    int c = 50;
    double d = 3.14;
    ....
    OurPrintf32_V3("a = %d, b = %d, c = %d, d = %f \n", a, b, c, d);
    return 0;
}

################################################################################


      High Address
                      __|_________________________|__  
                        |                         |    
                        |                         | 
                    a   |      30                 |  
                    b   |      40                 |  
                    c   |      50                 |   
                    d   |      3.14               |           
                        |                         |
         main()         |                         |         
                        |                         |  
                        |                         |
                        |         3.14            |                  "a = %d, b = %d, c = %d, d = %f \n"
                        |          50             |                   ^
                        |          40             |                   +
         +------------->|          30             |                   +         
         +              |          fmt            |-------------------+  
         +            __|_______return address____|__   
         +              |                         |                       Global Data Area
         ---------------|          ap             |  
                        |                         |        
      OurPrintf32_V3()  |                         | 
                        |_________________________|__
                        |                         | 

                              Call Stack

    
      Low Address
                                                       

```                                                       

## 3. Variadic functions on a 64-bit system 

Since some function arguments are passed via physical registers 
[System V AMD64 ABI](https://en.wikipedia.org/wiki/X86_calling_conventions) 
in AMD64 or Intel's CPU x86_64,
the memory layout of a variadic function on a 64-bit system is much more complex.

**OurPrintf32_V3()** is only used to show what's going on under the hood on a 32-bit system. 
It is not portable.

But the high-level usage of va_list, va_start, va_arg, and va_end on a 64-bit system
is still the same as a 32-bit system.

If we understand **OurPrintf32_V3()** and the **Call Stack**, a variadic function can be much easier to us.





