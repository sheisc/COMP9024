#include "decl.h"
#include "emit.h"
#include "error.h"
#include "expr.h"
#include "func.h"
#include "lex.h"
#include "stmt.h"
#include <stdio.h>  // printf()
#include <stdlib.h> // exit()
#include <string.h> // memset(), strncpy()

// scc source code in memory
static const char *srcCode =
    "main() { long year; year = 2024; output(year); return 0;}";

// get the next char from memory
//__attribute__((unused))
static int NextCharFromMem(void) {
    int ch = (unsigned char)(*srcCode);
    srcCode++;
    if (ch == 0) {
        return SCC_EOF;
    } else {
        return ch;
    }
}

// get the next char from an input file
static int NextCharFromInputFile(void) {
    SccIOFileInfo *fileInfo = GetSccIOFileInfo();
    int ch = fgetc(fileInfo->sccSrcFile);
    if (ch == EOF) {
        return SCC_EOF;
    } else {
        return ch;
    }
}

static void ReleaseResources(AstStmtNodePtr decls, AstFuncDefNodePtr funcs) {
    SccIOFileInfo *fileInfo = GetSccIOFileInfo();
    // close files
    if (fileInfo->sccSrcFile) {
        fclose(fileInfo->sccSrcFile);
    }
    if (fileInfo->sccAssemblyFile) {
        fclose(fileInfo->sccAssemblyFile);
    }
    // FIXME:
    // Free the heap memory allocated via malloc().
    // SCC is not a long-time running server.
    // Let the OS do it for us :)
}

/*
    1. When the number of command-line arguments is 2, 
     e.g., 

        ./scc ./tests/Factorial.scc

     scc will open the input file specified in arg[1] ("./tests/Factorial.scc")
     and 
     creat/open an output file ("./tests/Factorial.scc.s") for saving the assembly code generated.
  
    
    2. In other cases,
     scc will read the source code from the standard input (from the keyboard), 
     and write the assembly code generated to the standard output (to the screen).

     But SHELL allows us to do I/O redirection.
     In the following command, the standard input will be redirected to "./tests/Factorial.scc"
     and the standard output will be "./tests/Factorial.scc.s".

        ./scc < ./tests/Factorial.scc > ./tests/Factorial.scc.s

     So, scc will read from "./tests/Factorial.scc" and write to "./tests/Factorial.scc.s".

    3.     
    The global variables stdin/stdout/stderr in C library stand for Standard Input/Output/Error.
     
        int number;

        // read from the standard input
        scanf("%d", &number);
        fscanf(stdin, "%d", &number);

        // write to the standard output
        printf("%d\n", number);
        fprintf(stdout, "%d\n", number);

        // write to the standard error
        perror("something wrong\n");
        fprintf(stderr, "something wrong\n")

 */
void InitSccIOFileInfo(int argc, char **argv) {
    SccIOFileInfo *fileInfo = GetSccIOFileInfo();
    if (argc == 2) {
        fileInfo->sccSrcFile = fopen(argv[1], "r");
        fileInfo->inputFileName = argv[1];
        if (fileInfo->sccSrcFile == NULL) {
            Error(__FILE__, __LINE__, "Can't open the input file '%s'\n",
                  argv[1]);
            exit(-1);
        }
#define MAX_INPUT_LEN 1020
        char outputFileName[MAX_INPUT_LEN + 4];
        memset(outputFileName, 0, sizeof(outputFileName));
        // Demo.scc  ---> Demo.scc.s
        strncpy(outputFileName, argv[1], MAX_INPUT_LEN);
        strcat(outputFileName, ".s");
        fileInfo->sccAssemblyFile = fopen(outputFileName, "w");
        if (fileInfo->sccAssemblyFile == NULL) {
            Error(__FILE__, __LINE__, "Can't open the output file'%s'\n",
                  outputFileName);
            // close the input file.
            fclose(fileInfo->sccSrcFile);
            exit(-1);
        }
        InitLexer(NextCharFromInputFile);
    } else {
        // Use the default stand input/output
        fileInfo->sccSrcFile = stdin;
        fileInfo->sccAssemblyFile = stdout;
        fileInfo->inputFileName = "stdin";

        InitLexer(NextCharFromInputFile);
        /***************************************************
          If you want to get the next char from memory, rather than stdin,
          just call:
              InitLexer(NextCharFromMem);
    
          Note:
              No command-line arguments provided for scc.
              If you use Visual Studio Code to debug scc in this case, 
              set the "args" to be empty in launch.json (as follows)
                  "args": [],
         ***************************************************/
        //InitLexer(NextCharFromMem);
    }
}

int main(int argc, char **argv) {
    AstStmtNodePtr decls = NULL;
    AstFuncDefNodePtr funcs = NULL;

    InitSccIOFileInfo(argc, argv);

    /**************************************************************************
     1. Create an AST tree
     **************************************************************************/
    NEXT_TOKEN;
    decls = GlobalDeclStatements();
    funcs = FunctionDefinitions();
    Expect(TK_EOF);

    EmitIntroduction("# **************************** Auto-Generated by scc  "
                     "*******************************#");
    EmitIntroduction("#                                                        "
                     "                            #");
    EmitIntroduction("# 1. For better understanding of C, recursion, call "
                     "stack, tree and linked list.     #");
    EmitIntroduction("#                                                        "
                     "                            #");
    EmitIntroduction("# 2. No code optimization.                               "
                     "                            #");
    EmitIntroduction("#                                                        "
                     "                            #");
    EmitIntroduction("# 3. Three columns in the assembly file: assembly code, "
                     "IR, and comments             #");
    EmitIntroduction("#                                                        "
                     "                            #");
    EmitIntroduction("#                                                        "
                     "   COMP9024                 #");
    EmitIntroduction("# "
                     "*********************************************************"
                     "**************************#\n\n\n");

    EmitComments("# Data Section");
    /*******************************************************
        .data
        .input_fmtstr:	.string	"%ld"
        .output_fmtstr:	.string	"%ld\012"
     ********************************************************/
    EmitLabel(".data");
    EmitAssembly("%s: .string \"%%ld\"", INPUT_FORMAT_STR_NAME);
    EmitAssembly("%s: .string \"%%ld\\012\"", OUTPUT_FORMAT_STR_NAME);

    /**************************************************************************
        2. Traverse the created AST tree
     **************************************************************************/
    EmitStatementNode(decls);
    /*********************************
        .text
        .globl	main
        main:
    **********************************/
    EmitFuncDefNode(funcs);
    /* 
        Make the CSE vx19 machine happy.
        	  .section	.note.GNU-stack,"",@progbits
        Otherwise, it complains:
           /usr/bin/ld: warning: /tmp/ccbrczqL.o: missing .note.GNU-stack section implies executable stack
           /usr/bin/ld: NOTE: This behaviour is deprecated and will be removed in a future version of the linker
     */
    EmitAssembly("\n\n   .section .note.GNU-stack,\"\",@progbits\n");
    // Close opened files and free heap memory
    ReleaseResources(decls, funcs);
    return 0;
}
