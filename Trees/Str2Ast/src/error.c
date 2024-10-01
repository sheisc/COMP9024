#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void Error(const char *format, ...) {
    va_list ap;

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fprintf(stderr, "\n");

    exit(-1);
}
