#define XEB_C
#include "xeb.h"

void xeb_lexer(String_builder*path){
 if(lxer_load_alphabet(
    symbols, SYM_COUNT, 
    keywords, KWD_COUNT,
    definitions, DEF_COUNT,
    types, TYP_COUNT,
    comments, CMT_COUNT,
    operators, OPR_COUNT,
    separators, SPR_COUNT
  ) != 0){
    lxer_noty_error("Unable to load alphabet, failed on lexer startup not in actual code compilation\n");
    exit(0);
  }
  lxer_read_entire_file(path->string);

  TODO("Complete lexer usage in xeb_lexer");
  return;
  
  while(!lxer_eof()){
  }

}

int xeb_compiler(String_builder*path,String_builder*output_path){

  TODO("Complete the compiler");
  char*ptr = strstr(path->string, ".x");
  if(ptr == NULL) return -1;

  UNUSED(output_path);
  //UNUSED(path);
  
  //return;
  xeb_lexer(path); 

  return 0;
}
