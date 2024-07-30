#include <stdio.h>
#include <sys/time.h>

#define ROWS   1024
#define COLS   2048

/*
    img[0][0], img[0][1], ..., img[0][2047],
    img[1][0], img[1][1], ..., img[1][2047],

    ...

    img[1023][0], ....         img[1023][2047]
 */

int img[ROWS][COLS];

/**********************************************************
    Time complexity:

        O(ROWS * COLS)
  
    Access the image column by column.  (Much slower)

       TimeNeeded in F() = 12455973 us

       img[0][0], img[1][0], img[2][0], ...
 **********************************************************/
void F(){
    for(int col = 0; col < COLS; col++){
        for(int row = 0; row < ROWS; row++){
            img[row][col] = 0;
        }
    }
}
/************************************************************
    Time complexity:

        O(ROWS * COLS)

    Access the image row by row.   (Much faster)
  
        TimeNeeded in G() = 1135229 us

        img[0][0], img[0][1], img[0][2],  ...
 ************************************************************/
void G(){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLS; col++){
            img[row][col] = 0;
        }
    }
}

unsigned long get_time_in_us(void){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void Test(void (* func)(void), char *funcName){
    unsigned long from, to;
    from = get_time_in_us();
    for(int i = 0; i < 1000; i++){
        func();
    }
    to = get_time_in_us();
    printf("TimeNeeded in %s = %ld us\n", funcName, to - from);
}

/**************************************************

$ gcc AccessImage.c -o AccessImage
$ ./AccessImage

TimeNeeded in F() = 12455973 us
TimeNeeded in G() = 1135229 us
 **************************************************/
int main(void){
    Test(F, "F()");
    Test(G, "G()");
}




