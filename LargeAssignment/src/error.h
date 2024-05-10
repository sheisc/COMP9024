#ifndef ERROR_H
#define ERROR_H

// the declaration of a global variable in C
extern int ErrorCount;

void Error(const char *sccFile, int sccLine, const char *format, ...);
#endif
