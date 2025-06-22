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

  function_definition* fn_def = NULL;
  variable_definition* vd = NULL;
  size_t brackets_tracker = 0;

  XEB_FN_STATUS function_scope = NO_FN;
  XEB_SKIP comment_skip_status = NO_SKIP; 

  NOTY("XEB Compiler","Compilation process started\n", NULL);
  while(compiler.lh.lxer_tracker < compiler.lh.stream_out_len){
    LXR_TOKENS token = lxer_get_current_token(&compiler.lh);

    switch(token){
      case LXR_LINE_COMMENT:
        if(DEBUG) NOTY("XEB Comment LINE", "found single line comment, ignoring for now", NULL); 
        comment_skip_status = SINGLE_SKIP; 
        break;
      case LXR_OPEN_COMMENT:
        if(DEBUG) NOTY("XEB Comment SECTION", "comment section starter, a lot of lines are gonna be excluded this time", NULL); 
        comment_skip_status = START_LONG_SKIP;
        break;
      case LXR_CLOSE_COMMENT:
        if(DEBUG) NOTY("XEB Comment SECTION", "end comment section, this is the most reliable way to document you're code, well done", NULL); 
        comment_skip_status = END_LONG_SKIP;
      default: break;
    }


    if(comment_skip_status == NO_SKIP){
      switch(token){
        case LXR_FN:
          if(xeb_compiler_function_definition(fn_def, vd)){
            function_scope= FN_OPEN;
            xeb_function_definition_push(fn_def);
            if(DEBUG) DINFO("Function declaration pushed", NULL);
          }
          break;
        case LXR_RET_STATEMENT:
          /*
          if(xeb_compiler_function_termination(fn_dec)){
            function_scope = FN_CLOSED;
          }
          */
          break;
        
        case LXR_OPEN_CRL_BRK:
          if(function_scope == FN_OPEN){
            brackets_tracker += 1;
          }

        case LXR_CLOSE_CRL_BRK:
          if(function_scope == FN_CLOSED){
            brackets_tracker -= 1;
            if(brackets_tracker != 0) {
              XEB_PUSH_ERROR(XEB_ERROR_MISSING_BRACKETS,fn_def->definition_status);
              fn_def->definition_status = INCOMPLETE;
            }
            function_scope = NO_FN;
          }

        break;
        default: break;
      }
    }
    
    switch(comment_skip_status){
      case NO_SKIP: 
        lxer_next_token(&compiler.lh); 
        break;
      case SINGLE_SKIP: 
        xeb_skip_line();
        comment_skip_status = NO_SKIP;
        break;
      case START_LONG_SKIP: 
        lxer_next_token(&compiler.lh); 
        break;
      case END_LONG_SKIP:
        lxer_next_token(&compiler.lh); 
        comment_skip_status = NO_SKIP; 
        break;
      default: break;
    }
  }
}

void xeb_skip_line(){
  char* tracker = lxer_get_current_pointer(&compiler.lh);
  char* new_line = NULL;
  for(size_t i=0;i<compiler.loaded_slice-1;i++){
    if(compiler.source_lines[i].pointer >= tracker && tracker < compiler.source_lines[i+1].pointer){
      new_line = compiler.source_lines[i+1].pointer;
      break;
    }
  }
  lxer_set_new_target(&compiler.lh, new_line);
}

bool xeb_compiler_function_definition(function_definition* fn_def, variable_definition* vd){
  bool error_present = false;
  char* fn_name = lxer_get_rh(&compiler.lh, false);
  fn_def = (function_definition*)arena_alloc(&compiler_ah, sizeof(function_definition));
  fn_def->name = fn_name;
  if(!lxer_misc_expect_brk(&compiler.lh)){ XEB_PUSH_ERROR(XEB_ERROR_WRONG_FUNCTION_DEFINITION, error_present); }

  lxer_next_token(&compiler.lh);
  error_present = xeb_handle_parameter(fn_def, vd, error_present);
  lxer_next_token(&compiler.lh);

  if(lxer_get_current_token(&compiler.lh) == LXR_SUB_SYMB) lxer_next_token(&compiler.lh);

  if(lxer_get_current_token(&compiler.lh) != LXR_RETURN_ARROW){ 
    XEB_PUSH_ERROR(XEB_ERROR_NO_RETURN_ARROW_PROVIDED,error_present);
  }else{
    lxer_next_token(&compiler.lh);
    lxer_next_token(&compiler.lh);
    LXR_TOKENS current_position = lxer_get_current_token(&compiler.lh);
    fn_def->return_type = (LXR_TOKENS*)arena_alloc(&compiler_ah, sizeof(LXR_TOKENS)*DEFAULT_RETURN_LEN);
    fn_def->return_type_len = DEFAULT_PARAMETER_DEFINITION_LEN;
    fn_def->return_type_tracker = 0;

    if(lxer_is_type(current_position)){
      if(DEBUG) DINFO("Single return type found", NULL);
      LXR_TOKENS return_type = lxer_get_current_token(&compiler.lh);
      lxer_next_token(&compiler.lh);
      if(lxer_get_current_token(&compiler.lh) != LXR_OPEN_CRL_BRK){ 
        XEB_PUSH_ERROR(XEB_ERROR_WRONG_RETURN_DEFINITION,error_present);
        XEB_PUSH_ERROR(XEB_ERROR_MISSING_BRACKETS,error_present);
      }
      fn_def->return_type[0] = return_type;
      if(DEBUG) DINFO("Return type: '%s'\n", token_table_lh[fn_def->return_type[0]]);

    }else if(lxer_is_brk(current_position) && lxer_get_current_token(&compiler.lh) == LXR_OPEN_BRK){
      if(DEBUG) DINFO("At least two return types found", NULL);
      lxer_next_token(&compiler.lh);
      int return_token_counter = 0;
      while(lxer_get_current_token(&compiler.lh) != LXR_CLOSE_BRK && return_token_counter >= 0){
        if(lxer_get_current_token(&compiler.lh) == LXR_COMMA) lxer_next_token(&compiler.lh);
        if(lxer_get_current_token(&compiler.lh) == LXR_OPEN_CRL_BRK) { XEB_PUSH_ERROR(XEB_ERROR_RETURN_SCOPE_NOT_CLOSED_CORRECTLY,error_present);return_token_counter = -1; }
        LXR_TOKENS tok = lxer_get_current_token(&compiler.lh);
        if(!lxer_is_type(lxer_get_current_token(&compiler.lh))) { 
          XEB_PUSH_ERROR(XEB_ERROR_WRONG_RETURN_TYPE_DEFINITION, error_present);
          return_token_counter = -1;
        }else if(!lxer_type_expect_sep(&compiler.lh) && !lxer_type_expect_brk(&compiler.lh)) { 
          XEB_PUSH_ERROR(XEB_ERROR_MISSING_RETURN_OR_WRONG_SYNTAX,error_present); 
          return_token_counter = -1;
        }else{
          dapush(&compiler_ah, fn_def->return_type, &fn_def->return_type_tracker, &fn_def->return_type_len, LXR_TOKENS, tok);
          lxer_next_token(&compiler.lh);
          return_token_counter += 1;
          if(DEBUG) DINFO("Return type: '%s'", token_table_lh[fn_def->return_type[fn_def->return_type_tracker-1]]);
        }
      }
      if(return_token_counter < 2 && return_token_counter >= 0) { XEB_PUSH_ERROR(XEB_ERROR_EXPECTING_MULTIPLE_RETURN_TYPE, error_present); }
    }else{
      XEB_PUSH_ERROR(XEB_ERROR_WRONG_RETURN_DEFINITION,error_present);
      XEB_PUSH_ERROR(XEB_ERROR_NO_RETURN_PROVIDED, error_present);
    }
  }
  fn_def->definition_status = error_present;
  return !error_present;
}



bool xeb_handle_parameter(function_definition* fn_def, variable_definition* vd, bool error_present){
  if(!lxer_brk_expect_brk(&compiler.lh) && lxer_brk_expect_type(&compiler.lh)){
    // arguments present
  if(DEBUG) DINFO("Processing function arguments: at least one argument found", NULL);
    fn_def->function_parameter = (variable_definition**)arena_alloc(&compiler_ah, sizeof(variable_definition*)*DEFAULT_PARAMETER_DEFINITION_LEN);
    fn_def->parameter_len = DEFAULT_PARAMETER_DEFINITION_LEN;
    fn_def->parameter_tracker = 0;
    lxer_next_token(&compiler.lh);
    bool first = true;
    int parameter_token_counter = 0;
    while(lxer_get_current_token(&compiler.lh) != LXR_CLOSE_BRK && parameter_token_counter >= 0){
      if(!first && lxer_get_current_token(&compiler.lh) != LXR_COMMA) {XEB_PUSH_ERROR(XEB_ERROR_WRONG_PARAMETER_SEPARATOR,error_present);}
      if(lxer_get_current_token(&compiler.lh) == LXR_COMMA){lxer_next_token(&compiler.lh); first = false;}
      LXR_TOKENS parameter_type = lxer_get_current_token(&compiler.lh);
      if(lxer_is_type(parameter_type) && lxer_type_expect_statement(&compiler.lh)){
        lxer_next_token(&compiler.lh);
        if(lxer_get_current_token(&compiler.lh) != LXR_TYPE_ASSIGN) { XEB_PUSH_ERROR(XEB_ERROR_TYPE_ASSIGNMENT_MISSING,error_present); }
        char* par_name = lxer_get_rh(&compiler.lh, false);
        if(strlen(par_name) < 1) { 
          XEB_PUSH_ERROR(XEB_ERROR_PARAMETER_HAS_NO_NAME,error_present); 
        }else{
          vd = (variable_definition*)arena_alloc(&compiler_ah, sizeof(variable_definition));
          vd->name = par_name;
          vd->type = parameter_type;
          lxer_next_token(&compiler.lh);
          if(lxer_get_current_token(&compiler.lh) == LXR_COMMA && lxer_statement_expect_brk(&compiler.lh))  { 
            XEB_PUSH_ERROR(XEB_ERROR_COMMA_MUST_EXPECT_NEW_PARAMETER,error_present); 
            parameter_token_counter = -1;
          }else {
            dapush(&compiler_ah, fn_def->function_parameter, &fn_def->parameter_tracker, &fn_def->parameter_len, variable_definition*, vd);
            if(DEBUG) DINFO("Argument found for variable '%s' of type '%s'", vd->name, token_table_lh[vd->type]);
          }
        }
      }else{
        XEB_PUSH_ERROR(XEB_ERROR_WRONG_PARAMETER_DEFINITION,error_present);
        parameter_token_counter = -1;
      }
    }

  }else if(lxer_brk_expect_brk(&compiler.lh)){
    if(DEBUG) DINFO("Processing function arguments: no arguments found", NULL);
    fn_def->parameter_len = 0;
    fn_def->parameter_tracker = 0;
    fn_def->function_parameter = NULL;
    lxer_next_token(&compiler.lh);
  }else{
    XEB_PUSH_ERROR(XEB_ERROR_WRONG_PARAMETER_DEFINITION_FOR_A_FUNCTION,error_present);
  }

  return error_present;
}

void xeb_error_calculate_total_lines(){
  if(DEBUG) DINFO("Calculating total code line", NULL);
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
      line_slice* cache = &compiler.source_lines[compiler.loaded_slice];
      cache->pointer = start_line;
      cache->line = global_line_counter;
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

bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer, size_t line, bool* status){
  xeb_error_box* error_box = NULL;
  if(err >= 0 && err < XEB_END_ERROR){
    if(*status == false) *status = true;
    error_box = (xeb_error_box*)arena_alloc(&compiler_ah,sizeof(xeb_error_box));
    error_box->error = err;
    error_box->xeb_error_to_string = xeb_error_get_message(err);
    error_box->code_pointer = pointer;
    error_box->line_pointer = line;
    
    dapush(&compiler_ah, compiler.final_error_report, &compiler.error_tracker, &compiler.error_report_len, xeb_error_box*, error_box);
    return true;
  }
  return false;
}


char* xeb_error_get_message(XEB_COMPILER_ERRNO err){
  char* error_message = (char*)arena_alloc(&compiler_ah, sizeof(char)*256);
  error_message[0] = '\0';
  #define X(name)\
    case name:\
      strcpy(error_message,"\t"#name"\n");\
      break;


  switch(err){
     XEB_ERRNO();
    default:
      XEB_WARN_ERROR_MESSAGE();
      break;
  }

  #undef X
  return error_message;
}

void xeb_error_report(){
  if(compiler.error_tracker > 0){
    fprintf(stderr, "\x1b[31mError report before compilation: \n\x1b[0m");
    for(size_t i=0;i<compiler.error_tracker; i++){
      fprintf(stderr, "\x1b[31m->  Error in line %zu: \n\t%s\n\x1b[0m",compiler.final_error_report[i]->line_pointer, compiler.final_error_report[i]->xeb_error_to_string);
    }
  }
  return;
}

void xeb_error_send_error(XEB_COMPILER_ERRNO err){
  (void)err;
  XEB_NOT_IMPLEMENTED("'xeb_error_send_error(XEB_COMPILER_ERRNO err)'"); 
  return;
}

void xeb_error_open_public_hoterror_broadcaster(){
  XEB_NOT_IMPLEMENTED("'xeb_error_open_public_hoterror_broadcaster()'");
  hoterror_broadcaster_status = HEB_ENABLED;
}

void xeb_function_definition_push(function_definition* fn_def){
  if(fn_def_table == NULL){
    fn_def_table = (function_definition**)arena_alloc(&compiler_ah,sizeof(function_definition*)*DEFAULT_FN_DEC_LEN);
    fn_def_table_tracker = 0;
    fn_def_table_len = DEFAULT_FN_DEC_LEN;
  }
  
  dapush(&compiler_ah, fn_def_table, &fn_def_table_tracker, &fn_def_table_len, function_definition*, fn_def);
}


void xeb_close_compiler(){
  NOTY("XEB Compiler", "Compilation completed, exiting..", NULL);
  if(compiler_ah.arena_count >= 1) arena_free(&compiler_ah);
  if(compiler.lh.lxer_ah.arena_count >= 1) arena_free(&compiler.lh.lxer_ah);
}
