#ifndef LXER_H
#define LXER_H 


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <toolbox.h>
#include <stdbool.h>

typedef struct{
  char *symbols;
  size_t symbols_length;

  char* keywords;
  size_t keywords_length;

  char *definitions;
  size_t definitions_length;

  char *types;
  size_t types_length;

  char *operators;
  size_t operators_length;

  char *comments;
  size_t comments_length;

  char *separators;
  size_t separators_length;

  String_builder *file_word;
  size_t current_pointer;

}lxer;

static lxer l = {0};

#define lxer_notef(arg) printf(#arg);

// lxer symbols definition 

int lxer_load_symb(char *symbols, size_t symbols_length);
int lxer_load_keyw(char *keywords, size_t keywords_length);
int lxer_load_definitions(char *definitions, size_t definitions_length);
int lxer_load_types(char *types, size_t types_length);
int lxer_load_operators(char *operators, size_t operators_length);
int lxer_load_separators(char *separators, size_t separators_length);
int lxer_load_comments(char *comments, size_t comments_length);

int lxer_load_alphabet( char *symbols, size_t symbols_length,
                        char *keywords, size_t keywords_length,
                        char *definitions, size_t definitions_length,
                        char *comments, size_t comments_length,
                        char *types, size_t types_length,
                        char *operators, size_t operators_length,
                        char *separators, size_t separators_length );

// error, warning and notification 

void lxer_noty_error(const char*str);
void lxer_noty_warn(const char*str);
void lxer_noty_printf(const char*str);

// file loading

void lxer_read_entire_file(const char* filepath);
char lxer_get_token();
String_builder* lxer_get_phrase();
bool lxer_eof();


#ifndef LXER_IMPLEMENTATION
#define LXER_IMPLEMENTATION
#endif



#endif // lxer
