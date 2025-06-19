#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <misc.h>


#define ERROR_REPORT_BUFFER_DEFAULT_LEN 10
#define ERROR_BUFFER_DEFAULT_LEN 256

#define HEB_DISABLED false
#define HEB_ENABLED true

#define INCOMPLETE false 
#define COMPLETE true


#define DEFAULT_LINE_SLICE_LEN 10
#define DEFAULT_FN_DEC_LEN 64
#define DEFAULT_PARAMETER_DEFINITION_LEN 4

#define XEB_TODO(name)\
  fprintf(stdout, "[XEB Internal TODO]: ");\

#define XEB_WARN_ERROR_MESSAGE()\
  fprintf(stdout, "[XEB Error handler]: wrong error code encounter");

#define XEB_NOT_IMPLEMENTED(name)\
  fprintf(stdout, "[XEB Internal Message]: "name" is still under development!\n");


#define XEB_PUSH_ERROR(errno, flag)\
  xeb_error_push_error(errno, lxer_get_current_pointer(&compiler.lh), xeb_error_get_line(lxer_get_current_pointer(&compiler.lh)), &flag);

// internal error messag, shared between the internal stdbuffer and the error handler


#define XEB_ERRNO()\
  X(XEB_NOT_A_FUNCTION)\
  X(XEB_NO_VARIABLE_FOUND)\
  X(XEB_UNDECLARED_SCOPE)\
  X(XEB_NO_CONDITION)\
  X(XEB_NO_SEMICOLON)\
  X(XEB_MISSING_TYPE_ASSIGN)\
  X(XEB_NOT_AN_ARRAY)\
  X(XEB_NO_RETURN_PROVIDED)\
  X(XEB_MODULE_NOT_FOUND)\
  X(XEB_MISSING_ARGUMENTS)\
  X(XEB_NO_RETURN_ARROW_PROVIDED)\
  X(XEB_INCOMPLETE_SYNTAX)\
  X(XEB_MISSING_BRACKETS)\
  X(XEB_OUT_OF_BOUND)\
  X(XEB_MISSING_TYPE)\
  X(XEB_WRONG_RETURN_TYPE)\
  X(XEB_WRONG_DEFINITION)\
  X(XEB_MISSING_DEFINITION)\
  X(XEB_NO_SUB_VARIABLE_OR_FUNCTION)\
  X(XEB_WRONG_SYNTAX)\
  X(XEB_MISSING_PARAMETER_NAME)

#define X(name) name,

typedef enum{
  XEB_ERRNO()
  XEB_END_ERROR
}XEB_COMPILER_ERRNO;

#undef X


typedef struct{
  XEB_COMPILER_ERRNO error;
  char* xeb_error_to_string;
  char* code_pointer;
  size_t line_pointer;
}xeb_error_box;

typedef struct{
  char* pointer;
  size_t line;
}line_slice;


typedef struct{
  char* name;
  LXR_TOKENS type;
}variable_definition;

typedef struct{
  char*name;
  LXR_TOKENS return_type;
  variable_definition** function_parameter;
  size_t parameter_len;
  size_t parameter_tracker;
  bool definition_status;
}function_definition;



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


  // compiler error array, return error after the compilation
  xeb_error_box** final_error_report;
  size_t error_report_len;
  size_t error_tracker;

}xebc;




static xebc compiler = {0};
static Arena_header compiler_ah = {0};
static bool hoterror_broadcaster_status = HEB_DISABLED;

static function_definition** fn_def_table = {0};
static size_t fn_def_table_tracker = 0;
static size_t fn_def_table_len = 0;

void xeb_helper();

void xeb_error_init_handler(); 
bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer, size_t line, bool*flag);
char* xeb_error_get_message(XEB_COMPILER_ERRNO err);
void xeb_error_report();
void xeb_error_send_error(XEB_COMPILER_ERRNO err);
void xeb_error_open_public_hoterror_broadcaster();


void xeb_error_calculate_total_lines();
size_t xeb_error_get_line(char*ptr);

bool xeb_load_file(char* source_file);
void xeb_load_output_filename(char*filename);

void xeb_start_compiler(char*module_path);

void xeb_function_definition_push(function_definition* fn_def);
bool xeb_compiler_function_definition(function_definition* fn_def, variable_definition* vd);

void xeb_close_compiler();

#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
