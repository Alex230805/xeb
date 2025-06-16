#ifndef XEB_H
#define XEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lxer.h>
#include <misc.h>


typedef struct{
  // current lexer 
  lxer_head lh;
  char* source_code;
  size_t source_len;
}xebc;


static xebc compiler = {0};
static Arena_header compiler_ah = {0};

void xeb_helper();

void xeb_load_file(const char* source_file);
void xeb_close_compiler();

#ifndef XEB_C
#define XEB_C

#endif
#endif // xeb
