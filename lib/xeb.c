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
  xeb_error_calculate_total_lines();
  //if(DEBUG) lxer_get_lxer_content(&compiler.lh);

  while(compiler.lh.lxer_tracker < compiler.lh.stream_out_len){
   
    //XEB_PUSH_ERROR(XEB_NOT_A_FUNCTION);

    if(lxer_is_misc(lxer_get_current_token(&compiler.lh))){
      LXR_TOKENS misc_token = lxer_get_current_token(&compiler.lh);
      switch(misc_token){
        case LXR_RETURN_ARROW:

          break;

        case LXR_DEF_STATEMENT:

          break;
        case LXR_STRUCT:

          break;
        case LXR_ENUM:

          break;
        case LXR_FN:
          compilation_table.start_function_definition = true;
          char* function_name = lxer_get_rh(false);
          if(strlen(function_name) < 1){
            XEB_PUSH_ERROR(XEB_WRONG_DEFINITION);
            XEB_PUSH_ERROR(XEB_INCOMPLETE_SYNTAX);
          }
          break;
        case LXR_AS_CAST:


          break;
        default: break;
      }
    }



    lxer_next_token(&compiler.lh);
  }
}


void xeb_error_calculate_total_lines(){
  NOTY("XEB Error Handler","Calculating total code line", NULL);
  if(compiler.source_lines == NULL){
    compiler.source_lines = (line_slice*)arena_alloc(&compiler_ah, sizeof(line_slice)*DEFAULT_LINE_SLICE_LEN);
    compiler.source_lines_len = DEFAULT_LINE_SLICE_LEN;
    compiler.loaded_slice = 0;
  }
  size_t global_line_counter = 0;
  char* start_line = &compiler.source_code[0];
  char* cursor = NULL; 
  

  while(start_line < &compiler.source_code[0] + compiler.source_len){
    cursor = strchr(start_line, '\n');
    if(cursor != NULL){
      compiler.source_lines[compiler.loaded_slice].pointer = start_line;
      compiler.source_lines[compiler.loaded_slice].line = global_line_counter;
      global_line_counter+=1;
      start_line = cursor+1; 
      compiler.loaded_slice+=1;
      
      if(compiler.loaded_slice == compiler.source_lines_len){
        line_slice* new_source_lines = (line_slice*)arena_alloc(&compiler_ah,sizeof(line_slice)*compiler.source_lines_len*2);
        size_t new_size = compiler.source_lines_len*2;
        for(size_t i=0;i<compiler.source_lines_len;i++){
          new_source_lines[i] = compiler.source_lines[i];
        }
        compiler.source_lines = new_source_lines;
        compiler.source_lines_len = new_size;
      }
    }
  }
}


size_t xeb_error_get_line(char*ptr){
  for(size_t i=1;i<compiler.loaded_slice;i++){
    if(compiler.source_lines[i].pointer > ptr){
      return compiler.source_lines[i].line;
    }
  }
  return -1;
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
