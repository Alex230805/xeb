#define XEB_C
#include "xeb.h"


void xeb_load_file(const char* path){
  NOTY("XEB Compiler","loading source code", NULL);
  StringBuilder *sb = read_file(&compiler_ah,path);
  compiler.source_code = sb->string;
  compiler.source_len = sb->len;
  lxer_start_lexing(&compiler.lh, compiler.source_code);
  if(DEBUG) lxer_get_lxer_content(&compiler.lh);
}

void xeb_error_init_handler(){
  if(DEBUG) DINFO("Init error handler",NULL);
  compiler.final_error_report = (xeb_error_box**)arena_alloc(&compiler_ah,sizeof(xeb_error_box*)*ERROR_REPORT_BUFFER_DEFAULT_LEN);
  compiler.error_report_len = ERROR_REPORT_BUFFER_DEFAULT_LEN;
  compiler.error_tracker = 0;
  error_buffer = (XEB_COMPILER_ERRNO*)arena_alloc(&compiler_ah, sizeof(XEB_COMPILER_ERRNO)*ERROR_BUFFER_DEFAULT_LEN);
  error_buffer_tracker = (size_t*)arena_alloc(&compiler_ah, sizeof(size_t));
  error_buffer_package_sent = (bool*)arena_alloc(&compiler_ah, sizeof(bool));
  *error_buffer_package_sent = false;
  *error_buffer_tracker = 0;
}

bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer, size_t line){
  xeb_error_box* error_box = NULL;
  if(err >= 0 && err < XEB_END_ERROR){
    error_box = (xeb_error_box*)arena_alloc(&compiler_ah,sizeof(xeb_error_box));
    error_box->error = err;
    error_box->xeb_error_to_string = xeb_error_get_message(err);
    error_box->code_pointer = pointer;
    error_box->line_pointer = line;

    compiler.final_error_report[compiler.error_tracker] = error_box;
    compiler.error_tracker += 1;
    if(compiler.error_tracker == compiler.error_report_len){
      size_t new_report_len = compiler.error_report_len*2;
      xeb_error_box** new_error_report = (xeb_error_box**)arena_alloc(&compiler_ah, sizeof(xeb_error_box*)*new_report_len);
      for(size_t i=0;i<compiler.error_report_len;i++){
        new_error_report[i] = compiler.final_error_report[i];
      }
      compiler.error_report_len = new_report_len;
      compiler.final_error_report = new_error_report;
    }
    return true;
  }
  return false;
}


char* xeb_error_get_message(XEB_COMPILER_ERRNO err){
  char* error_message = (char*)arena_alloc(&compiler_ah, sizeof(char)*256);
  error_message[0] = '\0';
  #define X(name)\
    case name:
  
  switch(err){
     XEB_ERRNO();
        XEB_TODO("Complete error message generator");
        strcpy(error_message, "generic error message");
        break;
    default:
      XEB_WARN_ERROR_MESSAGE()
      break;
  }

  #undef X
  return error_message;
}

void xeb_error_report(){
  if(compiler.error_tracker > 0){
    fprintf(stderr, "Error report: \n");
    for(size_t i=0;i<compiler.error_tracker; i++){
      fprintf(stderr, "Error in line %zu: \n\t%s\n",compiler.final_error_report[i]->line_pointer, compiler.final_error_report[i]->xeb_error_to_string);
    }
  }
  return;
}

void xeb_error_send_error(XEB_COMPILER_ERRNO err){
  error_buffer[*error_buffer_tracker] = err;
  *error_buffer_tracker += 1;
  *error_buffer_package_sent = true;
  /*
   * sent package: this flag is used to track the status of the public error buffer used by external build system or build environment. By checking the flag "error_buffer_package_sent" the build system can check for new error during the compilation and sent back to the user custom error messages or custom action when an error occur. This function is activated if the compiler is called with the build system flag activated.
   */

  if(*error_buffer_tracker == ERROR_BUFFER_DEFAULT_LEN){
    *error_buffer_tracker = 0;
  }
  return;
}

void xeb_error_get_public_buffer_pointer(){
  // send to stdout the pointer to the internal static 
  // buffer for public error handling

  fprintf(stdout,"%zx", (size_t)error_buffer);
  fprintf(stdout,"%zx", (size_t)error_buffer_tracker);
  fprintf(stdout,"%zx", (size_t)error_buffer_package_sent);
 
}


void xeb_close_compiler(){
  NOTY("XEB Compiler", "Compilation completed, exiting..", NULL);
  arena_free(&compiler_ah);
  arena_free(&compiler.lh.lxer_ah);
}
