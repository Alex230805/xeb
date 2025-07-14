#define XEB_INST_IMP
#include "xeb_inst.h"



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
