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



void xeb_close_compiler(){
  NOTY("XEB Compiler", "Compilation completed, exiting..", NULL);
  arena_free(&compiler_ah);
  arena_free(&compiler.lh.lxer_ah);
}
