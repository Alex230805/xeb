#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <misc.h>


#define ERROR_REPORT_BUFFER_DEFAULT_LEN 10
#define ERROR_BUFFER_DEFAULT_LEN 256

#define XEB_TODO(name)\
  fprintf(stdout, "[XEB Internal TODO]: ");\

#define XEB_WARN_ERROR_MESSAGE()\
  fprintf(stdout, "[XEB Error handler]: wrong error code encounter");

#define XEB_NOT_IMPLEMENTED(name)\
  fprintf(stdout, "[XEB Internal Message]: "name" is still under development!\n");


// internal error messag, shared between the internal stdbuffer and the error handler


#define XEB_ERRNO()\
  X(XEB_NOT_A_FUNCTION)\
  X(XEB_NO_VARIABLE_FOUND)\
  X(XEB_UNDECLARED_SCOPE)\
  X(XEB_NO_CONDITION)\
  X(XEB_NO_SEMICOLON)\
  X(XEB_NOT_AN_ARRAY)\
  X(XEB_NO_RETURN_PROVIDED)\
  X(XEB_MODULE_NOT_FOUND)\
  X(XEB_MISSING_ARGUMENTS)\
  X(XEB_OUT_OF_BOUND)\
  X(XEB_MISSING_TYPE)\
  X(XEB_WRONG_RETURN_TYPE)\
  X(XEB_WRONG_DEFINITION)\
  X(XEB_MISSING_DEFINITION)\
  X(XEB_NO_SUB_VARIABLE_OR_FUNCTION)

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
  // compiler lexer and source code
  lxer_head lh;
  char* source_code;
  size_t source_len;
  
  // compiler error array, return error after the compilation
  xeb_error_box** final_error_report;
  size_t error_report_len;
  size_t error_tracker;

}xebc;

// public error buffer, used only if the build system mode is enabled
static XEB_COMPILER_ERRNO *error_buffer;
static size_t* error_buffer_tracker;
static bool*   error_buffer_package_sent;

static xebc compiler = {0};
static Arena_header compiler_ah = {0};

void xeb_helper();

void xeb_error_init_handler(); 
bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer, size_t line);
char* xeb_error_get_message(XEB_COMPILER_ERRNO err);
void xeb_error_report();
void xeb_error_send_error(XEB_COMPILER_ERRNO err);
void xeb_error_get_public_buffer_pointer();

void xeb_load_file(const char* source_file);
void xeb_close_compiler();

#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
