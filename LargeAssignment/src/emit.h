#ifndef EMIT_H
#define EMIT_H
#include <stdarg.h>
#include "func.h"
/*******************************************************
  .data
  .input_fmtstr:	.string	"%ld"
  .output_fmtstr:	.string	"%ld\012"
 *******************************************************/
#define INPUT_FORMAT_STR_NAME ".input_fmtstr"
#define OUTPUT_FORMAT_STR_NAME ".output_fmtstr"

//void EmitAssembly(const char *fmt, ...);
//void EmitLabel(const char *fmt, ...);
//#define EmitComments EmitLabel

void DoEmitAssembly(int indents, const char *fmt, ...);

/******************************************************************
                     Variadic Macros
 Example:
    Given a macro invocation

      EmitAssembly("subq $%d, %%rsp", STACK_SIZE_ALIGNED(frameSize))

    We have this expansion

      do {
        DoEmitAssembly(1, "subq $%d, %%rsp", STACK_SIZE_ALIGNED(frameSize));
      } while(0)


 See the first two paragraphs about eprintf(...) in the following web page.

    https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html

  For the "do {} while(0)" macro, please refer to 

    https://stackoverflow.com/questions/1067226/c-multi-line-macro-do-while0-vs-scope-block

 ******************************************************************/
#define EmitAssembly(...) do { \
  DoEmitAssembly(1, __VA_ARGS__); \
} while(0)

#define EmitComments(...) do { \
  DoEmitAssembly(9, __VA_ARGS__); \
} while(0)

#define EmitLabel(...) do { \
  DoEmitAssembly(0, __VA_ARGS__); \
} while(0)

#define EmitIntroduction(...) do { \
  DoEmitAssembly(0, __VA_ARGS__); \
} while(0)

#define EmitIR(...) do { \
  DoEmitAssembly(5, __VA_ARGS__); \
} while(0)

void EmitPrologue(AstFuncDefNodePtr func);
void EmitEpilogue(void);

#endif
