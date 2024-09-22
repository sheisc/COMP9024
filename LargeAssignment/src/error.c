#include "lex.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// The definition of a global variable in C
// A global variable's default value is 0.
// But a local variable needs to be explicitly initialized.
int ErrorCount;

/*
    Generate diagnostic messages as follows:

    Reported by (./src/stmt.c, 341):   ";" expected in the input file (stdin, 8).

    Reported by (./src/stmt.c, 341):   ";" expected in the input file (tests/Demo.scc, 8).

 */
void Error(const char *sccFile, int sccLine, const char *format, ...) {
    SccIOFileInfo *fileInfo = GetSccIOFileInfo();
    va_list ap;

    ErrorCount++;

    fprintf(stderr, "Reported by (%s, %d):   ", sccFile, sccLine);

    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    if (fileInfo->curLine > 0) {
        fprintf(stderr, " in the input file (%s, %d).\n",
                fileInfo->inputFileName, fileInfo->curLine);
    }
    exit(-1);
}
