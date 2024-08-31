#include <stdio.h>
#include <stdlib.h>     // srandom(), random()
#include <time.h>       // time()
#include "Maze.h"

int main(int argc, char **argv, char **env) {
    srandom(time(NULL));

    // create a sub-directory 'images' (if it is not present) in the current directory
    system("mkdir -p images");

    // remove the *.dot and *.png files in the directory 'images'
    system("rm -f images/*.dot images/*.png"); 
    
    ExploreMaze();

    printf("After completing the code in Q1-Q5 (Maze.c), please also uncomment line %d in %s:\n\n", (__LINE__ + 1), __FILE__);
    //ExploreMazeRandomly();

    return 0;
}

