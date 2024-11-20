#define XEB_C
#include "xeb.h"

void xeb_lexer(){
 if(lxer_load_alphabet(
    *symbols, SYM_COUNT, 
    *keywords, KWD_COUNT,
    *definitions, DEF_COUNT,
    *types, TYP_COUNT,
    *comments, CMT_COUNT,
    *operators, OPR_COUNT,
    *separators, SPR_COUNT
  ) != 0){
    lxer_noty_error("Unable to load alphabet, failed on lexer startup not in actual code compilation\n");
    exit(0);
  }
}

void xeb_compiler(String_builder*path,String_builder*output_path){
  TODO("Complete the compiler");
  xeb_lexer(); 
}
