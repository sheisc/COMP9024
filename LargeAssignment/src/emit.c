#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "emit.h"
#include "lex.h"



/* 
  16-byte aligned

  For example, when ALIGN is 4,  
    16 = 2 * 2 * 2 * 2 = pow(2, 4) = 2**4 (python style)
       = (1 << 4)

  we will have

    STACK_SIZE_ALIGNED(6)  ---> 16
    STACK_SIZE_ALIGNED(16) ---> 16
    STACK_SIZE_ALIGNED(18) ---> 32
    STACK_SIZE_ALIGNED(20) ---> 32

  Low-level bit operations in C, you can skip the following detail.

  To keep it simple, let us assume an integer is only 8-bit long here.

  For example, when s is 6, 

  1.
    s =  00000110

  2. (1 << ALIGN) - 1)  when ALIGN is 4

    (1 << ALIGN) - 1) = 15 = 00001111
  
  3. ~(1 << ALIGN) - 1)

    ~(1 << ALIGN) - 1) = 11110000

  4. ((1 << ALIGN) - 1) + (s)
  
       00000110       
     + 00001111
     ------------------
       00010101    

  5. (((1 << ALIGN) - 1) + (s)) & (~((1 << ALIGN) - 1))

       00010101
    &  11110000
    ----------------
       00010000      =  16                   

  So, we have
    STACK_SIZE_ALIGNED(6)  ---> 16

 */
#define ALIGN 4
#define STACK_SIZE_ALIGNED(s) (((1 << ALIGN) - 1) + (s)) & (~((1 << ALIGN) - 1))

void EmitPrologue(AstFuncDefNodePtr func) {
  /***********************************************************
  Generating the following assembly code at the entry of a function:

  Two physical registers in x86_64 are used to access the call stack.
  See the stack layout in func.c for more about rbp.

  rbp
        the frame pointer
  rsp
        the stack pointer
     
  Example:

    pushq %rbp
    movq %rsp, %rbp
    subq $80, %rsp
  ----------------------------------------------------------------
  The layout of a call stack when we are just entering a function.
  The return address (an address in the code section) on the above call stack 
  is pushed implicitly by the call instruction,
  such that when the callee returns, the control flow can go back to caller's code.

   High
              |             |  <----- caller's frame pointer rbp (active rbp)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  <----- rsp
              |             |   
   Low          

                Call Stack       (The unseen but maybe the most important stack)

  (1) pushq %rbp
       
       Save the caller's rbp (frame pointer)

              |             |  <----- caller's frame pointer rbp (active)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  
              |caller's rbp |  <----- rsp
              |             |

  (2) movq %rsp, %rbp

       Set callee's frame pointer by copying rsp to rbp
  
              |             |  <----- caller's frame pointer rbp  (inactive)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  
              |caller's rbp |  <----- rsp | callee's frame pointer rbp (active rbp)
              |             |
              

  (3) subq $80, %rsp

      Allocate enough stack space for local variables, temporary variables, ....
      The x86_64 call stack grows from high to low.
      So a subq instruction (subtraction) will allocate stack space for the callee.

      High
              |             |  <----- caller's frame pointer rbp (inactive)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  
              |caller's rbp |  <----- callee's frame pointer rbp (active rbp)
              |             |
              |  callee's   |
              |   frame     | 
              |             |
              |             |  <-----  rsp
      Low
  -----------------------------------------------------------              
  ************************************************************/
  EmitComments("# Function Prologue");
  EmitAssembly("pushq %%rbp");
  EmitAssembly("movq %%rsp, %%rbp");
  if (func->frameSize != 0) {
    EmitComments("# Frame size = %d", func->frameSize);
    EmitComments("# Number of Parameters/Locals/Temps = %d(%d registers), %d, %d",
                 func->para_cnt, NUM_OF_ARGS_IN_REGS, func->local_vars_cnt, func->tmpVarNum);
    // See the stack memory layout in func.c
    int result = (NUM_OF_ARGS_IN_REGS + func->local_vars_cnt + func->tmpVarNum) * CPU_WORD_SIZE_IN_BYTES;
    EmitComments("# Frame size = (%d + %d + %d) * %d = %d",
                 NUM_OF_ARGS_IN_REGS, 
                 func->local_vars_cnt, 
                 func->tmpVarNum,
                 CPU_WORD_SIZE_IN_BYTES,
                 result); 
    //assert(func->frameSize == result);
    EmitComments("# 16-byte-aligned frame size = %d", STACK_SIZE_ALIGNED(func->frameSize));
    EmitAssembly("subq $%d, %%rsp", STACK_SIZE_ALIGNED(func->frameSize));
  }
  EmitComments("\n");
}

void EmitEpilogue(void) {
  /********************************
  Generating the following assembly code at the exit of a function:

  Example:

     movq %rbp, %rsp
     popq %rbp
     ret

  The stack layout before the function returns.

      High
              |             |  <----- caller's frame pointer rbp (inactive)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  
              |caller's rbp |  <----- callee's frame pointer rbp (active rbp)
              |             |
              |  callee's   |
              |   frame     | 
              |             |
              |             |  <-----  rsp
      Low
                Call Stack

  (1) movq %rbp, %rsp
      
      Release the stack space allocated to callee, by copying %rbp to %rsp

      High
              |             |  <----- caller's frame pointer rbp (inactive)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  
              |caller's rbp |  <----- callee's frame pointer rbp (active rbp) | rsp
              |             |
              |  callee's   |
              |   frame     | 
              |             |
              |             |  <-----  
      Low

  (2) popq %rbp

      Restore the caller's frame pointer.
      Implicitly, rsp is increased by 8.
      Now, all the local variables in the callee function are released.

      High
              |             |  <----- caller's frame pointer rbp (active rbp)
              |  caller's   |
              |   frame     |
              |             |  
              |__ret_addr___|  <----- rsp
              |caller's rbp |  
              |             |
              |             |
      Low   

   (3) ret

      The return value has been saved in the %rax register by the callee before it returns.
      The ret instruction will pop the return address to the instruction pointer register %rip.
      Then the control flow will go back to the caller function.
      Again, rsp += 8
      

      High
              |             |  <----- caller's frame pointer rbp (active rbp)
              |  caller's   |
              |   frame     |
              |             |  <----- rsp
              |__ret_addr___|  
              |caller's rbp |  
              |             |
              |             |
      Low     

  -------------------------------------------------------------

  Sidetrack:


  C Example:
            long f() {
              ...
              return 9024;
            }

            void g() {
              ...
              n = f();          // we are here in C
            }
 --------------------------------------------------------------
  Assembly code in code section

      g:
              ...
              call f            # we are here in assembly
  ret_addr:
              movq %rax, n      # move the return value to n
              ....
  ---------------------------------
      f: 
              pushq %rbp        # Prologue
              movq %rsp, %rbp
              subq $80, %rsp              
              ...
              movq $9024, %rax

              movq %rbp, %rsp   # Epilogue
              popq %rbp
              ret
  ----------------------------------------------------------------


  *********************************/
  EmitComments("# Function Epilogue");
  EmitAssembly("movq %%rbp, %%rsp");
  EmitAssembly("popq %%rbp");
  EmitAssembly("ret");
  EmitComments("\n");
}

/*
  void DoEmitAssembly(int indents, const char *fmt, ...);

  DoEmitAssembly() is a variadic function in C.
  It can be called with a variable number of arguments.
  
  The number of arguments passed by its caller can be larger than
      the number of named parameters defined in DoEmitAssembly(),

  DoEmitAssembly() has only two named parameters (i.e., indents and fmt).

  So you can imagine that there are some unnamed parameters in a variadic function.
  
  To access these unnamed parameters, we can use the macros defined in <stdarg.h>.
  (va_start, var_arg, ... ).

  If you want to know it inside out, 
  start with 32-bit variadic functions (see COMP9024/Stacks/CallStack/OurPrintf32.c), 
  where all the arguments are pushed on the call stack.

 */
void DoEmitAssembly(int indents, const char *fmt, ...) {
  #define WHILE_SPACE_STRING  "        "

  SccIOFileInfo *fileInfo = GetSccIOFileInfo();
  do {
    /*
      On a 64-bit system, va_list is defined as an array type as follows.  

          typedef struct __va_list_tag __builtin_va_list[1];
          typedef __builtin_va_list va_list.
      
      So when ap is passed to vfprintf(), only ap's address is passed.
      We don't assume ap's content will be the same after vfprintf() returns.
      So in the following if-statement, we create a new va_list object (ap2).      
     */    
    va_list ap;
    va_start(ap, fmt); 
    for (int i = 0; i < indents; i++) {
      fprintf(fileInfo->sccAssemblyFile, WHILE_SPACE_STRING);
    }
    vfprintf(fileInfo->sccAssemblyFile, fmt, ap);
    fprintf(fileInfo->sccAssemblyFile, "\n");
    va_end(ap);
  } while(0);

  // Write output to stdout for debugging scc
  if (fileInfo->sccAssemblyFile != stdout) {
    va_list ap2;
    va_start(ap2, fmt); 
    for (int i = 0; i < indents; i++) {
      fprintf(stdout, WHILE_SPACE_STRING);
    }
    vfprintf(stdout, fmt, ap2);
    fprintf(stdout, "\n");
    va_end(ap2);    
  }
}

