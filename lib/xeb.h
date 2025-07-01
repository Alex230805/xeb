#ifndef XEB_H
#define XEB_H


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

/* Current compilation structure
*
* The file is analized and when a function declaration is found it will be processed and a function scope begin ( if the function declaration is done right )
* Inside this scope we can have switch cases, if statement, variable definition and assignment, conditions ... we can have basically anythings and those things will create what the xeb reference define as "code section". 
* Inside a code section we have the function declaration ( which is also inserted in the global function table, it doesn't need forward declarations ), the local variable declaration which create a local list of variable where the expression can access to by creating an "expression" tree.
* Basically whats happen is this: when a variable is allocated it will be inserted inside this local list with the name and the type of variable, when an expression is required inside a if statement or a switch statement this list is consulted to find the corresponding variable referencing and forming an intermediate representation of what the expression is. For example if we have such code:
*
*       module io.*;
* 
*       fn main() -> void {
*         int: number = 0;
*
*         if(number == 0){
*           printf("Hello world");
*         }
*         ret();
*       }
*
*   This code allocate a local variable named "number" of type "int" initializated with 0. Those are two differents expressions, we have a number allocation and a number assignment, so when the compiler find the definition it will push the variable inside the local list, then the assignment is composed as an expression which require a left hand argument "number" and with the assignment token "=" assign the implicit value "0" to the variable. This is important because yes in this part the compilation process can start right away, but we prefer to first analize the code and generate a local version defined in memory before require the intermediate representation layer to operate the actual translation. Since this compiler use the QBE backend this memory-like intermediate representation allow a much higher description level of what is processed without generating an assembly-like code that is still high level, bus sligly lower than xebc approach. 
*   After the expression has beed pushed inside the memory the if statement is processed, here the condition is threated outside of the compilation of the compilation process of the if statement with a function that is dedicated ONLY to expression parsing, in fact when the if is gonna be verified and compiled into the memory-like intermediate representation it will have the expression tree, the true branch and the false branch. Same things for the switch case which have a much complex structure:
* 
* module io.*;
*
* fn main() -> void {
*   int: name = 0;
*   bool: allowed = true;
*
*   switch(name){
*     (0) -> {
*       print("Still \{name}?");
*     }
*     (1),(2),( _ > 10 && allowed) ->{
*       print("Wow, such a progess, keep going mate!");
*     }
*   }else{
*     print("Not a valid number: \{name}");
*   }
*
*   ret();
* }
* 
* The xeb switch statemet is special because allow a similar if syntax, but with a much higher condition ergonomics rate. This imply that the switch have a variable reference ( taken by the main local variable list ) which is NOT an expression, but only a reference ( it can be function or whatever, just not a condition, so not an expression ), then we have the switch condition that are wrapped in the "(..) ->{}" syntax, basically whatever is inserted in this parentesis can directly be referred to the main variable used inside the switch statement, or it can be a condition which allow the expression to have a local reference to the switch condition and external reference for external condition like is shown in the example here. 
* At the end the intermediate representation have here a switch declaration with the reference in the local variable list and a list of expression with a true ( or verified/switch taken ) branches, basically is a wrapper for if statement (with address-pointing optimized condition) with a single else branch which is the main else of the switc statement
*/


////////////////////////////////////////////////////////////////////


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

bool xeb_compiler_function_definition();
bool xeb_handle_parameter(function_definition* fn_def);
bool xeb_handle_return_type(function_definition* fn_def);


bool xeb_compiler_variable_definition();
bool xeb_compiler_return_inst();


void xeb_compile_expression(LXR_TOKENS token, XEB_FN_STATUS* function_scope);

bool xeb_line_is_empty(size_t line_number);
void xeb_validate_line_status();

LXR_TOKENS xeb_get_current_token(line_slice* ctx);
bool xeb_next_token(line_slice* ctx);
void xeb_switch_context();

// compiler usage functions 

void xeb_start_compiler(char*module_path);
void xeb_close_compiler(bool fatal);
void xeb_helper();



#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
