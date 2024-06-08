#include <unistd.h>    // fork(), execl()
#include <stdio.h>     // fprintf()
#include <stdlib.h>    // system()
#include <string.h>    // strlen()
#include <sys/wait.h>  // waitpid()

/*
    In the command line:
    $ man fgets

    //////////////////////////////////////////////////////////////////////////
    char *fgets(char *s, int size, FILE *stream);
    DESCRIPTION
    fgets()  reads in at most one less than size characters from stream and
    stores them into the buffer pointed to by s.  Reading  stops  after  an
    EOF  or a newline.  If a newline is read, it is stored into the buffer.
    A terminating null byte ('\0') is stored after the  last  character  in
    the buffer.
    /////////////////////////////////////////////////////////////////////////

    $ man system

    int system(const char *command);
    The  system()  library  function uses fork() to create a child process
    that executes the shell command specified in command using execl()  as
    follows:
        execl("/bin/sh", "sh", "-c", command, (char *) NULL);
    system() returns after the command has been completed.
 */

#define MAX_LINE_LENGTH 1024

char commandBuf[MAX_LINE_LENGTH];

#if 1
int main(void) {
    printf("#############################   Simple Shell  ############################\n");
    printf("Press ctrl+c to exit.\n\n");
    //print prompt
    printf("%% ");
    while (fgets(commandBuf, MAX_LINE_LENGTH, stdin) != NULL) {
        // replace newline with null
        if (commandBuf[strlen(commandBuf) - 1] == '\n') {
            commandBuf[strlen(commandBuf) - 1] = 0; 
        }
        // On Linux: fork() + execl()
        system(commandBuf);
        printf("%% ");
    }
    return 0;
}

#else

int main(void) {
    pid_t pid;
    int status;
    printf("#############################   Simple Shell  ############################\n");
    printf("Only supporting commands without any arguments: python3, who, pwd, ls, ...\n");
    printf("Press ctrl+c to exit.\n\n");
    // print prompt
    printf("%% ");
    while (fgets(commandBuf, MAX_LINE_LENGTH, stdin) != NULL) {
        // replace newline with null
        if (commandBuf[strlen(commandBuf) - 1] == '\n') {
            commandBuf[strlen(commandBuf) - 1] = 0; 
        }

        if ((pid = fork()) < 0) {
            fprintf(stderr, "fork error\n");
        } else if (pid == 0) { // child process
            execlp(commandBuf, commandBuf, (char *)0);
            // execl("/bin/sh", "sh", "-c", commandBuf, (char *) NULL);
            fprintf(stderr, "couldn't execute: %s\n", commandBuf);
            exit(127);
        } else { // parent process
            if ((pid = waitpid(pid, &status, 0)) < 0) {
                fprintf(stderr, "waitpid error\n");
            }
        }
        printf("%% ");
    }
    return 0;
}
#endif

