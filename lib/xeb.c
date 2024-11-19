#define XEB_C
#include "xeb.h"


void xeb_lexer(){
  lxer_load_alphabet(
    *symbols, SYM_COUNT, 
    *keywords, KWD_COUNT,
    *definitions, DEF_COUNT,
    *types, TYP_COUNT,
    *comments, CMT_COUNT,
    *operators, OPR_COUNT,
    *separators, SPR_COUNT
  );
}

void xeb_compiler(String_builder*path,String_builder*output_path){
  
}
