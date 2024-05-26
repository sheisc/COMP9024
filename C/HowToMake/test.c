int lineNumber;
char *fileName;
#define THIS_YEAR 2024
int main(void) {
    long year = THIS_YEAR;
    lineNumber = __LINE__;
    fileName = __FILE__;
    return 0;
}
