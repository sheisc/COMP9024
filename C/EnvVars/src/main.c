#include <stdio.h>  // Standard library header files, printf(), scanf()
#include <stdlib.h> // getenv(), ...

/*
    Suppose we execute "./main aa bb cc dd".
    
    Memory Layout
   
    ///////////////// Command Line Arguments ///////////////////////

                 -------------
                     NULL
                  argv[argc-1]    ---------->  "dd"

                    ....

                    argv[1]       ---------->  "aa"
    argv  ------>   argv[0]       ---------->  "./main"
                  -------------     
    char **       array of (char *)
                 
    ///////////////// Environment Variables ///////////////////////

                 -------------
                     NULL
                     ...         ---------->  "...=....."

                     ...

                    env[1]       ---------->  "...=....."
    env  ------>    env[0]       ---------->  "SHELL=/bin/bash"
                  -------------     
    char **       array of (char *)                 
  */

int main(int argc, char **argv, char **env) {    
    printf("\n\n*************** COMP9024 TestEnvironment() ***************\n\n");

    // print the command-line arguments
    printf("\n");
    printf("argc = %d\n\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
        //printf("argv[%d] = %s\n", i, *(argv+i));
    }

    // print the command-line arguments in another way
    char **pStr = argv;
    printf("\n");
    while (*pStr != NULL) {
        printf("%s\n", *pStr);
        pStr++;
    }
   
    // print all the environment variables (passed to this program by the shell)
    pStr = env;
    printf("\n\n");
    while (*pStr != NULL) {
        printf("%s\n", *pStr);
        pStr++;
    }

    //
    printf("\n");
    char *name = getenv("HOME");
    char *shell = getenv("SHELL");
    if ((name != NULL) && shell) {
        printf("Your HOME directory is %s.\nYour SHELL is %s.\n", name, shell);    
    }  
    return 0;
}


