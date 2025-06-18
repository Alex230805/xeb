#define XEB_C
#include "xeb.h"


bool xeb_load_file(char* path){
  NOTY("XEB Compiler","loading source code", NULL);
  if(path[strlen(path)-1] != 'x') return false;
  StringBuilder *sb = read_file(&compiler_ah,path);
  compiler.source_code = sb->string;
  compiler.source_len = sb->len;
  return true;
}



void xeb_load_output_filename(char* filename){

  NOTY("XEB Compiler","Loading output filename inside the compiler memory", NULL);
  compiler.output_filename = filename;
  return;
}



void xeb_start_compiler(char*module_path){
  NOTY("XEB Compiler","Starting compilation process", NULL);
  compiler.module_path = module_path;

  lxer_start_lexing(&compiler.lh, compiler.source_code);
  if(DEBUG) lxer_get_lxer_content(&compiler.lh);

}


void xeb_error_init_handler(){
  if(DEBUG) DINFO("Init error handler",NULL);
  compiler.final_error_report = (xeb_error_box**)arena_alloc(&compiler_ah,sizeof(xeb_error_box*)*ERROR_REPORT_BUFFER_DEFAULT_LEN);
  compiler.error_report_len = ERROR_REPORT_BUFFER_DEFAULT_LEN;
  compiler.error_tracker = 0;
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
  XEB_NOT_IMPLEMENTED("'xeb_error_send_error(XEB_COMPILER_ERRNO err)'"); 
  return;
}

void xeb_error_open_public_hoterror_broadcaster(){
  XEB_NOT_IMPLEMENTED("'xeb_error_open_public_hoterror_broadcaster()'");
  hoterror_broadcaster_status = HEB_ENABLED;
}

void xeb_close_compiler(){
  NOTY("XEB Compiler", "Compilation completed, exiting..", NULL);
  if(compiler_ah.arena_count >= 1) arena_free(&compiler_ah);
  if(compiler.lh.lxer_ah.arena_count >= 1) arena_free(&compiler.lh.lxer_ah);
}
