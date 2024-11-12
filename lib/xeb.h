#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>

#define TYPE void

#include "Array.h"


// instruction list

#define XEB_INST_LIST() \
  X(some_instruction)


// xeb variable type and return type

#define XEB_TYPE()\
  X(let)\
  X(u8)\
  X(u16)\
  X(u32)\
  X(db)\
  X(fl)\
  X(str)

#define X(name) name,

typedef enum{
  XEB_INST_LIST()
}xeb_keyword;


typedef enum{
  XEB_TYPE()
}xeb_type;

#undef X

typedef struct{
  char* String;
  int len;
}String_builder;


// the idea is that the lexer can read a block of memory passed by argument where there is a description of what 
// he need to do for an instruction. Given the type and the information about the line parsed it construct the 
// machine language to match the purpose of the line.
// xeb_lexer_container is used to understand what the parser analize.

typedef struct{

  xeb_keyword       function;
  xeb_type          type;
  xeb_type          return_type;
  Array             *arg_list;
  Array             *string_args; // String_builder type

}xeb_lexer_container;

// store namespace for function reference

Array* namespace;

#define xeb_start()\
  array_new(namespace);\
  xeb_preprocessor();\
  xeb_compiler();


void xeb_preprocessor();      // check for namespace, remove comment's and check for macro
void xeb_compiler();          // check the sintax, check return type for function
void xeb_lexer();             // the lexer use the output from the compiler to analize and create an intepreded version of the line
void xeb_composer();          // compose the final lexer output
void xeb_binary_builder();    // use the output from the composer and translate the lexer into binary stream, then it compile


// note: xeb_binary_builder is probably never used because in that case i want to create a stream of instruction with the lexer to 
// be directly execute by JAAT. The lexer language is gonna be the JAAT assembly language, if some day i decide to create a platform 
// dedicated version of xeb i'll use the lexer language ( aka the JAAT language ) to create an assembly stream ready to be compiled in 
// a binary executable


#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
