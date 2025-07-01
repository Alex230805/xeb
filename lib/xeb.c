#define XEB_C
#include "xeb.h"


bool xeb_load_file(char* path){
  XEB_NOTY("XEB Compiler","loading source code", NULL);
  if(path[strlen(path)-1] != 'x') return false;
  StringBuilder *sb = read_file(&compiler_ah,path);
  compiler.source_code = sb->string;
  compiler.source_len = sb->len;
  return true;
}

void xeb_load_output_filename(char* filename){
  XEB_NOTY("XEB Compiler","Loading output filename inside the compiler memory", NULL);
  compiler.output_filename = filename;
  return;
}


void xeb_start_compiler(char*module_path){
  XEB_NOTY("XEB Compiler","Starting compilation process", NULL);
  compiler.module_path = module_path;
  lxer_start_lexing(&compiler.lh, compiler.source_code);
  xeb_error_calculate_total_lines();
  xeb_validate_line_status();
  
  if(DEBUG) lxer_get_lxer_content(&compiler.lh);

  XEB_SKIP comment_skip_status = NO_SKIP; 

  compiler.local_line_tracker = 0;
  compiler.ctx = &compiler.source_lines[compiler.local_line_tracker];

  LXR_TOKENS token = 0;
  XEB_FN_STATUS function_scope = NO_FN;
  XEB_NOTY("XEB Compiler","Compilation process started\n", NULL);
  while(compiler.local_line_tracker < compiler.loaded_slice){
    if(compiler.ctx->has_tokens){
      bool valid_token = true;
      while(valid_token && comment_skip_status != SINGLE_SKIP){
        token = XEB_GET_CURRENT_TOKEN();
        if(token < TOKEN_TABLE_END){
          switch(token){
            case LXR_LINE_COMMENT:
              if(DEBUG) XEB_NOTY("XEB Comment LINE", "found single line comment, ignoring for now", NULL); 
              comment_skip_status = SINGLE_SKIP; 
              break;
            case LXR_OPEN_COMMENT:
              if(DEBUG) XEB_NOTY("XEB Comment SECTION", "comment section starter, a lot of lines are gonna be excluded this time", NULL); 
              comment_skip_status = START_LONG_SKIP;
              break;
            case LXR_CLOSE_COMMENT:
              if(DEBUG) XEB_NOTY("XEB Comment SECTION", "end comment section, this is the most reliable way to document you're code, well done", NULL); 
              comment_skip_status = END_LONG_SKIP;
            default: break;
          }
          if(comment_skip_status == NO_SKIP){
            xeb_compile_expression(token, &function_scope);
          }
          if(comment_skip_status == END_LONG_SKIP) comment_skip_status = NO_SKIP; 
        }
        valid_token = XEB_NEXT_TOKEN();
      }
    }else if(!xeb_line_is_empty(compiler.local_line_tracker)){
      XEB_PUSH_ERROR_CUSTOM_LINE(XEB_ERROR_WRONG_SYNTAX,compiler.local_line_tracker);
    }
    xeb_switch_context();
    if(comment_skip_status == SINGLE_SKIP) comment_skip_status = NO_SKIP;
  }
  return;
}

// TODO: insert final validator after general parsing 
//



void xeb_compile_expression(LXR_TOKENS token, XEB_FN_STATUS* function_scope){
  code_section* cd = NULL;
  switch(token){
    case LXR_FN:
      if(*function_scope == NO_FN){
        bool definition_status = xeb_compiler_function_definition();
        XEB_NEXT_TOKEN();
        if(XEB_GET_CURRENT_TOKEN() == LXR_OPEN_CRL_BRK && definition_status){
          *function_scope = FN_OPEN;
          cd = (code_section*)arena_alloc(&compiler_ah, sizeof(code_section));
          cd->fn = xeb_function_definition_get(); 
          cd->local_var = (variable_definition**)arena_alloc(&compiler_ah, sizeof(variable_definition*)*5);
          cd->local_var_tracker = 0;
          cd->local_var_len = 5;
          cd->il = (instruction_list**)arena_alloc(&compiler_ah, sizeof(variable_definition*)*5);
          cd->instruction_list_tracker = 0;
          cd->instruction_list_len = 5;
          cd->code_section_completed = INCOMPLETE;
          xeb_code_section_push(cd);
          if(DEBUG) DINFO("Code section initialized", NULL);
        }else{
          XEB_PUSH_ERROR(XEB_ERROR_MISSING_BRACKETS, LXR_OPEN_CRL_BRK, XEB_GET_CURRENT_TOKEN());
          XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_INVALID_FUNCTION_SCOPE, "This is not a valid function scope, you must first declare a valid function and open it with the appropriate syntax");
        }
        break;
      }else{
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_INVALID_FUNCTION_SCOPE, "Invalid function scope, you cannot declare a function here");
      }
      break;
    case LXR_RET_STATEMENT:
          if(*function_scope == FN_OPEN){
            cd = xeb_code_section_get();
            bool return_status = xeb_compiler_return_inst();
            if(return_status){
              // TODO: code here to complete the return instruction 
            }else{
              XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, "Not a valid return");
              
            }
          }else{
            XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_RETURN_DEFINITION, "Unable to find a valid scope for the return statement");
          }
      break;

    case LXR_OPEN_CRL_BRK:

      break; 

    case LXR_CLOSE_CRL_BRK:
      if(*function_scope == FN_OPEN){
          *function_scope = NO_FN;
          //cd = xeb_code_section_get();
          //cd->code_section_completed = COMPLETE;
      }else{
        XEB_PUSH_ERROR(XEB_ERROR_WRONG_FUNCTION_DEFINITION, LXR_CLOSE_CRL_BRK, XEB_GET_CURRENT_TOKEN()); 
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_FUNCTION_SCOPE_NOT_CLOSED_CORRECTLY, "Unable to complete the function implementation without the scope being correclty closed");
      }
      break;
    case LXR_STRING_TYPE:
    case LXR_INT_TYPE: 
    case LXR_DOUBLE_TYPE:
    case LXR_FLOAT_TYPE:
    case LXR_CHAR_TYPE:
    case LXR_POINTER_TYPE:
    case LXR_VOID_TYPE:         
      if(*function_scope == FN_OPEN){
        bool variable_definition_status = xeb_compiler_variable_definition();
        if(!variable_definition_status){
          XEB_PUSH_CTX_ERROR_CUSTOM_MESSAGE(XEB_NOT_A_VALID_VARIABLE_DEFINIPTION, "This is not a valid variable definition, try to follow the documentation");
        }
      }else{
        XEB_NOT_IMPLEMENTED("global_variable_compilation()");
      }
      break;
    default: 
      /*
       * Default statement: this will process whatever expression or phrase that is not handled currently by the compiler.
       * This include invalid token nexting or expression and also unimplemented features
       *
       * */
      //XEB_PUSH_ONLY_ERROR(XEB_ERROR_WRONG_SYNTAX); 
    break;
  }
  return;
}



#define xeb_missing_semicolon(buffer, cat_string, errors)\
    buffer = (char*)arena_alloc(&compiler_ah, sizeof(char)*256);\
    cat_string = (char*)arena_alloc(&compiler_ah, sizeof(char)*12);\
    strcpy(buffer, "This is an incomplete declaration, you must conclude it with ");\
    sprintf(cat_string, "'%s', ", token_table_lh[LXR_SEMICOLON]);\
    strcat(buffer,cat_string);\
    strcat(buffer, "try adding it to proceed\n");\
    XEB_PUSH_CTX_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, buffer);\
    *errors = true;



bool xeb_compiler_variable_definition(){
  code_section* cd = xeb_code_section_get();
  if(cd == NULL) return false;

  bool errors = false;
  variable_definition* vd = NULL;
  if(lxer_type_expect_statement(&compiler.lh)){
    vd = (variable_definition*)arena_alloc(&compiler_ah, sizeof(variable_definition));
    vd->type = XEB_GET_CURRENT_TOKEN();
    XEB_NEXT_TOKEN();
    char* name = lxer_get_rh(&compiler.lh,false);
    if(strlen(name) < 1){
      XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_NO_VALID_VARIABLE_NAME,"The function must have a valid name"); 
      errors = true;
    }else{
      vd->name = name;
      XEB_NEXT_TOKEN();
      LXR_TOKENS tok = XEB_GET_CURRENT_TOKEN();
      char* buffer = (char*)arena_alloc(&compiler_ah,sizeof(char)*32);
      char* cat_string = (char*)arena_alloc(&compiler_ah, sizeof(char)*12);
      switch(tok){
        case LXR_SEMICOLON:
          if(DEBUG) DINFO("Variable deifnition with name '%s' initialized with type '%s'", vd->name, token_table_lh[vd->type]); 
          xeb_variable_definition_push(vd, cd); 
          break;
        case TOKEN_TABLE_END:
          xeb_missing_semicolon(buffer, cat_string, &errors);
          break;
        case LXR_ASSIGNMENT:
          NOTY("Xeb todo", "Implement inline assignment after declaration", NULL);
          XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, "Operation not supported yet");
          errors = true;
          break;
        default:
          XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, "Incomplete or wrong syntax");
          errors = true;
          break;
      }
    }
  }else{
    XEB_PUSH_ERROR(XEB_ERROR_TYPE_ASSIGNMENT_MISSING, LXR_TYPE_ASSIGN, XEB_GET_CURRENT_TOKEN());
    errors = true;
  }
  return !errors;
}

// TODO: reconfigure the lxer_get_rh() to identify strings with space and return a empty string 
// if a name between two tokens has a space, for instance if a function is declared as "yo mom" instead 
// of "yo_mom" it must return an emptry string and then report an error, but the reporting part is already 
// done by the function dedicated to the compilation process

bool xeb_compiler_function_definition(){
  function_definition* fn_def;
  char* fn_name = lxer_get_rh(&compiler.lh, false);
  bool parameter_error = false, return_error = false;
  fn_def = (function_definition*)arena_alloc(&compiler_ah, sizeof(function_definition));
  if(strlen(fn_name) < 2) { 
    XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_NO_VALID_FUNCTION_NAME,"The function must have a valid name"); 
  }
  fn_def->name = fn_name;
  if(DEBUG) DINFO("Function named '%s' found", fn_name);
  if(!lxer_misc_expect_brk(&compiler.lh)){ 
    XEB_PUSH_ERROR(XEB_ERROR_WRONG_FUNCTION_DEFINITION, LXR_OPEN_BRK, XEB_GET_CURRENT_TOKEN()); 
  }else{
    XEB_NEXT_TOKEN(); 
    parameter_error = xeb_handle_parameter(fn_def);
    XEB_NEXT_TOKEN();
    return_error = xeb_handle_return_type(fn_def);
  }
  if(parameter_error | return_error) {
    fn_def->definition_status = INCOMPLETE;
  }else{ 
    xeb_function_definition_push(fn_def);
    fn_def->definition_status = COMPLETE;
  }
  return !(parameter_error | return_error);
}

bool xeb_handle_return_type(function_definition* fn_def){
  bool errors = false;
  LXR_TOKENS return_token = 0;
  XEB_NEXT_TOKEN();
  fn_def->return_type = (LXR_TOKENS*)arena_alloc(&compiler_ah, sizeof(LXR_TOKENS)*DEFAULT_RETURN_LEN);
  fn_def->return_type_len = DEFAULT_RETURN_LEN;
  fn_def->return_type_tracker = 0;
  if(XEB_GET_CURRENT_TOKEN() == LXR_RETURN_ARROW){
    XEB_NEXT_TOKEN(); XEB_NEXT_TOKEN();
    if(XEB_GET_CURRENT_TOKEN() == LXR_OPEN_BRK){
      if(DEBUG) DINFO("Multiple return type found, start processing", NULL);
      if(lxer_brk_expect_type(&compiler.lh)){
        XEB_NEXT_TOKEN();
        bool first = true;
        while(XEB_GET_CURRENT_TOKEN() != LXR_CLOSE_BRK && !errors){
          if(!first && !(XEB_GET_CURRENT_TOKEN() == LXR_COMMA)){
            XEB_PUSH_ERROR(XEB_ERROR_MISSING_COMMA_SEPARATOR, LXR_COMMA, XEB_GET_CURRENT_TOKEN());
            errors = true;
          }else if(!first && XEB_GET_CURRENT_TOKEN() == LXR_COMMA) XEB_NEXT_TOKEN();
          if(lxer_is_type(XEB_GET_CURRENT_TOKEN())){
            first = false;
            return_token = XEB_GET_CURRENT_TOKEN();
            dapush(&compiler_ah, fn_def->return_type, &fn_def->return_type_tracker, &fn_def->return_type_len, LXR_TOKENS, return_token);
            if(DEBUG) DINFO("Found return type '%s'", token_table_lh[return_token]);
            XEB_NEXT_TOKEN();
          }else{
            XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_SYNTAX, "Missing return type or wrong syntax");
            errors = true;
          }
        }
      }else{
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_MISSING_RETURN_TYPE, "You did not provide a valid return type, maybe you miss it");
        errors = true;
      }
    }else if(lxer_is_type(XEB_GET_CURRENT_TOKEN())){
      if(DEBUG) DINFO("Processing single return type", NULL);
      return_token = XEB_GET_CURRENT_TOKEN();
      dapush(&compiler_ah, fn_def->return_type, &fn_def->return_type_tracker, &fn_def->return_type_len, LXR_TOKENS, return_token);
      if(DEBUG) DINFO("Found return type '%s'", token_table_lh[return_token]);
    }else{
      XEB_PUSH_ERROR(XEB_ERROR_MISSING_BRACKETS, LXR_OPEN_BRK, XEB_GET_CURRENT_TOKEN());
      errors = true;
    }
  }else{
    XEB_PUSH_ERROR(XEB_ERROR_NO_RETURN_ARROW_PROVIDED, LXR_RETURN_ARROW, XEB_GET_CURRENT_TOKEN());
    XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_MISSING_RETURN_OR_WRONG_SYNTAX, "You must specify the return type of the current function");
    errors = true;
  }
  return errors;
}

bool xeb_handle_parameter(function_definition* fn_def){
  LXR_TOKENS parameter_type;
  bool errors = false;
  variable_definition* vd = NULL;
  if(lxer_brk_expect_brk(&compiler.lh)){
    XEB_NEXT_TOKEN();
    if(XEB_GET_CURRENT_TOKEN() != LXR_CLOSE_BRK){
      XEB_PUSH_ERROR(XEB_ERROR_MISSING_BRACKETS, LXR_CLOSE_BRK, XEB_GET_CURRENT_TOKEN());
      errors = true;
    }else{
      if(DEBUG) DINFO("Processing function arguments: no arguments found", NULL);
      fn_def->parameter_len = 0;
      fn_def->parameter_tracker = 0;
      fn_def->function_parameter = NULL;
    }
  }else if(lxer_brk_expect_type(&compiler.lh)){
    if(DEBUG) DINFO("Processing function arguments: at least one parameter found", NULL);
    fn_def->function_parameter = (variable_definition**)arena_alloc(&compiler_ah, sizeof(variable_definition*)*DEFAULT_PARAMETER_DEFINITION_LEN);
    fn_def->parameter_len = DEFAULT_PARAMETER_DEFINITION_LEN;
    fn_def->parameter_tracker = 0;
    XEB_NEXT_TOKEN();
    bool first = true;
    while(XEB_GET_CURRENT_TOKEN() != LXR_CLOSE_BRK && !errors){
      if(!first && !(XEB_GET_CURRENT_TOKEN() == LXR_COMMA)){
        XEB_PUSH_ERROR(XEB_ERROR_WRONG_PARAMETER_DEFINITION, LXR_COMMA, XEB_GET_CURRENT_TOKEN());
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_MISSING_TYPE, "Expecting a type for a parameter definition");
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_PARAMETER_HAS_NO_NAME, "Missing parameter name");
        errors = true;
      }else if(!first && XEB_GET_CURRENT_TOKEN() == LXR_COMMA) XEB_NEXT_TOKEN();
      if(!lxer_is_type(XEB_GET_CURRENT_TOKEN())){
        XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_MISSING_TYPE, "Parameter has no valid type");
        errors = true;
      }else{
        first = false;
        parameter_type = XEB_GET_CURRENT_TOKEN();
        if(lxer_type_expect_statement(&compiler.lh)){
          XEB_NEXT_TOKEN();
          if(XEB_GET_CURRENT_TOKEN() == LXR_TYPE_ASSIGN){
            char* par_name = lxer_get_rh(&compiler.lh, false);
            if(strlen(par_name) < 0){
              XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_PARAMETER_HAS_NO_NAME, "Missing parameter name");
              errors = true;
            }else{
              vd = (variable_definition*)arena_alloc(&compiler_ah, sizeof(variable_definition));
              vd->name = par_name;
              vd->type = parameter_type;
              dapush(&compiler_ah, fn_def->function_parameter, &fn_def->parameter_tracker, &fn_def->parameter_len, variable_definition*, vd);
              if(DEBUG) DINFO("Argument found for variable '%s' of type '%s'", vd->name, token_table_lh[vd->type]);
              XEB_NEXT_TOKEN();
            }
          }else{
            XEB_PUSH_ERROR(XEB_ERROR_TYPE_ASSIGNMENT_MISSING, LXR_TYPE_ASSIGN, XEB_GET_CURRENT_TOKEN());
            errors = true;
          }
        }else{
            XEB_PUSH_ERROR(XEB_ERROR_TYPE_ASSIGNMENT_MISSING, LXR_TYPE_ASSIGN, XEB_GET_CURRENT_TOKEN());
            errors = true;
        }
      }
    }
  }else{
    XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_WRONG_PARAMETER_DEFINITION_FOR_A_FUNCTION,"Expecting a type but got some garbage");
    errors = true;
  }
  return errors;
}


// TODO: complete the return statement 

bool xeb_compiler_return_inst(){
  code_section* cd = xeb_code_section_get();
  if(cd == NULL || cd->fn == NULL) return false;
  function_definition* fn_def = cd->fn;

  bool errors = false;
  char* buffer = NULL;
  char* cat_string = NULL;
  instruction_list* il = (instruction_list*)arena_alloc(&compiler_ah, sizeof(instruction_list));
  return_inst_type* rit = (return_inst_type*)arena_alloc(&compiler_ah, sizeof(return_inst_type));
  XEB_NEXT_TOKEN();
  if(XEB_GET_CURRENT_TOKEN() == LXR_OPEN_BRK){
    char* content = lxer_get_rh(&compiler.lh, false);
    if(lxer_brk_expect_brk(&compiler.lh) && strlen(content) < 1){
      // No return content
      XEB_NEXT_TOKEN();
      if(XEB_GET_CURRENT_TOKEN() == LXR_CLOSE_BRK && lxer_brk_expect_sep(&compiler.lh)){
        XEB_NEXT_TOKEN();
        if(XEB_GET_CURRENT_TOKEN() != LXR_SEMICOLON){
          xeb_missing_semicolon(buffer, cat_string, &errors);
        }else{
          if(fn_def->return_type_tracker == 0){
            rit->variable_reference = NULL;
            rit->variable_reference_len = 0;
            rit->variable_reference_tracker = 0;
            rit->inline_static_strings = NULL;
            il->type = RETURN;
            il->inst = rit;
          }else{
            XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_MISMATCHING_RETURN_TYPE_OR_NUMBER, "The return type or return variable number is not equal to the function definition given");
            errors = true;
          }
        }
      }else{
        errors = true;
      }
    }else{
      // TODO: one or multiple return found, this has to be handled like in the function argument processing 
    }
  }else{
    XEB_PUSH_ERROR(XEB_ERROR_WRONG_SYNTAX, LXR_OPEN_BRK, XEB_GET_CURRENT_TOKEN());
    errors = true;
  }


  if(!errors){
    // TODO: push return insturction
  }
  return !errors;
}


// Internal table pushing function


void xeb_code_section_push(code_section* cd){
  if(code_section_list == NULL) {
    code_section_list = (code_section**)arena_alloc(&compiler_ah, sizeof(code_section*)*DEFAULT_CODE_SECTION_LENGTH);
    code_section_list_tracker = 0;
    code_section_list_len = 0;
  }
  dapush(&compiler_ah, code_section_list, &code_section_list_tracker, &code_section_list_len, code_section*, cd);
}

code_section* xeb_code_section_get(){
  return code_section_list[code_section_list_tracker-1];
}


size_t xeb_data_section_push(char* stream){
  if(data_section == NULL){
    data_section = (u8*)arena_alloc(&compiler_ah, sizeof(u8)*DEFAULT_DATA_SECTION_LEN);
    data_section_len = 0;
    data_section_tracker = 0;
  }
  if(data_section_tracker+strlen(stream) >= data_section_len){
    u8* new_data_section = (u8*)arena_alloc(&compiler_ah, sizeof(u8)*data_section_len*2);
    size_t new_len = data_section_len*2;
    memcpy(new_data_section, data_section, data_section_len);
    data_section = new_data_section;
    data_section_len = new_len; 
  }
  size_t return_index = data_section_tracker;
  memcpy(&data_section[data_section_tracker], stream, strlen(stream));
  data_section_tracker += strlen(stream);
  data_section[data_section_tracker] = '\0';
  data_section++;

  return return_index;
}

variable_definition* xeb_variable_definition_get(char*name, code_section* cd){
  variable_definition* vd = NULL;
  for(size_t i=0;i<cd->local_var_tracker && cd->local_var_tracker > 0; i++){
    if(strcmp(cd->local_var[i]->name, name) == 0){
      vd = cd->local_var[i];
      break;
    }
  }
  return vd;
}


void xeb_variable_definition_push(variable_definition* vd, code_section* cd){
  bool not_found = true;
  bool parameter_not_found = true;
  
  for(size_t i = 0; i<cd->local_var_tracker &&  cd->local_var_tracker> 0;i++){
    if(strcmp(vd->name, cd->local_var[i]->name) == 0){
      not_found = false;
      break;
    }
  }
  function_definition* fn = cd->fn;
  for(size_t i = 0; i < fn->parameter_tracker; i++){
    if(strcmp(vd->name, fn->function_parameter[i]->name) == 0){
      parameter_not_found = false;
      break;
    }
  }
  if(not_found && parameter_not_found){
    if(DEBUG) DINFO("Pushing variable definition inside '%s' code section", cd->fn->name);
    dapush(&compiler_ah, cd->local_var, &cd->local_var_tracker, &cd->local_var_len, variable_definition*, vd);
  }else{
    XEB_PUSH_ERROR_CUSTOM_MESSAGE(XEB_ERROR_VARIABLE_ALREADY_DEFINED, "This variable name is already taken, try change it");
  }
}

bool xeb_line_is_empty(size_t line){
  
  char* buffer = (char*)arena_alloc(&compiler_ah, sizeof(char)*512);
  char* nl = strchr(compiler.source_lines[line].pointer,'\n');
  buffer[0] = '\0';
  if(nl != NULL){
    strcpy(buffer, compiler.source_lines[line].pointer);
    buffer[nl - compiler.source_lines[line].pointer] = '\0';
  }
  if(strlen(buffer) > 0){
    return false;
  }
  return true;
}

// error-related functions

void xeb_validate_line_status(){
  size_t current_token_line = 0;
  compiler.lh.lxer_tracker = 0;
  line_slice* ctx = NULL;
  bool no_token_left = false;
  LXR_TOKENS token = 10;
  // initialize local array
  
  for(size_t i=0;i<compiler.loaded_slice; i++){
    compiler.source_lines[i].local_tokens = (LXR_TOKENS*)arena_alloc(&compiler_ah, sizeof(LXR_TOKENS)*8);
    compiler.source_lines[i].local_tokens_len = 128;
    compiler.source_lines[i].local_tokens_tracker = 0;
  }
  // enstablish tokens per line
  for(size_t i=0;i<compiler.loaded_slice;i++){
    ctx = &compiler.source_lines[i];
    no_token_left = false;
    compiler.source_lines[i].has_tokens = false;
    while(!no_token_left){
      token = lxer_get_current_token(&compiler.lh);
      if(token == TOKEN_TABLE_END) break;
      current_token_line = xeb_error_get_line(lxer_get_current_pointer(&compiler.lh));
      if(current_token_line != i){
        no_token_left = true;
      }else{
        //printf("Copying token '%s' found in line %zu\n", token_table_lh[token], current_token_line);
        dapush(&compiler_ah, ctx->local_tokens, &ctx->local_tokens_tracker, &ctx->local_tokens_len, LXR_TOKENS, token);
        lxer_next_token(&compiler.lh);
        compiler.source_lines[i].has_tokens = true;
      }
    }
  }
  compiler.lh.lxer_tracker = 0;
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
      cache->line = global_line_counter-1;
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


char* xeb_error_get_line_pointer(char* ptr){
  for(size_t i=1;i<compiler.loaded_slice-1;i++){
    if(compiler.source_lines[i].pointer > ptr){
      return compiler.source_lines[i-1].pointer;
    }
  }
  return NULL;
}

size_t xeb_error_get_line(char*ptr){
  for(size_t i=0;i<compiler.loaded_slice;i++){
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


bool xeb_error_push_error(XEB_COMPILER_ERRNO err, char*pointer,char* line_pointer ,size_t line, bool report, LXR_TOKENS lh, LXR_TOKENS rh, char* string){
  if(compiler.error_tracker > DEFAULT_ERROR_LIMIT){
    xeb_close_compiler(true);
  }else{
    
    xeb_error_box* error_box = NULL;
    error_box = (xeb_error_box*)arena_alloc(&compiler_ah,sizeof(xeb_error_box));
    error_box->error = err;
    error_box->xeb_error_to_string = xeb_error_get_message(err);
    error_box->code_pointer = pointer;
    error_box->line = line;
    error_box->line_pointer = line_pointer;
    error_box->complete_report = report;
    error_box->lh = lh;
    error_box->rh = rh;
    if(string != NULL){
      error_box->string = (char*)arena_alloc(&compiler_ah, sizeof(char)*strlen(string));
      strcpy(error_box->string, string);
    }else{
      error_box->string = NULL;
    }

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
      break;
  }

  #undef X
  return error_message;
}



void xeb_error_report(){
  if(compiler.error_tracker > 0){
    fprintf(stderr, "\x1b[31mError report before compilation: \n\x1b[0m");
    for(size_t i=0;i<compiler.error_tracker; i++){
      size_t fail_point = 1+(compiler.final_error_report[i]->code_pointer - compiler.final_error_report[i]->line_pointer);
      char* buffer = (char*)arena_alloc(&compiler_ah, sizeof(char)*256);
      strcpy(buffer, compiler.final_error_report[i]->line_pointer);
      char* tracker = strchr(buffer,'\n');
      char* custom_message = compiler.final_error_report[i]->string;
      if( tracker != NULL) buffer[(int)(tracker-buffer)] = '\0';
      if(compiler.final_error_report[i]->complete_report){
        fprintf(stderr, "\x1b[31m->  Error in line %zu: Expecting '%s' but got '%s': %s\n",compiler.final_error_report[i]->line+1,token_table_lh[compiler.final_error_report[i]->lh],token_table_lh[compiler.final_error_report[i]->rh],compiler.final_error_report[i]->xeb_error_to_string);
        if(custom_message != NULL){
          fprintf(stderr,"\t%s\n", custom_message);
        }
        char* cursor = "|";
        fprintf(stderr, "%s\n", buffer);
        for(size_t i=0;i<fail_point;i++){
          fprintf(stderr,"^");
        }
        fprintf(stderr,"\n%*s\n", (int)fail_point, cursor);
        for(size_t i=0;i<fail_point-1;i++){
          fprintf(stderr,"-");
        }
        fprintf(stderr, "+\n\x1b[0m");
      }else{
        fprintf(stderr, "\x1b[31m->  Error in line %zu: This is not a valid expression: %s  ",compiler.final_error_report[i]->line+1,compiler.final_error_report[i]->xeb_error_to_string);
        if(custom_message != NULL){
            fprintf(stderr,"\t%s\n",custom_message);
        }
        fprintf(stderr, "%s\n", buffer);
        for(size_t i=0;i<strlen(buffer); i++){
          fprintf(stderr, "^");
        }
        fprintf(stderr, "\x1b[0m\n\n");
      }
    }
    ERROR("Exiting due to %zu error", compiler.error_tracker);
  }
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
  
  bool not_found = true;

  if(!entry_point_present){
    if(strcmp(fn_def->name, "main") == 0){
      if(DEBUG) DINFO("Entry point found with name '%s'", fn_def->name);
      entry_point_present = true;
    }
  }

  for(size_t i = 0; i<fn_def_table_tracker && fn_def_table_tracker > 0;i++){
    if(strcmp(fn_def_table[i]->name, fn_def->name) == 0){
      not_found = false;
      break;
    }
  }

  if(not_found){
    dapush(&compiler_ah, fn_def_table, &fn_def_table_tracker, &fn_def_table_len, function_definition*, fn_def);
  }else{
    //XEB_PUSH_ONLY_ERROR(XEB_ERROR_FUNCTION_ALREADY_DEFINED);
  }
}

function_definition* xeb_function_definition_get(){
  return fn_def_table[fn_def_table_tracker-1];
}



void xeb_switch_context(){
  if(compiler.local_line_tracker < compiler.loaded_slice){
    compiler.ctx = &compiler.source_lines[compiler.local_line_tracker+1];
    compiler.local_line_tracker += 1;
  }
}


LXR_TOKENS xeb_get_current_token(line_slice* ctx){
  if(ctx->local_tokens_rp == ctx->local_tokens_tracker){
    xeb_switch_context();
    return TOKEN_TABLE_END;
  }
  return ctx->local_tokens[ctx->local_tokens_rp];
}


// TODO: implement context switching if the XEB_NEXT_TOKEN() call fails because the current contex of execution has reached his end.
// Keep in mind that this is only possible if the contex have indeed no tokens left and there is a valid line after it 

bool xeb_next_token(line_slice* ctx){
  if(ctx->local_tokens_rp >= ctx->local_tokens_tracker || compiler.lh.lxer_tracker == compiler.lh.stream_out_len){
    return false;
  }
  ctx->local_tokens_rp+=1;
  lxer_next_token(&compiler.lh);
  return true;
}


void xeb_close_compiler(bool fatal){
  if(!fatal){
    XEB_NOTY("XEB Compiler", "Compilation completed, exiting..", NULL);
  }else{
    ERROR("Fatal, exiting due to to much error", NULL);
  }
  if(compiler_ah.arena_count >= 1) arena_free(&compiler_ah);
  if(compiler.lh.lxer_ah.arena_count >= 1) arena_free(&compiler.lh.lxer_ah);
}
