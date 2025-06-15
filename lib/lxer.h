#ifndef LXER_H
#define LXER_H 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <misc.h>


// lexer tokenizer

typedef enum {
  LXR_COMMA = 0,
  LXR_SEMICOLON,

  LXR_OPEN_BRK = 10,
  LXR_CLOSE_BRK,
  LXR_OPEN_CRL_BRK,
  LXR_OPEN_SQR_BRK,
  LXR_CLOSE_SQR_BRK,
  LXR_QUOTE,
  LXR_DOUBLE_QUOTE,
  LXR_DOT,

  LXR_IF_STATEMENT = 20,
  LXR_WHILE_STATEMENT,
  LXR_RET_STATEMENT,
  LXR_ASSIGNMENT,

  LXR_RETURN_ARROW = 30,

  LXR_DEF_STATEMENT = 40,
  LXT_EXPORT_STATEMENT,
  LXR_MODULE_STATEMENT,
  LXR_IMPORT_STATEMENT,
  LXR_DOUBLE_DOTS,
  LXR_STRUCT,
  LXR_ENUM,
  LXR_FN,
  LXR_CAST,
  LXR_AS_CAST,
  
  LXR_STRING_TYPE = 50,
  LXR_INT_TYPE,
  LXR_DUBLE_TYPE,
  LXR_FLOAT_TYPE,
  LXR_CHAR_TYPE,
  LXR_POINTER_TYPE,

  LXR_LINE_COMMENT = 60,
  LXR_OPEN_COMMENT,
  LXR_CLOSE_COMMENT,


  LXR_SUM_SYMB = 70,
  LXR_SUB_SYMB,
  LXR_MLT_SYMB,
  LXR_DIV_SYMB,
  LXR_GRT_SYBM,
  LXR_LST_SYBM,
  LXR_EQL_SYBM,

  /*
   * Add more token for more 
   * funtionality
   *
   */

  LXR_END_LXR = 256
}LXR_TOKENS;

const char* TOKEN_WD[LXR_END_LXR];

// data structure that hold 
// the token associated with a specific 
// section of the code with a pointer and 
// the length of the element.

typedef struct{
  LXR_TOKENS token;
  char* byte_pointer;
  size_t word_size;
}TOKEN_SLICE;

// lxer data structure

// To use the lxer you must define a const table with the 
// string associated with the desired token, at least for now.
// you can create the table and load it inside the lxer structure.


typedef struct{
  char*   source;
  size_t  source_len;

  char* tokenixer_rh[LXR_END_LXR];
  
  TOKEN_SLICE* stream_out;
  size_t       stream_out_len;
}lxer_head;


void lxer_start_lexing(lxer_head* lh);


#ifndef LXER_IMPLEMENTATION
#define LXER_IMPLEMENTATION

#endif
#endif // lxer
