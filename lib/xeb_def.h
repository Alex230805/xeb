#ifndef XEB_DEF_H
#define XEB_DEF_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <misc.h>

#define u8 uint8_t 

#define ERROR_REPORT_BUFFER_DEFAULT_LEN 256
#define ERROR_BUFFER_DEFAULT_LEN 256

#define HEB_DISABLED false
#define HEB_ENABLED true

#define INCOMPLETE false 
#define COMPLETE true

#define DEFAULT_LINE_SLICE_LEN 64
#define DEFAULT_FN_DEC_LEN 64
#define DEFAULT_PARAMETER_DEFINITION_LEN 16
#define DEFAULT_RETURN_LEN 8
#define DEFAULT_CODE_SECTION_LENGTH 16
#define DEFAULT_DATA_SECTION_LEN 256
#define DEFAULT_ERROR_LIMIT 20

#define XEB_TODO(name)\
  TODO(name, NULL);

#define XEB_NOT_IMPLEMENTED(name)\
  NOTY("XEB Internal Message", name" is not implemented yet", NULL);


#define XEB_NOTY(tag,name, ...)\
  fprintf(stdout,"\e[1;32m["tag"]: "name"\e[0m\n",__VA_ARGS__);

#define XEB_PUSH_ERROR(errno, expect, but_got)\
  xeb_error_push_error(errno, lxer_get_current_pointer(&compiler.lh), xeb_error_get_line_pointer(lxer_get_current_pointer(&compiler.lh)), xeb_error_get_line(lxer_get_current_pointer(&compiler.lh)), true, expect, but_got, NULL);

#define XEB_PUSH_ERROR_CUSTOM_LINE(errno, local_source_tracker)\
  xeb_error_push_error(errno, compiler.source_lines[local_source_tracker].pointer,compiler.source_lines[local_source_tracker].pointer, local_source_tracker, false, TOKEN_TABLE_END, TOKEN_TABLE_END, NULL);

#define XEB_PUSH_ERROR_CUSTOM_MESSAGE(errno,string)\
  xeb_error_push_error(errno, lxer_get_current_pointer(&compiler.lh), xeb_error_get_line_pointer(lxer_get_current_pointer(&compiler.lh)), xeb_error_get_line(lxer_get_current_pointer(&compiler.lh)), false, TOKEN_TABLE_END, TOKEN_TABLE_END, string);

#define XEB_PUSH_CTX_ERROR_CUSTOM_MESSAGE(errno, string)\
  xeb_error_push_error(errno, compiler.ctx->pointer, compiler.ctx->pointer, compiler.ctx->line,false, TOKEN_TABLE_END, TOKEN_TABLE_END,string);

#define XEB_NEXT_TOKEN()\
  xeb_next_token(compiler.ctx)

#define XEB_GET_CURRENT_TOKEN()\
  xeb_get_current_token(compiler.ctx)


// internal error messag, used as error reporting tag to return errors before the compilation fully begins

#define XEB_ERRNO()\
  X(XEB_ERROR_WRONG_SYNTAX)\
  X(XEB_ERROR_WRONG_PARAMETER_DEFINITION_FOR_A_FUNCTION)\
  X(XEB_ERROR_WRONG_PARAMETER_SEPARATOR)\
  X(XEB_ERROR_TYPE_ASSIGNMENT_MISSING)\
  X(XEB_ERROR_PARAMETER_HAS_NO_NAME)\
  X(XEB_ERROR_COMMA_MUST_EXPECT_NEW_PARAMETER)\
  X(XEB_ERROR_WRONG_PARAMETER_DEFINITION)\
  X(XEB_ERROR_NO_RETURN_ARROW_PROVIDED)\
  X(XEB_ERROR_WRONG_FUNCTION_DEFINITION)\
  X(XEB_ERROR_NO_RETURN_PROVIDED)\
  X(XEB_ERROR_WRONG_RETURN_DEFINITION)\
  X(XEB_ERROR_MISSING_BRACKETS)\
  X(XEB_ERROR_EXPECTING_MULTIPLE_RETURN_TYPE)\
  X(XEB_ERROR_RETURN_SCOPE_NOT_CLOSED_CORRECTLY)\
  X(XEB_ERROR_WRONG_RETURN_TYPE_DEFINITION)\
  X(XEB_ERROR_MISSING_RETURN_OR_WRONG_SYNTAX)\
  X(XEB_INVALID_FUNCTION_SCOPE)\
  X(XEB_FUNCTION_INITIALIZATION_NOT_POSSIBLE)\
  X(XEB_NOT_A_VALID_VARIABLE_DEFINIPTION)\
  X(XEB_ERROR_NOT_A_VALID_ASSIGNMENT)\
  X(XEB_ERROR_NOT_A_VALID_INSTRUCTION_FOR_XEB_LANGUAGE)\
  X(XEB_ERROR_FUNCTION_ALREADY_DEFINED)\
  X(XEB_ERROR_NO_VALID_FUNCTION_NAME)\
  X(XEB_ERROR_NO_VALID_VARIABLE_NAME)\
  X(XEB_ERROR_VARIABLE_ALREADY_DEFINED)\
  X(XEB_ERROR_MISSING_COMMA_SEPARATOR)\
  X(XEB_ERROR_MISSING_RETURN_TYPE)\
  X(XEB_ERROR_MISSING_TYPE)\
  X(XEB_ERROR_FUNCTION_SCOPE_NOT_CLOSED_CORRECTLY)\
  X(XEB_MISMATCHING_RETURN_TYPE_OR_NUMBER)


#define X(name) name,

typedef enum{
  XEB_ERRNO()
  XEB_END_ERROR
}XEB_COMPILER_ERRNO;

#undef X

typedef enum {NO_SKIP, SINGLE_SKIP, START_LONG_SKIP, END_LONG_SKIP } XEB_SKIP;
typedef enum {NO_FN, FN_OPEN}XEB_FN_STATUS;
typedef enum {IF, SWITCH, ASSIGN, RETURN} XEB_INST_TYPE;


typedef struct{
  XEB_COMPILER_ERRNO error;
  char* xeb_error_to_string;
  char* code_pointer;
  char* line_pointer;
  size_t line;
  bool complete_report;
  LXR_TOKENS lh;
  LXR_TOKENS rh;
  char* string;
}xeb_error_box;

typedef struct{
  char* pointer;
  size_t line;
  bool has_tokens;
  LXR_TOKENS* local_tokens;
  size_t local_tokens_len;
  size_t local_tokens_tracker;
  size_t local_tokens_rp;
}line_slice;



////////////////////////////////////////////////////////

typedef struct{
  char* name;
  LXR_TOKENS type;
}variable_definition;

typedef struct{
  char*name;
  
  LXR_TOKENS* return_type;
  size_t return_type_len;
  size_t return_type_tracker;

  variable_definition** function_parameter;
  size_t parameter_len;
  size_t parameter_tracker;

  bool definition_status;
}function_definition;



typedef struct{
  void* lh;
  LXR_TOKENS exp;
  void* rh;
}expression;


typedef struct{
  XEB_INST_TYPE type;
  void* inst;
}instruction_list;

typedef struct{
  size_t* variable_reference;
  size_t variable_reference_len;
  size_t variable_reference_tracker;
  char* inline_static_strings;
}return_inst_type;

typedef struct{
  function_definition* fn;

  variable_definition** local_var;
  size_t local_var_tracker;
  size_t local_var_len;

  instruction_list** il;
  size_t instruction_list_tracker;
  size_t instruction_list_len;
  bool code_section_completed;
}code_section;


////////////////////////////////////////////////////////

typedef struct{
  // compiler lexer and source code
  lxer_head lh;
  char* source_code;
  size_t source_len;
  char *output_filename;
  char*module_path;
  
  line_slice* source_lines;
  size_t source_lines_len;
  size_t loaded_slice;
  size_t local_line_tracker;

  // compiler error array, return error after the compilation
  xeb_error_box** final_error_report;
  size_t error_report_len;
  size_t error_tracker;
  line_slice* ctx;
}xebc;


static xebc compiler = {0};
static Arena_header compiler_ah = {0};
static bool hoterror_broadcaster_status = HEB_DISABLED;

static function_definition** fn_def_table = {0};
static size_t fn_def_table_tracker = 0;
static size_t fn_def_table_len = 0;

static code_section** code_section_list = {0};
static size_t code_section_list_tracker = 0;
static size_t code_section_list_len = 0;

static u8* data_section = {0};
static size_t data_section_len = 0;
static size_t data_section_tracker = 0; 
static bool entry_point_present = false;

extern LXR_TOKENS xeb_get_current_token(line_slice* ctx);
extern bool xeb_next_token(line_slice* ctx);
extern void xeb_switch_context();

// some macro 

#define xeb_missing_semicolon(buffer, cat_string, errors)\
    buffer = (char*)arena_alloc(&compiler_ah, sizeof(char)*256);\
    cat_string = (char*)arena_alloc(&compiler_ah, sizeof(char)*12);\
    strcpy(buffer, "This is an incomplete declaration, you must conclude it with ");\
    sprintf(cat_string, "'%s', ", token_table_lh[LXR_SEMICOLON]);\
    strcat(buffer,cat_string);\
    strcat(buffer, "try adding it to proceed\n");\
    XEB_PUSH_CTX_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, buffer);\
    *errors = true;

#endif // XEB_DEF_H
