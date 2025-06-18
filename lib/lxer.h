#ifndef LXER_H
#define LXER_H 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <misc.h>

#define LXER_NOT_IMPLEMENTED(name)\
  fprintf(stdout, "[XEB Internal Message]: "name" is still under development!\n");

#define TAG_MATH()\
  X(LXR_SUM_SYMB)\
  X(LXR_SUB_SYMB)\
  X(LXR_MLT_SYMB)\
  X(LXR_DIV_SYMB)\
  X(LXR_GRT_SYBM)\
  X(LXR_LST_SYBM)\
  X(LXR_EQL_SYBM)

#define TAG_COMMENT()\
  X(LXR_LINE_COMMENT)\
  X(LXR_OPEN_COMMENT)\
  X(LXR_CLOSE_COMMENT)


#define TAG_TYPE()\
  X(LXR_STRING_TYPE)\
  X(LXR_INT_TYPE)\
  X(LXR_DOUBLE_TYPE)\
  X(LXR_FLOAT_TYPE)\
  X(LXR_CHAR_TYPE)\
  X(LXR_POINTER_TYPE)\
  X(LXR_VOID_TYPE)


#define TAG_SEP()\
  X(LXR_COMMA)\
  X(LXR_SEMICOLON)\
  X(LXR_DOUBLE_DOTS)\
  X(LXR_DOT)\
  X(LXR_QUOTE)\
  X(LXR_DOUBLE_QUOTE)\
  X(LXR_TYPE_ASSIGN)


#define TAG_BRK()\
  X(LXR_OPEN_BRK)\
  X(LXR_CLOSE_BRK)\
  X(LXR_OPEN_CRL_BRK)\
  X(LXR_CLOSE_CRL_BRK)\
  X(LXR_OPEN_SQR_BRK)\
  X(LXR_CLOSE_SQR_BRK)
  
#define TAG_STATEMENT()\
  X(LXR_IF_STATEMENT)\
  X(LXR_WHILE_STATEMENT)\
  X(LXR_RET_STATEMENT)\
  X(LXR_ASSIGNMENT)\
  X(LXT_EXPORT_STATEMENT)\
  X(LXR_MODULE_STATEMENT)\
  X(LXR_IMPORT_STATEMENT)

#define TAG_MISC()\
  X(LXR_RETURN_ARROW)\
  X(LXR_DEF_STATEMENT)\
  X(LXR_STRUCT)\
  X(LXR_ENUM)\
  X(LXR_FN)\
  X(LXR_AS_CAST)


// lexer tokenizer
#define X(name) name,

typedef enum {
  TAG_MATH()
  TAG_MATH_END,
  
  TAG_SEP()
  TAG_SEP_END,

  TAG_COMMENT()
  TAG_COMMENT_END,

  TAG_BRK()
  TAG_BRK_END,

  TAG_TYPE()
  TAG_TYPE_END,

  TAG_STATEMENT()
  TAG_STATEMENT_END,

  TAG_MISC()
  TOKEN_TABLE_END
}LXR_TOKENS;

#undef X

#define X(name) name,

static const LXR_TOKENS token_array[] = {
  TAG_MATH()
  TAG_MATH_END,
  TAG_COMMENT()
  TAG_COMMENT_END, 
  TAG_TYPE()
  TAG_TYPE_END,
  TAG_SEP()
  TAG_SEP_END,
  TAG_BRK()
  TAG_BRK_END,
  TAG_STATEMENT()
  TAG_STATEMENT_END,
  TAG_MISC()
};

#undef X

// data structure that hold 
// the token associated with a specific 
// section of the code with a pointer and 
// the length of the element.

typedef struct{
  LXR_TOKENS token;
  char* byte_pointer;
}token_slice;

// lxer data structure

// To use the lxer you must define a const table with the 
// string associated with the desired token, at least for now.
// you can create the table and load it inside the lxer structure.


typedef struct{
  char*   source;
  size_t  source_len;
  size_t  lxer_tracker;

  token_slice** stream_out;
  size_t       stream_out_len;
  Arena_header lxer_ah;
}lxer_head;


static char* token_table_lh[] = {
  [LXR_SUM_SYMB] = "+",
  [LXR_SUB_SYMB] = "-",
  [LXR_MLT_SYMB] = "*",
  [LXR_DIV_SYMB] = "/",
  [LXR_GRT_SYBM] = ">",
  [LXR_LST_SYBM] = "<",
  [LXR_EQL_SYBM] = "==",
  
  [LXR_LINE_COMMENT] = "--",
  [LXR_OPEN_COMMENT] = "-/",
  [LXR_CLOSE_COMMENT] = "/-",
  
  [LXR_STRING_TYPE] = "string",
  [LXR_INT_TYPE] = "int",
  [LXR_DOUBLE_TYPE] = "double",
  [LXR_FLOAT_TYPE] = "float",
  [LXR_CHAR_TYPE] = "char",
  [LXR_POINTER_TYPE] = "ptr",
  [LXR_VOID_TYPE] = "void",
  
  [LXR_COMMA] = ",",
  [LXR_SEMICOLON] = ";",
  [LXR_DOUBLE_DOTS] = "::",
  [LXR_DOT] = ".",
  [LXR_QUOTE] = "'",
  [LXR_DOUBLE_QUOTE] = "\"",
  [LXR_TYPE_ASSIGN] = ":",

  [LXR_OPEN_BRK] = "(",
  [LXR_CLOSE_BRK] = ")",
  [LXR_OPEN_CRL_BRK] = "{",
  [LXR_CLOSE_CRL_BRK] = "}",
  [LXR_OPEN_SQR_BRK] = "[",
  [LXR_CLOSE_SQR_BRK] = "]",

  [LXR_IF_STATEMENT] = "if",
  [LXR_WHILE_STATEMENT] = "while",
  [LXR_RET_STATEMENT] = "ret",
  [LXR_ASSIGNMENT] = "=",
  [LXT_EXPORT_STATEMENT] = "export ",
  [LXR_MODULE_STATEMENT] = "module ",
  [LXR_IMPORT_STATEMENT] = "import ",

  [LXR_RETURN_ARROW] = "->",
  [LXR_DEF_STATEMENT] = "def ",
  [LXR_STRUCT] = "struct ",
  [LXR_ENUM] = "enum ",
  [LXR_FN] = "fn ",
  [LXR_AS_CAST] = " as "
};


static size_t token_table_length = TOKEN_TABLE_END;

void lxer_start_lexing(lxer_head* lh, char* source_file);
void lxer_get_lxer_content(lxer_head*lh);

bool lxer_next_token(lxer_head*lh);
LXR_TOKENS lxer_get_current_token(lxer_head*lh);
char* lxer_get_current_pointer(lxer_head*lh);

bool lxer_next_token(lxer_head*lh);
bool lxer_is_math(LXR_TOKENS token);
bool lxer_is_comment(LXR_TOKENS token);
bool lxer_is_type(LXR_TOKENS token);
bool lxer_is_sep(LXR_TOKENS token);
bool lxer_is_brk(LXR_TOKENS token);
bool lxer_is_statement(LXR_TOKENS token);
bool lxer_is_misc(LXR_TOKENS token);

bool lxer_math_expect_math(lxer_head*lh);
bool lxer_math_expect_comment(lxer_head*lh);
bool lxer_math_expect_type(lxer_head*lh);
bool lxer_math_expect_sep(lxer_head*lh);
bool lxer_math_expect_brk(lxer_head*lh);
bool lxer_math_expect_statement(lxer_head*lh);
bool lxer_math_expect_misc(lxer_head*lh);

bool lxer_comment_expect_math(lxer_head*lh);
bool lxer_comment_expect_comment(lxer_head*lh);
bool lxer_comment_expect_type(lxer_head*lh);
bool lxer_comment_expect_sep(lxer_head*lh);
bool lxer_comment_expect_brk(lxer_head*lh);
bool lxer_comment_expect_statement(lxer_head*lh);
bool lxer_comment_expect_misc(lxer_head*lh);

bool lxer_type_expect_math(lxer_head*lh);
bool lxer_type_expect_comment(lxer_head*lh);
bool lxer_type_expect_type(lxer_head*lh);
bool lxer_type_expect_sep(lxer_head*lh);
bool lxer_type_expect_brk(lxer_head*lh);
bool lxer_type_expect_statement(lxer_head*lh);
bool lxer_type_expect_misc(lxer_head*lh);

bool lxer_sep_expect_math(lxer_head*lh);
bool lxer_sep_expect_comment(lxer_head*lh);
bool lxer_sep_expect_type(lxer_head*lh);
bool lxer_sep_expect_sep(lxer_head*lh);
bool lxer_sep_expect_brk(lxer_head*lh);
bool lxer_sep_expect_statement(lxer_head*lh);
bool lxer_sep_expect_misc(lxer_head*lh);


bool lxer_brk_expect_math(lxer_head*lh);
bool lxer_brk_expect_comment(lxer_head*lh);
bool lxer_brk_expect_type(lxer_head*lh);
bool lxer_brk_expect_sep(lxer_head*lh);
bool lxer_brk_expect_brk(lxer_head*lh);
bool lxer_brk_expect_statement(lxer_head*lh);
bool lxer_brk_expect_misc(lxer_head*lh);


bool lxer_statement_expect_math(lxer_head*lh);
bool lxer_statement_expect_comment(lxer_head*lh);
bool lxer_statement_expect_type(lxer_head*lh);
bool lxer_statement_expect_sep(lxer_head*lh);
bool lxer_statement_expect_brk(lxer_head*lh);
bool lxer_statement_expect_statement(lxer_head*lh);
bool lxer_statement_expect_misc(lxer_head*lh);

bool lxer_misc_expect_math(lxer_head*lh);
bool lxer_misc_expect_comment(lxer_head*lh);
bool lxer_misc_expect_type(lxer_head*lh);
bool lxer_misc_expect_sep(lxer_head*lh);
bool lxer_misc_expect_brk(lxer_head*lh);
bool lxer_misc_expect_statement(lxer_head*lh);
bool lxer_misc_expect_misc(lxer_head*lh);



char*   lxer_get_rh(lxer_head*lh, bool reverse);
char**  lxer_get_rh_lh(lxer_head*lh);


#ifndef LXER_IMPLEMENTATION
#define LXER_IMPLEMENTATION

#endif
#endif // lxer
