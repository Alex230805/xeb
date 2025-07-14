#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <misc.h>
#include "xeb_def.h"
#include "xeb_inst.h"


// error related functions

void xeb_error_init_handler(); 
bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer,char* line_pointer, size_t line, bool report, LXR_TOKENS lh, LXR_TOKENS rh, char* string);

char* xeb_error_get_message(XEB_COMPILER_ERRNO err);
void xeb_error_report();
void xeb_error_send_error(XEB_COMPILER_ERRNO err);
void xeb_error_open_public_hoterror_broadcaster();
char* xeb_error_get_line_pointer(char* ptr);
void xeb_error_calculate_total_lines();
size_t xeb_error_get_line(char*ptr);

// file related function

bool xeb_load_file(char* source_file);
void xeb_load_output_filename(char*filename);

// function related to pushing or getting something

void xeb_function_definition_push(function_definition* fn_def);
void xeb_variable_definition_push(variable_definition* vd, code_section* cd);
void xeb_code_section_push(code_section* cd);

function_definition* xeb_function_definition_get();
code_section* xeb_code_section_get();
variable_definition* xeb_variable_definition_get(char*name, code_section* cd);

// compiler functions related to parsing and creating the intermediate memory-like reperesentation

bool xeb_function_parse_definition();
bool xeb_function_handle_parameter(function_definition* fn_def);
bool xeb_function_handle_return_type(function_definition* fn_def);

bool xeb_compiler_variable_definition();

void xeb_compile_expression(LXR_TOKENS token, XEB_FN_STATUS* function_scope);
bool xeb_line_is_empty(size_t line_number);
void xeb_validate_line_status();

// compiler usage functions 

void xeb_start_compiler(char*module_path);
void xeb_close_compiler(bool fatal);
void xeb_helper();



#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
