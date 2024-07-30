#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include "Maze.h"

int main(int argc, char **argv, char **env) {
    srandom(time(NULL));

    ExploreMaze();

    printf("After completing the code in Q1-Q5 (Maze.c), please also uncomment line %d in %s:\n\n", (__LINE__ + 1), __FILE__);
    //ExploreMazeRandomly();

    return 0;
}

