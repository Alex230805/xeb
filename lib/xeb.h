#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <toolbox.h>

typedef enum {
  RETURN_ARROW,
  OPEN_PAR,
  CLOSE_PAR,
  OPEN_GRAPH,
  CLOSE_GRAPH,
  OPEN_QUAD,
  CLOSE_QUAD,
  DOUBLE_COLON,
  ASSIGNMENT,

  SYM_COUNT,
}SYM;


typedef enum {
  FUNCTION,
  DEFINITION,
  SYSTEM,
  MODULE,
  IMPORT,

  DEF_COUNT
}DEF;


typedef enum {
  VOID,
  LET,
  CONST,
  UINT8,
  UINT16,
  UINT32,
  UINT64,
  DOUBLE,
  FLOAT,
  STRING,

  TYP_COUNT
}TYP;

typedef enum {
  PLUS,
  MINUS,
  MULTIPLICATION,
  DIVISION,
  AND,
  OR,
  EQUAL,
  DIFFERENT,
  GREATER,
  MINOR,
  INC_GREATER,
  INC_MINOR,

  OPR_COUNT
}OPR;

typedef enum {
  DOUBLE_DASH,

  CMT_COUNT
}CMT;

typedef enum {
  COMMA,
  SEMICOLON,

  SPR_COUNT
}SPR;

typedef enum {
  IF,
  ELSE,
  FOR,
  RETURN,
  RET,

  KWD_COUNT
}KWD;

static char *symbols[SYM_COUNT] = {
  [RETURN_ARROW]       = "->",
  [OPEN_PAR]          = "(",
  [CLOSE_PAR]         = ")",
  [OPEN_GRAPH]        = "{",
  [CLOSE_GRAPH]       = "}",
  [OPEN_QUAD]         = "[",
  [CLOSE_QUAD]        = "]",
  [DOUBLE_COLON]      = "::",
  [ASSIGNMENT]        = "="
};

static char *keywords[KWD_COUNT] = {
  [IF]                = "if",
  [ELSE]              = "else",
  [RETURN]            = "return",
  [RET]               = "ret"
};

static char *definitions[DEF_COUNT] = {
  [FUNCTION]          = "fn",
  [DEFINITION]        = "def",
  [SYSTEM]            = "@system",
  [MODULE]            = "module",
  [IMPORT]            = "import"
};

static char *types[TYP_COUNT] = {
  [VOID]              = "void",
  [LET]               = "let",
  [CONST]             = "const",
  [UINT8]             = "u8",
  [UINT16]            = "u16",
  [UINT32]            = "u32",
  [UINT64]            = "u64",
  [DOUBLE]            = "db",
  [FLOAT]             = "fl",
  [STRING]            = "str"
};


static char *operators[OPR_COUNT] = {
  [PLUS]              = "+",
  [MINUS]             = "-",
  [MULTIPLICATION]    = "*",
  [DIVISION]          = "/",
  [AND]               = "&&",
  [OR]                = "||",
  [EQUAL]             = "==",
  [DIFFERENT]         = "!=",
  [GREATER]           = ">",
  [MINOR]             = "<",
  [INC_GREATER]       = ">=",
  [INC_MINOR]         = "<="
};

static char *comments[CMT_COUNT] = {
  [DOUBLE_DASH]       = "--"
};

static char *separators[SPR_COUNT] = {
  [COMMA]             = ",",
  [SEMICOLON]         = ";",
};

static Array* namespaces_occ;
static Array* function_definitions_occ;
static Array* linker_reference_occ;
static Array* comments_position_occ;


static Array* namespaces;
static Array* function_definitions;
static Array* linker_reference;
static Array* comments_position;

void xeb_lexer();
int xeb_compiler(String_builder*path,String_builder*output_path);          // check the sintax, check return type for function
//void xeb_composer();          // compose the final lexer output
void xeb_binary_builder();    // use the output from the composer and translate the lexer into binary stream, then it compile


// note: xeb_binary_builder is probably never used because in that case i want to create a stream of instruction with the lexer to 
// be directly execute by JAAT. The lexer language is gonna be the JAAT assembly language, if some day i decide to create a platform 
// dedicated version of xeb i'll use the lexer language ( aka the JAAT language ) to create an assembly stream ready to be compiled in 
// a binary executable


#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
